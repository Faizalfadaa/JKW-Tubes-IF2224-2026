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
                return Token(TokenType::PLUS, "+");
            } else if (c == '-') {
                return Token(TokenType::MINUS, "-");
            } else if (c == '*') {
                return Token(TokenType::TIMES, "*");
            } else if (c == '/') {
                return Token(TokenType::RDIV, "/");
            } else if (c == '(') {
                state = State::LPARENT; //TODO: Mendefinisikan state LPARENT agar dapat menangani kasus kurung biasa dan komentar
            } else if (c == ')') {
                return Token(TokenType::RPARENT, ")");
            } else if (c == '[') {
                return Token(TokenType::LBRACK, "[");
            } else if (c == ']') {
                return Token(TokenType::RBRACK, "]");
            } else if (c == ',') {
                return Token(TokenType::COMMA, ",");
            } else if (c == ';') {
                return Token(TokenType::SEMICOLON, ";");
            } else if (c == '.') {
                return Token(TokenType::PERIOD, ".");
            } else if (c == ' ' || c == '\n') {
                state = State::START;
            } else {
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