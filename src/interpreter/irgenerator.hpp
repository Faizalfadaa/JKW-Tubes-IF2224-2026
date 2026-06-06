#pragma once

#include <vector>
#include <map>
#include <cstdint>
#include <string>

#include "instruction.hpp"
#include "../utils/utils.hpp"
#include "../utils/exception.hpp"

class IntermediateCodeGenerator {
private:
    std::vector<Instruction> code;
    std::map<std::string, int> variableAddress;
    std::map<std::string, Value> constants;
    int nextAddress = 3; // 0 static link, 1 dynamic link, 2 return address

    int emit(const Instruction& instruction);

    void patchOperand(int index, int operand);

    int currentLine() const;

    static std::string variableName(ASTNode* node);

    int addressOf(const std::string& rawName) const;

    void collectDeclaration(ASTNode* node);

    Value parseNumberLiteral(const NumberNode* node);

    Value literalValue(ASTNode* node);

    int operationCodeFor(const std::string& op) const;

    void generateExpression(ASTNode* node);

    void generateStatement(ASTNode* node);

public:
    std::vector<Instruction> generate(ASTNode* root);

    const std::map<std::string, int>& getVariableAddressMap() const;
};