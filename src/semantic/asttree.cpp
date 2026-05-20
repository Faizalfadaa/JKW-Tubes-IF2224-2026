#include "asttree.hpp"

#include <cctype>
#include <string>
#include <utility>

std::unique_ptr<ASTNode> ASTBuilder::createAST(ParseNode* parseRoot) {
    if (!parseRoot) return nullptr;

    if (parseRoot->label == "<program>") {
        return makeProgramNode(parseRoot);
    }
    return nullptr;
}

std::unique_ptr<ASTNode> ASTBuilder::makeProgramNode(ParseNode* node) {
    if (!node || node->children.size() < 3) return nullptr;

    ParseNode* header = node->children[0].get();
    std::string progName = "";

    if (header && header->children.size() > 1 && header->children[1]) {
        progName = header->children[1]->label;
        size_t open = progName.find('(');
        if (open != std::string::npos && !progName.empty() && progName.back() == ')') {
            progName = progName.substr(open + 1, progName.size() - open - 2);
        }
    }

    std::vector<std::unique_ptr<ASTNode>> declarations = makeDeclarationPart(node->children[1].get());
    std::unique_ptr<ASTNode> mainBlock = makeCompoundNode(node->children[2].get());

    return std::make_unique<ProgramNode>(progName, std::move(declarations), std::move(mainBlock));
}

std::unique_ptr<ASTNode> ASTBuilder::makeCompoundNode(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> statements;
    if (!node) {
        return std::make_unique<CompoundNode>(std::move(statements));
    }

    for (const auto& childPtr : node->children) {
        ParseNode* child = childPtr.get();
        if (child && child->label == "<statement-list>") {
            statements = makeStatementList(child);
            break;
        }
    }

    return std::make_unique<CompoundNode>(std::move(statements));
}

std::vector<std::unique_ptr<ASTNode>> ASTBuilder::makeStatementList(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> stmts;
    if (!node) return stmts;

    for (const auto& childPtr : node->children) {
        ParseNode* child = childPtr.get();
        if (child && child->label == "<statement>") {
            std::unique_ptr<ASTNode> stmtNode = makeStatementNode(child);
            if (stmtNode) {
                stmts.push_back(std::move(stmtNode));
            }
        }
    }

    return stmts;
}

std::vector<std::unique_ptr<ASTNode>> ASTBuilder::makeDeclarationPart(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> allDecls;
    if (!node) return allDecls;

    for (const auto& childPtr : node->children) {
        ParseNode* child = childPtr.get();
        if (!child) continue;

        if (child->label == "<const-declaration>") {
            std::vector<std::unique_ptr<ASTNode>> cDecls = makeConstDeclarations(child);
            for (auto& decl : cDecls) {
                allDecls.push_back(std::move(decl));
            }
        }
        else if (child->label == "<type-declaration>") {
            std::vector<std::unique_ptr<ASTNode>> tDecls = makeTypeDeclarations(child);
            for (auto& decl : tDecls) {
                allDecls.push_back(std::move(decl));
            }
        }
        else if (child->label == "<var-declaration>") {
            std::vector<std::unique_ptr<ASTNode>> vDecls = makeVarDeclarations(child);
            for (auto& decl : vDecls) {
                allDecls.push_back(std::move(decl));
            }
        }
        else if (child->label == "<subprogram-declaration>") {
            std::unique_ptr<ASTNode> subDecl = makeSubprogramDeclaration(child);
            if (subDecl) {
                allDecls.push_back(std::move(subDecl));
            }
        }
    }

    return allDecls;
}

std::vector<std::unique_ptr<ASTNode>> ASTBuilder::makeConstDeclarations(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> result;
    if (!node) return result;

    for (size_t i = 1; i < node->children.size(); i += 4) {
        if (i + 2 >= node->children.size()) break;

        std::string name = node->children[i]->label;
        size_t open = name.find('(');
        if (open != std::string::npos && !name.empty() && name.back() == ')') {
            name = name.substr(open + 1, name.size() - open - 2);
        }

        std::unique_ptr<ASTNode> constVal = makeConstantNode(node->children[i + 2].get());
        result.push_back(std::make_unique<ConstDeclNode>(name, std::move(constVal)));
    }

    return result;
}

