#include "astvisitor.hpp"
#include "../utils/exception.hpp"

using namespace std;

ASTVisitor::ASTVisitor(){
    symtab = SymbolTable();
}

void ASTVisitor::visit(ProgramNode* node) {
    if (!node) throw SemanticError("Invalid node");

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
    if (!node) throw SemanticError("Invalid node");

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
    if (!node) throw SemanticError("Invalid node");

    if (node->value) {
        node->value->accept(this);
    }

    symtab.insertTab(node->constName, SymbolType::CONSTANT, node->value->exprType);
}

void ASTVisitor::visit(TypeDeclNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->typeDef) {
        node->typeDef->accept(this);
    }
    
    symtab.insertTab(node->typeName, SymbolType::TYPE, node->typeDef->exprType);
}

void ASTVisitor::visit(VarDeclNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->typeDef) {
        node->typeDef->accept(this);
    }

    BaseType baseType = node->typeDef->exprType;
    for(auto& name : node->varNames){
        symtab.insertTab(name, SymbolType::VARIABLE, baseType);
    }
}

void ASTVisitor::visit(ParamNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->typeDef) {
        node->typeDef->accept(this);
    }

    BaseType baseType = node->typeDef->exprType;
    for(auto& name : node->paramNames){
        symtab.insertTab(name, SymbolType::VARIABLE, baseType, false);
    }
}

void ASTVisitor::visit(SubprogramDeclNode* node) {
    if (!node) throw SemanticError("Invalid node");
    
    symtab.insertTab(node->subprogramName, node->isFunction ? SymbolType::FUNCTION : SymbolType::PROCEDURE, node->exprType);
    symtab.enterScope();

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
    
    symtab.insertBTab();
}

// ====================
// Type Nodes
// ====================

void ASTVisitor::visit(NamedTypeNode* node) {
    if (!node) throw SemanticError("Invalid node"); //FIXME
}

void ASTVisitor::visit(ArrayTypeNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->indexType) {
        node->indexType->accept(this);
    }

    if (node->elementType) {
        node->elementType->accept(this);
    }

    BaseType xtype = node->indexType.get()->exprType;
    BaseType etype = node->elementType.get()->exprType;
    int low = 0, high = 0; //TODO: asumsi low dan high adalah 0 jika index type != subrange
    if (xtype == BaseType::SUBRANGE){
        low = 0; //TODO: simpan low dan high di node array
        high = 0;
    }

    symtab.insertATab(xtype, etype, low, high);
}

void ASTVisitor::visit(RecordTypeNode* node) {
    if (!node) throw SemanticError("Invalid node");

    for (auto& field : node->fields) {
        if (field) field->accept(this);
    }

    symtab.insertBTab();
}

void ASTVisitor::visit(RangeNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->lowerBound) {
        node->lowerBound->accept(this);
    }

    if (node->upperBound) {
        node->upperBound->accept(this);
    }
}

void ASTVisitor::visit(EnumNode* node) {
    if (!node) throw SemanticError("Invalid node");
}

// ====================
// Statement Nodes
// ====================

void ASTVisitor::visit(AssignNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->target) {
        node->target->accept(this);
    }

    if (node->value) {
        node->value->accept(this);
    }
}

void ASTVisitor::visit(IfNode* node) {
    if (!node) throw SemanticError("Invalid node");

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
    if (!node) throw SemanticError("Invalid node");

    if (node->condition) {
        node->condition->accept(this);
    }

    for (auto& c : node->cases) {
        if (c) c->accept(this);
    }
}

void ASTVisitor::visit(CaseBlockNode* node) {
    if (!node) throw SemanticError("Invalid node");

    for (auto& c : node->constants) {
        if (c) c->accept(this);
    }

    if (node->statement) {
        node->statement->accept(this);
    }
}

void ASTVisitor::visit(WhileNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->condition) {
        node->condition->accept(this);
    }

    if (node->loopBlock) {
        node->loopBlock->accept(this);
    }
}

void ASTVisitor::visit(RepeatNode* node) {
    if (!node) throw SemanticError("Invalid node");

    for (auto& stmt : node->statements) {
        if (stmt) stmt->accept(this);
    }

    if (node->condition) {
        node->condition->accept(this);
    }
}

void ASTVisitor::visit(ForNode* node) {
    if (!node) throw SemanticError("Invalid node");

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
    if (!node) throw SemanticError("Invalid node");

    for (auto& arg : node->arguments) {
        if (arg) arg->accept(this);
    }
}

// ====================
// Variable Nodes
// ====================

void ASTVisitor::visit(VarNode* node) {
    if (!node) throw SemanticError("Invalid node");
}

void ASTVisitor::visit(ArrayAccessNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->arrayVar) {
        node->arrayVar->accept(this);
    }

    for (auto& idx : node->indices) {
        if (idx) idx->accept(this);
    }
}

void ASTVisitor::visit(RecordAccessNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->recordVar) {
        node->recordVar->accept(this);
    }
}

// ====================
// Expression Nodes
// ====================

void ASTVisitor::visit(BinOpNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->left) {
        node->left->accept(this);
    }

    if (node->right) {
        node->right->accept(this);
    }
}

void ASTVisitor::visit(UnaryOpNode* node) {
    if (!node) throw SemanticError("Invalid node");

    if (node->operand) {
        node->operand->accept(this);
    }
}

void ASTVisitor::visit(NumberNode* node) {
    if (!node) throw SemanticError("Invalid node");
}

void ASTVisitor::visit(StringNode* node) {
    if (!node) throw SemanticError("Invalid node");
}

void ASTVisitor::visit(CharNode* node) {
    if (!node) throw SemanticError("Invalid node");
}