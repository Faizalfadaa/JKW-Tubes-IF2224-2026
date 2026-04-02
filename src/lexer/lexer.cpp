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
            //Kosongkan buffer
            this->lexeme = "";

            if (isLetter){
                state = State::IDENT;
            } else if (isNumber) {
                state = State::INTCON;
            } else if (c == '=') {
                state = State::EQUALSIGN;
            } else if (c == '<') {
                state = State::LSS;
            } else if (c == '>') {
                state = State::GTR;
            } else if (c == ':') {
                state = State::COLON;
            } else if (c == '+') {
                state = State::PLUS;
            } else if (c == '-') {
                state = State::MINUS;
            } else if (c == '*') {
                state = State::TIMES;
            } else if (c == '/') {
                state = State::RDIV;
            } else if (c == '(') {
                state = State::LPARENT;
            } else if (c == ')') {
                state = State::RPARENT;
            } else if (c == '[') {
                state = State::LBRACK;
            } else if (c == ']') {
                state = State::RBRACK;
            } else if (c == ',') {
                state = State::COMMA;
            } else if (c == ';') {
                state = State::SEMICOLON;
            } else if (c == '.') {
                state = State::PERIOD;
            } else if (c == ' ' || c == '\n') {
                state = State::START;
            } else {
                state = State::UNKNOWN;
                return Token(TokenType::UNKNOWN, lexeme);
            }
            break;


    //Case Letter
        case State::IDENT:
            if (!isNumber && !isLetter){
                //Cek apakah ada keyword di tabel yang sama dengan lexeme
                auto it = keywordTable.find(lexeme);
                if (it != keywordTable.end()){
                    return Token(TokenType::IDENT, lexeme); 
                } else {
                    return Token(it->second, lexeme);//TODO state belum diurus setelah return
                }
            } else {
                state = State::IDENT;//TODO
            }
            break;
        

    //Case Number
        // case State::something:
        //     if (something) {

        //     } else {
                
        //     }
        

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


    // Case Binary Operator
        case State::EQUALSIGN:
            if (c == '=') {
                state = State::EQL;
            } else {
                state = State::UNKNOWN;
                return Token(TokenType::UNKNOWN, lexeme);
            }
            break;

        case State::EQL:
            state = State::FINISH;
            Token(TokenType::EQL, lexeme);
            break;

        case State::LSS:
            if (c == '=') {
                state = State::LEQ;
                return Token(TokenType::LEQ, lexeme);
            } else if (c == '>') {
                state = State::NEQ;
                return Token(TokenType::NEQ, lexeme);
            } else {
                state = State::LSS;
                return Token(TokenType::LSS, lexeme);
            }
            break;

        case State::GTR:
            if (c == '=') {
                state = State::GEQ;
                return Token(TokenType::GEQ, lexeme);
            } else {
                state = State::GTR;
                return Token(TokenType::GTR, lexeme);
            }
            break;

        case State::COLON:
            if (c == '=') {
                state = State::BECOMES;
                return Token(TokenType::BECOMES, lexeme);
            } else {
                state = State::COLON;
                return Token(TokenType::COLON, lexeme);
            }
            break;


    //Case Single Character Symbol
        // case State::something:
        //     if (something) {

        //     } else {
                
        //     }
    }

    lexeme.append(1, c);
    return Token(TokenType::IDENT, lexeme);
}