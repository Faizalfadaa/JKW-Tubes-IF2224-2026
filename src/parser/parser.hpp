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
        ParseNode* match(std::vector<TokenType> expectedTokens);

        ParseNode* program();
        ParseNode* programHeader();
        ParseNode* declarationPart();
        ParseNode* constDeclaration();
        ParseNode* constant();
        ParseNode* typeDeclaration();
        ParseNode* varDeclaration();
        ParseNode* identifierList();
        ParseNode* type();
        ParseNode* arrayType();
        
        ParseNode* range();
        ParseNode* enumerated();
        ParseNode* recordType();
        ParseNode* fieldList();
        ParseNode* fieldPart();
        ParseNode* subProgramDeclaration();
        ParseNode* procedureDeclaration();
        ParseNode* functionDeclaration();
        ParseNode* block();
        ParseNode* formalParameterList();
        
        ParseNode* parameterGroup();
        ParseNode* compoundStatement();
        ParseNode* statementList();
        ParseNode* statement();
        ParseNode* assignmentStatement();
        ParseNode* ifStatement();
        ParseNode* caseStatement();
        ParseNode* caseBlock();
        ParseNode* whileStatement();
        ParseNode* repeatStatement();
        
        ParseNode* forStatement();
        ParseNode* procedureFunctionCall();
        ParseNode* parameterList();
        ParseNode* expression();
        ParseNode* simpleExpression();
        ParseNode* term();
        ParseNode* factor();
        ParseNode* relationalOperator();
        ParseNode* additiveOperator();
        ParseNode* multiplicativeOperator();

        // Tambahan Method dari revisi 
        ParseNode* variable();
        ParseNode* componentVariable();
        ParseNode* indexList();

};


