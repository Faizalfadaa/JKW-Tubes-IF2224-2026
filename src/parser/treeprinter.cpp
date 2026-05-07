#include "treeprinter.hpp"
#include "sstream"
#include <iostream>

using namespace std;

std::string printTree(ParseNode* node, string indent, bool last) {
    std::ostringstream oss;
    oss << indent;

    if(last) {
        oss << "\\-- ";
        indent += "    ";
    } else {
        oss << "|-- ";
        indent += "|   ";
    }

    oss << node->label << endl;

    for(int i = 0; i < (int)node->children.size(); i++) {
        oss << printTree(
            node->children[i],
            indent,
            i == (int)node->children.size() - 1
        );
    }

    return oss.str();
}