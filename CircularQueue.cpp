#include "CircularQueue.h"
#include <cstring>

CircularQueue::CircularQueue() : head(0), tail(0), cnt(0) {
	std::memset(arr, 0, sizeof(arr));
}

CircularQueue::~CircularQueue() { clear(); }

void CircularQueue::clear() {
	head = tail = cnt = 0;
	std::memset(arr, 0, sizeof(arr));
}

void CircularQueue::push(LogNode* node) {
	if (!node) return;
	arr[tail] = node;
	tail = (tail + 1) % CAP;
	if (cnt < CAP) ++cnt; else head = (head + 1) % CAP; // overwrite oldest
}

void CircularQueue::removeNode(LogNode* node) {
	if (!node || cnt==0) return;
	// compact into new array
	LogNode* tmp[CAP];
	int idx = 0;
	for (int i = 0, p = head; i < cnt; ++i, p = (p+1)%CAP) {
		if (arr[p] != node) tmp[idx++] = arr[p];
	}
	// write back
	std::memset(arr, 0, sizeof(arr));
	head = 0; tail = 0; cnt = 0;
	for (int i = 0; i < idx; ++i) {
		arr[tail++] = tmp[i];
		++cnt;
	}
}

int CircularQueue::size() const { return cnt; }

void CircularQueue::traverse(void (*fn)(LogNode*)) {
    if (!fn) return;
    for (int i = 0, p = head; i < cnt; ++i, p = (p+1)%CAP) {
        fn(arr[p]);
    }
}

int CircularQueue::getLastN(int n, LogNode** out) {
    if (!out || n <= 0) return 0;
    if (cnt == 0) return 0;
    int need = n < cnt ? n : cnt;
    int start = cnt - need;
    for (int i = 0; i < need; ++i) {
        int idx = (head + start + i) % CAP;
        out[i] = arr[idx];
    }
    return need;
}