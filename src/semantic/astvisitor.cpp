#include "astvisitor.hpp"
#include "../utils/exception.hpp"

using namespace std;

ASTVisitor::ASTVisitor(){
    symtab = SymbolTable();
}

SymbolTable& ASTVisitor::getSymbolTable() {
    return symtab;
}

bool ASTVisitor::isRelationalOperator(const string& op) const {
    return op == "=" ||
           op == "<>" ||
           op == "<" ||
           op == "<=" ||
           op == ">" ||
           op == ">=";
}

bool ASTVisitor::isBooleanOperator(const string& op) const {
    return op == "and" || op == "or";
}

bool ASTVisitor::isArithmeticOperator(const string& op) const {
    return op == "+" ||
           op == "-" ||
           op == "*" ||
           op == "/" ||
           op == "div" ||
           op == "mod";
}

bool ASTVisitor::isAssignmentCompatible(BaseType targetType, BaseType valueType) const {
    if (targetType == valueType) return true;
    if (targetType == BaseType::REAL && valueType == BaseType::INTEGER) return true;
    if (targetType == BaseType::SUBRANGE && valueType == BaseType::INTEGER) return true;
    if (targetType == BaseType::INTEGER && valueType == BaseType::SUBRANGE) return true;
    if (targetType == BaseType::UNKNOWN || valueType == BaseType::UNKNOWN) return true;
    return false;
}

void ASTVisitor::visit(ProgramNode* node) {
    if (!node) throw SemanticError("Invalid ProgramNode");

    for (auto& decl : node->declarations){
        if (decl) {
            decl->accept(this);
        }
    }

    if (node->mainBlock){
        node->mainBlock->accept(this);
    }   
}

void ASTVisitor::visit(CompoundNode* node) {
    if (!node) throw SemanticError("Invalid CompoundNode");

    node->lev = symtab.getCurrentLevel();

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
    if (!node) throw SemanticError("Invalid ConstDeclNode");

    if (node->value){
        node->value->accept(this);
    }
    node->exprType = node->value->exprType;

    TabEntry* entry = symtab.insertTab(node->constName, SymbolType::CONSTANT, node->exprType);
    node->tabIndex = symtab.indexOf(entry);
    node->lev = entry->lev;
}

void ASTVisitor::visit(TypeDeclNode* node) {
    if (!node) throw SemanticError("Invalid TypeDeclNode");

    if (node->typeDef){
        node->typeDef->accept(this);
    }
    node->exprType = node->typeDef->exprType;
    
    TabEntry* entry = symtab.insertTab(node->typeName, SymbolType::TYPE, node->exprType);
    node->tabIndex = symtab.indexOf(entry);
    node->lev = entry->lev;
}

void ASTVisitor::visit(VarDeclNode* node) {
    if (!node) throw SemanticError("Invalid VarDeclNode");

    if (node->typeDef){
        node->typeDef->accept(this);
    }
    node->exprType = node->typeDef->exprType;

    TabEntry* lastEntry = nullptr;

    for(auto& name : node->varNames){
        lastEntry = symtab.insertTab(name, SymbolType::VARIABLE, node->exprType);
    }

    if (lastEntry){
        node->tabIndex = symtab.indexOf(lastEntry);
        node->lev = lastEntry->lev;
    }
}

void ASTVisitor::visit(ParamNode* node) {
    if (!node) throw SemanticError("Invalid ParamNode");

    if (node->typeDef){
        node->typeDef->accept(this);
    }
    node->exprType = node->typeDef->exprType;

    TabEntry* lastEntry = nullptr;
    for(auto& name : node->paramNames){
        symtab.insertTab(name, SymbolType::VARIABLE, node->exprType, !node->isVarParam);
    }

    if (lastEntry){
        node->tabIndex = symtab.indexOf(lastEntry);
        node->lev = lastEntry->lev;
    }
}

void ASTVisitor::visit(SubprogramDeclNode* node) {
    if (!node) throw SemanticError("Invalid SubprogramDeclNode");
    
    BaseType returnType = BaseType::VOID;

    if (node->returnType){
        node->returnType->accept(this);
    }
    returnType = node->returnType->exprType;
    node->exprType = returnType;

    TabEntry* entry = symtab.insertTab(
        node->subprogramName,
        node->isFunction ? SymbolType::FUNCTION : SymbolType::PROCEDURE,
        node->exprType
    );

    node->tabIndex = symtab.indexOf(entry);
    node->lev = entry->lev;

    symtab.enterScope();

    for (auto& param : node->parameters) {
        if (param) param->accept(this);
    }

    for (auto& decl : node->declarations) {
        if (decl) decl->accept(this);
    }

    if (node->body) {
        node->body->accept(this);
    }

    symtab.leaveScope();
}

// ====================
// Type Nodes
// ====================

