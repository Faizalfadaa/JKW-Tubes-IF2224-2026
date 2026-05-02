#include "treeprinter.hpp"
#include <iostream>

using namespace std;

void printTree(ParseNode* node, string indent, bool last) {

    cout << indent;

    if(last) {
        cout << "\\-- ";
        indent += "    ";
    } else {
        cout << "|-- ";
        indent += "|   ";
    }

    cout << node->label << endl;

    for(int i = 0; i < (int)node->children.size(); i++) {
        printTree(
            node->children[i],
            indent,
            i == (int)node->children.size() - 1
        );
    }
}