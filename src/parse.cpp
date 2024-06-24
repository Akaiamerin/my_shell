#include <string>
#include <vector>
#include <sstream>
#include "parse.h"

//解析命令行字符串，将其分割成单个命令参数
std::vector<std::string> parse_command(const std::string & str) {
    std::istringstream iss(str);
    std::vector<std::string> arg_vec = {};
    std::string arg = {};
    while (iss >> arg) {
        arg_vec.push_back(arg);
    }
    return arg_vec;
}

//解析包含管道的命令行字符串，将其分割成多个命令
std::vector<std::vector<std::string>> parse_pipe(const std::string & str) {
    std::istringstream iss(str);
    std::vector<std::vector<std::string>> command_vec = {};
    std::string segment = {};
    //管道符号 | 分割
    while (std::getline(iss, segment, '|')) {
        std::vector<std::string> command = parse_command(segment);
        command_vec.push_back(command);
    }
    return command_vec;
}