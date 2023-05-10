#ifndef TIMER_HANDLER_SQL_H
#define TIMER_HANDLER_SQL_H

#include <string>
#include <pqxx/pqxx>
#include <vector>

#include "../TimerHandler/TimerHandler.h"

namespace Timer
{
    class TimerHandlerSql : public TimerHandler {
        std::vector<std::string> cmds;
    public:
        TimerHandlerSql(std::tm& start, std::tm& period) : TimerHandler(start, period) {}
        void handle();
        void add_sql_cmd(std::string sql) {
            cmds.push_back(sql);
        }
        friend std::ostream& operator<<(std::ostream& os, const TimerHandlerSql& sql);
    };
} // namespace Timer

#endif