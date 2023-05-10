#ifndef TIMER_HANDLER_DUMP
#define TIMER_HANDLER_DUMP

#include "../TimerHandler/TimerHandler.h"

namespace Timer
{
    class TimerHandlerDump : public TimerHandler {
    public:
        TimerHandlerDump(std::tm date_start, std::tm date_period) : TimerHandler(date_start, date_period) {}
        void handle();
        friend std::ostream& operator<<(std::ostream& os, const TimerHandlerDump& dump);
    };
} // namespace Timer


#endif