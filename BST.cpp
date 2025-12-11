#include "BST.h"
#include <cstring>

BST::BST() : root_(nullptr) {}

BST::~BST() { clear(); }

void BST::clear() { clearNode(root_); root_ = nullptr; }

void BST::clearNode(BSTNode* node) {
    if (!node) return;
    clearNode(node->left);
    clearNode(node->right);
    delete node;
}

void BST::insertError(const std::string& module) { insertOrInc(root_, module); }

void BST::insertOrInc(BSTNode*& node, const std::string& module) {
    if (!node) { node = new BSTNode(module, 1); return; }
    if (module < node->module) insertOrInc(node->left, module);
    else if (module > node->module) insertOrInc(node->right, module);
    else node->count++;
}

void BST::decrementError(const std::string& module) { decOrRemove(root_, module); }

void BST::decOrRemove(BSTNode*& node, const std::string& module) {
    if (!node) return;
    if (module < node->module) decOrRemove(node->left, module);
    else if (module > node->module) decOrRemove(node->right, module);
    else {
        node->count--;
        if (node->count <= 0) {
            // remove node
            BSTNode* toDelete = node;
            if (!node->left) node = node->right;
            else if (!node->right) node = node->left;
            else {
                // find successor
                BSTNode* succParent = node;
                BSTNode* succ = node->right;
                while (succ->left) { succParent = succ; succ = succ->left; }
                node->module = succ->module;
                node->count = succ->count;
                // remove successor
                if (succParent->left == succ) succParent->left = succ->right; else succParent->right = succ->right;
                delete succ;
                return;
            }
            delete toDelete;
        }
    }
}

BSTNode* BST::find(const std::string& module) const {
    BSTNode* p = root_;
    while (p) {
        if (module < p->module) p = p->left;
        else if (module > p->module) p = p->right;
        else return p;
    }
    return nullptr;
}

BSTNode* BST::root() const { return root_; }

int BST::countNodes(BSTNode* node) const {
    if (!node) return 0;
    return 1 + countNodes(node->left) + countNodes(node->right);
}

int BST::nodeCount() const { return countNodes(root_); }

void BST::inorderCollect(BSTNode** out) { int idx = 0; collectInorder(root_, out, idx); }

void BST::collectInorder(BSTNode* node, BSTNode** out, int& idx) {
    if (!node) return;
    collectInorder(node->left, out, idx);
    out[idx++] = node;
    collectInorder(node->right, out, idx);
}
