#include "parser.hpp"
#include "node.hpp"

Parser::Parser(vector<Token> tokens) {
    parserTokens = tokens;
    pos = 0;
    currToken = parserTokens[pos].type;
    currLexeme = parserTokens[pos].lexeme;
}

void Parser::advance() {
    pos++;
    if(pos < (int)parserTokens.size()) {
        currToken = parserTokens[pos].type;
        currLexeme = parserTokens[pos].lexeme;
    }
}

ParseNode* Parser::match(TokenType expectedToken) {

    if(currToken == expectedToken) {
        string label = parserTokens[pos].str_type();

        if(currLexeme != "") {
            label += "(" + currLexeme + ")";
        }

        ParseNode* node = new ParseNode(label);
        advance();
        return node;
    }

    // error(expectedToken);
    return nullptr;
}
ParseNode* Parser::program() {

    ParseNode* node = new ParseNode("<program>");

    node->addChild(programHeader());
    node->addChild(declarationPart());

    return node;
}

ParseNode* Parser::programHeader() {

    ParseNode* node = new ParseNode("<program-header>");

    node->addChild(match(TokenType::PROGRAMSY));
    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::SEMICOLON));

    return node;
}

ParseNode* Parser::declarationPart() {

    //ini masih placeholder aja
    ParseNode* node = new ParseNode("<declaration-part>");
    return node;
}

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

// void Parser::error(TokenType expectedToken) {

//     //ini masih placeholder aja
//     cout << "Syntax Error!" << endl;
//     exit(1);
// }