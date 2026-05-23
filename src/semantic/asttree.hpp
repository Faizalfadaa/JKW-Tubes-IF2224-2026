#pragma once

#include "astnode.hpp"
#include "../parser/parser.hpp"


class ASTBuilder {
public:
    std::unique_ptr<ASTNode> createAST(ParseNode* parseRoot);

private:
    // STRUKTUR PROGRAM
    std::unique_ptr<ASTNode> makeProgramNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeCompoundNode(ParseNode* node);
    std::vector<std::unique_ptr<ASTNode>> makeStatementList(ParseNode* node);

    // DEKLARASI
    std::vector<std::unique_ptr<ASTNode>> makeDeclarationPart(ParseNode* node);
    std::vector<std::unique_ptr<ASTNode>> makeConstDeclarations(ParseNode* node);
    std::vector<std::unique_ptr<ASTNode>> makeTypeDeclarations(ParseNode* node);
    std::vector<std::unique_ptr<ASTNode>> makeVarDeclarations(ParseNode* node);
    std::vector<std::string> makeIdentifierList(ParseNode* node);
    std::unique_ptr<ASTNode> makeSubprogramDeclaration(ParseNode* node);
    std::vector<std::unique_ptr<ASTNode>> makeFormalParameterList(ParseNode* node);

    // TIPE DATA
    std::unique_ptr<ASTNode> makeTypeNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeConstantNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeArrayTypeNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeRangeNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeEnumNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeRecordTypeNode(ParseNode* node);

    // STATEMENT & COMPONENT VARIABLE
    std::unique_ptr<ASTNode> makeStatementNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeAssignNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeIfNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeCaseNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeCaseBlockNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeWhileNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeRepeatNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeForNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeProcCallNode(ParseNode* node);

    std::unique_ptr<ASTNode> makeVariableNode(ParseNode* node);
    std::vector<std::unique_ptr<ASTNode>> makeIndexList(ParseNode* node);
    std::vector<std::unique_ptr<ASTNode>> makeParameterList(ParseNode* node);

    // EXPRESSION & FACTOR
    std::unique_ptr<ASTNode> makeExpressionNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeSimpleExpressionNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeTermNode(ParseNode* node);
    std::unique_ptr<ASTNode> makeFactorNode(ParseNode* node);
};