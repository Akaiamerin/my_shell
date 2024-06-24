#include <string>
#include <unistd.h>
#include <pwd.h>
#include "prompt.h"

//获取系统信息 "主机名@用户名:当前目录$ "
std::string get_prompt_info() {
    char hostname[256] = {};
    gethostname(hostname, sizeof(hostname));
    char * username = getpwuid(getuid())->pw_name;
    char cwd[1024] = {};
    getcwd(cwd, sizeof(cwd));
    return std::string(username) + "@" + std::string(hostname) + ":" + std::string(cwd) + "$ ";
}