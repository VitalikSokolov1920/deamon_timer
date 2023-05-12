#include <iostream>

#include "TimerHandlerSql.h"
#include "../DbConnectionInfo/DbConnectionInfo.h"

namespace Timer
{
    void TimerHandlerSql::handle() {
        std::string connection_info("user=");
        connection_info+=DbInfo::DbInfo::getLoginValue() + " ";
        connection_info+= "dbname=" + DbInfo::DbInfo::getDbNameValue() + " ";

        try {
            pqxx::connection c(connection_info);

            pqxx::work txn(c);

            try {
                std::for_each(cmds.cbegin(), cmds.cend(), [&txn, this](std::string cmd) {
                    pqxx::result result = txn.exec(cmd);
                });
    
                txn.commit();
    
                logger->info(*this);
            } catch (const std::exception& e) {
                logger->error(e.what());
            }
        } catch (const std::exception& err) {
            logger->error(err.what());

            return;
        }
    }

    std::ostream& operator<<(std::ostream& os, const TimerHandlerSql& sql) {

        os << "| SQL SCRIPT\n";

        os << "| Time start:   "
        << std::setw(4) << std::setfill('0') << sql.time_start.tm_year + 1900 << "-"
        << std::setw(2) << std::setfill('0') << sql.time_start.tm_mon + 1 << "-"
        << std::setw(2) << std::setfill('0') << sql.time_start.tm_mday << " "
        << std::setw(2) << std::setfill('0') << sql.time_start.tm_hour << ":"
        << std::setw(2) << std::setfill('0') << sql.time_start.tm_min << ":"
        << std::setw(2) << std::setfill('0') << sql.time_start.tm_sec 
        << std::endl;

        os << "| Time period:  "
        << std::setw(4) << std::setfill('0') << sql.time_period.tm_year - 70 << "-"
        << std::setw(2) << std::setfill('0') << sql.time_period.tm_mon << "-"
        << std::setw(2) << std::setfill('0') << sql.time_period.tm_mday - 1 << " "
        << std::setw(2) << std::setfill('0') << sql.time_period.tm_hour << ":"
        << std::setw(2) << std::setfill('0') << sql.time_period.tm_min << ":"
        << std::setw(2) << std::setfill('0') << sql.time_period.tm_sec
        << std::endl;

        auto tp = std::chrono::system_clock::now();

        const time_t time = std::chrono::system_clock::to_time_t(tp);

        struct tm* now = localtime(&time);

        os << "| Current time: "
        << std::setw(4) << std::setfill('0') << now->tm_year + 1900 << "-" 
        << std::setw(2) << std::setfill('0') << now->tm_mon + 1 << "-"
        << std::setw(2) << std::setfill('0') << now->tm_mday << " " 
        << std::setw(2) << std::setfill('0') << now->tm_hour << ":" 
        << std::setw(2) << std::setfill('0') << now->tm_min << ":"
        << std::setw(2) << std::setfill('0') << now->tm_sec
        << std::endl;

        os << "+-------------------------------SQL COMMANDS-----------------------------------------";

        std::for_each(sql.cmds.begin(), sql.cmds.end(), [&os](std::string cmd) {
            os << "\n| " << cmd;
        });

        return os;
    }
} // namespace Timer
