#include "lexer.hpp"

using namespace std;

std::unordered_map<std::string, TokenType> keywordTable = {
    {"and", TokenType::ANDSY},
    {"else", TokenType::ARRAYSY}
};

Lexer::Lexer() : state(State::START), lexeme("") {}

Token Lexer::processChar(char c){
    //Untuk mempermudah pengecekan angka dan huruf
    bool isNumber = isdigit(c), isLetter = isalpha(c);

    //Proses karakter sesuai dengan state saat ini
    switch (state){
        case State::START:
            this->lexeme = "";
            if (isLetter){
                state = State::IDENT;
            } else if (isNumber) {

            }

            //lanjut
            break;
        case State::IDENT:
            if (std::isblank(c)){
                return Token(TokenType::IDENT, lexeme);
            }

        //lanjut
    }

    lexeme.append(1, c);
    return Token(TokenType::IDENT, lexeme);
}