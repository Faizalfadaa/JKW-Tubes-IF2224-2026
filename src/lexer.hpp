#pragma once

#include "reader.hpp"

class Lexer{
private:
    int state;
    std::string buffer;
public:
    void process(char c);
};