void ASTVisitor::visit(NamedTypeNode* node) {
    if (!node) throw SemanticError("Invalid NamedTypeNode");

    TabEntry* entry = symtab.lookup(node->typeName);

    if (entry && entry->obj == SymbolType::TYPE){
        node->exprType = entry->type;
        node->tabIndex = symtab.indexOf(entry);
        node->lev = entry->lev;
        return;
    }

    BaseType builtin = symtab.toBaseType(node->typeName);
    if (builtin != BaseType::UNKNOWN) {
        node->exprType = builtin;
        node->lev = symtab.getCurrentLevel();
        return;
    }

    throw SemanticError("Unknown type: " + node->typeName);
}

void ASTVisitor::visit(ArrayTypeNode* node) {
    if (!node) throw SemanticError("Invalid ArrayTypeNode");
    if (!node->indexType) throw SemanticError("ArrayTypeNode has null indexType");
    if (!node->elementType) throw SemanticError("ArrayTypeNode has null elementType");

    if (node->indexType->exprType == BaseType::REAL) {
        throw SemanticError("Array index type cannot be real");
    }

    ATabEntry* entry = symtab.insertATab(
        node->indexType->exprType,
        node->elementType->exprType,
        0,
        0
    );

    node->exprType = BaseType::ARRAY;
    node->tabIndex = symtab.indexOf(entry);
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(RecordTypeNode* node) {
    if (!node) throw SemanticError("Invalid RecordTypeNode");

    symtab.enterScope();

    for (auto& field : node->fields) {
        if (field) field->accept(this);
    }

    symtab.leaveScope();
    BTabEntry* entry = symtab.insertBTab();
    node->exprType = BaseType::RECORD;
    node->tabIndex = symtab.indexOf(entry);
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(RangeNode* node) {
    if (!node) throw SemanticError("Invalid RangeNode");
    if (!node->lowerBound) throw SemanticError("RangeNode has null lowerBound");
    if (!node->upperBound) throw SemanticError("RangeNode has null upperBound");

    node->lowerBound->accept(this);
    node->upperBound->accept(this);

    if (node->lowerBound->exprType == BaseType::REAL ||
        node->upperBound->exprType == BaseType::REAL){
        throw SemanticError("Subrange cannot use real bound");
    }

    node->exprType = BaseType::SUBRANGE;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(EnumNode* node) {
    if (!node) throw SemanticError("Invalid EnumNode");

    node->exprType = BaseType::ENUM;
    node->lev = symtab.getCurrentLevel();

    TabEntry* lastEntry = nullptr;

    for (auto& identifier : node->identifiers) {
        lastEntry = symtab.insertTab(
            identifier,
            SymbolType::CONSTANT,
            BaseType::ENUM
        );
    }

    if (lastEntry) {
        node->tabIndex = symtab.indexOf(lastEntry);
    }
}

// ====================
// Statement Nodes
// ====================

void ASTVisitor::visit(AssignNode* node) {
    if (!node) throw SemanticError("Invalid AssignNode");
    if (!node->target) throw SemanticError("AssignNode has null target");
    if (!node->value) throw SemanticError("AssignNode has null value");

    node->target->accept(this);
    node->value->accept(this);

    if (!isAssignmentCompatible(node->target->exprType, node->value->exprType)) {
        throw SemanticError("Assignment incompatible type");
    }

    node->exprType = BaseType::VOID;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(IfNode* node) {
    if (!node) throw SemanticError("Invalid IfNode");
    if (!node->condition) throw SemanticError("IfNode has null condition");

    node->condition->accept(this);

    if (node->condition->exprType != BaseType::BOOLEAN &&
        node->condition->exprType != BaseType::UNKNOWN) {
        throw SemanticError("If condition must be boolean");
    }

    if (node->thenBlock) node->thenBlock->accept(this);
    if (node->elseBlock) node->elseBlock->accept(this);

    node->exprType = BaseType::VOID;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(CaseNode* node) {
    if (!node) throw SemanticError("Invalid CaseNode");

    if (node->condition) node->condition->accept(this);

    for (auto& c : node->cases) {
        if (c) c->accept(this);
    }

    node->exprType = BaseType::VOID;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(CaseBlockNode* node) {
    if (!node) throw SemanticError("Invalid CaseBlockNode");

    for (auto& c : node->constants) {
        if (c) c->accept(this);
    }

    if (node->statement) node->statement->accept(this);

    node->exprType = BaseType::VOID;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(WhileNode* node) {
    if (!node) throw SemanticError("Invalid WhileNode");
    if (!node->condition) throw SemanticError("WhileNode has null condition");

    node->condition->accept(this);

    if (node->condition->exprType != BaseType::BOOLEAN &&
        node->condition->exprType != BaseType::UNKNOWN) {
        throw SemanticError("While condition must be boolean");
    }

    if (node->loopBlock) node->loopBlock->accept(this);

    node->exprType = BaseType::VOID;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(RepeatNode* node) {
    if (!node) throw SemanticError("Invalid RepeatNode");

    for (auto& stmt : node->statements) {
        if (stmt) stmt->accept(this);
    }

    if (!node->condition) throw SemanticError("RepeatNode has null condition");

    node->condition->accept(this);

    if (node->condition->exprType != BaseType::BOOLEAN &&
        node->condition->exprType != BaseType::UNKNOWN) {
        throw SemanticError("Repeat until condition must be boolean");
    }

    node->exprType = BaseType::VOID;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(ForNode* node) {
    if (!node) throw SemanticError("Invalid ForNode");

    TabEntry* iteratorEntry = symtab.lookup(node->iteratorName);

    if (!iteratorEntry) {
        throw SemanticError("Undeclared for iterator: " + node->iteratorName);
    }

    if (node->startValue) node->startValue->accept(this);
    if (node->endValue) node->endValue->accept(this);
    if (node->loopBlock) node->loopBlock->accept(this);

    node->exprType = BaseType::VOID;
    node->tabIndex = symtab.indexOf(iteratorEntry);
    node->lev = iteratorEntry->lev;
}

void ASTVisitor::visit(ProcCallNode* node) {
    if (!node) throw SemanticError("Invalid ProcCallNode");

    TabEntry* entry = symtab.lookup(node->procName);

    if (!entry) {
        throw SemanticError("Undeclared procedure/function: " + node->procName);
    }

    for (auto& arg : node->arguments) {
        if (arg) arg->accept(this);
    }

    node->exprType = entry->type;
    node->tabIndex = symtab.indexOf(entry);
    node->lev = entry->lev;
}

// ====================
// Variable Nodes
// ====================

void ASTVisitor::visit(VarNode* node) {
    if (!node) throw SemanticError("Invalid VarNode");

    TabEntry* entry = symtab.lookup(node->name);

    if (!entry) {
        throw SemanticError("Undeclared identifier: " + node->name);
    }

    node->exprType = entry->type;
    node->tabIndex = symtab.indexOf(entry);
    node->lev = entry->lev;
}

void ASTVisitor::visit(ArrayAccessNode* node) {
    if (!node) throw SemanticError("Invalid ArrayAccessNode");
    if (!node->arrayVar) throw SemanticError("ArrayAccessNode has null arrayVar");

    node->arrayVar->accept(this);

    for (auto& idx : node->indices) {
        if (idx) idx->accept(this);
    }

    node->exprType = BaseType::UNKNOWN;
    node->tabIndex = node->arrayVar->tabIndex;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(RecordAccessNode* node) {
    if (!node) throw SemanticError("Invalid RecordAccessNode");
    if (!node->recordVar) throw SemanticError("RecordAccessNode has null recordVar");

    node->recordVar->accept(this);

    node->exprType = BaseType::UNKNOWN;
    node->tabIndex = node->recordVar->tabIndex;
    node->lev = symtab.getCurrentLevel();
}

// ====================
// Expression Nodes
// ====================

void ASTVisitor::visit(BinOpNode* node) {
    if (!node) throw SemanticError("Invalid BinOpNode");
    if (!node->left) throw SemanticError("BinOpNode has null left");
    if (!node->right) throw SemanticError("BinOpNode has null right");

    node->left->accept(this);
    node->right->accept(this);

    BaseType leftType = node->left->exprType;
    BaseType rightType = node->right->exprType;

    if (isRelationalOperator(node->op)) {
        node->exprType = BaseType::BOOLEAN;
    }
    else if (isBooleanOperator(node->op)) {
        if (leftType != BaseType::BOOLEAN || rightType != BaseType::BOOLEAN) {
            throw SemanticError("Boolean operator requires boolean operands");
        }

        node->exprType = BaseType::BOOLEAN;
    }
    else if (isArithmeticOperator(node->op)) {
        if (node->op == "/" ||
            leftType == BaseType::REAL ||
            rightType == BaseType::REAL) {
            node->exprType = BaseType::REAL;
        }
        else {
            node->exprType = BaseType::INTEGER;
        }
    }
    else {
        node->exprType = BaseType::UNKNOWN;
    }

    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(UnaryOpNode* node) {
    if (!node) throw SemanticError("Invalid UnaryOpNode");
    if (!node->operand) throw SemanticError("UnaryOpNode has null operand");

    node->operand->accept(this);

    if (node->op == "not") {
        if (node->operand->exprType != BaseType::BOOLEAN &&
            node->operand->exprType != BaseType::UNKNOWN) {
            throw SemanticError("not operator requires boolean operand");
        }

        node->exprType = BaseType::BOOLEAN;
    }
    else {
        node->exprType = node->operand->exprType;
    }

    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(NumberNode* node) {
    if (!node) throw SemanticError("Invalid NumberNode");

    node->exprType = node->isReal ? BaseType::REAL : BaseType::INTEGER;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(StringNode* node) {
    if (!node) throw SemanticError("Invalid StringNode");

    node->exprType = BaseType::STRING;
    node->lev = symtab.getCurrentLevel();
}

void ASTVisitor::visit(CharNode* node) {
    if (!node) throw SemanticError("Invalid CharNode");

    node->exprType = BaseType::CHAR;
    node->lev = symtab.getCurrentLevel();
}