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

ParseNode* Parser::program(){}
ParseNode* Parser::programHeader(){}
ParseNode* Parser::declarationPart(){}
ParseNode* Parser::constDeclaration(){}
ParseNode* Parser::constant(){}
ParseNode* Parser::typeDeclaration(){}
ParseNode* Parser::varDeclaration(){}
ParseNode* Parser::identifierList(){}
ParseNode* Parser::type(){}
ParseNode* Parser::arrayType(){}

ParseNode* Parser::range(){}
ParseNode* Parser::enumerated(){}
ParseNode* Parser::recordType(){}
ParseNode* Parser::fieldList(){}
ParseNode* Parser::fieldPart(){}
ParseNode* Parser::subProgramDeclaration(){}
ParseNode* Parser::procedureDeclaration(){}
ParseNode* Parser::functionDeclaration(){}
ParseNode* Parser::block(){}
ParseNode* Parser::formalParameterList(){}

ParseNode* Parser::parameterGroup(){}
ParseNode* Parser::compoundStatement(){}
ParseNode* Parser::statementList(){}
ParseNode* Parser::statement(){}
ParseNode* Parser::assignmentStatement(){}
ParseNode* Parser::ifStatement(){}
ParseNode* Parser::caseStatement(){}
ParseNode* Parser::caseBlock(){}
ParseNode* Parser::whileStatement(){}
ParseNode* Parser::repeatStatement(){}

ParseNode* Parser::forStatement(){}
ParseNode* Parser::procedureFunctionCall(){}
ParseNode* Parser::parameterList(){}
ParseNode* Parser::expression(){}
ParseNode* Parser::simpleExpression(){}
ParseNode* Parser::term(){}
ParseNode* Parser::factor(){}
ParseNode* Parser::relationalOperator(){}
ParseNode* Parser::additiveOperator(){}
ParseNode* Parser::multiplicativeOperator(){}

