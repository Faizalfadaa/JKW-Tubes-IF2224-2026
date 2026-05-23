#pragma once

#ifndef ASTVISITOR_HPP
#define ASTVISITOR_HPP

#include "astnode.hpp"

class ASTVisitor {
public:

    // Struktur program / block
    virtual void visitProgramNode(ProgramNode* node) = 0;
    virtual void visitCompoundNode(CompoundNode* node) = 0;

    // Declaration nodes
    virtual void visitConstDeclNode(ConstDeclNode* node) = 0;
    virtual void visitTypeDeclNode(TypeDeclNode* node) = 0;
    virtual void visitVarDeclNode(VarDeclNode* node) = 0;
    virtual void visitParamNode(ParamNode* node) = 0;
    virtual void visitSubprogramDeclNode(SubprogramDeclNode* node) = 0;

    // Type nodes
    virtual void visitNamedTypeNode(NamedTypeNode* node) = 0;
    virtual void visitArrayTypeNode(ArrayTypeNode* node) = 0;
    virtual void visitRecordTypeNode(RecordTypeNode* node) = 0;
    virtual void visitRangeNode(RangeNode* node) = 0;
    virtual void visitEnumNode(EnumNode* node) = 0;

    // Statement nodes
    virtual void visitAssignNode(AssignNode* node) = 0;
    virtual void visitIfNode(IfNode* node) = 0;
    virtual void visitCaseNode(CaseNode* node) = 0;
    virtual void visitCaseBlockNode(CaseBlockNode* node) = 0;
    virtual void visitWhileNode(WhileNode* node) = 0;
    virtual void visitRepeatNode(RepeatNode* node) = 0;
    virtual void visitForNode(ForNode* node) = 0;
    virtual void visitProcCallNode(ProcCallNode* node) = 0;

    // Variable / access nodes
    virtual void visitVarNode(VarNode* node) = 0;
    virtual void visitArrayAccessNode(ArrayAccessNode* node) = 0;
    virtual void visitRecordAccessNode(RecordAccessNode* node) = 0;

    // Expression / literal nodes
    virtual void visitBinOpNode(BinOpNode* node) = 0;
    virtual void visitUnaryOpNode(UnaryOpNode* node) = 0;
    virtual void visitNumberNode(NumberNode* node) = 0;
    virtual void visitStringNode(StringNode* node) = 0;
    virtual void visitCharNode(CharNode* node) = 0;
};

#endif