#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sys/wait.h>
#include <fcntl.h>
#include "handle.h"
#include "command.h"

//处理重定向
void handle_redirection(std::vector<std::string> & arg_vec) {
    int input_fd = -1; //输入文件描述符
    int output_fd = -1; //输出文件描述符
    for (int i = 0; i < arg_vec.size(); i++) {
        //输入重定向
        if (arg_vec[i] == "<") {
            input_fd = open(arg_vec[i + 1].c_str(), O_RDONLY);
            if (input_fd == -1) {
                std::cerr << std::strerror(errno) << std::endl;
                return;
            }
            arg_vec.erase(arg_vec.begin() + i, arg_vec.begin() + i + 2);
            i--;
        }
        //输出重定向
        else if (arg_vec[i] == ">") {
            output_fd = open(arg_vec[i + 1].c_str(), O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (output_fd == -1) {
                std::cerr << std::strerror(errno) << std::endl;
                return;
            }
            arg_vec.erase(arg_vec.begin() + i, arg_vec.begin() + i + 2);
            i--;
        }
        //Here Document 重定向
        else if (arg_vec[i] == "<<") {
            std::string end_flag = arg_vec[i + 1];
            std::string content = {};
            std::string line = {};
            while (true) {
                std::cout << "> ";
                std::getline(std::cin, line);
                if (line == end_flag) {
                    break;
                }
                content += line + "\n";
            }
            int fd[2] = {};
            if (pipe(fd) == -1) {
                std::cerr << std::strerror(errno) << std::endl;
                return;
            }
            write(fd[1], content.c_str(), content.size());
            input_fd = fd[0];
            close(fd[1]);
            arg_vec.erase(arg_vec.begin() + i, arg_vec.begin() + i + 2);
            i--;
        }
        //追加输出重定向
        else if (arg_vec[i] == ">>") {
            output_fd = open(arg_vec[i + 1].c_str(), O_CREAT | O_WRONLY | O_APPEND, 0644);
            if (output_fd == -1) {
                std::cerr << std::strerror(errno) << std::endl;
                return;
            }
            arg_vec.erase(arg_vec.begin() + i, arg_vec.begin() + i + 2);
            i--;
        }
    }
    //重定向标准输入
    if (input_fd != -1) {
        dup2(input_fd, STDIN_FILENO);
        close(input_fd);
    }
    //重定向标准输出
    if (output_fd != -1) {
        dup2(output_fd, STDOUT_FILENO);
        close(output_fd);
    }
}

//处理管道
void handle_pipe(std::vector<std::vector<std::string>> & command_vec) {
    std::vector<int> pipe_fd_vec(2 * (command_vec.size() - 1), 0); //管道文件描述符
    for (int i = 0; i < pipe_fd_vec.size(); i++) {
        if (pipe(pipe_fd_vec.data() + 2 * i) == -1) {
            std::cerr << std::strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    for (int i = 0; i < command_vec.size(); i++) {
        pid_t pid = fork();
        if (pid == -1) {
            std::cerr << std::strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }
        else if (pid == 0) {
            //将当前命令的标准输入重定向到前一个命令的输出
            if (i > 0) {
                dup2(pipe_fd_vec[2 * (i - 1)], STDIN_FILENO);
            }
            //将当前命令的标准输出重定向到后一个命令的输入
            if (i < command_vec.size() - 1) {
                dup2(pipe_fd_vec[2 * i + 1], STDOUT_FILENO);
            }
            for (int j = 0; j < pipe_fd_vec.size(); j++) {
                close(pipe_fd_vec[j]);
            }
            std::vector<std::string> arg_vec = command_vec[i];
            handle_redirection(arg_vec);
            run_command(arg_vec);
        }
    }
    for (int i = 0; i < pipe_fd_vec.size(); i++) {
        close(pipe_fd_vec[i]);
    }
    for (int i = 0; i < command_vec.size(); i++) {
        wait(nullptr);
    }
}