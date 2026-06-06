#pragma once

#include <string>

//Instruksi stack-machine sesuai spesifikasi
enum class OpCode { LIT, LOD, STO, CAL, INT_, JMP, JPC, OPR, RET };

std::string opcodeName(OpCode op) {
    switch (op) {   
        case OpCode::LIT:  return "LIT";
        case OpCode::LOD:  return "LOD";
        case OpCode::STO:  return "STO";
        case OpCode::CAL:  return "CAL";
        case OpCode::INT_: return "INT";
        case OpCode::JMP:  return "JMP";
        case OpCode::JPC:  return "JPC";
        case OpCode::OPR:  return "OPR";
        case OpCode::RET:  return "RET";
    }
}