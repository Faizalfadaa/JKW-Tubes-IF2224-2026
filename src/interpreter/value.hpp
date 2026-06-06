#pragma once

#include <cmath>

#include "../semantic/astnode.hpp"
#include "../utils/exception.hpp"

struct Value {
    BaseType type = BaseType::UNKNOWN;
    long long integerValue = 0;
    double realValue = 0.0;
    bool booleanValue = false;
    std::string stringValue;

    static Value integer(long long value) {
        Value v;
        v.type = BaseType::INTEGER;
        v.integerValue = value;
        v.realValue = static_cast<double>(value);
        return v;
    }

    static Value real(double value) {
        Value v;
        v.type = BaseType::REAL;
        v.realValue = value;
        v.integerValue = static_cast<long long>(value);
        return v;
    }

    static Value boolean(bool value) {
        Value v;
        v.type = BaseType::BOOLEAN;
        v.booleanValue = value;
        v.integerValue = value ? 1 : 0;
        v.realValue = value ? 1.0 : 0.0;
        return v;
    }

    static Value string(const std::string& value) {
        Value v;
        v.type = BaseType::STRING;
        v.stringValue = value;
        return v;
    }

    static Value character(const std::string& value) {
        Value v;
        v.type = BaseType::CHAR;
        v.stringValue = value;
        return v;
    }

    static Value unknown() {
        return Value{};
    }

    bool isNumeric() const {
        return type == BaseType::INTEGER || type == BaseType::REAL || type == BaseType::BOOLEAN;
    }

    double asReal() const {
        if (type == BaseType::REAL) return realValue;
        if (type == BaseType::INTEGER) return static_cast<double>(integerValue);
        if (type == BaseType::BOOLEAN) return booleanValue ? 1.0 : 0.0;
        throw InterpreterRuntimeError("value bukan numerik");
    }

    long long asInteger() const {
        if (type == BaseType::INTEGER) return integerValue;
        if (type == BaseType::BOOLEAN) return booleanValue ? 1 : 0;
        if (type == BaseType::REAL) return static_cast<long long>(realValue);
        throw InterpreterRuntimeError("value bukan integer");
    }

    bool asBoolean() const {
        if (type == BaseType::BOOLEAN) return booleanValue;
        if (type == BaseType::INTEGER) return integerValue != 0;
        if (type == BaseType::REAL) return std::fabs(realValue) > 1e-12;
        if (type == BaseType::STRING || type == BaseType::CHAR) return !stringValue.empty();
        return false;
    }

    std::string toOutputString() const {
        std::ostringstream oss;
        switch (type) {
            case BaseType::INTEGER:
                return std::to_string(integerValue);
            case BaseType::REAL:
                oss << std::setprecision(12) << realValue;
                return oss.str();
            case BaseType::BOOLEAN:
                return booleanValue ? "true" : "false";
            case BaseType::CHAR:
            case BaseType::STRING:
                return stringValue;
            case BaseType::UNKNOWN:
                return "0";
            default:
                return stringValue;
        }
    }

    std::string toCodeLiteral() const {
        switch (type) {
            case BaseType::STRING:
            case BaseType::CHAR:
                return "'" + stringValue + "'";
            case BaseType::BOOLEAN:
                return booleanValue ? "true" : "false";
            default:
                return toOutputString();
        }
    }
};