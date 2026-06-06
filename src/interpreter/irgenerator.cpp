#include "irgenerator.hpp"

int IntermediateCodeGenerator::emit(const Instruction& instruction) {
    code.push_back(instruction);
    return static_cast<int>(code.size()) - 1;
}

void IntermediateCodeGenerator::patchOperand(int index, int operand) {
    if (index < 0 || index >= static_cast<int>(code.size())) {
        throw InterpreterGenerateError("patch target intermediate code tidak valid");
    }

    code[index].operand = operand;
}

int IntermediateCodeGenerator::currentLine() const {
    return static_cast<int>(code.size());
}

std::string IntermediateCodeGenerator::variableName(ASTNode* node) {
    if (auto* var = dynamic_cast<VarNode*>(node)) {
        return toUpper(var->name);
    }

    if (auto* arr = dynamic_cast<ArrayAccessNode*>(node)) {
        std::string name = variableName(arr->arrayVar.get());

        for (auto& idx : arr->indices) {
            Value v = literalValue(idx.get());
            name += "[" + v.toOutputString() + "]";
        }

        return name;
    }

    if (auto* rec = dynamic_cast<RecordAccessNode*>(node)) {
        return variableName(rec->recordVar.get()) +
               "." +
               toUpper(rec->fieldName);
    }

    throw InterpreterGenerateError("target assignment tidak valid");
}

int IntermediateCodeGenerator::addressOf(const std::string& rawName) const {
    std::string name = toUpper(rawName);
    auto it = variableAddress.find(name);
    if (it == variableAddress.end()) {
        throw InterpreterGenerateError("alamat variabel tidak ditemukan: " + rawName);
    }

    return it->second;
}

void IntermediateCodeGenerator::collectDeclaration(ASTNode* node) {
    if (!node) return;

    if (auto* varDecl = dynamic_cast<VarDeclNode*>(node)) {
        for (const std::string& rawName : varDecl->varNames) {
            std::string name = toUpper(rawName);
            if (variableAddress.find(name) == variableAddress.end()) {
                variableAddress[name] = nextAddress++;
            }
        }
        return;
    }

    if (auto* constDecl = dynamic_cast<ConstDeclNode*>(node)) {
        constants[toUpper(constDecl->constName)] = literalValue(constDecl->value.get());
        return;
    }
}

Value IntermediateCodeGenerator::parseNumberLiteral(const NumberNode* node) {
    if (!node) return Value::unknown();
    try {
        if (node->isReal) {
            return Value::real(std::stod(node->value));
        }
        return Value::integer(std::stoll(node->value));
    } catch (...) {
        throw InterpreterGenerateError("literal angka tidak valid: " + node->value);
    }
}

Value IntermediateCodeGenerator::literalValue(ASTNode* node) {
    if (!node) return Value::unknown();

    if (auto* num = dynamic_cast<NumberNode*>(node)) {
        return parseNumberLiteral(num);
    }

    if (auto* str = dynamic_cast<StringNode*>(node)) {
        return Value::string(stripQuotes(str->value));
    }

    if (auto* ch = dynamic_cast<CharNode*>(node)) {
        return Value::character(stripQuotes(ch->value));
    }

    if (auto* var = dynamic_cast<VarNode*>(node)) {
        std::string name = toUpper(var->name);
        if (name == "TRUE") return Value::boolean(true);
        if (name == "FALSE") return Value::boolean(false);
        auto it = constants.find(name);
        if (it != constants.end()) return it->second;
    }

    throw InterpreterGenerateError("konstanta tidak dapat dievaluasi saat generate intermediate code");
}

int IntermediateCodeGenerator::operationCodeFor(const std::string& op) const {
    if (op == "+")  return 2;
    if (op == "-")  return 3;
    if (op == "*")  return 4;
    if (op == "/")  return 5;
    if (op == "div") return 5;
    if (op == "mod") return 6;
    if (op == "=")  return 7;
    if (op == "<>") return 8;
    if (op == "<")  return 9;
    if (op == ">=") return 10;
    if (op == ">")  return 11;
    if (op == "<=") return 12;
    if (op == "and") return 15;
    if (op == "or")  return 16;
    throw InterpreterGenerateError("operator tidak valid: " + op);
}

