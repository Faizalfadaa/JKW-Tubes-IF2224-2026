#pragma once

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
        unique_ptr<ParseNode> match(TokenType expectedToken);
        unique_ptr<ParseNode> match(std::vector<TokenType> expectedTokens);
        unique_ptr<ParseNode> error(TokenType expectedToken, TokenType found);
        unique_ptr<ParseNode> error(std::vector<TokenType> expectedTokens, TokenType found);

        unique_ptr<ParseNode> program();
        unique_ptr<ParseNode> programHeader();
        unique_ptr<ParseNode> declarationPart();
        unique_ptr<ParseNode> constDeclaration();
        unique_ptr<ParseNode> constant();
        unique_ptr<ParseNode> typeDeclaration();
        unique_ptr<ParseNode> varDeclaration();
        unique_ptr<ParseNode> identifierList();
        unique_ptr<ParseNode> type();
        unique_ptr<ParseNode> arrayType();
        
        unique_ptr<ParseNode> range();
        unique_ptr<ParseNode> enumerated();
        unique_ptr<ParseNode> recordType();
        unique_ptr<ParseNode> fieldList();
        unique_ptr<ParseNode> fieldPart();
        unique_ptr<ParseNode> subProgramDeclaration();
        unique_ptr<ParseNode> procedureDeclaration();
        unique_ptr<ParseNode> functionDeclaration();
        unique_ptr<ParseNode> block();
        unique_ptr<ParseNode> formalParameterList();
        
        unique_ptr<ParseNode> parameterGroup();
        unique_ptr<ParseNode> compoundStatement();
        unique_ptr<ParseNode> statementList();
        unique_ptr<ParseNode> statement();
        unique_ptr<ParseNode> assignmentStatement();
        unique_ptr<ParseNode> ifStatement();
        unique_ptr<ParseNode> caseStatement();
        unique_ptr<ParseNode> caseBlock();
        unique_ptr<ParseNode> whileStatement();
        unique_ptr<ParseNode> repeatStatement();
        
        unique_ptr<ParseNode> forStatement();
        unique_ptr<ParseNode> procedureFunctionCall();
        unique_ptr<ParseNode> parameterList();
        unique_ptr<ParseNode> expression();
        unique_ptr<ParseNode> simpleExpression();
        unique_ptr<ParseNode> term();
        unique_ptr<ParseNode> factor();
        unique_ptr<ParseNode> relationalOperator();
        unique_ptr<ParseNode> additiveOperator();
        unique_ptr<ParseNode> multiplicativeOperator();

        // Tambahan Method dari revisi 
        unique_ptr<ParseNode> variable();
        unique_ptr<ParseNode> componentVariable();
        unique_ptr<ParseNode> indexList();

};


