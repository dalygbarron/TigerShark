#define CATCH_CONFIG_MAIN
#include "test.hh"
#include <iostream>
#include <spdlog/spdlog.h>

char const *tempFile(char const *content) {
    char const *tmp = std::tmpnam(NULL);
    if (!tmp) {
        spdlog::error("Can't make a temporary file on this system.");
        return NULL;
    }
    std::ofstream file;
    file.open(tmp);
    file << content;
    file.close();
    return tmp;
}
