#ifndef FILE_PARSER_H
#define FILE_PARSER_H

#include <string>
#include <fstream>
#include <vector>
#include <exception>

#include "../TimerHandler/TimerHandler.h"

namespace FileParser
{
    class FileParser {
        std::ifstream file;
        std::vector<std::string> get_str_parts(std::string& str);
    public:
        FileParser(std::string path);
        std::vector<Timer::TimerHandler*> getCommands();
        virtual ~FileParser();
    };

    std::tm parse_date(const std::string& date);
    // void print_date(const std::tm& date, std::fstream& file);
} // namespace FileParser


#endif