std::vector<std::unique_ptr<ASTNode>> ASTBuilder::makeTypeDeclarations(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> result;
    if (!node) return result;

    for (size_t i = 1; i < node->children.size(); i += 4) {
        if (i + 2 >= node->children.size()) break;

        std::string name = node->children[i]->label;
        size_t open = name.find('(');
        if (open != std::string::npos && !name.empty() && name.back() == ')') {
            name = name.substr(open + 1, name.size() - open - 2);
        }

        std::unique_ptr<ASTNode> typeDef = makeTypeNode(node->children[i + 2].get());
        result.push_back(std::make_unique<TypeDeclNode>(name, std::move(typeDef)));
    }

    return result;
}

std::vector<std::unique_ptr<ASTNode>> ASTBuilder::makeVarDeclarations(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> result;
    if (!node) return result;

    for (size_t i = 1; i < node->children.size(); i += 4) {
        if (i + 2 >= node->children.size()) break;

        std::vector<std::string> names = makeIdentifierList(node->children[i].get());
        std::unique_ptr<ASTNode> typeDef = makeTypeNode(node->children[i + 2].get());
        result.push_back(std::make_unique<VarDeclNode>(std::move(names), std::move(typeDef)));
    }

    return result;
}

std::vector<std::string> ASTBuilder::makeIdentifierList(ParseNode* node) {
    std::vector<std::string> names;
    if (!node) return names;

    for (size_t i = 0; i < node->children.size(); i += 2) {
        if (!node->children[i]) continue;

        std::string name = node->children[i]->label;
        size_t open = name.find('(');
        if (open != std::string::npos && !name.empty() && name.back() == ')') {
            name = name.substr(open + 1, name.size() - open - 2);
        }

        names.push_back(name);
    }

    return names;
}

std::unique_ptr<ASTNode> ASTBuilder::makeSubprogramDeclaration(ParseNode* node) {
    if (!node || node->children.empty()) return nullptr;

    ParseNode* actualDecl = node->children[0].get();
    if (!actualDecl || actualDecl->children.size() < 2) return nullptr;

    bool isFunc = (actualDecl->label == "<function-declaration>");

    std::string subName = actualDecl->children[1]->label;
    size_t open = subName.find('(');
    if (open != std::string::npos && !subName.empty() && subName.back() == ')') {
        subName = subName.substr(open + 1, subName.size() - open - 2);
    }

    std::vector<std::unique_ptr<ASTNode>> params;
    std::unique_ptr<ASTNode> retType = nullptr;
    std::vector<std::unique_ptr<ASTNode>> localDecls;
    std::unique_ptr<ASTNode> bodyBlock = nullptr;

    for (const auto& childPtr : actualDecl->children) {
        ParseNode* child = childPtr.get();
        if (!child) continue;

        if (child->label == "<formal-parameter-list>") {
            params = makeFormalParameterList(child);
        }
        else if (isFunc && !retType) {
            std::string tokenName = child->label;
            size_t tokenOpen = tokenName.find('(');
            if (tokenOpen != std::string::npos) {
                tokenName = tokenName.substr(0, tokenOpen);
            }

            if (tokenName == "IDENT") {
                std::string typeName = child->label;
                size_t typeOpen = typeName.find('(');
                if (typeOpen != std::string::npos && !typeName.empty() && typeName.back() == ')') {
                    typeName = typeName.substr(typeOpen + 1, typeName.size() - typeOpen - 2);
                }
                retType = std::make_unique<NamedTypeNode>(typeName);
            }
        }
        else if (child->label == "<block>") {
            if (child->children.size() >= 2) {
                localDecls = makeDeclarationPart(child->children[0].get());
                bodyBlock = makeCompoundNode(child->children[1].get());
            }
        }
    }

    return std::make_unique<SubprogramDeclNode>(
        isFunc,
        subName,
        std::move(params),
        std::move(retType),
        std::move(localDecls),
        std::move(bodyBlock)
    );
}

