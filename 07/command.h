#pragma once

#include <ostream>
#include <string>

class Command {
public:
Command(std::string c) : command(c) {}

friend std::ostream& operator << (std::ostream& os, const Command& c) {
    os << c.command << '\n';
    return os;
}

private:
std::string command;
};