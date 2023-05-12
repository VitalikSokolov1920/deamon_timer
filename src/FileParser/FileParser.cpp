#include <sstream>

#include "FileParser.h"
#include "../TimerHandlerSql/TimerHandlerSql.h"
#include "../TimerHandlerDump/TimerHandlerDump.h"

namespace FileParser
{
    std::tm parse_date(const std::string& date) {
        std::stringstream date_str(date);
        std::string date_part;
        std::tm date_tm = std::tm();
        unsigned date_count = 0;

        while(std::getline(date_str, date_part, '-')) {
            int part = std::atoi(date_part.c_str());
            switch (date_count)
            {
            // год
            case 0:
                date_tm.tm_year = part;
                date_count++;
                break;
            // месяц
            case 1:
                date_tm.tm_mon = part;
                date_count++;
                break;
            // день
            case 2:
                date_tm.tm_mday = part;
                date_count++;
                break;
            // час
            case 3:
                date_tm.tm_hour = part;
                date_count++;
                break;
            // минуты
            case 4:
                date_tm.tm_min = part;
                date_count++;
                break;
            // секунды
            case 5:
                date_tm.tm_sec = part;
                date_count++;
                break;
            default:
                break;
            }
        }

        return date_tm;
    }

    FileParser::FileParser(std::string path) {
        file.open(path);
    }

    std::vector<Timer::TimerHandler*> FileParser::getCommands()  {
        std::vector<Timer::TimerHandler*> cmds;
        if (!file.is_open()) {
            throw std::invalid_argument("Невозможно открыть файл конфигурации");
        } else {
            if (file.peek() == EOF) {
                throw std::invalid_argument("Файл конфигурации пуст");
            }

            std::string line, transform_line;
            std::vector<std::string> parts;
            bool read_last = false;
            
            while (true) {
                if (!read_last) {
                    if (file.eof()) {
                        break;
                    }
                    std::getline(file, line);
                } else {
                    read_last = false;
                }

                transform_line = line;

                std::transform(transform_line.begin(), transform_line.end(), transform_line.begin(), ::tolower);

                parts = get_str_parts(transform_line);

                if (parts.size() != 3) {

                    std::for_each(cmds.begin(), cmds.end(), [](Timer::TimerHandler* t) {
                        delete t;
                    });

                    throw std::invalid_argument("Некорректная команда: " + line);
                }

                if ("sql" == parts[0] || "dump" == parts[0]) { // cmd
                    
                    std::tm date_start = parse_date(parts[1]);
                    if (date_start.tm_year > 1900) {
                        date_start.tm_year -= 1900;
                    }
                    if (date_start.tm_mon > 0) {
                        date_start.tm_mon--;
                    }

                    std::tm date_period = parse_date(parts[2]);
                    date_period.tm_year += 70;
                    if (date_period.tm_mon > 0) {
                        date_period.tm_mon--;
                    }
                    date_period.tm_mday++;

                    if ("sql" == parts[0]) { // sql cmd

                        Timer::TimerHandlerSql* sql_cmd = new Timer::TimerHandlerSql(date_start, date_period);

                        for (std::getline(file, line); ; std::getline(file, line)) {

                            transform_line = line;

                            std::transform(transform_line.begin(), transform_line.end(), transform_line.begin(), ::tolower);

                            parts = get_str_parts(transform_line);

                            if (parts[0] == "sql" || parts[0] == "dump") {
                                read_last = true;

                                break;
                            }

                            sql_cmd->add_sql_cmd(line);

                            if (file.eof()) {
                                break;
                            }
                        }

                        cmds.push_back(sql_cmd);
                    } else { // dump cmd
                        Timer::TimerHandlerDump* dump_cmd = new Timer::TimerHandlerDump(date_start, date_period);

                        cmds.push_back(dump_cmd);
                    }
                }
            }
        }

        return cmds;
    }

    std::vector<std::string> FileParser::get_str_parts(std::string& str) {
        std::vector<std::string> parts;
        std::string part;

        std::stringstream stline(str);

        while (std::getline(stline, part, ' ')) {
            parts.push_back(part);
        }

        return parts;
    }

    FileParser::~FileParser() {
        file.close();
    }
} // namespace FileParser
