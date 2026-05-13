#include "calc_cli/cache_service/storage.h"

namespace calc_cli {

Storage::Storage(const std::string& connectionString)
    : connection_(nullptr, &PQfinish){
    connection_.reset(PQconnectdb(connectionString.c_str()));

    if (connection_ == nullptr)
        throw std::runtime_error("Failed to allocate PostgreSQL");

    if (PQstatus(connection_.get()) != CONNECTION_OK) {
        std::string errorMessage = PQerrorMessage(connection_.get());
        
        PQfinish(connection_.get());
        connection_ = nullptr;

        throw std::runtime_error(
            "PostgreSQL connection failed: " + errorMessage 
        );
    }
}

Storage::PgResultPtr Storage::makeResult(
    PGresult* rawResult
) const {
    if (rawResult == nullptr)
        throw std::runtime_error("PostrgeSQL returned null result");

    return PgResultPtr(rawResult, &PQclear);
}

void Storage::checkResultStatus(
    const PgResultPtr& result,
    ExecStatusType expectedStatus,
    const std::string& errorPrefix
) const {
    ExecStatusType actualStatus = PQresultStatus(result.get());

    if (actualStatus != expectedStatus) {
        std::string errorMessage = PQresultErrorMessage(result.get());
        
        throw std::runtime_error(errorPrefix + ": " + errorMessage);
    }
}

std::unordered_map<std::string, ApplicationContext> Storage::loadAll() {
    const std::string sql = 
        "SELECT "
        "normalized_key, "
        "operation_name, "
        "left_operand, "
        "right_operand, "
        "result_value, "
        "status_code "
        "FROM calculation_results";

    PgResultPtr result = makeResult(PQexec(connection_.get(), sql.c_str()));

    checkResultStatus(
        result,
        PGRES_TUPLES_OK,
        "Failed to load calculation results"
    );

    std::unordered_map<std::string, ApplicationContext> records;

    int rowsCount = PQntuples(result.get());

    for (int row = 0; row < rowsCount; ++row) {
        std::string key = PQgetvalue(result.get(), row, 0);
        std::string operationName = PQgetvalue(result.get(), row, 1);

        ApplicationContext context;

        context.operation = OperationTraits::fromString(operationName);
        context.left = std::stoll(PQgetvalue(result.get(), row, 2));
        context.right = std::stoll(PQgetvalue(result.get(), row, 3));
        context.result = std::stoll(PQgetvalue(result.get(), row, 4));
        context.parse_status = std::stoi(PQgetvalue(result.get(), row, 5));

        context.help_requested = 0;

        records.emplace(std::move(key), context);
    }

    return records;
}

void Storage::save(
    const ApplicationContext& context
) {
    const std::string sql =
        "INSERT INTO calculation_results ("
        "normalized_key, "
        "operation_name, "
        "left_operand, "
        "right_operand, "
        "result_value, "
        "status_code"
        ") "
        "VALUES ($1, $2, $3, $4, $5, $6) "
        "ON CONFLICT (normalized_key) "
        "DO UPDATE SET "
        "operation_name = EXCLUDED.operation_name, "
        "left_operand = EXCLUDED.left_operand, "
        "right_operand = EXCLUDED.right_operand, "
        "result_value = EXCLUDED.result_value, "
        "status_code = EXCLUDED.status_code, "
        "updated_at = now()";

    std::string key = OperationTraits::makeNormalizedKey(context);
    std::string operationName = OperationTraits::toString(context.operation);
    std::string leftOperand = std::to_string(context.left);
    std::string rightOperand = std::to_string(context.right);
    std::string resultValue = std::to_string(context.result);
    std::string statusCode = std::to_string(context.parse_status);

    auto values = std::make_unique<const char*[]>(6);

    values[0] = key.c_str();
    values[1] = operationName.c_str();
    values[2] = leftOperand.c_str();
    values[3] = rightOperand.c_str();
    values[4] = resultValue.c_str();
    values[5] = statusCode.c_str();

    PgResultPtr result = makeResult(
        PQexecParams(
            connection_.get(),
            sql.c_str(),
            6,
            nullptr,
            values.get(),
            nullptr,
            nullptr,
            0
        )
    );

    checkResultStatus(
        result,
        PGRES_COMMAND_OK,
        "Failed to save calculation result"
    );
}

}