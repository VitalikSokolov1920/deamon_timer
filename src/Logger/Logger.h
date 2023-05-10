#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <mutex>

namespace Logger
{
    class Logger {
        static Logger* logger;
        std::fstream os;
        Logger() {}
        Logger& operator=(const Logger& logger);
        Logger(const Logger&);
        static std::mutex m;
    public:
        static Logger* get_logger();

        template<class T>
        const Logger& info(const T& info) {
            m.lock();

            this->os 
            << "+------------------------------------------------------------------------------------\n"
            << "| [INFO]\n" 
            << info
            << "\n++------------------------------------------------------------------------------------\n\n";

            os.flush();

            m.unlock();

            return *this;
        }
        
        template<class T>
        const Logger& error(const T& error) {
            m.lock();

            auto tp = std::chrono::system_clock::now();

            const time_t time = std::chrono::system_clock::to_time_t(tp);

            struct tm* now = localtime(&time);

            this->os 
            << "+------------------------------------------------------------------------------------\n"
            << "| [ERROR]\n" 
            << error
            << "\nError time: "
            << std::setw(4) << std::setfill('0') << now->tm_year + 1900 << "-" 
            << std::setw(2) << std::setfill('0') << now->tm_mon + 1 << "-"
            << std::setw(2) << std::setfill('0') << now->tm_mday << " " 
            << std::setw(2) << std::setfill('0') << now->tm_hour << ":" 
            << std::setw(2) << std::setfill('0') << now->tm_min << ":"
            << std::setw(2) << std::setfill('0') << now->tm_sec
            << "\n++------------------------------------------------------------------------------------\n\n";

            os.flush();

            m.unlock();

            return *this;
        }
        
        template<class T>
        std::ostream& operator<<(const T& info) {
            m.lock();

            this->os << info;

            m.unlock();

            return this->os;
        }
    };
} // namespace Logger


#endif