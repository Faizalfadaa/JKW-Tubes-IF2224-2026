#include "stackinterpreter.hpp"

void StackInterpreter::validateInstructionPointer(int target) const {
    if (target < 0 || target >= static_cast<int>(code.size())){
        throw InterpreterRuntimeError("InvalidJumpTarget: target " + std::to_string(target) + " di luar intermediate code");
    }
}

void StackInterpreter::push(const Value& value){
    if (stack.size() >= maxStackSize){
        throw InterpreterRuntimeError("StackOverflow: stack melebihi batas interpreter");
    }
    stack.push_back(value);
}

Value StackInterpreter::pop(){
    if (stack.empty()){
        throw InterpreterRuntimeError("StackUnderflow: pop pada stack kosong");
    }
    Value value = stack.back();
    stack.pop_back();

    return value;
}

Value StackInterpreter::load(int address) const {
    if (address < 0 || address >= static_cast<int>(stack.size())){
        throw InterpreterRuntimeError("IndexOutOfBoundsException: address " + std::to_string(address) + " tidak ada di stack");
    }

    return stack[address];
}

void StackInterpreter::store(int address, const Value& value){
    if (address < 0 || address >= static_cast<int>(stack.size())){
        throw InterpreterRuntimeError("IndexOutOfBoundsException: address " + std::to_string(address) + " tidak ada di stack");
    }

    stack[address] = value;
}

void StackInterpreter::ensureInt32(long long value){
    if (value > std::numeric_limits<int32_t>::max()) {
        throw InterpreterRuntimeError("OverflowError: hasil integer melebihi batas 32-bit");
    }
    if (value < std::numeric_limits<int32_t>::min()) {
        throw InterpreterRuntimeError("UnderflowError: hasil integer di bawah batas 32-bit");
    }
}

Value StackInterpreter::numericBinary(const Value& left, const Value& right, int operation){
    if (!left.isNumeric() || !right.isNumeric()){
        if (operation == 2){
            return Value::string(left.toOutputString() + right.toOutputString());
        }
        throw InterpreterRuntimeError("operasi aritmatika membutuhkan operand numerik");
    }

    bool resultIsReal = left.type == BaseType::REAL || right.type == BaseType::REAL;
    if (operation == 5 && resultIsReal){
        double divisor = right.asReal();
        if (std::fabs(divisor) <= 1e-12) throw InterpreterRuntimeError("division by zero");
        return Value::real(left.asReal() / divisor);
    }

    if (resultIsReal && operation != 6){
        double a = left.asReal();
        double b = right.asReal();
        switch (operation){
            case 2: return Value::real(a + b);
            case 3: return Value::real(a - b);
            case 4: return Value::real(a * b);
            case 5:
                if (std::fabs(b) <= 1e-12) throw InterpreterRuntimeError("division by zero");
                return Value::real(a / b);
            default: break;
        }
    }

    long long a = left.asInteger();
    long long b = right.asInteger();
    long long result = 0;
    switch (operation){
        case 2:
            result = a + b;
            break;
        case 3:
            result = a - b;
            break;
        case 4:
            result = a * b;
            break;
        case 5:
            if (b == 0) throw InterpreterRuntimeError("division by zero");
            result = a / b;
            break;
        case 6:
            if (b == 0) throw InterpreterRuntimeError("modulo by zero");
            result = a % b;
            break;
        default:
            throw InterpreterRuntimeError("operasi numerik tidak dikenali");
    }

    ensureInt32(result);
    return Value::integer(result);
}

Value StackInterpreter::compareValues(const Value& left, const Value& right, int operation){
    bool result = false;
    if (left.isNumeric() && right.isNumeric()){
        double a = left.asReal();
        double b = right.asReal();
        switch (operation){
            case 7:
                result = std::fabs(a - b) <= 1e-12; break;
            case 8:
                result = std::fabs(a - b) > 1e-12; break;
            case 9:
                result = a < b; break;
            case 10:
                result = a >= b; break;
            case 11:
                result = a > b; break;
            case 12:
                result = a <= b; break;
            default:
                break;
        }
    }
    else{
        std::string a = left.toOutputString();
        std::string b = right.toOutputString();
        switch (operation){
            case 7:
                result = a == b;
                break;
            case 8:
                result = a != b;
                break;
            case 9:
                result = a < b;
                break;
            case 10:
                result = a >= b;
                break;
            case 11:
                result = a > b;
                break;
            case 12:
                result = a <= b;
                break;
            default:
                break;
        }
    }

    return Value::boolean(result);
}

void StackInterpreter::executeOperation(int operation){
    if (operation == 1){
        Value value = pop();
        if (value.type == BaseType::REAL){
            push(Value::real(-value.realValue));
        }
        else{
            long long result = -value.asInteger();
            ensureInt32(result);
            push(Value::integer(result));
        }

        return;
    }

    if (operation >= 2 && operation <= 6){
        Value right = pop();
        Value left = pop();
        push(numericBinary(left, right, operation));
        return;
    }

    if (operation >= 7 && operation <= 12){
        Value right = pop();
        Value left = pop();
        push(compareValues(left, right, operation));
        return;
    }

    if (operation == 13 || operation == 14){
        Value value = pop();
        output << value.toOutputString();
        if (operation == 14) output << "\n";
        return;
    }

    if (operation == 15 || operation == 16){
        Value right = pop();
        Value left = pop();
        bool result = (operation == 15) ? (left.asBoolean() && right.asBoolean()) : (left.asBoolean() || right.asBoolean());
        push(Value::boolean(result));
        return;
    }

    if (operation == 17){
        Value value = pop();
        push(Value::boolean(!value.asBoolean()));
        return;
    }

    throw InterpreterRuntimeError("OPR tidak dikenal: " + std::to_string(operation));
}

std::string StackInterpreter::run() {
    stack.clear();
    output.str("");
    output.clear();
    ip = 0;

    while (ip >= 0 && ip < static_cast<int>(code.size())){
        const Instruction& instruction = code[ip];
        switch (instruction.op){
            case OpCode::INT_:
                if (instruction.operand < 0) throw InterpreterRuntimeError("ukuran INT negatif");
                stack.resize(static_cast<size_t>(instruction.operand), Value::integer(0));
                ++ip;
                break;
            case OpCode::LIT:
                push(instruction.literal);
                ++ip;
                break;
            case OpCode::LOD:
                push(load(instruction.operand));
                ++ip;
                break;
            case OpCode::STO: {
                Value value = pop();
                store(instruction.operand, value);
                ++ip;
                break;
            }
            case OpCode::JMP:
                validateInstructionPointer(instruction.operand);
                ip = instruction.operand;
                break;
            case OpCode::JPC: {
                Value condition = pop();
                if (!condition.asBoolean()){
                    validateInstructionPointer(instruction.operand);
                    ip = instruction.operand;
                }
                else{
                    ++ip;
                }
                break;
            }
            case OpCode::OPR:
                executeOperation(instruction.operand);
                ++ip;
                break;
            case OpCode::CAL:
                throw InterpreterRuntimeError("CAL/function call belum diimplementasikan di interpreter sederhana ini");
            case OpCode::RET:
                return output.str();
        }
    }

    return output.str();
}