#pragma once

#include "opcode.hpp"
#include "value.hpp"

struct Instruction {
    OpCode op = OpCode::RET;
    int level = 0;
    int operand = 0;
    Value literal;
    bool hasLiteral = false;

    Instruction() = default;

    Instruction(OpCode opCode, int instructionLevel, int instructionOperand):
        op(opCode), level(instructionLevel), operand(instructionOperand) {}

    Instruction(const Value& literalValue):
        op(OpCode::LIT), level(0), operand(0), literal(literalValue), hasLiteral(true) {}
};

std::string instructionToString(const Instruction& instruction, int index) {
    std::ostringstream oss;
    oss << index << " " << opcodeName(instruction.op);
    if (instruction.op == OpCode::RET) {
        return oss.str();
    }
    if (instruction.op == OpCode::LIT && instruction.hasLiteral) {
        oss << " " << instruction.level << " " << instruction.literal.toCodeLiteral();
    } else {
        oss << " " << instruction.level << " " << instruction.operand;
    }
    return oss.str();
}

std::string instructionsToString(const std::vector<Instruction>& code) {
    std::ostringstream oss;
    for (size_t i = 0; i < code.size(); ++i) {
        oss << instructionToString(code[i], static_cast<int>(i)) << "\n";
    }
    return oss.str();
}