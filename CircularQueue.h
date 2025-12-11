#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include "LinkedList.h"

class CircularQueue {
public:
    CircularQueue();
    ~CircularQueue();

    void clear();
    void push(LogNode* node); // push pointer to node; overwrite oldest when full
    void removeNode(LogNode* node); // remove matching pointer if exists
    int size() const;
    // traverse from oldest to newest, calling provided function for each
    void traverse(void (*fn)(LogNode*));
    // get last n nodes into array (caller allocates array of size >= n)
    int getLastN(int n, LogNode** out);private:
	static const int CAP = 1000;
	LogNode* arr[CAP];
	int head; // index of oldest
	int tail; // index of next insert
	int cnt;
};

#endif // CIRCULARQUEUE_H
