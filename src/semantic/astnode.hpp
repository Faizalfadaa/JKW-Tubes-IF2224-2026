#pragma once

#include <string>
#include <vector>
#include <utility>
#include <memory>
#include "symboltable.hpp"

// Forward declaration
class ASTVisitor;

// Abstract Base Class

class ASTNode {
public:
    BaseType exprType = BaseType::UNKNOWN;
    int tabIndex = -1;  // indeks ke tab di symbol table
    int lev      = -1;  // lexical level

    virtual ~ASTNode() = default;
    virtual void accept(ASTVisitor* visitor) = 0;
};


// STRUKTUR PROGRAM

class ProgramNode : public ASTNode {
public:
    std::string programName;
    std::vector<std::unique_ptr<ASTNode>> declarations;
    std::unique_ptr<ASTNode> mainBlock;

    ProgramNode(std::string name, std::vector<std::unique_ptr<ASTNode>> decls, std::unique_ptr<ASTNode> block);
    void accept(ASTVisitor* visitor) override;
};

class CompoundNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;

    CompoundNode(std::vector<std::unique_ptr<ASTNode>> stmts);
    void accept(ASTVisitor* visitor) override;
};


// DEKLARASI

class ConstDeclNode : public ASTNode {
public:
    std::string constName;
    std::unique_ptr<ASTNode> value;

    ConstDeclNode(std::string name, std::unique_ptr<ASTNode> val);
    void accept(ASTVisitor* visitor) override;
};

class TypeDeclNode : public ASTNode {
public:
    std::string typeName;
    std::unique_ptr<ASTNode> typeDef;

    TypeDeclNode(std::string name, std::unique_ptr<ASTNode> def);
    void accept(ASTVisitor* visitor) override;
};

class VarDeclNode : public ASTNode {
public:
    std::vector<std::string> varNames;
    std::unique_ptr<ASTNode> typeDef;

    VarDeclNode(std::vector<std::string> names, std::unique_ptr<ASTNode> def);
    void accept(ASTVisitor* visitor) override;
};

class ParamNode : public ASTNode {
public:
    std::vector<std::string> paramNames;
    std::unique_ptr<ASTNode> typeDef;
    bool isVarParam;

    ParamNode(std::vector<std::string> names, std::unique_ptr<ASTNode> def, bool isVar);
    void accept(ASTVisitor* visitor) override;
};

class SubprogramDeclNode : public ASTNode {
public:
    bool isFunction;
    std::string subprogramName;
    std::vector<std::unique_ptr<ASTNode>> parameters;
    std::unique_ptr<ASTNode> returnType;  // nullptr jika procedure
    std::vector<std::unique_ptr<ASTNode>> declarations;
    std::unique_ptr<ASTNode> body;

    SubprogramDeclNode(bool isFunc, std::string name, std::vector<std::unique_ptr<ASTNode>> params,
                       std::unique_ptr<ASTNode> retType, std::vector<std::unique_ptr<ASTNode>> decls,
                       std::unique_ptr<ASTNode> bodyBlock);
    void accept(ASTVisitor* visitor) override;
};


// DEFINISI TIPE

class NamedTypeNode : public ASTNode {
public:
    std::string typeName;

    NamedTypeNode(std::string name);
    void accept(ASTVisitor* visitor) override;
};

class ArrayTypeNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> indexType;
    std::unique_ptr<ASTNode> elementType;

    ArrayTypeNode(std::unique_ptr<ASTNode> idxType, std::unique_ptr<ASTNode> elemType);
    void accept(ASTVisitor* visitor) override;
};

class RecordTypeNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> fields;

    RecordTypeNode(std::vector<std::unique_ptr<ASTNode>> recFields);
    void accept(ASTVisitor* visitor) override;
};

class RangeNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> lowerBound;
    std::unique_ptr<ASTNode> upperBound;

    RangeNode(std::unique_ptr<ASTNode> low, std::unique_ptr<ASTNode> high);
    void accept(ASTVisitor* visitor) override;
};

class EnumNode : public ASTNode {
public:
    std::vector<std::string> identifiers;

    EnumNode(std::vector<std::string> ids);
    void accept(ASTVisitor* visitor) override;
};


