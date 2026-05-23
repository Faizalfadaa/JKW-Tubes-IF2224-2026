#pragma once

#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

class ASTNode;

static std::string joinNames(const std::vector<std::string>& names);
static std::string printAST(ASTNode* node, std::string indent = "", bool last = true);
