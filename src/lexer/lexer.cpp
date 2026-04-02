#include "lexer.hpp"

using namespace std;

std::unordered_map<std::string, TokenType> keywordTable = {
    {"not", TokenType::NOTSY},
    {"and", TokenType::ANDSY},
    {"or", TokenType::ORSY},
    {"const", TokenType::CONSTSY},
    {"type", TokenType::TYPESY},
    {"var", TokenType::VARSY},
    {"function", TokenType::FUNCTIONSY},
    {"procedure", TokenType::PROCEDURESY},
    {"array", TokenType::ARRAYSY},
    {"record", TokenType::RECORDSY},
    {"program", TokenType::PROGRAMSY},
    {"begin", TokenType::BEGINSY},
    {"if", TokenType::IFSY},
    {"case", TokenType::CASESY},
    {"repeat", TokenType::REPEATSY},
    {"while", TokenType::WHILESY},
    {"for", TokenType::FORSY},
    {"end", TokenType::ENDSY},
    {"else", TokenType::ELSESY},
    {"until", TokenType::UNTILSY},
    {"of", TokenType::OFSY},
    {"do", TokenType::DOSY},
    {"to", TokenType::TOSY},
    {"downto", TokenType::DOWNTOSY},
    {"then", TokenType::THENSY},
};

Lexer::Lexer() : state(State::START), lexeme("") {}

Token Lexer::processChar(char c){
    //Untuk mempermudah pengecekan angka dan huruf
    bool isNumber = isdigit(c), isLetter = isalpha(c);

    //Proses karakter sesuai dengan state saat ini
    switch (state){
    //Initial state
        case State::START:
            this->lexeme = "";
            if (isLetter){
                state = State::IDENT;
            } else if (isNumber) {
                state = State::INTCON;
            }

            //lanjut
            break;

    //Case Letter
        case State::IDENT:
            if (std::isblank(c)){
                return Token(TokenType::IDENT, lexeme);
            }
        

    //Case Number
        case State::INTCON:
            if (isNumber) {
                state = State::INTCON;
            } else if (c == '.'){
                state = State::INTDOT;
            } else if (isalpha(c)) {
                state = State::UNKNOWN;
            } else if (isblank(c)) {
                return Token(TokenType::INTCON, lexeme);
            }

        case State::INTDOT:
            if(isNumber) {
                state = State::REALCON;
            } else {
                state = State::UNKNOWN;
            } 

        case State::REALCON:
            if(isNumber) {
                state = State::REALCON; 
            } else if (isLetter) {
                state = State::UNKNOWN;
            } else if (isblank(c)) {
                return Token(TokenType::REALCON, lexeme);
            }

    //Case String & Char
        // case State::something:
        //     if (something) {

        //     } else {
                
        //     }


    //Case Comment
        // case State::something:
        //     if (something) {

        //     } else {
                
        //     }


    //Case Binary Operator
        // case State::something:
        //     if (something) {

        //     } else {
                
        //     }


    //Case Single Character Symbol
        // case State::something:
        //     if (something) {

        //     } else {
                
        //     }
    }

    lexeme.append(1, c);
    return Token(TokenType::IDENT, lexeme);
}