#include "parser.h"

Parser::Parser(std::istream &is) : stream(is) {
    has_next = !stream.eof();
}

Command Parser::command() {
    std::string line;
    has_next = std::getline(stream, line);
    Command c(std::move(line));
    return c;
}

bool Parser::hasNext() const {
    return has_next;
}
