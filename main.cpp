#include <iostream>
#include <pqxx/pqxx>
#include <chrono>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "src/FileParser/FileParser.h"
#include "src/TimerHandlerSql/TimerHandlerSql.h"
#include "src/DbConnectionInfo/DbConnectionInfo.h"

void print_proc_id_terminated(void);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Не указан путь к конфигурационному файлу\n";

        exit(EXIT_FAILURE);
    }

    pid_t pid;
    Logger::Logger* logger;

    try {
        logger = Logger::Logger::get_logger();
    } catch (const std::invalid_argument& err) {
        std::cerr << err.what() << std::endl;

        exit(EXIT_FAILURE);
    }

    if ((pid = fork()) == -1) {
        logger->error("| Ошибка при создании демона");

        exit(EXIT_FAILURE);
    }
    if (!pid) {
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);

        logger->info("| PID процесса: " + std::to_string(getpid()));

        atexit(print_proc_id_terminated);

        setsid();
    }
    if (pid) {
        exit(EXIT_SUCCESS);
    }
/*
    close(STDIN_FILENO);
    close(STDERR_FILENO);

    int fd;

    fd = open("../logger.log", O_CREAT | O_RDWR | O_APPEND, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);

    if (fd == -1) {
        std::cout << "Ошибка при создания файла логирования\n";

        return 1;
    }

    dup2(fd, 2);
*/
    std::string path = std::string(argv[1]);

    FileParser::FileParser parser(path);

    std::vector<std::thread> threads;

    try {
        std::vector<Timer::TimerHandler*> cmds = parser.getCommands();

        std::for_each(cmds.begin(), cmds.end(), [&threads](Timer::TimerHandler* timer) {
            threads.emplace_back(timer->start());
        });
    } catch (const std::invalid_argument& err) {
        logger->error(err.what());
    }

    for (auto&& t : threads) {
        try {
            t.join();
        } catch(const std::system_error& err) {
            std::cout << err.what() << std::endl;
        }
    }
    /*
    std::condition_variable cv;
    std::mutex m;
    std::unique_lock<std::mutex> lock(m);

    cv.wait(lock, []() { return false; });
    */

    return 0;
}

void print_proc_id_terminated(void) {
    Logger::Logger::get_logger()->info("| Процесс с PID " + std::to_string(getpid()) + " завершил работу");
}