std::vector<std::unique_ptr<ASTNode>> ASTBuilder::makeFormalParameterList(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> params;
    if (!node || node->children.size() < 3) return params;

    for (size_t i = 1; i + 1 < node->children.size(); i += 2) {
        ParseNode* group = node->children[i].get();
        if (!group || group->label != "<parameter-group>" || group->children.size() < 3) continue;

        std::vector<std::string> names = makeIdentifierList(group->children[0].get());
        std::unique_ptr<ASTNode> typeDef = nullptr;
        ParseNode* typeNode = group->children[2].get();

        if (typeNode) {
            if (typeNode->label == "<array-type>") {
                typeDef = makeArrayTypeNode(typeNode);
            }
            else if (typeNode->label == "<type>") {
                typeDef = makeTypeNode(typeNode);
            }
            else {
                std::string typeName = typeNode->label;
                size_t open = typeName.find('(');
                if (open != std::string::npos && !typeName.empty() && typeName.back() == ')') {
                    typeName = typeName.substr(open + 1, typeName.size() - open - 2);
                }
                typeDef = std::make_unique<NamedTypeNode>(typeName);
            }
        }

        params.push_back(std::make_unique<ParamNode>(std::move(names), std::move(typeDef), false));
    }

    return params;
}

std::unique_ptr<ASTNode> ASTBuilder::makeTypeNode(ParseNode* node) {
    if (!node) return nullptr;

    ParseNode* actualType = node;
    if (node->label == "<type>") {
        if (node->children.empty()) return nullptr;
        actualType = node->children[0].get();
    }

    if (!actualType) return nullptr;

    if (actualType->label == "<array-type>") {
        return makeArrayTypeNode(actualType);
    }
    if (actualType->label == "<range>") {
        return makeRangeNode(actualType);
    }
    if (actualType->label == "<enumerated>") {
        return makeEnumNode(actualType);
    }
    if (actualType->label == "<record-type>") {
        return makeRecordTypeNode(actualType);
    }

    std::string typeName = actualType->label;
    size_t open = typeName.find('(');
    if (open != std::string::npos && !typeName.empty() && typeName.back() == ')') {
        typeName = typeName.substr(open + 1, typeName.size() - open - 2);
    }

    return std::make_unique<NamedTypeNode>(typeName);
}

std::unique_ptr<ASTNode> ASTBuilder::makeConstantNode(ParseNode* node) {
    if (!node) return nullptr;

    size_t idx = 0;
    std::string sign = "";

    if (node->label == "<constant>") {
        if (node->children.empty()) return nullptr;

        std::string firstToken = node->children[0]->label;
        size_t firstOpen = firstToken.find('(');
        if (firstOpen != std::string::npos) {
            firstToken = firstToken.substr(0, firstOpen);
        }

        if (firstToken == "PLUS") {
            sign = "+";
            idx++;
        }
        else if (firstToken == "MINUS") {
            sign = "-";
            idx++;
        }

        if (idx >= node->children.size()) return nullptr;
        node = node->children[idx].get();
    }

    std::string tokenName = node->label;
    std::string value = node->label;
    size_t open = tokenName.find('(');
    if (open != std::string::npos) {
        tokenName = tokenName.substr(0, open);
        if (!value.empty() && value.back() == ')') {
            value = value.substr(open + 1, value.size() - open - 2);
        }
    }

    value = sign + value;

    if (tokenName == "CHARCON") {
        return std::make_unique<CharNode>(value);
    }
    if (tokenName == "STRING") {
        return std::make_unique<StringNode>(value);
    }
    if (tokenName == "REALCON") {
        return std::make_unique<NumberNode>(value, true);
    }
    if (tokenName == "INTCON") {
        return std::make_unique<NumberNode>(value, false);
    }

    return std::make_unique<VarNode>(value);
}

