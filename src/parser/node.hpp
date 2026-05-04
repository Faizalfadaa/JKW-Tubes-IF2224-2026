#ifndef NODE_HPP
#define NODE_HPP

#include <string>
#include <vector>

using namespace std;

class ParseNode {
public:
    string label;
    vector<ParseNode*> children; //TODO: ganti ke unique pointer agar tidak  memory leak

    ParseNode(string lbl) {
        label = lbl;
    }

    void addChild(ParseNode* child) {
        children.push_back(child);
    }
};

#endif