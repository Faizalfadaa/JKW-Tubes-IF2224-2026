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
    node->addChild(compoundStatement());
    node->addChild(match(TokenType::PERIOD));

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

    ParseNode* node = new ParseNode("<declaration-part>");

    while (currToken == TokenType::CONSTSY) {
        node->addChild(constDeclaration());
    }

    while (currToken == TokenType::TYPESY) {
        node->addChild(typeDeclaration());
    }

    while (currToken == TokenType::VARSY) {
        node->addChild(varDeclaration());
    }

    while (
        currToken == TokenType::PROCEDURESY ||
        currToken == TokenType::FUNCTIONSY
    ) {
        node->addChild(subProgramDeclaration());
    }

    return node;
}

ParseNode* Parser::constDeclaration() {

    ParseNode* node = new ParseNode("<const-declaration>");

    node->addChild(match(TokenType::CONSTSY));

    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::EQL));
    node->addChild(constant());
    node->addChild(match(TokenType::SEMICOLON));

    while (currToken == TokenType::IDENT) {
        node->addChild(match(TokenType::IDENT));
        node->addChild(match(TokenType::EQL));
        node->addChild(constant());
        node->addChild(match(TokenType::SEMICOLON));
    }

    return node;
}

ParseNode* Parser::constant() {

    ParseNode* node = new ParseNode("<constant>");

    if (currToken == TokenType::CHARCON) {
        node->addChild(match(TokenType::CHARCON));
    }
    else if (currToken == TokenType::STRING) {
        node->addChild(match(TokenType::STRING));
    }
    else {

        if (currToken == TokenType::PLUS) {
            node->addChild(match(TokenType::PLUS));
        }
        else if (currToken == TokenType::MINUS) {
            node->addChild(match(TokenType::MINUS));
        }

        if (currToken == TokenType::IDENT) {
            node->addChild(match(TokenType::IDENT));
        }
        else if (currToken == TokenType::INTCON) {
            node->addChild(match(TokenType::INTCON));
        }
        else if (currToken == TokenType::REALCON) {
            node->addChild(match(TokenType::REALCON));
        }
    }

    return node;
}

ParseNode* Parser::typeDeclaration() {

    ParseNode* node = new ParseNode("<type-declaration>");

    node->addChild(match(TokenType::TYPESY));

    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::EQL));
    node->addChild(type());
    node->addChild(match(TokenType::SEMICOLON));

    while (currToken == TokenType::IDENT) {
        node->addChild(match(TokenType::IDENT));
        node->addChild(match(TokenType::EQL));
        node->addChild(type());
        node->addChild(match(TokenType::SEMICOLON));
    }

    return node;
}

ParseNode* Parser::varDeclaration() {

    ParseNode* node = new ParseNode("<var-declaration>");

    node->addChild(match(TokenType::VARSY));

    node->addChild(identifierList());
    node->addChild(match(TokenType::COLON));
    node->addChild(type());
    node->addChild(match(TokenType::SEMICOLON));

    while (currToken == TokenType::IDENT) {
        node->addChild(identifierList());
        node->addChild(match(TokenType::COLON));
        node->addChild(type());
        node->addChild(match(TokenType::SEMICOLON));
    }

    return node;
}

ParseNode* Parser::identifierList() {

    ParseNode* node = new ParseNode("<identifier-list>");

    node->addChild(match(TokenType::IDENT));

    while (currToken == TokenType::COMMA) {
        node->addChild(match(TokenType::COMMA));
        node->addChild(match(TokenType::IDENT));
    }

    return node;
}

ParseNode* Parser::type() {

    ParseNode* node = new ParseNode("<type>");

    if (currToken == TokenType::IDENT) {
        node->addChild(match(TokenType::IDENT));
    }
    else if (currToken == TokenType::ARRAYSY) {
        node->addChild(arrayType());
    }
    else if (
        currToken == TokenType::PLUS ||
        currToken == TokenType::MINUS ||
        currToken == TokenType::INTCON ||
        currToken == TokenType::REALCON ||
        currToken == TokenType::CHARCON ||
        currToken == TokenType::STRING
    ) {
        node->addChild(range());
    }
    else if (currToken == TokenType::LPARENT) {
        node->addChild(enumerated());
    }
    else if (currToken == TokenType::RECORDSY) {
        node->addChild(recordType());
    }

    return node;
}

