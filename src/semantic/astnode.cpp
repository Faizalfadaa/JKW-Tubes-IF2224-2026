#include "astnode.hpp"
#include "astvisitor.hpp"

ProgramNode::ProgramNode(std::string name, std::vector<std::unique_ptr<ASTNode>> decls, std::unique_ptr<ASTNode> block)
    : programName(std::move(name)), declarations(std::move(decls)), mainBlock(std::move(block)) {}
void ProgramNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

CompoundNode::CompoundNode(std::vector<std::unique_ptr<ASTNode>> stmts)
    : statements(std::move(stmts)) {}
void CompoundNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

ConstDeclNode::ConstDeclNode(std::string name, std::unique_ptr<ASTNode> val)
    : constName(std::move(name)), value(std::move(val)) {}
void ConstDeclNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

TypeDeclNode::TypeDeclNode(std::string name, std::unique_ptr<ASTNode> def)
    : typeName(std::move(name)), typeDef(std::move(def)) {}
void TypeDeclNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

VarDeclNode::VarDeclNode(std::vector<std::string> names, std::unique_ptr<ASTNode> def)
    : varNames(std::move(names)), typeDef(std::move(def)) {}
void VarDeclNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

ParamNode::ParamNode(std::vector<std::string> names, std::unique_ptr<ASTNode> def, bool isVar)
    : paramNames(std::move(names)), typeDef(std::move(def)), isVarParam(isVar) {}
void ParamNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

SubprogramDeclNode::SubprogramDeclNode(bool isFunc, std::string name,
                                       std::vector<std::unique_ptr<ASTNode>> params,
                                       std::unique_ptr<ASTNode> retType,
                                       std::vector<std::unique_ptr<ASTNode>> decls,
                                       std::unique_ptr<ASTNode> bodyBlock)
    : isFunction(isFunc), subprogramName(std::move(name)), parameters(std::move(params)),
      returnType(std::move(retType)), declarations(std::move(decls)), body(std::move(bodyBlock)) {}
void SubprogramDeclNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

NamedTypeNode::NamedTypeNode(std::string name)
    : typeName(std::move(name)) {}
void NamedTypeNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

ArrayTypeNode::ArrayTypeNode(std::unique_ptr<ASTNode> idxType, std::unique_ptr<ASTNode> elemType)
    : indexType(std::move(idxType)), elementType(std::move(elemType)) {}
void ArrayTypeNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

RecordTypeNode::RecordTypeNode(std::vector<std::unique_ptr<ASTNode>> recFields)
    : fields(std::move(recFields)) {}
void RecordTypeNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

RangeNode::RangeNode(std::unique_ptr<ASTNode> low, std::unique_ptr<ASTNode> high)
    : lowerBound(std::move(low)), upperBound(std::move(high)) {}
void RangeNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

EnumNode::EnumNode(std::vector<std::string> ids)
    : identifiers(std::move(ids)) {}
void EnumNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

AssignNode::AssignNode(std::unique_ptr<ASTNode> tgt, std::unique_ptr<ASTNode> val)
    : target(std::move(tgt)), value(std::move(val)) {}
void AssignNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

IfNode::IfNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> thenBlk, std::unique_ptr<ASTNode> elseBlk)
    : condition(std::move(cond)), thenBlock(std::move(thenBlk)), elseBlock(std::move(elseBlk)) {}
void IfNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

CaseBlockNode::CaseBlockNode(std::vector<std::unique_ptr<ASTNode>> consts, std::unique_ptr<ASTNode> stmt)
    : constants(std::move(consts)), statement(std::move(stmt)) {}
void CaseBlockNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

CaseNode::CaseNode(std::unique_ptr<ASTNode> cond, std::vector<std::unique_ptr<ASTNode>> caseList)
    : condition(std::move(cond)), cases(std::move(caseList)) {}
void CaseNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

WhileNode::WhileNode(std::unique_ptr<ASTNode> cond, std::unique_ptr<ASTNode> block)
    : condition(std::move(cond)), loopBlock(std::move(block)) {}
void WhileNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

RepeatNode::RepeatNode(std::vector<std::unique_ptr<ASTNode>> stmts, std::unique_ptr<ASTNode> cond)
    : statements(std::move(stmts)), condition(std::move(cond)) {}
void RepeatNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

ForNode::ForNode(std::string iterName, std::unique_ptr<ASTNode> startVal,
                 std::unique_ptr<ASTNode> endVal, bool downto, std::unique_ptr<ASTNode> block)
    : iteratorName(std::move(iterName)), startValue(std::move(startVal)),
      endValue(std::move(endVal)), isDownto(downto), loopBlock(std::move(block)) {}
void ForNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

ProcCallNode::ProcCallNode(std::string name, std::vector<std::unique_ptr<ASTNode>> args)
    : procName(std::move(name)), arguments(std::move(args)) {}
void ProcCallNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

VarNode::VarNode(std::string n)
    : name(std::move(n)) {}
void VarNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

ArrayAccessNode::ArrayAccessNode(std::unique_ptr<ASTNode> arrVar, std::vector<std::unique_ptr<ASTNode>> idxs)
    : arrayVar(std::move(arrVar)), indices(std::move(idxs)) {}
void ArrayAccessNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

RecordAccessNode::RecordAccessNode(std::unique_ptr<ASTNode> recVar, std::string fName)
    : recordVar(std::move(recVar)), fieldName(std::move(fName)) {}
void RecordAccessNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

BinOpNode::BinOpNode(std::string oper, std::unique_ptr<ASTNode> l, std::unique_ptr<ASTNode> r)
    : op(std::move(oper)), left(std::move(l)), right(std::move(r)) {}
void BinOpNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

UnaryOpNode::UnaryOpNode(std::string oper, std::unique_ptr<ASTNode> expr)
    : op(std::move(oper)), operand(std::move(expr)) {}
void UnaryOpNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

NumberNode::NumberNode(std::string val, bool isR)
    : value(std::move(val)), isReal(isR) {}
void NumberNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

StringNode::StringNode(std::string val)
    : value(std::move(val)) {}
void StringNode::accept(ASTVisitor* visitor) {visitor->visit(this);}

CharNode::CharNode(std::string val)
    : value(std::move(val)) {}
void CharNode::accept(ASTVisitor* visitor) {visitor->visit(this);}