std::unique_ptr<ASTNode> ASTBuilder::makeArrayTypeNode(ParseNode* node) {
    if (!node || node->children.size() < 6) return nullptr;

    std::unique_ptr<ASTNode> indexType = nullptr;
    ParseNode* idxNode = node->children[2].get();

    if (idxNode) {
        if (idxNode->label == "<range>") {
            indexType = makeRangeNode(idxNode);
        }
        else if (idxNode->label == "<type>" || idxNode->label == "<array-type>" ||
                 idxNode->label == "<enumerated>" || idxNode->label == "<record-type>") {
            indexType = makeTypeNode(idxNode);
        }
        else {
            std::string typeName = idxNode->label;
            size_t open = typeName.find('(');
            if (open != std::string::npos && !typeName.empty() && typeName.back() == ')') {
                typeName = typeName.substr(open + 1, typeName.size() - open - 2);
            }
            indexType = std::make_unique<NamedTypeNode>(typeName);
        }
    }

    std::unique_ptr<ASTNode> elementType = makeTypeNode(node->children[5].get());

    return std::make_unique<ArrayTypeNode>(std::move(indexType), std::move(elementType));
}

std::unique_ptr<ASTNode> ASTBuilder::makeRangeNode(ParseNode* node) {
    if (!node || node->children.size() < 4) return nullptr;

    std::unique_ptr<ASTNode> lowerBound = makeConstantNode(node->children[0].get());
    std::unique_ptr<ASTNode> upperBound = makeConstantNode(node->children[3].get());

    return std::make_unique<RangeNode>(std::move(lowerBound), std::move(upperBound));
}

std::unique_ptr<ASTNode> ASTBuilder::makeEnumNode(ParseNode* node) {
    std::vector<std::string> identifiers;
    if (!node) {
        return std::make_unique<EnumNode>(std::move(identifiers));
    }

    for (size_t i = 1; i + 1 < node->children.size(); i += 2) {
        if (!node->children[i]) continue;

        std::string name = node->children[i]->label;
        size_t open = name.find('(');
        if (open != std::string::npos && !name.empty() && name.back() == ')') {
            name = name.substr(open + 1, name.size() - open - 2);
        }

        identifiers.push_back(name);
    }

    return std::make_unique<EnumNode>(std::move(identifiers));
}

std::unique_ptr<ASTNode> ASTBuilder::makeRecordTypeNode(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> fields;
    if (!node || node->children.size() < 3) {
        return std::make_unique<RecordTypeNode>(std::move(fields));
    }

    ParseNode* fieldList = node->children[1].get();
    if (!fieldList) {
        return std::make_unique<RecordTypeNode>(std::move(fields));
    }

    for (size_t i = 0; i < fieldList->children.size(); i += 2) {
        ParseNode* fieldPart = fieldList->children[i].get();
        if (!fieldPart || fieldPart->label != "<field-part>" || fieldPart->children.size() < 3) continue;

        std::vector<std::string> names = makeIdentifierList(fieldPart->children[0].get());
        std::unique_ptr<ASTNode> typeDef = makeTypeNode(fieldPart->children[2].get());

        fields.push_back(std::make_unique<VarDeclNode>(std::move(names), std::move(typeDef)));
    }

    return std::make_unique<RecordTypeNode>(std::move(fields));
}

std::unique_ptr<ASTNode> ASTBuilder::makeStatementNode(ParseNode* node) {
    if (!node || node->children.empty()) return nullptr;

    ParseNode* actualStmt = node->children[0].get();
    if (!actualStmt) return nullptr;

    if (actualStmt->label == "<assignment-statement>") return makeAssignNode(actualStmt);
    if (actualStmt->label == "<if-statement>") return makeIfNode(actualStmt);
    if (actualStmt->label == "<case-statement>") return makeCaseNode(actualStmt);
    if (actualStmt->label == "<while-statement>") return makeWhileNode(actualStmt);
    if (actualStmt->label == "<repeat-statement>") return makeRepeatNode(actualStmt);
    if (actualStmt->label == "<for-statement>") return makeForNode(actualStmt);
    if (actualStmt->label == "<procedure/function-call>") return makeProcCallNode(actualStmt);
    if (actualStmt->label == "<compound-statement>") return makeCompoundNode(actualStmt);

    return nullptr;
}

