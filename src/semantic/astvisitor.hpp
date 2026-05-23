#pragma once

#include "astnode.hpp"

class ASTVisitor {
public:
    // Struktur program / block
    virtual void visit(ProgramNode* node);
    virtual void visit(CompoundNode* node);

    // Declaration nodes
    virtual void visit(ConstDeclNode* node);
    virtual void visit(TypeDeclNode* node);
    virtual void visit(VarDeclNode* node);
    virtual void visit(ParamNode* node);
    virtual void visit(SubprogramDeclNode* node);

    // Type nodes
    virtual void visit(NamedTypeNode* node);
    virtual void visit(ArrayTypeNode* node);
    virtual void visit(RecordTypeNode* node);
    virtual void visit(RangeNode* node);
    virtual void visit(EnumNode* node);

    // Statement nodes
    virtual void visit(AssignNode* node);
    virtual void visit(IfNode* node);
    virtual void visit(CaseNode* node);
    virtual void visit(CaseBlockNode* node);
    virtual void visit(WhileNode* node);
    virtual void visit(RepeatNode* node);
    virtual void visit(ForNode* node);
    virtual void visit(ProcCallNode* node);

    // Variable / access nodes
    virtual void visit(VarNode* node);
    virtual void visit(ArrayAccessNode* node);
    virtual void visit(RecordAccessNode* node);

    // Expression / literal nodes
    virtual void visit(BinOpNode* node);
    virtual void visit(UnaryOpNode* node);
    virtual void visit(NumberNode* node);
    virtual void visit(StringNode* node);
    virtual void visit(CharNode* node);
};