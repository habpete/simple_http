#include "internal/service.h"

#include <iostream>
#include <string>

int main (int argc, char* argv[]) {
    /* if (argc <= 1) {
        std::cout << "empty string" << std::endl;
        return -1;
    }

    std::string filePath = argv[1]; */
    std::string filePath = "/Users/habpete/some_projects/simple_http/tests/test_curl.txt";
    std::cout << Execute(filePath) << std::endl;

    return 0;
}