std::unique_ptr<ASTNode> ASTBuilder::makeAssignNode(ParseNode* node) {
    if (!node || node->children.size() < 3) return nullptr;

    std::unique_ptr<ASTNode> target = makeVariableNode(node->children[0].get());
    std::unique_ptr<ASTNode> value = makeExpressionNode(node->children[2].get());

    return std::make_unique<AssignNode>(std::move(target), std::move(value));
}

std::unique_ptr<ASTNode> ASTBuilder::makeIfNode(ParseNode* node) {
    if (!node || node->children.size() < 4) return nullptr;

    std::unique_ptr<ASTNode> condition = makeExpressionNode(node->children[1].get());
    std::unique_ptr<ASTNode> thenBlock = makeStatementNode(node->children[3].get());
    std::unique_ptr<ASTNode> elseBlock = nullptr;

    if (node->children.size() > 5) {
        elseBlock = makeStatementNode(node->children[5].get());
    }

    return std::make_unique<IfNode>(std::move(condition), std::move(thenBlock), std::move(elseBlock));
}

std::unique_ptr<ASTNode> ASTBuilder::makeCaseNode(ParseNode* node) {
    if (!node || node->children.size() < 4) return nullptr;

    std::unique_ptr<ASTNode> condition = makeExpressionNode(node->children[1].get());
    std::vector<std::unique_ptr<ASTNode>> caseBlocks;

    ParseNode* currentBlock = nullptr;
    for (const auto& childPtr : node->children) {
        ParseNode* child = childPtr.get();
        if (child && child->label == "<case-block>") {
            currentBlock = child;
            break;
        }
    }

    while (currentBlock != nullptr) {
        std::unique_ptr<ASTNode> caseBlock = makeCaseBlockNode(currentBlock);
        if (caseBlock) {
            caseBlocks.push_back(std::move(caseBlock));
        }

        ParseNode* nextBlock = nullptr;
        for (const auto& childPtr : currentBlock->children) {
            ParseNode* child = childPtr.get();
            if (child && child->label == "<case-block>") {
                nextBlock = child;
                break;
            }
        }

        currentBlock = nextBlock;
    }

    return std::make_unique<CaseNode>(std::move(condition), std::move(caseBlocks));
}

std::unique_ptr<ASTNode> ASTBuilder::makeCaseBlockNode(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> constants;
    std::unique_ptr<ASTNode> statement = nullptr;

    if (!node) {
        return std::make_unique<CaseBlockNode>(std::move(constants), std::move(statement));
    }

    for (const auto& childPtr : node->children) {
        ParseNode* child = childPtr.get();
        if (!child) continue;

        if (child->label == "<constant>") {
            std::unique_ptr<ASTNode> constant = makeConstantNode(child);
            if (constant) {
                constants.push_back(std::move(constant));
            }
        }
        else if (child->label == "<statement>") {
            statement = makeStatementNode(child);
        }
    }

    return std::make_unique<CaseBlockNode>(std::move(constants), std::move(statement));
}

std::unique_ptr<ASTNode> ASTBuilder::makeWhileNode(ParseNode* node) {
    if (!node || node->children.size() < 3) return nullptr;

    std::unique_ptr<ASTNode> condition = makeExpressionNode(node->children[1].get());
    std::unique_ptr<ASTNode> loopBlock = nullptr;

    for (size_t i = 2; i < node->children.size(); i++) {
        if (!node->children[i]) continue;

        if (node->children[i]->label == "<compound-statement>") {
            loopBlock = makeCompoundNode(node->children[i].get());
            break;
        }
        else if (node->children[i]->label == "<statement>") {
            loopBlock = makeStatementNode(node->children[i].get());
            break;
        }
    }

    return std::make_unique<WhileNode>(std::move(condition), std::move(loopBlock));
}

