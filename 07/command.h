#pragma once

#include <ostream>
#include <string>

class Command {
public:

void toStream(std::ostream& os) const { print(os); };

private:
virtual void print(std::ostream &os) const = 0;
};

class DumpCommand : public Command {
void print(std::ostream &os) const override {
}
};

enum class Segment {
    local_ = 0,
    argument_ = 1,
    this_ = 2,
    that_ = 3,
};

class PushConstantCommand : public Command {
public:
PushConstantCommand(int c) : constant(c) {}

private:
void print(std::ostream& os) const override {
    // @constant
    // D = A
    // @SP
    // A = M
    // M = D
    // @SP
    // M = M + 1
    os << "// push constant " << constant << '\n';
    os << "@" << constant << '\n';
    os << "D = A" << '\n';
    os << "@SP\n";
    os << "A = M\n";
    os << "M = D\n";
    os << "@SP\n";
    os << "M = M + 1" << std::endl;
}

int constant;
};

class PushSegmentCommand : public Command {
public:
PushSegmentCommand(Segment s, int c) : segment(s), constant(c) {}

private:
void print(std::ostream& os) const override {
    static const char* segmentsNames[] = { "LCL", "ARG", "THIS", "THAT" };

    // @constant
    // D = A
    // @segment
    // A = A + D
    // D = M
    // @SP
    // A = M
    // M = D
    // @SP
    // M = M + 1
    os << "// push " << segmentsNames[static_cast<int>(segment)] << " " <<constant << '\n'
        << "@" << constant << "\n"
        << "D = A\n"
        << "@" << segmentsNames[static_cast<int>(segment)] << '\n'
        << "A = M + D\n"
        << "D = M\n"
        << "@SP\n"
        << "A = M\n"
        << "M = D\n"
        << "@SP\n"
        << "M = M + 1"
        << std::endl;
}

Segment segment;
int constant;
};

class PopSegmentCommand : public Command {
public:
PopSegmentCommand(Segment s, int c) : segment(s), constant(c) {}

private:
void print(std::ostream& os) const override {
    static const char* segmentsNames[] = { "LCL", "ARG", "THIS", "THAT" };
    // @constant
    // D = A
    // @segment
    // D = M + D
    // @adr
    // M = D
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @adr
    // A = M
    // M = D
    os << "// pop " << segmentsNames[static_cast<int>(segment)] << " " <<constant << '\n'
        << "@" << constant << "\n"
        << "D = A\n"
        << "@" << segmentsNames[static_cast<int>(segment)] << '\n'
        << "D = M + D\n"
        << "@adr\n"
        << "M = D\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@adr\n"
        << "A = M\n"
        << "M = D" << std::endl;
}

Segment segment;
int constant;
};

class PushTempCommand : public Command {
public:
PushTempCommand(int c) : constant(c) {}

private:
void print(std::ostream& os) const override {
    // @TEMP{constant}
    // D = M
    // @SP
    // A = M
    // M = D
    // @SP
    // M = M + 1
    os << "// push temp " << constant << '\n';
    os << "@" << (5 + constant) << '\n';
    os << "D = M" << '\n';
    os << "@SP\n";
    os << "A = M\n";
    os << "M = D\n";
    os << "@SP\n";
    os << "M = M + 1" << std::endl;
}
int constant;
};

class PopTempCommand : public Command {
public:
PopTempCommand(int c) : constant(c) {}

private:
void print(std::ostream& os) const override {
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @TEMP{constant}
    // M = D
    os << "// pop temp " << constant << '\n'
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@" << (5 + constant) << "\n"
        << "M = D" << std::endl;
}
int constant;
};

class PushPointerCommand : public Command {
public:
PushPointerCommand(int c) : constant(c) {}

private:
void print(std::ostream& os) const override {
    std::string segment = "THIS";
    if (constant == 1) {
        segment = "THAT";
    }

    // @segment
    // D = M
    // @SP
    // A = M
    // M = D
    // @SP
    // M = M + 1
    os << "// push pointer " << constant << '\n'
        << "@" << segment << "\n"
        << "D = M\n"
        << "@SP\n"
        << "A = M\n"
        << "M = D\n"
        << "@SP\n"
        << "M = M + 1" << std::endl;
}
int constant;
};

class PopPointerCommand : public Command {
public:
PopPointerCommand(int c) : constant(c) {}

private:
void print(std::ostream& os) const override {
    std::string segment = "THIS";
    if (constant == 1) {
        segment = "THAT";
    }

    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @segment
    // M = D
    os << "// pop pointer " << constant << '\n'
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@" << segment << "\n"
        << "M = D" << std::endl;
}
int constant;
};

