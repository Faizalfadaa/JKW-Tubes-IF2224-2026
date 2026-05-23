#include "lexer/lexer.hpp"
#include "parser/parser.hpp"
#include "parser/treeprinter.hpp"
#include "semantic/asttree.hpp"
#include "semantic/astnode.hpp"

#include <exception>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static string joinNames(const vector<string>& names) {
    ostringstream oss;
    for (size_t i = 0; i < names.size(); ++i) {
        if (i > 0) oss << ", ";
        oss << names[i];
    }
    return oss.str();
}

static string printAST(ASTNode* node, string indent = "", bool last = true) {
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
        oss << "ProgramNode(name: '" << n->programName << "')\n";

        oss << indent << "|-- Declarations\n";
        for (size_t i = 0; i < n->declarations.size(); ++i) {
            oss << printAST(n->declarations[i].get(), indent + "|   ", i == n->declarations.size() - 1);
        }

        oss << indent << "\\-- Block\n";
        oss << printAST(n->mainBlock.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<CompoundNode*>(node)) {
        oss << "CompoundNode\n";
        for (size_t i = 0; i < n->statements.size(); ++i) {
            oss << printAST(n->statements[i].get(), indent, i == n->statements.size() - 1);
        }
    }
    else if (auto* n = dynamic_cast<ConstDeclNode*>(node)) {
        oss << "ConstDeclNode(name: '" << n->constName << "')\n";
        oss << printAST(n->value.get(), indent, true);
    }
    else if (auto* n = dynamic_cast<TypeDeclNode*>(node)) {
        oss << "TypeDeclNode(name: '" << n->typeName << "')\n";
        oss << printAST(n->typeDef.get(), indent, true);
    }
    else if (auto* n = dynamic_cast<VarDeclNode*>(node)) {
        oss << "VarDeclNode(names: [" << joinNames(n->varNames) << "])\n";
        oss << printAST(n->typeDef.get(), indent, true);
    }
    else if (auto* n = dynamic_cast<ParamNode*>(node)) {
        oss << "ParamNode(names: [" << joinNames(n->paramNames)
            << "], isVarParam: " << (n->isVarParam ? "true" : "false") << ")\n";
        oss << printAST(n->typeDef.get(), indent, true);
    }
    else if (auto* n = dynamic_cast<SubprogramDeclNode*>(node)) {
        oss << "SubprogramDeclNode(kind: " << (n->isFunction ? "function" : "procedure")
            << ", name: '" << n->subprogramName << "')\n";

        oss << indent << "|-- Parameters\n";
        for (size_t i = 0; i < n->parameters.size(); ++i) {
            oss << printAST(n->parameters[i].get(), indent + "|   ", i == n->parameters.size() - 1);
        }

        oss << indent << "|-- ReturnType\n";
        oss << printAST(n->returnType.get(), indent + "|   ", true);

        oss << indent << "|-- Declarations\n";
        for (size_t i = 0; i < n->declarations.size(); ++i) {
            oss << printAST(n->declarations[i].get(), indent + "|   ", i == n->declarations.size() - 1);
        }

        oss << indent << "\\-- Body\n";
        oss << printAST(n->body.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<NamedTypeNode*>(node)) {
        oss << "NamedTypeNode(name: '" << n->typeName << "')\n";
    }
    else if (auto* n = dynamic_cast<ArrayTypeNode*>(node)) {
        oss << "ArrayTypeNode\n";
        oss << indent << "|-- IndexType\n";
        oss << printAST(n->indexType.get(), indent + "|   ", true);
        oss << indent << "\\-- ElementType\n";
        oss << printAST(n->elementType.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<RecordTypeNode*>(node)) {
        oss << "RecordTypeNode\n";
        for (size_t i = 0; i < n->fields.size(); ++i) {
            oss << printAST(n->fields[i].get(), indent, i == n->fields.size() - 1);
        }
    }
    else if (auto* n = dynamic_cast<RangeNode*>(node)) {
        oss << "RangeNode\n";
        oss << indent << "|-- LowerBound\n";
        oss << printAST(n->lowerBound.get(), indent + "|   ", true);
        oss << indent << "\\-- UpperBound\n";
        oss << printAST(n->upperBound.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<EnumNode*>(node)) {
        oss << "EnumNode(values: [" << joinNames(n->identifiers) << "])\n";
    }
    else if (auto* n = dynamic_cast<AssignNode*>(node)) {
        oss << "AssignNode\n";
        oss << indent << "|-- Target\n";
        oss << printAST(n->target.get(), indent + "|   ", true);
        oss << indent << "\\-- Value\n";
        oss << printAST(n->value.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<IfNode*>(node)) {
        oss << "IfNode\n";
        oss << indent << "|-- Condition\n";
        oss << printAST(n->condition.get(), indent + "|   ", true);
        oss << indent << "|-- Then\n";
        oss << printAST(n->thenBlock.get(), indent + "|   ", true);
        oss << indent << "\\-- Else\n";
        oss << printAST(n->elseBlock.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<CaseNode*>(node)) {
        oss << "CaseNode\n";
        oss << indent << "|-- Condition\n";
        oss << printAST(n->condition.get(), indent + "|   ", true);
        oss << indent << "\\-- Cases\n";
        for (size_t i = 0; i < n->cases.size(); ++i) {
            oss << printAST(n->cases[i].get(), indent + "    ", i == n->cases.size() - 1);
        }
    }
    else if (auto* n = dynamic_cast<CaseBlockNode*>(node)) {
        oss << "CaseBlockNode\n";
        oss << indent << "|-- Constants\n";
        for (size_t i = 0; i < n->constants.size(); ++i) {
            oss << printAST(n->constants[i].get(), indent + "|   ", i == n->constants.size() - 1);
        }
        oss << indent << "\\-- Statement\n";
        oss << printAST(n->statement.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<WhileNode*>(node)) {
        oss << "WhileNode\n";
        oss << indent << "|-- Condition\n";
        oss << printAST(n->condition.get(), indent + "|   ", true);
        oss << indent << "\\-- LoopBlock\n";
        oss << printAST(n->loopBlock.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<RepeatNode*>(node)) {
        oss << "RepeatNode\n";
        oss << indent << "|-- Statements\n";
        for (size_t i = 0; i < n->statements.size(); ++i) {
            oss << printAST(n->statements[i].get(), indent + "|   ", i == n->statements.size() - 1);
        }
        oss << indent << "\\-- UntilCondition\n";
        oss << printAST(n->condition.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<ForNode*>(node)) {
        oss << "ForNode(iterator: '" << n->iteratorName
            << "', direction: " << (n->isDownto ? "downto" : "to") << ")\n";
        oss << indent << "|-- StartValue\n";
        oss << printAST(n->startValue.get(), indent + "|   ", true);
        oss << indent << "|-- EndValue\n";
        oss << printAST(n->endValue.get(), indent + "|   ", true);
        oss << indent << "\\-- LoopBlock\n";
        oss << printAST(n->loopBlock.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<ProcCallNode*>(node)) {
        oss << "ProcCallNode(name: '" << n->procName << "')\n";
        for (size_t i = 0; i < n->arguments.size(); ++i) {
            oss << printAST(n->arguments[i].get(), indent, i == n->arguments.size() - 1);
        }
    }
    else if (auto* n = dynamic_cast<VarNode*>(node)) {
        oss << "VarNode(name: '" << n->name << "')\n";
    }
    else if (auto* n = dynamic_cast<ArrayAccessNode*>(node)) {
        oss << "ArrayAccessNode\n";
        oss << indent << "|-- Array\n";
        oss << printAST(n->arrayVar.get(), indent + "|   ", true);
        oss << indent << "\\-- Indices\n";
        for (size_t i = 0; i < n->indices.size(); ++i) {
            oss << printAST(n->indices[i].get(), indent + "    ", i == n->indices.size() - 1);
        }
    }
    else if (auto* n = dynamic_cast<RecordAccessNode*>(node)) {
        oss << "RecordAccessNode(field: '" << n->fieldName << "')\n";
        oss << printAST(n->recordVar.get(), indent, true);
    }
    else if (auto* n = dynamic_cast<BinOpNode*>(node)) {
        oss << "BinOpNode(op: '" << n->op << "')\n";
        oss << indent << "|-- Left\n";
        oss << printAST(n->left.get(), indent + "|   ", true);
        oss << indent << "\\-- Right\n";
        oss << printAST(n->right.get(), indent + "    ", true);
    }
    else if (auto* n = dynamic_cast<UnaryOpNode*>(node)) {
        oss << "UnaryOpNode(op: '" << n->op << "')\n";
        oss << printAST(n->operand.get(), indent, true);
    }
    else if (auto* n = dynamic_cast<NumberNode*>(node)) {
        oss << "NumberNode(value: '" << n->value << "', kind: " << (n->isReal ? "real" : "integer") << ")\n";
    }
    else if (auto* n = dynamic_cast<StringNode*>(node)) {
        oss << "StringNode(value: " << n->value << ")\n";
    }
    else if (auto* n = dynamic_cast<CharNode*>(node)) {
        oss << "CharNode(value: " << n->value << ")\n";
    }
    else {
        oss << "UnknownASTNode\n";
    }

    return oss.str();
}

int main() {
    Reader reader;
    string path;

    do {
        cout << "Input path file: ";
        cin >> path;
    } while (!reader.open(path));

    Lexer lex(reader);
    vector<Token> tokens;
    while (!reader.isEOF()) {
        Token token = lex.getNextToken();
        tokens.push_back(token);
    }

#ifdef _WIN32
    system("if not exist test mkdir test");
    system("if not exist test\\milestone-1 mkdir test\\milestone-1");
    system("if not exist test\\milestone-2 mkdir test\\milestone-2");
    system("if not exist test\\milestone-3 mkdir test\\milestone-3");
#else
    system("mkdir -p test/milestone-1 test/milestone-2 test/milestone-3");
#endif

    ofstream output1("test/milestone-1/output.txt");
    vector<Token> parserTokens;

    for (const Token& token : tokens) {
        output1 << token.str_type();
        if (token.lexeme != "") output1 << "(" << token.lexeme << ")";
        output1 << '\n';

        if (token.type == TokenType::COMMENT) continue;
        if (token.type == TokenType::NOTDETERMINED) continue;
        parserTokens.push_back(token);
    }
    output1.close();

    try {
        Parser parser(parserTokens);
        unique_ptr<ParseNode> root = parser.program();

        string parseTreeOutput = printTree(root.get(), "", true);
        cout << "===== PARSE TREE =====\n";
        cout << parseTreeOutput;

        ofstream output2("test/milestone-2/output.txt");
        output2 << parseTreeOutput;
        output2.close();

        // Input untuk ASTBuilder adalah parse tree hasil parser.
        ASTBuilder astBuilder;
        unique_ptr<ASTNode> astRoot = astBuilder.createAST(root.get());

        string astTreeOutput = printAST(astRoot.get(), "", true);
        cout << "\n===== AST TREE =====\n";
        cout << astTreeOutput;

        ofstream output3("test/milestone-3/output.txt");
        output3 << astTreeOutput;
        output3.close();
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
