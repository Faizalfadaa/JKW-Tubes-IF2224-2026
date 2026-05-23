#include "astvisitor.hpp"

    void ASTVisitor::visitProgramNode(ProgramNode* node) {
        if (!node) return;

        for (auto& decl : node->declarations) {
            if (decl) {
                decl->accept(this);
            }
        }

        if (node->mainBlock) {
            node->mainBlock->accept(this);
        }
    }

    void ASTVisitor::visitCompoundNode(CompoundNode* node) {
        if (!node) return;

        for (auto& stmt : node->statements) {
            if (stmt) {
                stmt->accept(this);
            }
        }
    }