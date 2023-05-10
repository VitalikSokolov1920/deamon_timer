#ifndef TIMER_HANDLER_H
#define TIMER_HANDLER_H

#include <chrono>
#include <string>
#include <ctime>
#include <future>
#include <functional>
#include <iostream>
#include "../Logger/Logger.h"

namespace Timer
{
    class TimerHandler {
    public:
        std::tm time_start;
        std::tm time_period;
        Logger::Logger* logger;
    public:
        TimerHandler(std::tm& start, std::tm& period) : time_start(start), time_period(period) {
            logger = Logger::Logger::get_logger();
        }
        std::thread start();
        virtual void handle() = 0;
    };
} // namespace Timer


#endif