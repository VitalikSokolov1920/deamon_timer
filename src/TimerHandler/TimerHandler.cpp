#include "TimerHandler.h"

namespace Timer
{
    std::thread TimerHandler::start() {
        std::thread t = std::thread([this]() {
            time_t now = time(nullptr);

            const time_t t_start = mktime(&time_start), t_period = timegm(&time_period);

            std::this_thread::sleep_for(std::chrono::seconds(t_start - now));

            handle();

            if (t_period) {
                while(true) {
                    now = time(nullptr);
                    auto tp = std::chrono::system_clock::now() + std::chrono::seconds(t_period);
                    std::this_thread::sleep_until(tp);

                    handle();
                }
            }
        });

        // t.detach();

        return t;
    }
} // namespace Timer
