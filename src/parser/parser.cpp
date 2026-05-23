#include "parser.hpp"
#include "node.hpp"
#include "../utils/exception.hpp"

Parser::Parser(vector<Token> tokens) {
    parserTokens = tokens;
    pos = 0;
    currToken = parserTokens[pos].type;
    currLexeme = parserTokens[pos].lexeme;
}

//Fungsi untuk lompat ke token selanjutnya
void Parser::advance() {
    pos++;
    if(pos < (int)parserTokens.size()) {
        currToken = parserTokens[pos].type;
        currLexeme = parserTokens[pos].lexeme;
    }
}

//Fungsi untuk mengecek apakah currToken sama dengan expectedToken
unique_ptr<ParseNode> Parser::match(TokenType expectedToken) {
    //Cek apakah token sesuai
    if(currToken == expectedToken) {
        string label = parserTokens[pos].str_type();

        //Cek apakah lexeme perlu di masukkan atau tidak
        if(currLexeme != "") {
            label += "(" + currLexeme + ")";
        }

        unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode(label));
        advance();
        return node;
    }

    return error(expectedToken, currToken);
}

//Fungsi untuk mengecek apakah currToken sama dengan expectedTokens dengan masukan berupa vector
unique_ptr<ParseNode> Parser::match(std::vector<TokenType> expectedTokens){
    for (TokenType expectedToken : expectedTokens){
        //Cek apakah token sesuai
        if(currToken == expectedToken) {
            string label = parserTokens[pos].str_type();

            //Cek apakah lexeme perlu di masukkan atau tidak
            if(currLexeme != "") {
                label += "(" + currLexeme + ")";
            }

            unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode(label));
            advance();
            return node;
        }
    }

    return error(expectedTokens, currToken);
}

//Fungsi untuk membuat ErrorNode dengan informasi expected dan found
unique_ptr<ParseNode> Parser::error(TokenType expectedToken, TokenType found){
    ParsingError e = ParsingError(expectedToken, found);
    string label = "<ERROR> ";
    label += e.what();
    advance();
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode(label));
    hasErrorNode = true;
    return node;
}

//Fungsi untuk membuat ErrorNode dengan informasi expected dan found dengan expected berupa vector
unique_ptr<ParseNode> Parser::error(std::vector<TokenType> expectedTokens, TokenType found){
    ParsingError e = ParsingError(expectedTokens, found);
    string label = "<ERROR> ";
    label += e.what();
    advance();
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode(label));
    hasErrorNode = true;
    return node;
}

//Fungsi untuk aturan produksi program
unique_ptr<ParseNode> Parser::program() {
    //Buat label node
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<program>"));

    //Tambahkan semua hasil aturan produksi sebagai child
    node->addChild(programHeader());
    node->addChild(declarationPart());
    node->addChild(compoundStatement());
    node->addChild(match(TokenType::PERIOD));

    return node;
}

//Fungsi untuk aturan produksi programHeader
unique_ptr<ParseNode> Parser::programHeader() {
    //Buat label node
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<program-header>"));

    //Tambahkan semua hasil aturan produksi sebagai child
    node->addChild(match(TokenType::PROGRAMSY));
    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::SEMICOLON));

    return node;
}

