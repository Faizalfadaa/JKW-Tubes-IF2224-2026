#include "parser.hpp"
#include "node.hpp"
#include "../utils/exception.hpp"

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

    return error(expectedToken, currToken);
}

ParseNode* Parser::match(std::vector<TokenType> expectedTokens){
    for (TokenType expectedToken : expectedTokens){
        if(currToken == expectedToken) {
            string label = parserTokens[pos].str_type();

            if(currLexeme != "") {
                label += "(" + currLexeme + ")";
            }

            ParseNode* node = new ParseNode(label);
            advance();
            return node;
        }
    }

    return error(expectedTokens, currToken);
}

ParseNode* Parser::error(TokenType expectedToken, TokenType found){
    ParsingError e = ParsingError(expectedToken, found);
    string label = "<ERROR> ";
    label += e.what();
    advance();
    return new ParseNode(label);
}

ParseNode* Parser::error(std::vector<TokenType> expectedTokens, TokenType found){
    ParsingError e = ParsingError(expectedTokens, found);
    string label = "<ERROR> ";
    label += e.what();
    advance();
    return new ParseNode(label);
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
        else return error(vector<TokenType>{
            TokenType::CHARCON,
            TokenType::STRING,
            TokenType::IDENT,
            TokenType::INTCON,
            TokenType::REALCON
        }, currToken);
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
    else{
        return error(vector<TokenType>{
            TokenType::IDENT,
            TokenType::ARRAYSY,
            TokenType::PLUS,
            TokenType::MINUS,
            TokenType::INTCON,
            TokenType::REALCON,
            TokenType::STRING,
            TokenType::LPARENT,
            TokenType::RECORDSY
        }, currToken);
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
    } else {
        return error(vector<TokenType>{
            TokenType::PROCEDURESY,
            TokenType::FUNCTIONSY
        }, currToken);
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
    } else {
        return error(vector<TokenType>{
            TokenType::IDENT,
            TokenType::ARRAYSY
        }, currToken);
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
        if(pos + 1 < (int) parserTokens.size() && parserTokens[pos + 1].type == TokenType::BECOMES) {
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

    node->addChild(variable());
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
        node->addChild(error(vector<TokenType>{
            TokenType::TOSY,
            TokenType::DOWNTOSY
        }, currToken));
    }
    node->addChild(Parser::expression());
    node->addChild(match(TokenType::DOSY));
    node->addChild(Parser::statement());

    return node;
}

ParseNode* Parser::procedureFunctionCall(){
    ParseNode* node = new ParseNode("<procedure/function-call>");

    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::LPARENT));
    if (currToken != TokenType::RPARENT){
        node->addChild(Parser::parameterList());
    }
    node->addChild(match(TokenType::RPARENT));

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
    if (
        currToken == TokenType::EQL ||
        currToken == TokenType::NEQ ||
        currToken == TokenType::GTR ||
        currToken == TokenType::GEQ ||
        currToken == TokenType::LSS ||
        currToken == TokenType::LEQ
    ){
        node->addChild(Parser::relationalOperator());
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
    while (
        currToken == TokenType::PLUS ||
        currToken == TokenType::MINUS ||
        currToken == TokenType::ORSY
    ){
        node->addChild(Parser::additiveOperator());
        node->addChild(Parser::term());
    }

    return node;
}

ParseNode* Parser::term(){
    ParseNode* node = new ParseNode("<term>");

    node->addChild(Parser::factor());
    while (
        currToken == TokenType::TIMES ||
        currToken == TokenType::RDIV ||
        currToken == TokenType::IDIV ||
        currToken == TokenType::IMOD ||
        currToken == TokenType::ANDSY
    ){
        node->addChild(Parser::multiplicativeOperator());
        node->addChild(Parser::factor());
    }

    return node;
}

