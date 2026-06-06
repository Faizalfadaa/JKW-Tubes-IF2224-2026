#pragma once

#include <vector>
#include <ostream>
#include <cstdint>

#include "instruction.hpp"
#include "value.hpp"

class StackInterpreter {
private:
    std::vector<Instruction> code;
    std::vector<Value> stack;
    std::ostringstream output;
    int ip = 0;
    const size_t maxStackSize = 100000;

    void validateInstructionPointer(int target) const;

    void push(const Value& value);

    Value pop();

    Value load(int address) const;

    void store(int address, const Value& value);

    static void ensureInt32(long long value);

    static Value numericBinary(const Value& left, const Value& right, int operation);

    static Value compareValues(const Value& left, const Value& right, int operation);

    void executeOperation(int operation);

public:
    StackInterpreter(std::vector<Instruction> instructions) : code(std::move(instructions)) {}

    std::string run();
};