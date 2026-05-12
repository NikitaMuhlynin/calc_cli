#include <libpq-fe.h>

#include <string>
#include <unordered_map>
#include <memory>

#include "calc_cli/application/context.h"

namespace calc_cli {

class Storage {
public:
    explicit Storage(const std::string& connectionString);
    ~Storage();

    Storage(const Storage& other) = delete;
    Storage& operator=(const Storage& other) = delete;

    Storage(Storage&& other) noexcept;
    Storage& operator=(Storage&& other) noexcept;

    std::unordered_map<std::string, ApplicationContext> loadAll();
    void save(const ApplicationContext& context);

private:
    using PgResultPtr = std::unique_ptr<PGresult, decltype(&PQclear)>;

    PgResultPtr makeResult(PGresult* rawResult) const;

    void checkResultStatus(
        PGresult* result,
        ExecStatusType expectedStatus,
        const std::string& errorPrefix
    ) const;

private:
    PGconn* connection_ = nullptr;

};

}