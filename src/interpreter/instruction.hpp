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

std::string instructionToString(const Instruction& instruction, int index);

std::string instructionsToString(const std::vector<Instruction>& code);