#include "calc_cli/cache_service/storage.h"

namespace calc_cli {

Storage::Storage(const std::string& connectionString) {
    connection_ = PQconnectdb(connectionString.c_str());

    if (connection_ == nullptr)
        throw std::runtime_error("Failed to allocate PostgreSQL");

    if (PQstatus(connection_) != CONNECTION_OK) {
        std::string errorMessage = PQerrorMessage(connection_);
        
        PQfinish(connection_);
        connection_ = nullptr;

        throw std::runtime_error(
            "PostgreSQL connection failed: " + errorMessage 
        );
    }
}

Storage::~Storage() {
    if (connection_ != nullptr)
        PQfinish(connection_);
}

Storage::Storage(Storage&& other) noexcept
    : connection_(other.connection_) {
    other.connection_ = nullptr;
}

Storage& Storage::operator=(Storage&& other) noexcept {
    if (this != &other) {
        if (connection_ != nullptr) {
            PQfinish(connection_);
        }

        connection_ = other.connection_;
        other.connection_ = nullptr;
    }

    return *this;
}

Storage::PgResultPtr Storage::makeResult(
    PGresult* rawResult
) const {
    if (rawResult == nullptr)
        throw std::runtime_error("PostrgeSQL returned null result");

    return PgResultPtr(rawResult, &PQclear);
}

void Storage::checkResultStatus(
    PGresult* result,
    ExecStatusType expectedStatus,
    const std::string& errorPrefix
) const {
    ExecStatusType actualStatus = PQresultStatus(result);

    if (actualStatus != expectedStatus) {
        std::string errorMessage = PQresultErrorMessage(result);
        
        throw std::runtime_error(errorPrefix + ": " + errorMessage);
    }
}

std::unordered_map<std::string, ApplicationContext> Storage::loadAll() {
    const char* sql = 
        "SELECT "
        "normalized_key, "
        "operation_name, "
        "left_operand, "
        "right_operand, "
        "result_value, "
        "status_code "
        "FROM calculation_results";

    PgResultPtr result = makeResult(PQexec(connection_, sql));

    checkResultStatus(
        result.get(),
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
    const char* sql =
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

    const char* values[6] = {
        key.c_str(),
        operationName.c_str(),
        leftOperand.c_str(),
        rightOperand.c_str(),
        resultValue.c_str(),
        statusCode.c_str()
    };

    PgResultPtr result = makeResult(
        PQexecParams(
            connection_,
            sql,
            6,
            nullptr,
            values,
            nullptr,
            nullptr,
            0
        )
    );

    checkResultStatus(
        result.get(),
        PGRES_COMMAND_OK,
        "Failed to save calculation result"
    );
}

}