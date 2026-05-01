#include "iostream"
#include "vector"
#include "src/lexer/token.hpp"

using namespace std;

class Parser {
    private:
        vector<Token> parserTokens;
        int pos;
        void advance();

    public:
        TokenType currToken;
        std::string currLexeme;

        void match(TokenType expectedToken);
        void program();
        void programHeader();
        void declarationPart();

};


