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

    //Baca karakter selanjutnya hingga berada di state FINISH
    while (state != State::FINISH && state != State::DETERMINED && state != State::UNKNOWN && !reader.isEOF()){
        //Process every character
        tokenType = processChar(reader.get());
        if (state == State::FINISH){
            break;
        }
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
            //Empty buffer
            lexeme = "";

            if (isLetter){
                state = State::IDENT;
                lexeme += c;
            } 
            else if (isNumber) {
                state = State::INTCON;
                lexeme += c;
            } 
            else if (c == '=') {
                state = State::EQUALSIGN;
            } 
            else if (c == '<') {
                state = State::LSS;
            } 
            else if (c == '>') {
                state = State::GTR;
            } 
            else if (c == ':') {
                state = State::COLON;
            } 
            else if (c == '+') {
                state = State::DETERMINED;
                return TokenType::PLUS;
            } 
            else if (c == '-') {//TODO remove this
                state = State::DETERMINED;
                return TokenType::MINUS;
            } 
            else if (c == '*') {
                state = State::DETERMINED;
                return TokenType::TIMES;
            } 
            else if (c == '/') {
                state = State::DETERMINED;
                return TokenType::RDIV;
            } 
            else if (c == ')') {
                state = State::DETERMINED;
                return TokenType::RPARENT;
            } 
            else if (c == '[') {
                state = State::DETERMINED;
                return TokenType::LBRACK;
            } 
            else if (c == ']') {
                state = State::DETERMINED;
                return TokenType::RBRACK;
            } 
            else if (c == ',') {
                state = State::DETERMINED;
                return TokenType::COMMA;
            } 
            else if (c == ';') {
                state = State::DETERMINED;
                return TokenType::SEMICOLON;
            } 
            else if (c == '.') {
                state = State::DETERMINED;
                return TokenType::PERIOD;
            } 
            else if (c == '\''){
                state = State::OPENQUOTE;
                lexeme += c;
            }
            else if (c == '('){
                state = State::LPARENT;
            }
            else if (c == '{'){
                state = State::COMMENTCUR;
            }
            else if (isblank(c) || c == '\n') {
                state = State::START;
            } 
            else {
                state = State::UNKNOWN;
                return TokenType::UNKNOWN;
            }
            break;


    //Case Letter
        case State::IDENT:
            if (!isNumber && !isLetter){
                //Cek apakah ada keyword di tabel yang sama dengan lexeme
                auto it = keywordTable.find(toLower(lexeme));
                if (it != keywordTable.end()){
                    lexeme = "";
                    state = State::FINISH;
                    return it->second;
                } 
                else {
                    state = State::FINISH;
                    return TokenType::IDENT; 
                }
            } 
            lexeme += c;
            // else {
            //     state = State::IDENT;
            // }
            break;


    //Case Number
        case State::INTCON:
            if (c == '.'){
                state = State::INTDOT;
            } 
            else if (isNumber) {
                // state = State::INTCON;
            } 
            else {
                state = State::FINISH;
                return TokenType::INTCON;
            }
            lexeme += c;
            break;

        case State::INTDOT:
            if(isNumber) {
                lexeme += c;
                state = State::REALCON;
            } 
            else {
                state = State::UNKNOWN;
                return TokenType::UNKNOWN;
            } 
            break;

        case State::REALCON:
            if(isNumber) {
                // state = State::REALCON; 
            } 
            else {
                state = State::FINISH;
                return TokenType::REALCON;
            }
            break;


    //Case String & Char
        case State::OPENQUOTE:
            if (c == '\'') {
                state = State::STRINGCLOSEQUOTE;
            } 
            else {
                state = State::CHARCON;
            }
            lexeme += c;
            break;

        case State::CHARCON:
            if (c == '\'') {
                state = State::CHARCLOSEQUOTE;  
            } 
            else {
                state = State::STRING;
            }
            lexeme += c;
            break;

        case State::CHARCLOSEQUOTE:
            if (c == '\'') {
                state = State::STRING;
            } 
            else {
                state = State::FINISH;
                return TokenType::CHARCON;
            }
            break;

        case State::STRING:
            if (c == '\'') {
                state = State::STRINGCLOSEQUOTE;
            } 
            lexeme += c;
            break;

        case State::STRINGCLOSEQUOTE:
            if (c == '\'') {
                state = State::STRING;
            } 
            else {
                state = State::FINISH;
                return TokenType::STRING;
            }


    //Case Comment
        // Ini buat komen yang format {}
        case State::OPENCUR:
            if (c == '}') {
                state = State::DETERMINED;
                return TokenType::COMMENT;
            } 
            else {
                state = State::COMMENTCUR;      
            }
            break;

         case State::COMMENTCUR:
            if (c == '}') {
                state = State::DETERMINED;
                return TokenType::COMMENT;
            }
            lexeme += c;
            break;
        
        // Ini buat komen yang format (* *)
        case State::LPARENT:
            if (c == '*') {
                state = State::COMMENTPARAST;
            } 
            else {
                state = State::FINISH;
                return TokenType::LPARENT;
            }
            break;

        case State::COMMENTPARAST:
            if (c == '*') {
                state = State::COMMENTCLOSEAST;
            } 
            else {
                lexeme += c;
            }

        case State::COMMENTCLOSEAST:
            if (c == ')') {
                state = State::DETERMINED;
                return TokenType::COMMENT;
            } 
            else if (c == '*') {
                lexeme += c;
                state = State::COMMENTCLOSEAST;
            }
            else {
                lexeme += c;
                state = State::COMMENTPARAST;
            }
            break;


    // Case Binary Operator
        case State::EQUALSIGN:
            if (c == '=') {
                state = State::DETERMINED;
                return TokenType::EQL;
            } else {
                state = State::UNKNOWN;
                return TokenType::UNKNOWN;
            }
            break;

        case State::LSS:
            if (c == '=') {
                state = State::DETERMINED;
                return TokenType::LEQ;
            } else if (c == '>') {
                state = State::DETERMINED;
                return TokenType::NEQ;
            } else {
                state = State::FINISH;
                return TokenType::LSS;
            }
            break;

        case State::GTR:
            if (c == '=') {
                state = State::DETERMINED;
                return TokenType::GEQ;
            } else {
                state = State::FINISH;
                return TokenType::GTR;
            }
            break;

        case State::COLON:
            if (c == '=') {
                state = State::DETERMINED;
                return TokenType::BECOMES;
            } else {
                state = State::FINISH;
                return TokenType::COLON;
            }
            break;


    //Case Single Character Symbol
        //Handled in initial state
    }

    return TokenType::NOTDETERMINED;
}