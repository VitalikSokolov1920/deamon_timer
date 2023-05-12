#include <iostream>
#include <iomanip>
#include <sys/stat.h>

#include "TimerHandlerDump.h"
#include "../DbConnectionInfo/DbConnectionInfo.h"

namespace Timer
{
    void TimerHandlerDump::handle() {
        const time_t now = time(NULL);

        struct tm* now_tm = localtime(&now);

        std::string dump_path;

        if (!getenv("DUMP_PATH")) {
            dump_path = "./backup";
        } else {
            dump_path = getenv("DUMP_PATH");
        }

        struct stat st{};
        int ret;

        if ((ret = stat(dump_path.c_str(), &st)) == -1) {
            mkdir(dump_path.c_str(), S_IRWXU | S_IRWXG | S_IROTH);
        } else {
            if (!S_ISDIR(st.st_mode)) {
                logger->error("Невозможно создать директрию для резервного копирования, т.к. существует файл с таким названием");

                return;
            }
        }

        std::string dump_file_name = dump_path + "/dump_";

        dump_file_name += std::to_string(now_tm->tm_year + 1900) + "_";
        dump_file_name += std::to_string(now_tm->tm_mon + 1) + "_";
        dump_file_name += std::to_string(now_tm->tm_mday) + "_";
        dump_file_name += std::to_string(now_tm->tm_hour) + "_";
        dump_file_name += std::to_string(now_tm->tm_min) + "_";
        dump_file_name += std::to_string(now_tm->tm_sec);

        std::string command("pg_dump -U ");

        command.append(DbInfo::DbInfo::getLoginValue() + " ");
        command.append(DbInfo::DbInfo::getDbNameValue() + " ");
        command.append("> ");
        command.append(dump_file_name);
        command.append("\n");

        system(command.c_str());

        logger->info(*this);
    }

    std::ostream& operator<<(std::ostream& os, const TimerHandlerDump& dump) {

        os << "| DUMP DATABASE\n";

        os << "| Time start:   " 
        << std::setw(4) << std::setfill('0') << dump.time_start.tm_year + 1900 << "-" 
        << std::setw(2) << std::setfill('0') << dump.time_start.tm_mon + 1 << "-"
        << std::setw(2) << std::setfill('0') << dump.time_start.tm_mday << " " 
        << std::setw(2) << std::setfill('0') << dump.time_start.tm_hour << ":" 
        << std::setw(2) << std::setfill('0') << dump.time_start.tm_min << ":"
        << std::setw(2) << std::setfill('0') << dump.time_start.tm_sec 
        << std::endl;

        os << "| Time period:  " 
        << std::setw(4) << std::setfill('0') << dump.time_period.tm_year - 70 << "-" 
        << std::setw(2) << std::setfill('0') << dump.time_period.tm_mon << "-"
        << std::setw(2) << std::setfill('0') << dump.time_period.tm_mday - 1 << " " 
        << std::setw(2) << std::setfill('0') << dump.time_period.tm_hour << ":" 
        << std::setw(2) << std::setfill('0') << dump.time_period.tm_min << ":"
        << std::setw(2) << std::setfill('0') << dump.time_period.tm_sec
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
        << std::setw(2) << std::setfill('0') << now->tm_sec;
        
        return os;
    }
} // namespace Timer
