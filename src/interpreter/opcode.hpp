#pragma once

#include <string>

//Instruksi stack-machine sesuai spesifikasi
enum class OpCode { LIT, LOD, STO, CAL, INT_, JMP, JPC, OPR, RET };

std::string opcodeName(OpCode op);