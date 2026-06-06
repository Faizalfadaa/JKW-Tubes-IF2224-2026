#include "instruction.hpp"

//Fungsi untuk mengubah instruksi menjadi string
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

//Fungsi untuk mengubah rangkaian instruksi menjadi string
std::string instructionsToString(const std::vector<Instruction>& code) {
    std::ostringstream oss;
    for (size_t i = 0; i < code.size(); ++i) {
        oss << instructionToString(code[i], static_cast<int>(i)) << "\n";
    }
    return oss.str();
}