void IntermediateCodeGenerator::generateExpression(ASTNode* node) {
    if (!node) throw InterpreterGenerateError("ekspresi kosong");

    if (auto* number = dynamic_cast<NumberNode*>(node)) {
        emit(Instruction(parseNumberLiteral(number)));
        return;
    }

    if (auto* stringNode = dynamic_cast<StringNode*>(node)) {
        emit(Instruction(Value::string(stripQuotes(stringNode->value))));
        return;
    }

    if (auto* charNode = dynamic_cast<CharNode*>(node)) {
        emit(Instruction(Value::character(stripQuotes(charNode->value))));
        return;
    }

    if (auto* var = dynamic_cast<VarNode*>(node)) {
        std::string name = toUpper(var->name);
        if (name == "TRUE" || name == "FALSE") {
            emit(Instruction(Value::boolean(name == "TRUE")));
            return;
        }

        auto constIt = constants.find(name);
        if (constIt != constants.end()) {
            emit(Instruction(constIt->second));
            return;
        }

        emit(Instruction(OpCode::LOD, 0, addressOf(var->name)));
        return;
    }

    if (auto* binOp = dynamic_cast<BinOpNode*>(node)) {
        generateExpression(binOp->left.get());
        generateExpression(binOp->right.get());
        emit(Instruction(OpCode::OPR, 0, operationCodeFor(binOp->op)));
        return;
    }

    if (auto* unaryOp = dynamic_cast<UnaryOpNode*>(node)) {
        generateExpression(unaryOp->operand.get());
        if (unaryOp->op == "-") {
            emit(Instruction(OpCode::OPR, 0, 1));
        } else if (unaryOp->op == "not") {
            emit(Instruction(OpCode::OPR, 0, 17));
        }
        
        return;
    }

    if (auto* call = dynamic_cast<ProcCallNode*>(node)) {
        throw InterpreterGenerateError("terjadi kesalahan pada pemanggilan fungsi: " + call->procName); //TODO: 
    }

    if (auto* arr = dynamic_cast<ArrayAccessNode*>(node)) {
        std::string name = variableName(arr);
        emit(Instruction(OpCode::LOD, 0, addressOf(name)));
        return;
    }

    if (auto* rec = dynamic_cast<RecordAccessNode*>(node)) {
        emit(Instruction(OpCode::LOD, 0, addressOf(variableName(rec))));
        return;
    }

    throw InterpreterGenerateError("jenis ekspresi tidak valid");
}

