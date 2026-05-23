#pragma once

#include <sstream>
#include <string>
#include <vector>

class ASTNode;

std::string joinNames(const std::vector<std::string>& names);
std::string printAST(ASTNode* node, std::string indent = "", bool last = true);
std::string printDecoratedAST(ASTNode* node, std::string indent = "", bool last = true);