std::unique_ptr<ASTNode> ASTBuilder::makeRepeatNode(ParseNode* node) {
    if (!node || node->children.size() < 4) return nullptr;

    std::vector<std::unique_ptr<ASTNode>> stmts = makeStatementList(node->children[1].get());
    std::unique_ptr<ASTNode> condition = makeExpressionNode(node->children[3].get());

    return std::make_unique<RepeatNode>(std::move(stmts), std::move(condition));
}

std::unique_ptr<ASTNode> ASTBuilder::makeForNode(ParseNode* node) {
    if (!node || node->children.size() < 8) return nullptr;

    std::string iteratorName = node->children[1]->label;
    size_t open = iteratorName.find('(');
    if (open != std::string::npos && !iteratorName.empty() && iteratorName.back() == ')') {
        iteratorName = iteratorName.substr(open + 1, iteratorName.size() - open - 2);
    }

    std::unique_ptr<ASTNode> startVal = makeExpressionNode(node->children[3].get());

    std::string dir = node->children[4]->label;
    size_t dirOpen = dir.find('(');
    if (dirOpen != std::string::npos) {
        dir = dir.substr(0, dirOpen);
    }
    bool isDownto = (dir == "DOWNTOSY");

    std::unique_ptr<ASTNode> endVal = makeExpressionNode(node->children[5].get());
    std::unique_ptr<ASTNode> loopBlock = nullptr;

    for (size_t i = 6; i < node->children.size(); i++) {
        if (!node->children[i]) continue;

        if (node->children[i]->label == "<compound-statement>") {
            loopBlock = makeCompoundNode(node->children[i].get());
            break;
        }
        else if (node->children[i]->label == "<statement>") {
            loopBlock = makeStatementNode(node->children[i].get());
            break;
        }
    }

    return std::make_unique<ForNode>(iteratorName, std::move(startVal), std::move(endVal), isDownto, std::move(loopBlock));
}

std::unique_ptr<ASTNode> ASTBuilder::makeProcCallNode(ParseNode* node) {
    if (!node || node->children.empty()) return nullptr;

    std::string procName = node->children[0]->label;
    size_t open = procName.find('(');
    if (open != std::string::npos && !procName.empty() && procName.back() == ')') {
        procName = procName.substr(open + 1, procName.size() - open - 2);
    }

    std::vector<std::unique_ptr<ASTNode>> args;

    for (const auto& childPtr : node->children) {
        ParseNode* child = childPtr.get();
        if (child && child->label == "<parameter-list>") {
            args = makeParameterList(child);
            break;
        }
    }

    return std::make_unique<ProcCallNode>(procName, std::move(args));
}

std::unique_ptr<ASTNode> ASTBuilder::makeVariableNode(ParseNode* node) {
    if (!node || node->children.empty()) return nullptr;

    std::string baseName = node->children[0]->label;
    size_t open = baseName.find('(');
    if (open != std::string::npos && !baseName.empty() && baseName.back() == ')') {
        baseName = baseName.substr(open + 1, baseName.size() - open - 2);
    }

    std::unique_ptr<ASTNode> currentVar = std::make_unique<VarNode>(baseName);

    for (size_t i = 1; i < node->children.size(); i++) {
        ParseNode* comp = node->children[i].get();
        if (!comp || comp->children.empty()) continue;

        std::string firstComp = comp->children[0]->label;
        size_t firstOpen = firstComp.find('(');
        if (firstOpen != std::string::npos) {
            firstComp = firstComp.substr(0, firstOpen);
        }

        if (firstComp == "LBRACK") {
            if (comp->children.size() > 1) {
                std::vector<std::unique_ptr<ASTNode>> indices = makeIndexList(comp->children[1].get());
                currentVar = std::make_unique<ArrayAccessNode>(std::move(currentVar), std::move(indices));
            }
        }
        else if (firstComp == "PERIOD") {
            if (comp->children.size() > 1) {
                std::string fieldName = comp->children[1]->label;
                size_t fieldOpen = fieldName.find('(');
                if (fieldOpen != std::string::npos && !fieldName.empty() && fieldName.back() == ')') {
                    fieldName = fieldName.substr(fieldOpen + 1, fieldName.size() - fieldOpen - 2);
                }
                currentVar = std::make_unique<RecordAccessNode>(std::move(currentVar), fieldName);
            }
        }
    }

    return currentVar;
}