ParseNode* Parser::arrayType() {

    ParseNode* node = new ParseNode("<array-type>");

    node->addChild(match(TokenType::ARRAYSY));
    node->addChild(match(TokenType::LBRACK));

    if (currToken == TokenType::IDENT) {
        node->addChild(match(TokenType::IDENT));
    }
    else {
        node->addChild(range());
    }

    node->addChild(match(TokenType::RBRACK));
    node->addChild(match(TokenType::OFSY));
    node->addChild(type());

    return node;
}

ParseNode* Parser::range(){
    ParseNode* node = new ParseNode("<range>");

    node->addChild(constant());
    node->addChild(match(TokenType::PERIOD));
    node->addChild(match(TokenType::PERIOD));
    node->addChild(constant());

    return node;
}

ParseNode* Parser::enumerated(){
    ParseNode* node = new ParseNode("<enumerated>");

    node->addChild(match(TokenType::LPARENT));
    node->addChild(match(TokenType::IDENT));
    while (currToken == TokenType::COMMA) {
        node->addChild(match(TokenType::COMMA));
        node->addChild(match(TokenType::IDENT));
    }
    node->addChild(match(TokenType::RPARENT));

    return node;
}

ParseNode* Parser::recordType(){
    ParseNode* node = new ParseNode("<record-type>");

    node->addChild(match(TokenType::RECORDSY));
    node->addChild(fieldList());
    node->addChild(match(TokenType::ENDSY));

    return node;
}

ParseNode* Parser::fieldList(){
    ParseNode* node = new ParseNode("<field-list>");

    node->addChild(fieldPart());
    while (currToken == TokenType::SEMICOLON) {
        node->addChild(match(TokenType::SEMICOLON));
        if (currToken == TokenType::ENDSY) {
            break;
        }
        node->addChild(fieldPart());
    }

    return node;
}

ParseNode* Parser::fieldPart(){
    ParseNode* node = new ParseNode("<field-part>");

    node->addChild(identifierList());
    node->addChild(match(TokenType::COLON));
    node->addChild(type());

    return node;
}

ParseNode* Parser::subProgramDeclaration(){
    ParseNode* node = new ParseNode("<subprogram-declaration>");

    if (currToken == TokenType::PROCEDURESY) {
        node->addChild(procedureDeclaration());
    } else if (currToken == TokenType::FUNCTIONSY) {
        node->addChild(functionDeclaration());
    }

    return node;
}

ParseNode* Parser::procedureDeclaration(){
    ParseNode* node = new ParseNode("<procedure-declaration>");

    node->addChild(match(TokenType::PROCEDURESY));
    node->addChild(match(TokenType::IDENT));
    if (currToken == TokenType::LPARENT) {
        node->addChild(formalParameterList());
    }
    node->addChild(match(TokenType::SEMICOLON));
    node->addChild(block());
    node->addChild(match(TokenType::SEMICOLON));

    return node;
}

ParseNode* Parser::functionDeclaration(){
    ParseNode* node = new ParseNode("<function-declaration>");

    node->addChild(match(TokenType::FUNCTIONSY));
    node->addChild(match(TokenType::IDENT));
    if (currToken == TokenType::LPARENT) {
        node->addChild(formalParameterList());
    }
    node->addChild(match(TokenType::COLON));
    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::SEMICOLON));
    node->addChild(block());
    node->addChild(match(TokenType::SEMICOLON));

    return node;
}

ParseNode* Parser::block(){
    ParseNode* node = new ParseNode("<block>");

    node->addChild(declarationPart());
    node->addChild(compoundStatement());

    return node;
}

ParseNode* Parser::formalParameterList(){
    ParseNode* node = new ParseNode("<formal-parameter-list>");

    node->addChild(match(TokenType::LPARENT));
    node->addChild(parameterGroup());
    while (currToken == TokenType::SEMICOLON) {
        node->addChild(match(TokenType::SEMICOLON));
        node->addChild(parameterGroup());
    }
    node->addChild(match(TokenType::RPARENT));

    return node;
}

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