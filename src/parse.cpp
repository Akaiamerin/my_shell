#include <vector>
#include <string>
#include <sstream>
#include "parse.h"

std::vector<std::string> parse_command(const std::string & str) {
    std::istringstream iss(str);
    std::vector<std::string> arg_vec = {};
    std::string arg = {};
    while (iss >> arg) {
        arg_vec.push_back(arg);
    }
    return arg_vec;
}

std::vector<std::vector<std::string>> parse_pipe(const std::string & str) {
    std::istringstream iss(str);
    std::vector<std::vector<std::string>> command_vec = {};
    std::string segment = {};
    while (std::getline(iss, segment, '|')) {
        std::vector<std::string> command = parse_command(segment);
        command_vec.push_back(command);
    }
    return command_vec;
}