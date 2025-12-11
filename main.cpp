#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <cstring>
#include "LinkedList.h"
#include "CircularQueue.h"
#include "BST.h"
#include "KMP.h"
#include "UndoStack.h"

// Global system state
static LinkedList* g_list = nullptr;
static CircularQueue* g_queue = nullptr;
static BST* g_bst = nullptr;
static UndoStack* g_undo = nullptr;

void ensureInit() {
	if (!g_list) g_list = new LinkedList();
	if (!g_queue) g_queue = new CircularQueue();
	if (!g_bst) g_bst = new BST();
	if (!g_undo) g_undo = new UndoStack();
}

std::string trim(const std::string& s) {
	size_t a = 0, b = s.size();
	while (a < b && std::isspace((unsigned char)s[a])) ++a;
	while (b > a && std::isspace((unsigned char)s[b-1])) --b;
	return s.substr(a, b-a);
}

// parse a log line into components; returns nullptr on failure
LogNode* parseLogLine(const std::string& line) {
	// format: [YYYY-MM-DD HH:MM:SS] LEVEL MODULE message...
	size_t l = line.find(']');
	if (l == std::string::npos) return nullptr;
	std::string ts = line.substr(1, l-1);
	std::string rest = trim(line.substr(l+1));
	std::istringstream ss(rest);
	std::string level, module;
	if (!(ss >> level)) return nullptr;
	if (!(ss >> module)) return nullptr;
	std::string message;
	std::getline(ss, message);
	message = trim(message);
	long long key = LinkedList::parseTimeKey(ts);
	LogNode* node = new LogNode(ts, key, level, module, message);
	return node;
}

// LOAD command
void cmdLoad(const std::string& filename) {
	ensureInit();
	std::ifstream ifs(filename.c_str());
	if (!ifs.is_open()) { std::cout << "Loaded 0 entries\n"; return; }
	// clear existing state
	delete g_list; delete g_queue; delete g_bst; delete g_undo;
	g_list = new LinkedList(); g_queue = new CircularQueue(); g_bst = new BST(); g_undo = new UndoStack();
	std::string line;
	int count = 0;
	while (std::getline(ifs, line)) {
		if (trim(line).empty()) continue;
		LogNode* node = parseLogLine(line);
		if (!node) continue;
		g_list->push_back(node);
		g_queue->push(node);
		if (node->level == "ERROR") g_bst->insertError(node->module);
		++count;
	}
	std::cout << "Loaded " << count << " entries\n";
}

// FILTER start end (with underscore)
void cmdFilter(const std::string& sstart, const std::string& send) {
	ensureInit();
	std::string st = sstart; for (char& c: st) if (c=='_') c=' ';
	std::string ed = send; for (char& c: ed) if (c=='_') c=' ';
	long long kstart = LinkedList::parseTimeKey(st);
	long long kend = LinkedList::parseTimeKey(ed);
	LogNode* cur = g_list ? g_list->head() : nullptr;
	while (cur) {
		if (cur->timeKey >= kstart && cur->timeKey <= kend) {
			std::cout << LinkedList::formatFullLine(cur) << "\n";
		}
		cur = cur->next;
	}
}

// SEARCH keyword (case-sensitive)
void cmdSearch(const std::string& keyword) {
	ensureInit();
	int found = 0;
	LogNode* cur = g_list ? g_list->head() : nullptr;
	while (cur) {
		if (KMP::contains(cur->message, keyword)) {
			++found;
		}
		cur = cur->next;
	}
	std::cout << "Found " << found << " match(es)\n";
	cur = g_list ? g_list->head() : nullptr;
	while (cur) {
		if (KMP::contains(cur->message, keyword)) std::cout << LinkedList::formatFullLine(cur) << "\n";
		cur = cur->next;
	}
}

// STATS: gather BST nodes into array and sort by count desc, name asc
void cmdStats() {
	ensureInit();
	int n = g_bst->nodeCount();
	if (n==0) return;
	BSTNode** arr = new BSTNode*[n];
	g_bst->inorderCollect(arr);
	// now we have inorder by name; perform simple insertion sort by count desc, name asc
	for (int i = 1; i < n; ++i) {
		BSTNode* key = arr[i];
		int j = i-1;
		while (j>=0) {
			bool take = false;
			if (arr[j]->count < key->count) take = true;
			else if (arr[j]->count == key->count && arr[j]->module > key->module) take = true;
			if (take) { arr[j+1] = arr[j]; --j; } else break;
		}
		arr[j+1] = key;
	}
	for (int i = 0; i < n; ++i) {
		if (arr[i]->count > 0)
			std::cout << arr[i]->module << ": " << arr[i]->count << " errors\n";
	}
	delete [] arr;
}

// DELETE index
void cmdDelete(int index) {
	ensureInit();
	if (!g_list || g_list->size() < index || index < 1) return;
	// take snapshot
	Snapshot* s = makeSnapshot(g_list, g_bst, g_queue);
	g_undo->push(s);
	// remove node
	LogNode* removed = g_list->remove_at(index);
	if (!removed) return;
	// update queue
	g_queue->removeNode(removed);
	// update bst
	if (removed->level == "ERROR") g_bst->decrementError(removed->module);
	delete removed;
	std::cout << "Deleted entry " << index << "\n";
}

// UNDO
void cmdUndo() {
	ensureInit();
	if (g_undo->empty()) { std::cout << "No more undo\n"; return; }
	Snapshot* s = g_undo->pop();
	// replace current state with snapshot
	delete g_list; delete g_bst; delete g_queue;
	g_list = s->list; g_bst = s->bst;
	g_queue = new CircularQueue();
	// restore queue from snapshot
	for (int i = 0; i < s->queueSize; ++i) g_queue->push(s->queueArr[i]);
	// free snapshot container but not its internals
	delete s->queueArr; s->queueArr = nullptr; delete s; // NOTE: s->list and s->bst are now owned by globals
	std::cout << "Undo successful\n";
}

// RECENT N
void cmdRecent(int n) {
	ensureInit();
	if (n <= 0) return;
	if (!g_queue) return;
	int total = g_queue->size();
	int need = n < total ? n : total;
	if (need == 0) return;
	LogNode** buf = new LogNode*[need];
	int cnt = g_queue->getLastN(need, buf);
	for (int i = 0; i < cnt; ++i) {
		if (buf[i]) std::cout << LinkedList::formatFullLine(buf[i]) << "\n";
	}
	delete [] buf;
}

int main(int argc, char** argv) {
	ensureInit();
	std::string line;
	while (true) {
		if (!std::getline(std::cin, line)) break;
		line = trim(line);
		if (line.empty()) continue;
		// parse command
		std::istringstream ss(line);
		std::string cmd;
		ss >> cmd;
		// uppercase
		for (char& c: cmd) c = (char)std::toupper((unsigned char)c);
		if (cmd == "EXIT") break;
		else if (cmd == "LOAD") {
			std::string fname; ss >> fname; cmdLoad(fname);
		} else if (cmd == "FILTER") {
			std::string sstart, send; ss >> sstart >> send; cmdFilter(sstart, send);
		} else if (cmd == "SEARCH") {
			std::string keyword; ss >> keyword; cmdSearch(keyword);
		} else if (cmd == "STATS") {
			cmdStats();
		} else if (cmd == "DELETE") {
			int idx; ss >> idx; cmdDelete(idx);
		} else if (cmd == "UNDO") {
			cmdUndo();
		} else if (cmd == "RECENT") {
			int n; ss >> n; cmdRecent(n);
		} else {
			// ignore unknown
		}
	}
	return 0;
}
