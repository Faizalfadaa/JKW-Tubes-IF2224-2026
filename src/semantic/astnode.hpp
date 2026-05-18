#pragma once

#include <string>
#include <vector>
#include <memory>

//Forward declaration
class ParseNode;

class ASTNode {
protected:
    std::string label;
    std::vector<std::string> attributes;
    std::vector<std::unique_ptr<ASTNode>> children; 

public:
    virtual ~ASTNode() = default;
    std::string getLabel(){
        return label;
    }

    std::vector<std::string> getAttribute(){
        return attributes;
    }

    std::vector<std::unique_ptr<ASTNode>> getChild(){
        return children;
    }
};

class ProgramNode : ASTNode {
public:    
    ProgramNode(ParseNode node);
};

class DeclarationNode : ASTNode {
public:
    DeclarationNode(ParseNode node);
};

