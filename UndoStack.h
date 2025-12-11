#ifndef UNDOSTACK_H
#define UNDOSTACK_H

#include "LinkedList.h"
#include "BST.h"
#include "CircularQueue.h"

// Snapshot of entire system state
struct Snapshot {
    LinkedList* list; // deep-copied list
    BST* bst; // deep-copied bst
    LogNode** queueArr; // array of pointers into copied list
    int queueSize;
    Snapshot() : list(nullptr), bst(nullptr), queueArr(nullptr), queueSize(0) {}
};

class UndoStack {
public:
    UndoStack();
    ~UndoStack();
    void clear();
    void push(Snapshot* s);
    Snapshot* pop();
    bool empty() const;

private:
    static const int CAP = 5;
    Snapshot* arr[CAP];
    int top; // next insert index
    int cnt;
};

// deep-copy structures
Snapshot* makeSnapshot(LinkedList* list, BST* bst, CircularQueue* queue);
void freeSnapshot(Snapshot* s);

#endif
