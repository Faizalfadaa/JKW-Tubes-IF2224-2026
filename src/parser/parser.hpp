#include "iostream"
#include "vector"
#include "../lexer/token.hpp"
#include "node.hpp"

using namespace std;

class Parser {
    private:
        vector<Token> parserTokens;
        int pos;
        void advance();

    public:
        TokenType currToken;
        std::string currLexeme;

        Parser(vector<Token> tokens);
        ParseNode* match(TokenType expectedToken);
        ParseNode* program();
        ParseNode* programHeader();
        ParseNode* declarationPart();

    // void error(TokenType expectedToken);

};


