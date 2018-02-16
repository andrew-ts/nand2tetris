#pragma once
#include <istream>

#include "command.h"

class Parser {
public:
    Parser(std::istream &is);
    Command command();
    bool hasNext() const;

private:
    std::istream &stream;
    bool has_next;
};