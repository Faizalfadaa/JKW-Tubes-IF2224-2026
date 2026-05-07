#pragma once

#include "reader.hpp"
#include "token.hpp"
#include "../utils/utils.hpp"
#include <unordered_map>
#include <queue>

enum class State{
    //Error State
    UNKNOWN,

    //Start State
    START,
    FINISH,
    DETERMINED,

    //Angka
    MINUS,
    INTCON,
    INTDOT,
    REALCON,
    //End symbol = !angka

    //String dan Char
    OPENQUOTE,
    CHARCON,
    STRING,
    CHARCLOSEQUOTE,
    STRINGCLOSEQUOTE,
    //End symbol = '

    //Biner Character Symbol
    EQUALSIGN,
    LSS,
    GTR,
    COLON,

    //Single Character Symbol
    //Removed

    //Huruf
    IDENT,

    //Comment
    OPENCUR,
    COMMENTCUR,
    LPARENT,
    COMMENTPARAST,
    COMMENTCLOSEAST
};

class Lexer{
private:
    Reader& reader;
    State state;
    std::string lexeme;
    std::queue<TokenType> tokenBuffer;

public:
    Token getNextToken();
    TokenType processChar(char c);
    Lexer(Reader& reader);
};