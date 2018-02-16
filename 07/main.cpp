#include <iostream>
#include <string>
#include <fstream>

#include "parser.h"
#include "command.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cout << "source vm file is missed\n";
        return -1;
    }

    std::string src(argv[1]);
    size_t dotPos = src.find('.');
    if (dotPos == std::string::npos) {
        std::cout << "file must be of vm extension\n";
        return -1;
    }

    std::string baseName = src.substr(0, dotPos);
    std::string ext = src.substr(dotPos + 1);
    if (ext != "vm") {
        std::cout << "file must be of vm extension\n";
    }
    std::fstream input(src, std::ios_base::in);
    Parser parser(input);
    while (parser.hasNext()) {
        std::cout << parser.command();
    }
    return 0;
}