std::vector<std::unique_ptr<ASTNode>> ASTBuilder::makeIndexList(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> indices;
    if (!node) return indices;

    for (size_t i = 0; i < node->children.size(); i += 2) {
        ParseNode* valNode = node->children[i].get();
        if (!valNode) continue;

        if (valNode->label == "<index-list>") {
            std::vector<std::unique_ptr<ASTNode>> nestedIndices = makeIndexList(valNode);
            for (auto& index : nestedIndices) {
                indices.push_back(std::move(index));
            }
            continue;
        }

        std::string tokenName = valNode->label;
        std::string value = valNode->label;
        size_t open = tokenName.find('(');
        if (open != std::string::npos) {
            tokenName = tokenName.substr(0, open);
            if (!value.empty() && value.back() == ')') {
                value = value.substr(open + 1, value.size() - open - 2);
            }
        }

        if (tokenName == "INTCON") {
            indices.push_back(std::make_unique<NumberNode>(value, false));
        }
        else if (tokenName == "CHARCON") {
            indices.push_back(std::make_unique<CharNode>(value));
        }
        else {
            indices.push_back(std::make_unique<VarNode>(value));
        }
    }

    return indices;
}

std::vector<std::unique_ptr<ASTNode>> ASTBuilder::makeParameterList(ParseNode* node) {
    std::vector<std::unique_ptr<ASTNode>> args;
    if (!node) return args;

    for (size_t i = 0; i < node->children.size(); i += 2) {
        if (!node->children[i]) continue;

        std::unique_ptr<ASTNode> expr = makeExpressionNode(node->children[i].get());
        if (expr) {
            args.push_back(std::move(expr));
        }
    }

    return args;
}

std::unique_ptr<ASTNode> ASTBuilder::makeExpressionNode(ParseNode* node) {
    if (!node || node->children.empty()) return nullptr;

    std::unique_ptr<ASTNode> left = makeSimpleExpressionNode(node->children[0].get());

    if (node->children.size() > 2) {
        std::string op = "";
        ParseNode* opNode = node->children[1].get();

        if (opNode) {
            if (!opNode->children.empty() && opNode->children[0]) {
                op = opNode->children[0]->label;
            } else {
                op = opNode->label;
            }
        }

        size_t open = op.find('(');
        if (open != std::string::npos) {
            op = op.substr(0, open);
        }

        if (op == "EQL") op = "=";
        else if (op == "NEQ") op = "<>";
        else if (op == "LSS") op = "<";
        else if (op == "LEQ") op = "<=";
        else if (op == "GTR") op = ">";
        else if (op == "GEQ") op = ">=";

        std::unique_ptr<ASTNode> right = makeSimpleExpressionNode(node->children[2].get());
        return std::make_unique<BinOpNode>(op, std::move(left), std::move(right));
    }

    return left;
}

