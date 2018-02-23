#include "parser.h"

#include <cassert>
#include <vector>
#include <iostream>
#include <sstream>

namespace {
bool starts_with(const std::string &s, const char* prefix) {
    auto pos = s.find(prefix);
    return pos == 0;
}

std::vector<std::string> split(const std::string& s, const std::string &delimiter, bool trimEmpty = true) {
    std::vector<std::string> result;
    size_t lastPos = 0;
    size_t pos = s.find_first_of(delimiter, lastPos);
    while (pos != std::string::npos) {
            std::string word = s.substr(lastPos, pos - lastPos);
            lastPos = pos + 1;
            pos = s.find_first_of(delimiter, lastPos);
            if (trimEmpty && word.empty()) continue;
            result.push_back(word);
    }
    std::string word = s.substr(lastPos);
    if (!word.empty() || !trimEmpty) {
        result.push_back(word);
    }
    return result;
}

}

Parser::Parser(std::istream &is) : stream(is) {
    has_next = !stream.eof();
}

std::unique_ptr<Command> Parser::command() {
    std::string line;
    has_next = !std::getline(stream, line).eof();

    auto tokens = split(line, " \n\r");

    if (tokens.empty()) {
        return std::make_unique<DumpCommand>();
    } else if (tokens.front() == "push") {
        return parsePush(tokens);
    } else if (tokens.front() == "pop") {
        return parsePop(tokens);
    } else if (tokens[0] == "add") {
        return std::make_unique<AddCommand>();
    } else if (tokens[0] == "sub") {
        return std::make_unique<SubCommand>();
    } else if (tokens[0] == "neg") {
        return std::make_unique<NegCommand>();
    } else if (tokens[0] == "and") {
        return std::make_unique<AndCommand>();
    } else if (tokens[0] == "or") {
        return std::make_unique<OrCommand>();
    } else if (tokens[0] == "not") {
        return std::make_unique<NotCommand>();
    } else if (tokens[0] == "eq") {
        return std::make_unique<EqCommand>();
    } else if (tokens[0] == "lt") {
        return std::make_unique<LTCommand>();
    } else if (tokens[0] == "gt") {
        return std::make_unique<GTCommand>();
    }

    return std::make_unique<DumpCommand>();
}

bool Parser::hasNext() const {
    return has_next;
}

std::unique_ptr<Command> Parser::parsePush(const std::vector<std::string> &tokens) {
    assert(tokens.size() == 3);
    assert(tokens.front() == "push");
    int param = std::stoi(tokens[2]);

    if (tokens[1] == "constant") {
        return std::make_unique<PushConstantCommand>(param);
    } else if (tokens[1] == "local") {
        return std::make_unique<PushSegmentCommand>(Segment::local_, param);
    } else if (tokens[1] == "argument") {
        return std::make_unique<PushSegmentCommand>(Segment::argument_, param);
    } else if (tokens[1] == "this") {
        return std::make_unique<PushSegmentCommand>(Segment::this_, param);
    } else if (tokens[1] == "that") {
        return std::make_unique<PushSegmentCommand>(Segment::that_, param);
    } else if (tokens[1] == "temp") {
        return std::make_unique<PushTempCommand>(param);
    }
    return std::make_unique<DumpCommand>();
}

std::unique_ptr<Command> Parser::parsePop(const std::vector<std::string> &tokens) {
    assert(tokens.size() == 3);
    assert(tokens.front() == "pop");
    int param = std::stoi(tokens[2]);

    if (tokens[1] == "local") {
        return std::make_unique<PopSegmentCommand>(Segment::local_, param);
    } else if (tokens[1] == "argument") {
        return std::make_unique<PopSegmentCommand>(Segment::argument_, param);
    } else if (tokens[1] == "this") {
        return std::make_unique<PopSegmentCommand>(Segment::this_, param);
    } else if (tokens[1] == "that") {
        return std::make_unique<PopSegmentCommand>(Segment::that_, param);
    } else if (tokens[1] == "temp") {
        return std::make_unique<PopTempCommand>(param);
    }
    return std::make_unique<DumpCommand>();
}
