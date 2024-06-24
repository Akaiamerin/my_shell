#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sys/wait.h>
#include "command.h"

//cd
void builtin_cd(const std::vector<std::string> & arg_vec) {
    std::string dir = {};
    //切换到根目录
    if (arg_vec.size() == 1) {
        dir = "/";
    }
    //切换到指定目录
    else {
        
        dir = arg_vec[1].c_str();
    }
    if (chdir(dir.c_str()) == -1) {
        std::cerr << std::strerror(errno) << std::endl;
    }
}

//外部命令
void external_command(const std::vector<std::string> & arg_vec) {
    pid_t pid = fork();
    if (pid == -1) {
        std::cerr << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    else if (pid == 0) {
        std::vector<char *> vec = {};
        for (int i = 0; i < arg_vec.size(); i++) {
            vec.push_back(const_cast<char *>(arg_vec[i].c_str()));
        }
        vec.push_back(nullptr);
        //使用 execvp 执行外部命令
        if (execvp(vec[0], vec.data()) == -1) {
            std::cerr << std::strerror(errno) << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    //父进程等待子进程结束
    else {
        int status = 0;
        waitpid(pid, &status, 0);
    }
}

//运行命令
void run_command(const std::vector<std::string> & arg_vec) {
    if (arg_vec.empty() == true) {
        return;
    }
    //cd
    else if (arg_vec[0] == "cd") {
        builtin_cd(arg_vec);
    }
    //外部命令
    else {
        external_command(arg_vec);
        std::exit(EXIT_SUCCESS);
    }
}