class PushStaticCommand : public Command {
public:
PushStaticCommand(const std::string &name, int c) : constant(c), label(name) {}

private:
void print(std::ostream& os) const override {
    // @label.constant
    // D = M
    // @SP
    // A = M
    // M = D
    // @SP
    // M = M + 1
    os << "// push static " << constant << '\n'
        << "@" << label << "." << constant << "\n"
        << "D = M\n"
        << "@SP\n"
        << "A = M\n"
        << "M = D\n"
        << "@SP\n"
        << "M = M + 1" << std::endl;
}
int constant;
std::string label;
};

class PopStaticCommand : public Command {
public:
PopStaticCommand(const std::string& name, int c) : constant(c), label(name) {}

private:
void print(std::ostream& os) const override {
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @label.constant
    // M = D
    os << "// pop pointer " << constant << '\n'
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@" << label << "." << constant << "\n"
        << "M = D" << std::endl;
}
int constant;
std::string label;
};

class AddCommand : public Command {
private:
void print(std::ostream& os) const override {
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @SP
    // M = M - 1
    // A = M
    // M = M + D
    // @SP
    // M = M + 1
    os << "// add\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "M = M + D\n"
        << "@SP\n"
        << "M = M + 1" << std::endl;
}
};

class SubCommand : public Command {
private:
void print(std::ostream& os) const override {
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @SP
    // M = M - 1
    // A = M
    // M = M - D
    // @SP
    // M = M + 1
    os << "// sub\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "M = M - D\n"
        << "@SP\n"
        << "M = M + 1" << std::endl;
}
};

class NegCommand : public Command {
private:
void print(std::ostream& os) const override {
    // @SP
    // M = M - 1
    // A = M
    // M = -M
    // @SP
    // M = M + 1
    os << "// neg\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "M = -M\n"
        << "@SP\n"
        << "M = M + 1" << std::endl;
}
};

class AndCommand : public Command {
private:
void print(std::ostream& os) const override {
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @SP
    // M = M - 1
    // A = M
    // M = M & D
    // @SP
    // M = M + 1
    os << "// and\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "M = M & D\n"
        << "@SP\n"
        << "M = M + 1" << std::endl;
}
};

class OrCommand : public Command {
private:
void print(std::ostream& os) const override {
// @SP
    // M = M - 1
    // A = M
    // D = M
    // @SP
    // M = M - 1
    // A = M
    // M = M | D
    // @SP
    // M = M + 1
    os << "// or\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "M = M | D\n"
        << "@SP\n"
        << "M = M + 1" << std::endl;
}
};

class NotCommand : public Command {
private:
void print(std::ostream& os) const override {
    // @SP
    // M = M - 1
    // A = M
    // M = !M
    // @SP
    // M = M + 1
    os << "// neg\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "M = !M\n"
        << "@SP\n"
        << "M = M + 1" << std::endl;
}
};

class EqCommand : public Command {
private:
void print(std::ostream& os) const override {
    static int count = 0;
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @SP
    // M = M - 1
    // A = M
    // MD = M - D
    // @EQ
    // D; JEQ
    // @SP
    // A = M
    // M = -1
    // (EQ)
    // @SP
    // M = M + 1
    os << "// eq\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M - D\n"
        << "M = -1\n"
        << "@EQ" << count << "\n"
        << "D; JEQ\n"
        << "@SP\n"
        << "A = M\n"
        << "M = 0\n"
        << "(EQ" << count << ")\n"
        << "@SP\n"
        << "M = M + 1\n"
        << std::endl;
        ++count;
}
};

class LTCommand : public Command {
private:
void print(std::ostream& os) const override {
    static int count = 0;
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @SP
    // M = M - 1
    // A = M
    // D = M - D
    // M = 0
    // @LT
    // D; JGE
    // @SP
    // A = M
    // M = -1
    // (LT)
    // @SP
    // M = M + 1
    os << "// lt\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M - D\n"
        << "M = -1\n"
        << "@LT"<< count << "\n"
        << "D; JLT\n"
        << "@SP\n"
        << "A = M\n"
        << "M = 0\n"
        << "(LT" << count << ")\n"
        << "@SP\n"
        << "M = M + 1\n"
        << std::endl;
    ++count;
}
};

class GTCommand : public Command {
private:
void print(std::ostream& os) const override {
    static int count = 0;
    // @SP
    // M = M - 1
    // A = M
    // D = M
    // @SP
    // M = M - 1
    // A = M
    // D = M - D
    // M = 0
    // @GT
    // D; JGT
    // @SP
    // A = M
    // M = -1
    // (GT)
    // @SP
    // M = M + 1
    os << "// gt\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M\n"
        << "@SP\n"
        << "M = M - 1\n"
        << "A = M\n"
        << "D = M - D\n"
        << "M = -1\n"
        << "@GT"<< count << "\n"
        << "D; JGT\n"
        << "@SP\n"
        << "A = M\n"
        << "M = 0\n"
        << "(GT" << count << ")\n"
        << "@SP\n"
        << "M = M + 1\n"
        << std::endl;
    ++count;
}
};