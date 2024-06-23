#include <iostream>
#include <string>
#include <vector>
#include "prompt.h"
#include "parse.h"

void execute_line(const std::string & str) {
    if (str.empty() == true || str == "exit") {
        return;
    }
    std::vector<std::vector<std::string>> command_vec = parse_pipe(str);
    for (int i = 0; i < command_vec.size(); i++) {
        for (int j = 0; j < command_vec[i].size(); j++) {
            std::cout << command_vec[i][j] << std::endl;
        }
    }
}

int main() {
    while (true) {
        std::cout << get_prompt_info();
        std::string str = {};
        std::getline(std::cin, str);
        execute_line(str);
    }
    return 0;
}