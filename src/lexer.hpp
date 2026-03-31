#pragma once

#include "reader.hpp"
#include "token.hpp"
#include "state.hpp"

class Lexer{
private:
    State state;
    std::string buffer;
public:
    Token process(char c);
};