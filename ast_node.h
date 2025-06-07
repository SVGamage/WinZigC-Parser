#ifndef AST_NODE_H
#define AST_NODE_H

#include <iostream>
#include <vector>
#include <string>

class ASTNode {
public:
    std::string nodeType;
    std::vector<ASTNode*> children;
    std::string value;
    
    ASTNode(const std::string& type, const std::string& val = "") 
        : nodeType(type), value(val) {}
    
    void addChild(ASTNode* child);
    void print(int depth = 0, bool isLast = false) const;
};

#endif // AST_NODE_H
