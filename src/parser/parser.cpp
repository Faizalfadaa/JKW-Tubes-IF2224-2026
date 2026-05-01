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

ParseNode* Parser::parameterGroup(){
    ParseNode* node = new ParseNode("<parameter-group>");

    node->addChild(identifierList());
    node->addChild(match(TokenType::COLON));
    if(currToken == TokenType::IDENT) {
        node->addChild(match(TokenType::IDENT));
    } else if (currToken == TokenType::ARRAYSY){
        node->addChild(arrayType());
    }

    return node;

}

ParseNode* Parser::compoundStatement(){
    ParseNode* node = new ParseNode("<compound-statement>");

    node->addChild(match(TokenType::BEGINSY));
    node->addChild(statementList());
    node->addChild(match(TokenType::ENDSY));

    return node;
}

ParseNode* Parser::statementList(){
    ParseNode* node = new ParseNode("<statement-list>");

    node->addChild(statement());
    while(currToken == TokenType::SEMICOLON) {
        node->addChild(match(TokenType::SEMICOLON));
        
        if(currToken == TokenType::UNTILSY || currToken == TokenType::ENDSY) {
            break;
        }
        
        node->addChild(statement());
    }

    return node;
}

ParseNode* Parser::statement(){
    ParseNode* node = new ParseNode("<statement>");

    if(currToken == TokenType::IDENT) {
        if(parserTokens[pos + 1].type == TokenType::BECOMES && pos + 1 < parserTokens.size()) {
            node->addChild(assignmentStatement());
        } else {
            node->addChild(procedureFunctionCall());
        }
        
    } else if (currToken == TokenType::IFSY) {
        node->addChild(ifStatement());
    } else if (currToken == TokenType::CASESY) {
        node->addChild(caseStatement());
    } else if (currToken == TokenType::WHILESY) {
        node->addChild(whileStatement());
    } else if (currToken == TokenType::REPEATSY) {
        node->addChild(repeatStatement());
    } else if (currToken == TokenType::FORSY) {
        node->addChild(forStatement());
    } 

    return node;
}

ParseNode* Parser::assignmentStatement(){
    ParseNode* node = new ParseNode("<assignment-statement>");

    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::BECOMES));
    node->addChild(expression());

    return node;
}

ParseNode* Parser::ifStatement(){
    ParseNode* node = new ParseNode("<if-statement>");

    node->addChild(match(TokenType::IFSY));
    node->addChild(expression());
    node->addChild(match(TokenType::THENSY));
    node->addChild(statement());

    if(currToken == TokenType::ELSESY) {
        node->addChild(match(TokenType::ELSESY));
        node->addChild(statement());
    }

    return node;
}

ParseNode* Parser::caseStatement(){
    ParseNode* node = new ParseNode("<case-statement>");

    node->addChild(match(TokenType::CASESY));
    node->addChild(expression());
    node->addChild(match(TokenType::OFSY));
    node->addChild(caseBlock());
    node->addChild(match(TokenType::ENDSY));

    return node;
}

ParseNode* Parser::caseBlock(){
    ParseNode* node = new ParseNode("<case-block>");

    node->addChild(constant());
    while(currToken == TokenType::COMMA) {
        node->addChild(match(TokenType::COMMA));
        node->addChild(constant());
    }

    node->addChild(match(TokenType::COLON));
    node->addChild(statement());

    while(currToken == TokenType::SEMICOLON) {
        node->addChild(match(TokenType::SEMICOLON));

        if (currToken == TokenType::ENDSY) {
            break;
        }

        node->addChild(caseBlock());
    }

    return node;
}

ParseNode* Parser::whileStatement(){
    ParseNode* node = new ParseNode("<while-statement>");

    node->addChild(match(TokenType::WHILESY));
    node->addChild(expression());
    node->addChild(match(TokenType::DOSY));
    node->addChild(statement());

    return node;
}

ParseNode* Parser::repeatStatement(){
    ParseNode* node = new ParseNode("<repeat-statement>");

    node->addChild(match(TokenType::REPEATSY));
    node->addChild(statementList());
    node->addChild(match(TokenType::UNTILSY));
    node->addChild(expression());

    return node;
}

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