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

Lexer::Lexer(Reader& reader) : state(State::START), lexeme(""), reader(reader) {}

Token Lexer::getNextToken(){
    //Inisialisasi
    TokenType tokenType;
    state = State::START;
    lexeme = "";

    //Baca karakter selanjutnya hingga berada di state FINISH
    while (state != State::FINISH && state != State::UNKNOWN){
        // std::cout << reader.get(); //process every char here
        tokenType = processChar(reader.get());
        reader.next();
    }
    return Token(tokenType, lexeme);
}

TokenType Lexer::processChar(char c){
    //Untuk mempermudah pengecekan angka dan huruf
    bool isNumber = isdigit(c), isLetter = isalpha(c);

    //Proses karakter sesuai dengan state saat ini
    switch (state){
    //Initial state
        case State::START:
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
                return TokenType::UNKNOWN;
            }
            break;


    //Case Letter
        case State::IDENT:
            if (!isNumber && !isLetter){
                //Cek apakah ada keyword di tabel yang sama dengan lexeme
                auto it = keywordTable.find(lexeme);
                if (it != keywordTable.end()){
                    return it->second;//TODO state belum diurus setelah return
                } else {
                    return TokenType::IDENT; 
                }
            } else {
                state = State::IDENT;//TODO
            }
            break;
        

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


    // Case Binary Operator
        case State::EQUALSIGN:
            if (c == '=') {
                state = State::EQL;
            } else {
                state = State::UNKNOWN;
                return TokenType::UNKNOWN;
            }
            break;

        case State::EQL:
            state = State::FINISH;
            TokenType::EQL;
            break;

        case State::LSS:
            if (c == '=') {
                state = State::LEQ;
                return TokenType::LEQ;
            } else if (c == '>') {
                state = State::NEQ;
                return TokenType::NEQ;
            } else {
                state = State::LSS;
                return TokenType::LSS;
            }
            break;

        case State::GTR:
            if (c == '=') {
                state = State::GEQ;
                return TokenType::GEQ;
            } else {
                state = State::GTR;
                return TokenType::GTR;
            }
            break;

        case State::COLON:
            if (c == '=') {
                state = State::BECOMES;
                return TokenType::BECOMES;
            } else {
                state = State::COLON;
                return TokenType::COLON;
            }
            break;


    //Case Single Character Symbol
        // case State::something:
        //     if (something) {

        //     } else {
                
        //     }
    }

    lexeme += c;
    return TokenType::NOTDETERMINED;
}