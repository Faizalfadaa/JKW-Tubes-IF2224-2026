#include "src/parser/parser.hpp"

void Parser::advance() {
    pos++;
    if(pos < parserTokens.size()) {
        currToken = parserTokens[pos].type;
        currLexeme = parserTokens[pos].lexeme;
    }
}

void Parser::match(TokenType expectedToken) {
    if(currToken == expectedToken) {
        advance();
        return;
    }


}

void Parser::program() {
    programHeader();
    declarationPart();
}

void Parser::programHeader() {

}

void Parser::declarationPart() {

}

