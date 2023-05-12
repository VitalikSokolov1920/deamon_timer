#include "DbConnectionInfo.h"

namespace DbInfo
{
    const char* DbInfo::getLoginName() {
        return "PGUSER";
    }

    const std::string& DbInfo::getLoginValue() {
        static std::string login = getenv(DbInfo::getLoginName());

        return login;
    }

    const char* DbInfo::getPasswordName() {
        return "PGPASSWORD";
    }

    const std::string& DbInfo::getPasswordValue() {
        static std::string password = getenv(DbInfo::getPasswordName());

        return password;
    }

    const char* DbInfo::getDbName() {
        return "PGDBNAME";
    }

    const std::string& DbInfo::getDbNameValue() {
        static std::string dbName = getenv(DbInfo::getDbName());

        return dbName;
    }
} // namespace Db
