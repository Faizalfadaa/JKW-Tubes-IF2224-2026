#pragma once

#include <string>
#include <vector>
#include <memory>

using namespace std;

class ParseNode {
public:
    string label;
    vector<unique_ptr<ParseNode>> children;

    ParseNode(string lbl) {
        label = lbl;
    }

    void addChild(unique_ptr<ParseNode> child) {
        children.push_back(move(child));
    }
};