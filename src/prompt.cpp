#include <unistd.h>
#include <pwd.h>
#include <string>
#include "prompt.h"

std::string get_prompt_info() {
    char hostname[256] = {};
    gethostname(hostname, sizeof(hostname));
    char * username = getpwuid(getuid())->pw_name;
    char cwd[1024] = {};
    getcwd(cwd, sizeof(cwd));
    return std::string(username) + "@" + std::string(hostname) + ":" + std::string(cwd) + "$ ";
}