#include "astnode.hpp"
#include "../parser/node.hpp"

using namespace std;

ProgramNode::ProgramNode(ParseNode* node){
    //Assign label
    this->label = "ProgramNode";

    //Validasi node
    if (node->label != "<program>"){
        //throw or assert something
    }

    //Assign attribute
    string attribute = node->children[0].get()->children[0].get()->label;
    this->attributes.push_back(make_pair("name", attribute));
    
    //Dapatkan child dari ParseNode untuk diproses
    vector<unique_ptr<ParseNode>> parseChild = node->children;
    this->children = {};

    //Rekursi node child
    children.push_back(make_unique<ASTNode>(DeclarationNode(parseChild.at(1).get())));
    children.push_back(make_unique<ASTNode>(DeclarationNode(parseChild.at(2).get())));
}