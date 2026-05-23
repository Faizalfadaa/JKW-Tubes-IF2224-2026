#include "astvisitor.hpp"

void ASTVisitor::visit(ProgramNode* node) {
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

void ASTVisitor::visit(CompoundNode* node) {
    if (!node) return;

    for (auto& stmt : node->statements) {
        if (stmt) {
            stmt->accept(this);
        }
    }
}

// ====================
// Declaration Nodes
// ====================

void ASTVisitor::visit(ConstDeclNode* node) {
    if (!node) return;

    if (node->value) {
        node->value->accept(this);
    }
}

void ASTVisitor::visit(TypeDeclNode* node) {
    if (!node) return;

    if (node->typeDef) {
        node->typeDef->accept(this);
    }
}

void ASTVisitor::visit(VarDeclNode* node) {
    if (!node) return;

    if (node->typeDef) {
        node->typeDef->accept(this);
    }
}

void ASTVisitor::visit(ParamNode* node) {
    if (!node) return;

    if (node->typeDef) {
        node->typeDef->accept(this);
    }
}

void ASTVisitor::visit(SubprogramDeclNode* node) {
    if (!node) return;

    for (auto& param : node->parameters) {
        if (param) param->accept(this);
    }

    if (node->returnType) {
        node->returnType->accept(this);
    }

    for (auto& decl : node->declarations) {
        if (decl) decl->accept(this);
    }

    if (node->body) {
        node->body->accept(this);
    }
}

// ====================
// Type Nodes
// ====================

void ASTVisitor::visit(NamedTypeNode* node) {
    if (!node) return;
}

void ASTVisitor::visit(ArrayTypeNode* node) {
    if (!node) return;

    if (node->indexType) {
        node->indexType->accept(this);
    }

    if (node->elementType) {
        node->elementType->accept(this);
    }
}

void ASTVisitor::visit(RecordTypeNode* node) {
    if (!node) return;

    for (auto& field : node->fields) {
        if (field) field->accept(this);
    }
}

void ASTVisitor::visit(RangeNode* node) {
    if (!node) return;

    if (node->lowerBound) {
        node->lowerBound->accept(this);
    }

    if (node->upperBound) {
        node->upperBound->accept(this);
    }
}

void ASTVisitor::visit(EnumNode* node) {
    if (!node) return;
}

// ====================
// Statement Nodes
// ====================

void ASTVisitor::visit(AssignNode* node) {
    if (!node) return;

    if (node->target) {
        node->target->accept(this);
    }

    if (node->value) {
        node->value->accept(this);
    }
}

void ASTVisitor::visit(IfNode* node) {
    if (!node) return;

    if (node->condition) {
        node->condition->accept(this);
    }

    if (node->thenBlock) {
        node->thenBlock->accept(this);
    }

    if (node->elseBlock) {
        node->elseBlock->accept(this);
    }
}

void ASTVisitor::visit(CaseNode* node) {
    if (!node) return;

    if (node->condition) {
        node->condition->accept(this);
    }

    for (auto& c : node->cases) {
        if (c) c->accept(this);
    }
}

void ASTVisitor::visit(CaseBlockNode* node) {
    if (!node) return;

    for (auto& c : node->constants) {
        if (c) c->accept(this);
    }

    if (node->statement) {
        node->statement->accept(this);
    }
}

void ASTVisitor::visit(WhileNode* node) {
    if (!node) return;

    if (node->condition) {
        node->condition->accept(this);
    }

    if (node->loopBlock) {
        node->loopBlock->accept(this);
    }
}

void ASTVisitor::visit(RepeatNode* node) {
    if (!node) return;

    for (auto& stmt : node->statements) {
        if (stmt) stmt->accept(this);
    }

    if (node->condition) {
        node->condition->accept(this);
    }
}

void ASTVisitor::visit(ForNode* node) {
    if (!node) return;

    if (node->startValue) {
        node->startValue->accept(this);
    }

    if (node->endValue) {
        node->endValue->accept(this);
    }

    if (node->loopBlock) {
        node->loopBlock->accept(this);
    }
}

void ASTVisitor::visit(ProcCallNode* node) {
    if (!node) return;

    for (auto& arg : node->arguments) {
        if (arg) arg->accept(this);
    }
}

// ====================
// Variable Nodes
// ====================

void ASTVisitor::visit(VarNode* node) {
    if (!node) return;
}

void ASTVisitor::visit(ArrayAccessNode* node) {
    if (!node) return;

    if (node->arrayVar) {
        node->arrayVar->accept(this);
    }

    for (auto& idx : node->indices) {
        if (idx) idx->accept(this);
    }
}

void ASTVisitor::visit(RecordAccessNode* node) {
    if (!node) return;

    if (node->recordVar) {
        node->recordVar->accept(this);
    }
}

// ====================
// Expression Nodes
// ====================

void ASTVisitor::visit(BinOpNode* node) {
    if (!node) return;

    if (node->left) {
        node->left->accept(this);
    }

    if (node->right) {
        node->right->accept(this);
    }
}

void ASTVisitor::visit(UnaryOpNode* node) {
    if (!node) return;

    if (node->operand) {
        node->operand->accept(this);
    }
}

void ASTVisitor::visit(NumberNode* node) {
    if (!node) return;
}

void ASTVisitor::visit(StringNode* node) {
    if (!node) return;
}

void ASTVisitor::visit(CharNode* node) {
    if (!node) return;
}