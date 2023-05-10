#ifndef DB_CONNECTION_INFO
#define DB_CONNECTION_INFO

#include <string>

namespace DbInfo
{
    // для переменных окружения
    class DbInfo {
        DbInfo() = delete;
        DbInfo(const DbInfo& dbi) = delete;
        DbInfo operator=(const DbInfo& dbi) = delete;
    public:
        static const char* getLoginName();
        static const std::string& getLoginValue();
        static const char* getPasswordName();
        static const std::string& getPasswordValue();
        static const char* getDbName();
        static const std::string& getDbNameValue();
    };
} // namespace DbInfo


#endif