ParseNode* Parser::factor(){
    ParseNode* node = new ParseNode("<factor>");

    if (currToken == TokenType::IDENT){
        if (pos + 1 < (int) parserTokens.size() && parserTokens[pos + 1].type == TokenType::LPARENT){
            node->addChild(Parser::procedureFunctionCall());
        } else if ((pos + 1 < (int) parserTokens.size() && parserTokens[pos + 1].type == TokenType::LBRACK) // Tolong ini banyak banget revisi Spesifikasi
                    || (pos + 1 < (int) parserTokens.size() && parserTokens[pos + 1].type == TokenType::PERIOD)) { // TODO: Asumsi kalau IDENT saja masuk ke token IDENT
            node->addChild(variable());
        }
        else{
            node->addChild(match(TokenType::IDENT));
        }
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
    else{
        return error(vector<TokenType>{
            TokenType::IDENT,
            TokenType::INTCON,
            TokenType::REALCON,
            TokenType::CHARCON,
            TokenType::STRING,
            TokenType::LPARENT,
            TokenType::NOTSY
        }, currToken);
    }

    return node;
}

ParseNode* Parser::relationalOperator(){
    ParseNode* node = new ParseNode("<relational-operator>");

    node->addChild(match(vector<TokenType>{
        TokenType::EQL,
        TokenType::NEQ,
        TokenType::GTR,
        TokenType::GEQ,
        TokenType::LSS,
        TokenType::LEQ
    }));

    return node;
}

ParseNode* Parser::additiveOperator(){
    ParseNode* node = new ParseNode("<additive-operator>");

    node->addChild(match(vector<TokenType>{
        TokenType::PLUS,
        TokenType::MINUS,
        TokenType::ORSY
    }));

    return node;
}

ParseNode* Parser::multiplicativeOperator(){
    ParseNode* node = new ParseNode("<multiplicative-operator>");

    node->addChild(match(vector<TokenType>{
        TokenType::TIMES,
        TokenType::RDIV,
        TokenType::IDIV,
        TokenType::IMOD,
        TokenType::ANDSY
    }));

    return node;
}

// Tambahan Method dari revisi 
ParseNode* Parser::variable() {
    ParseNode* node = new ParseNode("<variable>");

    node->addChild(match(vector<TokenType> {
        TokenType::IDENT
    }));

    while(currToken == TokenType::LBRACK || currToken == TokenType::PERIOD) {
        node->addChild(componentVariable());
    }

    return node;

};

ParseNode* Parser::componentVariable() {
    ParseNode* node = new ParseNode("<component-variable>");

    if(currToken == TokenType::LBRACK) {
        node->addChild(match(vector<TokenType> {
            TokenType::LBRACK
        }));
        node->addChild(indexList());
        node->addChild(match(vector<TokenType> {
            TokenType::RBRACK
        }));        

    } else if (currToken == TokenType::PERIOD) {
        node->addChild(match(vector<TokenType> {
            TokenType::PERIOD
        }));

        node->addChild(match(vector<TokenType> {
            TokenType::IDENT
        }));        
    } else {
        return error(vector<TokenType>{
            TokenType::LBRACK,
            TokenType::PERIOD
        }, currToken);
    }

    return node;
};

ParseNode* Parser::indexList() {
    ParseNode* node = new ParseNode("<index-list>");

    if(currToken == TokenType::INTCON) {
        node->addChild(match(vector<TokenType> {
            TokenType::INTCON
        }));
    } else if (currToken == TokenType::CHARCON) {
        node->addChild(match(vector<TokenType> {
            TokenType::CHARCON
        }));       
    } else if (currToken == TokenType::IDENT) {
        node->addChild(match(vector<TokenType> {
            TokenType::IDENT
        }));
    } else {
        return error(vector<TokenType>{
            TokenType::INTCON,
            TokenType::CHARCON,
            TokenType::IDENT
        }, currToken);
    }

    while(currToken == TokenType::COMMA) {
        node->addChild(match(vector<TokenType> {
            TokenType::COMMA
        }));

        node->addChild(indexList());
    }

    return node;
};