#ifndef BST_H
#define BST_H

#include <string>
#include "LinkedList.h"

struct BSTNode {
    std::string module;
    int count;
    BSTNode* left;
    BSTNode* right;
    BSTNode(const std::string& m = "", int c = 0) : module(m), count(c), left(nullptr), right(nullptr) {}
};

class BST {
public:
    BST();
    ~BST();
    void clear();
    void insertError(const std::string& module);
    void decrementError(const std::string& module);
    BSTNode* find(const std::string& module) const;
    BSTNode* root() const;
    int nodeCount() const;
    void inorderCollect(BSTNode** out);
private:
    BSTNode* root_;
    void clearNode(BSTNode* node);
    void insertOrInc(BSTNode*& node, const std::string& module);
    void decOrRemove(BSTNode*& node, const std::string& module);
    int countNodes(BSTNode* node) const;
    void collectInorder(BSTNode* node, BSTNode** out, int& idx);
};

#endif