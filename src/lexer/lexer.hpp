#pragma once

#include "reader.hpp"
#include "token.hpp"
#include "unordered_map"

enum class State{
    //Error State
    UNKNOWN,

    //Start State
    START,

    //Angka
    INTCON,
    INTDOT,
    REALCON,
    //End symbol = !angka

    //String dan Char
    OPENQUOTE,
    CHARCON,
    STRING,
    INQUOTE,
    CLOSEQUOTE,
    //End symbol = '

    //Biner Character Symbol
    EQUALSIGN,
    EQL,
    LSS,
    LEQ,
    NEQ,
    GTR,
    GEQ,
    COLON,
    BECOMES,

    //Single Character Symbol
    PLUS,
    MINUS,
    TIMES,
    RDIV,
    LPARENT,
    RPARENT,
    LBRACK,
    RBRACK,
    COMMA,
    SEMICOLON,
    PERIOD,

    //Huruf
    ANDSY,
    ARRAYSY,
    BEGINSY,
    CASESY,
    CONSTSY,
    IDIV,
    DOSY,
    DOWNTOSY,
    ELSESY,
    ENDSY,
    FORSY,
    FUNCTIONSY,
    IFSY,
    IMOD,
    NOTSY,
    OFSY,
    ORSY,
    PROCEDURESY,
    PROGRAMSY,
    RECORDSY,
    REPEATSY,
    THENSY,
    TOSY,
    TYPESY,
    UNTILSY,
    VARSY,
    WHILESY,
    IDENT,

    //Comment
    OPENCUR,
    COMMENT,
    CLOSECUR,
    //TODO: State for (*...*) comment style
};

class Lexer{
private:
    State state;
    std::string lexeme;
public:
    Token processChar(char c);
    Lexer();
};