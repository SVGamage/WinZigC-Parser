#include "ast_node.h"

using namespace std;

void ASTNode::addChild(ASTNode* child) {
    if (child) children.push_back(child);
}

void ASTNode::print(int depth, bool isLast) const {
    // Print indentation
    for (int i = 0; i < depth; i++) {
        cout << ". ";
    }
    
    // Print node type and child count
    cout << nodeType << "(" << children.size() << ")";
    
    // Print value if it's a leaf node
    if (children.empty() && !value.empty()) {
        cout << "\n";
        for (int i = 0; i <= depth; i++) {
            cout << ". ";
        }
        cout << value << "(0)";
    }
    
    // Print children
    for (size_t i = 0; i < children.size(); i++) {
        cout << "\n";
        children[i]->print(depth + 1, false);
    }
    
    // Only add final newline if this is not the root node or not the last element
    if (depth == 0 && isLast) {
        // Don't add newline for the root node when it's the last
    } else if (depth > 0) {
        // For non-root nodes, newlines are handled by the parent
    }
}
