#pragma once

#include "reader.hpp"
#include "token.hpp"
#include "unordered_map"

enum class State{
    //Error State
    UNKNOWN,

    //Start State
    START,
    FINISH,
    DETERMINED,

    //Angka
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
    COMMENTPAR,
    COMMENTPARAST,
    COMMENTCLOSEAST
};

class Lexer{
private:
    Reader& reader;
    State state;
    std::string lexeme;
public:
    Token getNextToken();
    TokenType processChar(char c);
    Lexer(Reader& reader);
};