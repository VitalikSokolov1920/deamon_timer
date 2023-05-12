#include "Logger.h"

namespace Logger
{
    Logger* Logger::logger = nullptr;
    std::mutex Logger::m = std::mutex();
    Logger* Logger::get_logger() {
        m.lock();
        if (!logger) {
            std::string path_to_log;

            if (!getenv("LOG_PATH")) {
                path_to_log = "./ivt_log.log";
            } else {
                path_to_log = getenv("LOG_PATH");
            }

            logger = new Logger;

            logger->os.open(path_to_log, std::ios::app);

            if (!logger->os.is_open()) {
                throw std::invalid_argument("Невозможно открыть файл " + path_to_log + " для логирования");
            }
        }

        m.unlock();

        return logger;
    }
} // namespace Logger
