#pragma once
#include <iosfwd>
#include <vector>
#include <string>
#include <memory>

#include "command.h"

class Parser {
public:
    Parser(std::istream &is);
    std::unique_ptr<Command> command();
    bool hasNext() const;

private:
    std::unique_ptr<Command> parsePush(const std::vector<std::string> &tokens);
    std::unique_ptr<Command> parsePop(const std::vector<std::string> &tokens);

    std::istream &stream;
    bool has_next;
};