std::unique_ptr<ASTNode> ASTBuilder::makeSimpleExpressionNode(ParseNode* node) {
    if (!node || node->children.empty()) return nullptr;

    size_t idx = 0;
    std::unique_ptr<ASTNode> currentLeft = nullptr;

    std::string first = node->children[0]->label;
    size_t firstOpen = first.find('(');
    if (firstOpen != std::string::npos) {
        first = first.substr(0, firstOpen);
    }

    if (first == "PLUS" || first == "MINUS") {
        std::string sign = (first == "MINUS") ? "-" : "+";
        idx++;
        if (idx >= node->children.size()) return nullptr;

        std::unique_ptr<ASTNode> termNode = makeTermNode(node->children[idx].get());
        currentLeft = std::make_unique<UnaryOpNode>(sign, std::move(termNode));
    }
    else {
        currentLeft = makeTermNode(node->children[idx].get());
    }

    idx++;

    while (idx + 1 < node->children.size()) {
        std::string op = "";
        ParseNode* opNode = node->children[idx].get();

        if (opNode) {
            if (!opNode->children.empty() && opNode->children[0]) {
                op = opNode->children[0]->label;
            } else {
                op = opNode->label;
            }
        }

        size_t open = op.find('(');
        if (open != std::string::npos) {
            op = op.substr(0, open);
        }

        if (op == "PLUS") op = "+";
        else if (op == "MINUS") op = "-";
        else if (op == "ORSY") op = "or";

        idx++;

        std::unique_ptr<ASTNode> nextRight = makeTermNode(node->children[idx].get());
        idx++;

        currentLeft = std::make_unique<BinOpNode>(op, std::move(currentLeft), std::move(nextRight));
    }

    return currentLeft;
}

std::unique_ptr<ASTNode> ASTBuilder::makeTermNode(ParseNode* node) {
    if (!node || node->children.empty()) return nullptr;

    std::unique_ptr<ASTNode> currentLeft = makeFactorNode(node->children[0].get());
    size_t idx = 1;

    while (idx + 1 < node->children.size()) {
        std::string op = "";
        ParseNode* opNode = node->children[idx].get();

        if (opNode) {
            if (!opNode->children.empty() && opNode->children[0]) {
                op = opNode->children[0]->label;
            } else {
                op = opNode->label;
            }
        }

        size_t open = op.find('(');
        if (open != std::string::npos) {
            op = op.substr(0, open);
        }

        if (op == "TIMES") op = "*";
        else if (op == "RDIV") op = "/";
        else if (op == "IDIV") op = "div";
        else if (op == "IMOD") op = "mod";
        else if (op == "ANDSY") op = "and";

        idx++;

        std::unique_ptr<ASTNode> nextRight = makeFactorNode(node->children[idx].get());
        idx++;

        currentLeft = std::make_unique<BinOpNode>(op, std::move(currentLeft), std::move(nextRight));
    }

    return currentLeft;
}

std::unique_ptr<ASTNode> ASTBuilder::makeFactorNode(ParseNode* node) {
    if (!node || node->children.empty()) return nullptr;

    ParseNode* child = node->children[0].get();
    if (!child) return nullptr;

    if (child->label == "<procedure/function-call>") {
        return makeProcCallNode(child);
    }

    if (child->label == "<variable>") {
        return makeVariableNode(child);
    }

    if (child->label == "<constant>") {
        return makeConstantNode(child);
    }

    std::string tokenName = child->label;
    std::string value = child->label;
    size_t open = tokenName.find('(');
    if (open != std::string::npos) {
        tokenName = tokenName.substr(0, open);
        if (!value.empty() && value.back() == ')') {
            value = value.substr(open + 1, value.size() - open - 2);
        }
    }

    if (tokenName == "LPARENT") {
        if (node->children.size() > 1) {
            return makeExpressionNode(node->children[1].get());
        }
        return nullptr;
    }

    if (tokenName == "NOTSY") {
        if (node->children.size() > 1) {
            std::unique_ptr<ASTNode> operand = makeFactorNode(node->children[1].get());
            return std::make_unique<UnaryOpNode>("not", std::move(operand));
        }
        return nullptr;
    }

    if (tokenName == "CHARCON") {
        return std::make_unique<CharNode>(value);
    }
    if (tokenName == "STRING") {
        return std::make_unique<StringNode>(value);
    }
    if (tokenName == "REALCON") {
        return std::make_unique<NumberNode>(value, true);
    }
    if (tokenName == "INTCON") {
        return std::make_unique<NumberNode>(value, false);
    }

    return std::make_unique<VarNode>(value);
}