//Fungsi untuk aturan produksi declarationPart
unique_ptr<ParseNode> Parser::declarationPart() {

    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<declaration-part>"));

    //Loop selama token masih sesuai
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

//Fungsi untuk aturan produksi constDeclaration
unique_ptr<ParseNode> Parser::constDeclaration() {

    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<const-declaration>"));

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

//Fungsi untuk aturan produksi constant
unique_ptr<ParseNode> Parser::constant() {

    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<constant>"));

    //Cek token selanjutnya untuk mengetahui bentuk yang sesuai
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

//Fungsi untuk aturan produksi typeDeclaration
unique_ptr<ParseNode> Parser::typeDeclaration() {

    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<type-declaration>"));

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

//Fungsi untuk aturan produksi varDeclaration
unique_ptr<ParseNode> Parser::varDeclaration() {

    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<var-declaration>"));

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

//Fungsi untuk aturan produksi identifierList
unique_ptr<ParseNode> Parser::identifierList() {

    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<identifier-list>"));

    node->addChild(match(TokenType::IDENT));

    while (currToken == TokenType::COMMA) {
        node->addChild(match(TokenType::COMMA));
        node->addChild(match(TokenType::IDENT));
    }

    return node;
}

//Fungsi untuk aturan produksi type
unique_ptr<ParseNode> Parser::type() {

    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<type>"));

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
        //Return ErrorNode jika tidak ada token yang sesuai aturan
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

//Fungsi untuk aturan produksi arrayType
unique_ptr<ParseNode> Parser::arrayType() {

    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<array-type>"));

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

//Fungsi untuk aturan produksi range
unique_ptr<ParseNode> Parser::range(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<range>"));

    node->addChild(constant());
    node->addChild(match(TokenType::PERIOD));
    node->addChild(match(TokenType::PERIOD));
    node->addChild(constant());

    return node;
}

//Fungsi untuk aturan produksi enumerated
unique_ptr<ParseNode> Parser::enumerated(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<enumerated>"));

    node->addChild(match(TokenType::LPARENT));
    node->addChild(match(TokenType::IDENT));
    while (currToken == TokenType::COMMA) {
        node->addChild(match(TokenType::COMMA));
        node->addChild(match(TokenType::IDENT));
    }
    node->addChild(match(TokenType::RPARENT));

    return node;
}

//Fungsi untuk aturan produksi recordType
unique_ptr<ParseNode> Parser::recordType(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<record-type>"));

    node->addChild(match(TokenType::RECORDSY));
    node->addChild(fieldList());
    node->addChild(match(TokenType::ENDSY));

    return node;
}

//Fungsi untuk aturan produksi fieldList
unique_ptr<ParseNode> Parser::fieldList(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<field-list>"));

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

//Fungsi untuk aturan produksi fieldPart
unique_ptr<ParseNode> Parser::fieldPart(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<field-part>"));

    node->addChild(identifierList());
    node->addChild(match(TokenType::COLON));
    node->addChild(type());

    return node;
}

//Fungsi untuk aturan produksi subProgramDeclaration
unique_ptr<ParseNode> Parser::subProgramDeclaration(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<subprogram-declaration>"));

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

//Fungsi untuk aturan produksi procedureDeclaration
unique_ptr<ParseNode> Parser::procedureDeclaration(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<procedure-declaration>"));

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

//Fungsi untuk aturan produksi functionDeclaration
unique_ptr<ParseNode> Parser::functionDeclaration(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<function-declaration>"));

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

//Fungsi untuk aturan produksi block
unique_ptr<ParseNode> Parser::block(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<block>"));

    node->addChild(declarationPart());
    node->addChild(compoundStatement());

    return node;
}

//Fungsi untuk aturan produksi formalParameterList
unique_ptr<ParseNode> Parser::formalParameterList(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<formal-parameter-list>"));

    node->addChild(match(TokenType::LPARENT));
    node->addChild(parameterGroup());
    while (currToken == TokenType::SEMICOLON) {
        node->addChild(match(TokenType::SEMICOLON));
        node->addChild(parameterGroup());
    }
    node->addChild(match(TokenType::RPARENT));

    return node;
}

//Fungsi untuk aturan produksi parameterGroup
unique_ptr<ParseNode> Parser::parameterGroup(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<parameter-group>"));

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

//Fungsi untuk aturan produksi compoundStatement
unique_ptr<ParseNode> Parser::compoundStatement(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<compound-statement>"));

    node->addChild(match(TokenType::BEGINSY));
    node->addChild(statementList());
    node->addChild(match(TokenType::ENDSY));

    return node;
}

//Fungsi untuk aturan produksi statementList
unique_ptr<ParseNode> Parser::statementList(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<statement-list>"));

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

//Fungsi untuk aturan produksi statement
unique_ptr<ParseNode> Parser::statement(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<statement>"));

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

//Fungsi untuk aturan produksi assignmentStatement
unique_ptr<ParseNode> Parser::assignmentStatement(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<assignment-statement>"));

    node->addChild(variable());
    node->addChild(match(TokenType::BECOMES));
    node->addChild(expression());

    return node;
}

//Fungsi untuk aturan produksi ifStatement
unique_ptr<ParseNode> Parser::ifStatement(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<if-statement>"));

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

//Fungsi untuk aturan produksi caseStatement
unique_ptr<ParseNode> Parser::caseStatement(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<case-statement>"));

    node->addChild(match(TokenType::CASESY));
    node->addChild(expression());
    node->addChild(match(TokenType::OFSY));
    node->addChild(caseBlock());
    node->addChild(match(TokenType::ENDSY));

    return node;
}

//Fungsi untuk aturan produksi caseBlock
unique_ptr<ParseNode> Parser::caseBlock(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<case-block>"));

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

//Fungsi untuk aturan produksi whileStatement
unique_ptr<ParseNode> Parser::whileStatement(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<while-statement>"));

    node->addChild(match(TokenType::WHILESY));
    node->addChild(expression());
    node->addChild(compoundStatement());
    node->addChild(match(TokenType::SEMICOLON));

    return node;
}

//Fungsi untuk aturan produksi repeatStatement
unique_ptr<ParseNode> Parser::repeatStatement(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<repeat-statement>"));

    node->addChild(match(TokenType::REPEATSY));
    node->addChild(statementList());
    node->addChild(match(TokenType::UNTILSY));
    node->addChild(expression());

    return node;
}

//Fungsi untuk aturan produksi forStatement
unique_ptr<ParseNode> Parser::forStatement(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<for-statement>"));

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
    
    node->addChild(Parser::expression());
    node->addChild(match(TokenType::DOSY));
    node->addChild(compoundStatement());
    node->addChild(match(TokenType::SEMICOLON));

    return node;
}

//Fungsi untuk aturan produksi procedureFunctionCall
unique_ptr<ParseNode> Parser::procedureFunctionCall(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<procedure/function-call>"));

    node->addChild(match(TokenType::IDENT));
    node->addChild(match(TokenType::LPARENT));
    if (currToken != TokenType::RPARENT){
        node->addChild(Parser::parameterList());
    }
    node->addChild(match(TokenType::RPARENT));

    return node;
}

//Fungsi untuk aturan produksi parameterList
unique_ptr<ParseNode> Parser::parameterList(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<parameter-list>"));

    node->addChild(Parser::expression());
    while (currToken == TokenType::COMMA){
        node->addChild(match(TokenType::COMMA));
        node->addChild(Parser::expression());
    }

    return node;
}

//Fungsi untuk aturan produksi expression
unique_ptr<ParseNode> Parser::expression(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<expression>"));

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

//Fungsi untuk aturan produksi simpleExpression
unique_ptr<ParseNode> Parser::simpleExpression(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<simple-expression>"));

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

//Fungsi untuk aturan produksi term
unique_ptr<ParseNode> Parser::term(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<term>"));

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

//Fungsi untuk aturan produksi factor
unique_ptr<ParseNode> Parser::factor(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<factor>"));

    if (currToken == TokenType::IDENT){
        if (pos + 1 < (int) parserTokens.size() && parserTokens[pos + 1].type == TokenType::LPARENT){
            node->addChild(Parser::procedureFunctionCall());
        } else if ((pos + 1 < (int) parserTokens.size() && parserTokens[pos + 1].type == TokenType::LBRACK)
                    || (pos + 1 < (int) parserTokens.size() && parserTokens[pos + 1].type == TokenType::PERIOD)) { // Asumsi kalau IDENT saja masuk ke token IDENT
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

//Fungsi untuk aturan produksi relationalOperator
unique_ptr<ParseNode> Parser::relationalOperator(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<relational-operator>"));

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

//Fungsi untuk aturan produksi additiveOperator
unique_ptr<ParseNode> Parser::additiveOperator(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<additive-operator>"));

    node->addChild(match(vector<TokenType>{
        TokenType::PLUS,
        TokenType::MINUS,
        TokenType::ORSY
    }));

    return node;
}

//Fungsi untuk aturan produksi multiplicativeOperator
unique_ptr<ParseNode> Parser::multiplicativeOperator(){
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<multiplicative-operator>"));

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
//Fungsi untuk aturan produksi variable
unique_ptr<ParseNode> Parser::variable() {
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<variable>"));

    node->addChild(match(vector<TokenType> {
        TokenType::IDENT
    }));

    while(currToken == TokenType::LBRACK || currToken == TokenType::PERIOD) {
        node->addChild(componentVariable());
    }

    return node;

};

//Fungsi untuk aturan produksi componentVariable
unique_ptr<ParseNode> Parser::componentVariable() {
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<component-variable>"));

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

//Fungsi untuk aturan produksi 
unique_ptr<ParseNode> Parser::indexList() {
    unique_ptr<ParseNode> node = std::make_unique<ParseNode>(ParseNode("<index-list>"));

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