#include "astprinter.hpp"
#include "astnode.hpp"
#include "symboltable.hpp"
#include "../utils/utils.hpp"

using namespace std;

string printASTInternal(ASTNode* node, string indent, bool last, bool decorated);

string joinNames(const vector<string>& names) {
    ostringstream oss;
    for (size_t i = 0; i < names.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << names[i];
    }
    return oss.str();
}

string printAST(ASTNode* node, string indent, bool last) {
    return printASTInternal(node, indent, last, false);
}

string printDecoratedAST(ASTNode* node, string indent, bool last) {
    return printASTInternal(node, indent, last, true);
}

static string annotation(ASTNode* node, bool decorated) {
    if (!decorated || !node) return "";

    ostringstream oss;

    oss << "  {type: " << SymbolTable::toString(node->exprType)
        << ", tabIndex: " << node->tabIndex
        << ", lev: " << node->lev
        << "}";

    return oss.str();
}

string printASTInternal(ASTNode* node, string indent, bool last, bool decorated) {
    ostringstream oss;

    oss << indent;
    if (last) {
        oss << "\\-- ";
        indent += "    ";
    } else {
        oss << "|-- ";
        indent += "|   ";
    }

    if (node == nullptr) {
        oss << "<null>\n";
        return oss.str();
    }

    if (auto* n = dynamic_cast<ProgramNode*>(node)) {
        oss << "ProgramNode(name: '" << n->programName << "')" << annotation(node, decorated) << "\n";

        oss << indent << "|-- Declarations\n";
        for (size_t i = 0; i < n->declarations.size(); ++i) {
            oss << printASTInternal(n->declarations[i].get(), indent + "|   ", i == n->declarations.size() - 1, decorated);
        }

        oss << indent << "\\-- Block\n";
        oss << printASTInternal(n->mainBlock.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<CompoundNode*>(node)) {
        oss << "CompoundNode" << annotation(node, decorated) << "\n";

        for (size_t i = 0; i < n->statements.size(); ++i) {
            oss << printASTInternal(n->statements[i].get(), indent, i == n->statements.size() - 1, decorated);
        }
    }
    else if (auto* n = dynamic_cast<ConstDeclNode*>(node)) {
        oss << "ConstDeclNode(name: '" << n->constName << "')" << annotation(node, decorated) << "\n";
        oss << printASTInternal(n->value.get(), indent, true, decorated);
    }
    else if (auto* n = dynamic_cast<TypeDeclNode*>(node)) {
        oss << "TypeDeclNode(name: '" << n->typeName << "')" << annotation(node, decorated) << "\n";
        oss << printASTInternal(n->typeDef.get(), indent, true, decorated);
    }
    else if (auto* n = dynamic_cast<VarDeclNode*>(node)) {
        oss << "VarDeclNode(names: [" << joinNames(n->varNames) << "])" << annotation(node, decorated) << "\n";
        oss << printASTInternal(n->typeDef.get(), indent, true, decorated);
    }
    else if (auto* n = dynamic_cast<ParamNode*>(node)) {
        oss << "ParamNode(names: [" << joinNames(n->paramNames)
            << "], isVarParam: " << (n->isVarParam ? "true" : "false") << ")"
            << annotation(node, decorated) << "\n";

        oss << printASTInternal(n->typeDef.get(), indent, true, decorated);
    }
    else if (auto* n = dynamic_cast<SubprogramDeclNode*>(node)) {
        oss << "SubprogramDeclNode(kind: " << (n->isFunction ? "function" : "procedure")
            << ", name: '" << n->subprogramName << "')"
            << annotation(node, decorated) << "\n";

        oss << indent << "|-- Parameters\n";
        for (size_t i = 0; i < n->parameters.size(); ++i) {
            oss << printASTInternal(n->parameters[i].get(), indent + "|   ", i == n->parameters.size() - 1, decorated);
        }

        oss << indent << "|-- ReturnType\n";
        oss << printASTInternal(n->returnType.get(), indent + "|   ", true, decorated);

        oss << indent << "|-- Declarations\n";
        for (size_t i = 0; i < n->declarations.size(); ++i) {
            oss << printASTInternal(n->declarations[i].get(), indent + "|   ", i == n->declarations.size() - 1, decorated);
        }

        oss << indent << "\\-- Body\n";
        oss << printASTInternal(n->body.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<NamedTypeNode*>(node)) {
        oss << "NamedTypeNode(name: '" << n->typeName << "')" << annotation(node, decorated) << "\n";
    }
    else if (auto* n = dynamic_cast<ArrayTypeNode*>(node)) {
        oss << "ArrayTypeNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- IndexType\n";
        oss << printASTInternal(n->indexType.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- ElementType\n";
        oss << printASTInternal(n->elementType.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<RecordTypeNode*>(node)) {
        oss << "RecordTypeNode" << annotation(node, decorated) << "\n";

        for (size_t i = 0; i < n->fields.size(); ++i) {
            oss << printASTInternal(n->fields[i].get(), indent, i == n->fields.size() - 1, decorated);
        }
    }
    else if (auto* n = dynamic_cast<RangeNode*>(node)) {
        oss << "RangeNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- LowerBound\n";
        oss << printASTInternal(n->lowerBound.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- UpperBound\n";
        oss << printASTInternal(n->upperBound.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<EnumNode*>(node)) {
        oss << "EnumNode(values: [" << joinNames(n->identifiers) << "])"
            << annotation(node, decorated) << "\n";
    }
    else if (auto* n = dynamic_cast<AssignNode*>(node)) {
        oss << "AssignNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- Target\n";
        oss << printASTInternal(n->target.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- Value\n";
        oss << printASTInternal(n->value.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<IfNode*>(node)) {
        oss << "IfNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- Condition\n";
        oss << printASTInternal(n->condition.get(), indent + "|   ", true, decorated);

        oss << indent << "|-- Then\n";
        oss << printASTInternal(n->thenBlock.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- Else\n";
        oss << printASTInternal(n->elseBlock.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<CaseNode*>(node)) {
        oss << "CaseNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- Condition\n";
        oss << printASTInternal(n->condition.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- Cases\n";
        for (size_t i = 0; i < n->cases.size(); ++i) {
            oss << printASTInternal(n->cases[i].get(), indent + "    ", i == n->cases.size() - 1, decorated);
        }
    }
    else if (auto* n = dynamic_cast<CaseBlockNode*>(node)) {
        oss << "CaseBlockNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- Constants\n";
        for (size_t i = 0; i < n->constants.size(); ++i) {
            oss << printASTInternal(n->constants[i].get(), indent + "|   ", i == n->constants.size() - 1, decorated);
        }

        oss << indent << "\\-- Statement\n";
        oss << printASTInternal(n->statement.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<WhileNode*>(node)) {
        oss << "WhileNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- Condition\n";
        oss << printASTInternal(n->condition.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- LoopBlock\n";
        oss << printASTInternal(n->loopBlock.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<RepeatNode*>(node)) {
        oss << "RepeatNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- Statements\n";
        for (size_t i = 0; i < n->statements.size(); ++i) {
            oss << printASTInternal(n->statements[i].get(), indent + "|   ", i == n->statements.size() - 1, decorated);
        }

        oss << indent << "\\-- UntilCondition\n";
        oss << printASTInternal(n->condition.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<ForNode*>(node)) {
        oss << "ForNode(iterator: '" << n->iteratorName
            << "', direction: " << (n->isDownto ? "downto" : "to") << ")"
            << annotation(node, decorated) << "\n";

        oss << indent << "|-- StartValue\n";
        oss << printASTInternal(n->startValue.get(), indent + "|   ", true, decorated);

        oss << indent << "|-- EndValue\n";
        oss << printASTInternal(n->endValue.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- LoopBlock\n";
        oss << printASTInternal(n->loopBlock.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<ProcCallNode*>(node)) {
        oss << "ProcCallNode(name: '" << n->procName << "')"
            << annotation(node, decorated) << "\n";

        for (size_t i = 0; i < n->arguments.size(); ++i) {
            oss << printASTInternal(n->arguments[i].get(), indent, i == n->arguments.size() - 1, decorated);
        }
    }
    else if (auto* n = dynamic_cast<VarNode*>(node)) {
        oss << "VarNode(name: '" << n->name << "')"
            << annotation(node, decorated) << "\n";
    }
    else if (auto* n = dynamic_cast<ArrayAccessNode*>(node)) {
        oss << "ArrayAccessNode" << annotation(node, decorated) << "\n";

        oss << indent << "|-- Array\n";
        oss << printASTInternal(n->arrayVar.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- Indices\n";
        for (size_t i = 0; i < n->indices.size(); ++i) {
            oss << printASTInternal(n->indices[i].get(), indent + "    ", i == n->indices.size() - 1, decorated);
        }
    }
    else if (auto* n = dynamic_cast<RecordAccessNode*>(node)) {
        oss << "RecordAccessNode(field: '" << n->fieldName << "')"
            << annotation(node, decorated) << "\n";

        oss << printASTInternal(n->recordVar.get(), indent, true, decorated);
    }
    else if (auto* n = dynamic_cast<BinOpNode*>(node)) {
        oss << "BinOpNode(op: '" << n->op << "')"
            << annotation(node, decorated) << "\n";

        oss << indent << "|-- Left\n";
        oss << printASTInternal(n->left.get(), indent + "|   ", true, decorated);

        oss << indent << "\\-- Right\n";
        oss << printASTInternal(n->right.get(), indent + "    ", true, decorated);
    }
    else if (auto* n = dynamic_cast<UnaryOpNode*>(node)) {
        oss << "UnaryOpNode(op: '" << n->op << "')"
            << annotation(node, decorated) << "\n";

        oss << printASTInternal(n->operand.get(), indent, true, decorated);
    }
    else if (auto* n = dynamic_cast<NumberNode*>(node)) {
        oss << "NumberNode(value: '" << n->value
            << "', kind: " << (n->isReal ? "real" : "integer") << ")"
            << annotation(node, decorated) << "\n";
    }
    else if (auto* n = dynamic_cast<StringNode*>(node)) {
        oss << "StringNode(value: " << n->value << ")"
            << annotation(node, decorated) << "\n";
    }
    else if (auto* n = dynamic_cast<CharNode*>(node)) {
        oss << "CharNode(value: " << n->value << ")"
            << annotation(node, decorated) << "\n";
    }
    else {
        oss << "UnknownASTNode" << annotation(node, decorated) << "\n";
    }

    return oss.str();
}