void IntermediateCodeGenerator::generateStatement(ASTNode* node) {
    if (!node) return;

    if (auto* compound = dynamic_cast<CompoundNode*>(node)) {
        for (auto& stmt : compound->statements) {
            generateStatement(stmt.get());
        }
        return;
    }

    if (auto* assign = dynamic_cast<AssignNode*>(node)) {
        generateExpression(assign->value.get());
        emit(Instruction(OpCode::STO, 0, addressOf(variableName(assign->target.get()))));
        return;
    }

    if (auto* ifNode = dynamic_cast<IfNode*>(node)) {
        generateExpression(ifNode->condition.get());
        int jpcIndex = emit(Instruction(OpCode::JPC, 0, 0));
        generateStatement(ifNode->thenBlock.get());

        if (ifNode->elseBlock) {
            int jumpEnd = emit(Instruction(OpCode::JMP, 0, 0));
            patchOperand(jpcIndex, currentLine());
            generateStatement(ifNode->elseBlock.get());
            patchOperand(jumpEnd, currentLine());
        } else {
            patchOperand(jpcIndex, currentLine());
        }
        return;
    }

    if (auto* whileNode = dynamic_cast<WhileNode*>(node)) {
        int startLine = currentLine();
        generateExpression(whileNode->condition.get());
        int jpcIndex = emit(Instruction(OpCode::JPC, 0, 0));
        generateStatement(whileNode->loopBlock.get());
        emit(Instruction(OpCode::JMP, 0, startLine));
        patchOperand(jpcIndex, currentLine());
        return;
    }

    if (auto* repeatNode = dynamic_cast<RepeatNode*>(node)) {
        int startLine = currentLine();
        for (auto& stmt : repeatNode->statements) {
            generateStatement(stmt.get());
        }
        generateExpression(repeatNode->condition.get());
        emit(Instruction(OpCode::JPC, 0, startLine));
        return;
    }

    if (auto* forNode = dynamic_cast<ForNode*>(node)) {
        int iteratorAddress = addressOf(forNode->iteratorName);
        generateExpression(forNode->startValue.get());
        emit(Instruction(OpCode::STO, 0, iteratorAddress));

        int startLine = currentLine();
        emit(Instruction(OpCode::LOD, 0, iteratorAddress));
        generateExpression(forNode->endValue.get());
        emit(Instruction(OpCode::OPR, 0, forNode->isDownto ? 10 : 12)); // >= untuk downto, <= untuk to
        int jpcIndex = emit(Instruction(OpCode::JPC, 0, 0));

        generateStatement(forNode->loopBlock.get());

        emit(Instruction(OpCode::LOD, 0, iteratorAddress));
        emit(Instruction(Value::integer(1)));
        emit(Instruction(OpCode::OPR, 0, forNode->isDownto ? 3 : 2));
        emit(Instruction(OpCode::STO, 0, iteratorAddress));
        emit(Instruction(OpCode::JMP, 0, startLine));
        patchOperand(jpcIndex, currentLine());
        return;
    }

    if (auto* caseNode = dynamic_cast<CaseNode*>(node)) {
        std::vector<int> jumpToEnd;
        for (auto& casePtr : caseNode->cases) {
            auto* block = dynamic_cast<CaseBlockNode*>(casePtr.get());
            if (!block) continue;

            for (auto& constPtr : block->constants) {
                generateExpression(caseNode->condition.get());
                generateExpression(constPtr.get());
                emit(Instruction(OpCode::OPR, 0, 7));
                int nextCheck = emit(Instruction(OpCode::JPC, 0, 0));
                generateStatement(block->statement.get());
                jumpToEnd.push_back(emit(Instruction(OpCode::JMP, 0, 0)));
                patchOperand(nextCheck, currentLine());
            }
        }
        for (int jumpIndex : jumpToEnd) {
            patchOperand(jumpIndex, currentLine());
        }
        return;
    }

    if (auto* procCall = dynamic_cast<ProcCallNode*>(node)) {
        std::string name = toUpper(procCall->procName);
        if (name == "WRITE" || name == "WRITELN") {
            if (procCall->arguments.empty()) {
                emit(Instruction(Value::string("")));
                emit(Instruction(OpCode::OPR, 0, name == "WRITELN" ? 14 : 13));
                return;
            }

            for (size_t i = 0; i < procCall->arguments.size(); ++i) {
                generateExpression(procCall->arguments[i].get());
                bool isLast = i == procCall->arguments.size() - 1;
                emit(Instruction(OpCode::OPR, 0, (name == "WRITELN" && isLast) ? 14 : 13));
            }
            return;
        }

        throw InterpreterGenerateError("terjadi kesalahan saat pemanggilan fungsi/prosedur: " + procCall->procName); //TODO: 
    }

    throw InterpreterGenerateError("jenis statement tidak valid");
}

std::vector<Instruction> IntermediateCodeGenerator::generate(ASTNode* root) {
    code.clear();
    variableAddress.clear();
    constants.clear();
    nextAddress = 3;

    auto* program = dynamic_cast<ProgramNode*>(root);
    if (!program) {
        throw InterpreterGenerateError("root AST bukan ProgramNode");
    }

    for (auto& decl : program->declarations) {
        collectDeclaration(decl.get());
    }

    emit(Instruction(OpCode::INT_, 0, nextAddress));
    generateStatement(program->mainBlock.get());
    emit(Instruction(OpCode::RET, 0, 0));
    return code;
}

const std::map<std::string, int>& IntermediateCodeGenerator::getVariableAddressMap() const {
    return variableAddress;
}