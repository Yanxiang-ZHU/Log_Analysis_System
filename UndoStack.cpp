#include "UndoStack.h"
#include <cstring>

UndoStack::UndoStack() : top(0), cnt(0) { std::memset(arr, 0, sizeof(arr)); }

UndoStack::~UndoStack() { clear(); }

void UndoStack::clear() {
    for (int i = 0; i < CAP; ++i) if (arr[i]) freeSnapshot(arr[i]);
    std::memset(arr, 0, sizeof(arr)); top = cnt = 0;
}

void UndoStack::push(Snapshot* s) {
    if (!s) return;
    // if full, drop oldest (arr[0]) shifting left
    if (cnt == CAP) {
        freeSnapshot(arr[0]);
        for (int i = 1; i < CAP; ++i) arr[i-1] = arr[i];
        arr[CAP-1] = s;
    } else {
        arr[cnt++] = s; arr[cnt] = nullptr;
    }
}

Snapshot* UndoStack::pop() {
    if (cnt == 0) return nullptr;
    Snapshot* s = arr[cnt-1]; arr[cnt-1] = nullptr; --cnt; return s;
}

bool UndoStack::empty() const { return cnt==0; }

// deep-copy helper implementations
Snapshot* makeSnapshot(LinkedList* list, BST* bst, CircularQueue* queue) {
    Snapshot* s = new Snapshot();
    // copy list
    s->list = new LinkedList();
    // mapping original -> copied pointers via arrays
    int n = list ? list->size() : 0;
    LogNode** origArr = nullptr;
    LogNode** copyArr = nullptr;
    if (n>0) {
        origArr = new LogNode*[n]; copyArr = new LogNode*[n];
        LogNode* cur = list->head(); int ii = 0;
        while (cur) {
            origArr[ii] = cur;
            copyArr[ii] = new LogNode(cur->timestamp, cur->timeKey, cur->level, cur->module, cur->message);
            s->list->push_back(copyArr[ii]);
            cur = cur->next; ++ii;
        }
    }

    s->bst = new BST();
    if (n>0) {
        for (int i=0;i<n;++i) {
            if (copyArr[i]->level == "ERROR") s->bst->insertError(copyArr[i]->module);
        }
    }
    // copy last n items from copies array
    s->queueSize = 0; s->queueArr = nullptr;
    if (queue && n>0) {
        int qsz = queue->size();
        if (qsz>0) {
            int need = qsz < n ? qsz : n;
            s->queueArr = new LogNode*[need];
            s->queueSize = need;
            // copy last 'need' nodes from copyArr array
            int start = n - need;
            for (int t = 0; t < need; ++t) {
                s->queueArr[t] = copyArr[start + t];
            }
        }
    }
    if (origArr) delete [] origArr;
    if (copyArr) delete [] copyArr;
    return s;
}

void freeSnapshot(Snapshot* s) {
    if (!s) return;
    if (s->queueArr) delete [] s->queueArr;
    if (s->bst) delete s->bst;
    if (s->list) delete s->list;
    delete s;
}
