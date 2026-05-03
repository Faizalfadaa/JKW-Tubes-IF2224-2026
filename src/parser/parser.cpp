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

    // TODO: throw error(expectedToken); //Buat untuk semua kasus fungsi
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

ParseNode* Parser::forStatement(){
    ParseNode* node = new ParseNode("<for-statement>");

    node->addChild(match(TokenType::FORSY));
    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::BECOMES));
    node->addChild(Parser::expression());
    if (currToken == TokenType::TOSY){
        node->addChild(match(TokenType::TOSY));
    }
    else if (currToken == TokenType::DOWNTOSY){
        node->addChild(match(TokenType::DOWNTOSY));
    }
    else {
        return nullptr; //TODO: Throw error
    }
    node->addChild(Parser::expression());
    node->addChild(match(TokenType::DOSY));
    node->addChild(Parser::statement());

    return node;
}

ParseNode* Parser::procedureFunctionCall(){
    ParseNode* node = new ParseNode("<procedure/function-call>");

    node->addChild(match(TokenType::IDENT));
    if (currToken == TokenType::LPARENT){
        node->addChild(match(TokenType::LPARENT));
        if (currToken != TokenType::RPARENT){
            node->addChild(Parser::parameterList());
        }
        node->addChild(match(TokenType::RPARENT));
    }

    return node;
}

ParseNode* Parser::parameterList(){
    ParseNode* node = new ParseNode("<parameter-list>");

    node->addChild(Parser::expression());
    while (currToken == TokenType::COMMA){
        node->addChild(match(TokenType::COMMA));
        node->addChild(Parser::expression());
    }

    return node;
}

ParseNode* Parser::expression(){
    ParseNode* node = new ParseNode("<expression>");

    node->addChild(Parser::simpleExpression());
    if (ParseNode* temp = Parser::relationalOperator()){
        node->addChild(temp);
        node->addChild(Parser::simpleExpression());
    }

    return node;
}

ParseNode* Parser::simpleExpression(){
    ParseNode* node = new ParseNode("<simple-expression>");

    if (currToken == TokenType::PLUS){
        node->addChild(match(TokenType::PLUS));
    }
    else if (currToken == TokenType::MINUS){
        node->addChild(match(TokenType::MINUS));
    }
    node->addChild(Parser::term());
    while (ParseNode* temp = Parser::additiveOperator()){
        node->addChild(temp);
        node->addChild(Parser::term());
    }

    return node;
}

ParseNode* Parser::term(){
    ParseNode* node = new ParseNode("<term>");

    node->addChild(Parser::factor());
    while (ParseNode* temp = Parser::multiplicativeOperator()){
        node->addChild(temp);
        node->addChild(Parser::factor());
    }

    return node;
}

ParseNode* Parser::factor(){
    ParseNode* node = new ParseNode("<factor>");

    if (currToken == TokenType::IDENT){
        node->addChild(match(TokenType::IDENT));
    }
    else if (currToken == TokenType::INTCON){
        node->addChild(match(TokenType::INTCON));
    }
    else if (currToken == TokenType::REALCON){
        node->addChild(match(TokenType::REALCON));
    }
    else if (currToken == TokenType::CHARCON){
        node->addChild(match(TokenType::CHARCON));
    }
    else if (currToken == TokenType::STRING){
        node->addChild(match(TokenType::STRING));
    }
    else if (currToken == TokenType::LPARENT){
        node->addChild(match(TokenType::LPARENT));
        node->addChild(Parser::expression());
        node->addChild(match(TokenType::RPARENT));
    }
    else if (currToken == TokenType::NOTSY){
        node->addChild(match(TokenType::NOTSY));
        node->addChild(Parser::factor());
    }
    else if (ParseNode* temp = Parser::procedureFunctionCall()){
        node->addChild(temp);
    }
    // else if (ParseNode* temp = Parser::variable()){ //TODO: fungsi dari revisi 2 MEI
    //     node->addChild(temp);
    // }
    else{
        return nullptr;
    }

    return node;
}

ParseNode* Parser::relationalOperator(){
    ParseNode* node = new ParseNode("<relational-operator>");

    if (currToken == TokenType::EQL){
        node->addChild(match(TokenType::EQL));
    }
    else if (currToken == TokenType::NEQ){
        node->addChild(match(TokenType::NEQ));
    }
    else if (currToken == TokenType::GTR){
        node->addChild(match(TokenType::GTR));
    }
    else if (currToken == TokenType::GEQ){
        node->addChild(match(TokenType::GEQ));
    }
    else if (currToken == TokenType::LSS){
        node->addChild(match(TokenType::LSS));
    }
    else if (currToken == TokenType::LEQ){
        node->addChild(match(TokenType::LEQ));
    }

    return node;
}

ParseNode* Parser::additiveOperator(){
    ParseNode* node = new ParseNode("<additive-operator>");

    if (currToken == TokenType::PLUS){
        node->addChild(match(TokenType::PLUS));
    }
    else if (currToken == TokenType::MINUS){
        node->addChild(match(TokenType::MINUS));
    }
    else if (currToken == TokenType::ORSY){
        node->addChild(match(TokenType::ORSY));
    }

    return node;
}

ParseNode* Parser::multiplicativeOperator(){
    ParseNode* node = new ParseNode("<multiplicative-operator>");

    if (currToken == TokenType::TIMES){
        node->addChild(match(TokenType::TIMES));
    }
    else if (currToken == TokenType::RDIV){
        node->addChild(match(TokenType::RDIV));
    }
    else if (currToken == TokenType::IDIV){
        node->addChild(match(TokenType::IDIV));
    }
    else if (currToken == TokenType::IMOD){
        node->addChild(match(TokenType::IMOD));
    }
    else if (currToken == TokenType::ANDSY){
        node->addChild(match(TokenType::ANDSY));
    }

    return node;
}

// void Parser::error(TokenType expectedToken) {

//     //ini masih placeholder aja
//     cout << "Syntax Error!" << endl;
//     exit(1);
// }