// STATEMENT

class AssignNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> target;
    std::unique_ptr<ASTNode> value;

    AssignNode(std::unique_ptr<ASTNode> tgt, std::unique_ptr<ASTNode> val);
    void accept(ASTVisitor* visitor) override;
};

class IfNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> thenBlock;
    std::unique_ptr<ASTNode> elseBlock;  // nullptr jika tidak ada else

    IfNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenBlk, std::unique_ptr<ASTNode> elseBlk);
    void accept(ASTVisitor* visitor) override;
};

class CaseBlockNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> constants;
    std::unique_ptr<ASTNode> statement;

    CaseBlockNode(std::vector<std::unique_ptr<ASTNode>> consts, std::unique_ptr<ASTNode> stmt);
    void accept(ASTVisitor* visitor) override;
};

class CaseNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::vector<std::unique_ptr<ASTNode>> cases;

    CaseNode(std::unique_ptr<ASTNode> cond, std::vector<std::unique_ptr<ASTNode>> caseList);
    void accept(ASTVisitor* visitor) override;
};

class WhileNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> loopBlock;

    WhileNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> block);
    void accept(ASTVisitor* visitor) override;
};

class RepeatNode : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode>> statements;
    std::unique_ptr<ASTNode> condition;

    RepeatNode(std::vector<std::unique_ptr<ASTNode>> stmts, std::unique_ptr<ASTNode> cond);
    void accept(ASTVisitor* visitor) override;
};

class ForNode : public ASTNode {
public:
    std::string iteratorName;
    std::unique_ptr<ASTNode> startValue;
    std::unique_ptr<ASTNode> endValue;
    bool isDownto;
    std::unique_ptr<ASTNode> loopBlock;

    ForNode(std::string iterName, std::unique_ptr<ASTNode> startVal, std::unique_ptr<ASTNode> endVal,
            bool downto, std::unique_ptr<ASTNode> block);
    void accept(ASTVisitor* visitor) override;
};

class ProcCallNode : public ASTNode {
public:
    std::string procName;
    std::vector<std::unique_ptr<ASTNode>> arguments;

    ProcCallNode(std::string name, std::vector<std::unique_ptr<ASTNode>> args);
    void accept(ASTVisitor* visitor) override;
};


// COMPONENT VARIABLE

class VarNode : public ASTNode {
public:
    std::string name;

    VarNode(std::string n);
    void accept(ASTVisitor* visitor) override;
};

class ArrayAccessNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> arrayVar;
    std::vector<std::unique_ptr<ASTNode>> indices;

    ArrayAccessNode(std::unique_ptr<ASTNode> arrVar, std::vector<std::unique_ptr<ASTNode>> idxs);
    void accept(ASTVisitor* visitor) override;
};

class RecordAccessNode : public ASTNode {
public:
    std::unique_ptr<ASTNode> recordVar;
    std::string fieldName;

    RecordAccessNode(std::unique_ptr<ASTNode> recVar, std::string fName);
    void accept(ASTVisitor* visitor) override;
};


// EXPRESSION & FACTOR

class BinOpNode : public ASTNode {
public:
    std::string op;  // "+", "-", "*", "/", "div", "mod", "and", "or", "=", "<>", "<", "<=", ">", ">="
    std::unique_ptr<ASTNode> left;
    std::unique_ptr<ASTNode> right;

    BinOpNode(std::string oper, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r);
    void accept(ASTVisitor* visitor) override;
};

class UnaryOpNode : public ASTNode {
public:
    std::string op;  // "+", "-", "not"
    std::unique_ptr<ASTNode> operand;

    UnaryOpNode(std::string oper, std::unique_ptr<ASTNode> expr);
    void accept(ASTVisitor* visitor) override;
};

class NumberNode : public ASTNode {
public:
    std::string value;
    bool isReal;

    NumberNode(std::string val, bool isR);
    void accept(ASTVisitor* visitor) override;
};

class StringNode : public ASTNode {
public:
    std::string value;

    StringNode(std::string val);
    void accept(ASTVisitor* visitor) override;
};

class CharNode : public ASTNode {
public:
    std::string value;

    CharNode(std::string val);
    void accept(ASTVisitor* visitor) override;
};