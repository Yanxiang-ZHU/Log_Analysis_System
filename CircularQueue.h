#ifndef CIRCULARQUEUE_H
#define CIRCULARQUEUE_H

#include "LinkedList.h"


class CircularQueue {
public:
    CircularQueue();
    ~CircularQueue();

    void clear();
    void push(LogNode* node);
    void removeNode(LogNode* node);
    int size() const;
    void traverse(void (*fn)(LogNode*));
    // get last n nodes into array
    int getLastN(int n, LogNode** out);private:
	static const int CAP = 1000;
	LogNode* arr[CAP];
	int head;
	int tail;
	int cnt;
};

#endif // CIRCULARQUEUE_H
