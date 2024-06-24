#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sys/wait.h>
#include <fcntl.h>
#include "prompt.h"
#include "parse.h"
#include "handle.h"
#include "command.h"

//执行命令行字符串
void execute_line(const std::string & str) {
    if (str.empty() == true) {
        return;
    }
    //exit，退出程序
    else if (str == "exit") {
        std::exit(EXIT_SUCCESS);
    }
    std::vector<std::vector<std::string>> command_vec = parse_pipe(str);
    // for (int i = 0; i < command_vec.size(); i++) {
    //     for (int j = 0; j < command_vec[i].size(); j++) {
    //         std::cout << command_vec[i][j] << std::endl;
    //     }
    // }
    //一个命令
    if (command_vec.size() == 1) {
        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << std::strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }
        //子进程执行命令
        else if (pid == 0) {
            std::vector<std::string> arg_vec = command_vec[0];
            handle_redirection(arg_vec);
            run_command(arg_vec);
        }
        //父进程等待子进程结束
        else {
            wait(nullptr);
        }
    }
    //有多个命令
    else {
        handle_pipe(command_vec);
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