#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <string>


struct LogNode {
	std::string timestamp;
	long long timeKey; // number for fast compare
	std::string level;
	std::string module;
	std::string message;
	LogNode* prev;
	LogNode* next;
	LogNode(const std::string& ts = "", long long tk = 0,
			const std::string& lvl = "", const std::string& mod = "", const std::string& msg = "")
		: timestamp(ts), timeKey(tk), level(lvl), module(mod), message(msg), prev(nullptr), next(nullptr) {}
};

class LinkedList {
public:
	LinkedList();
	~LinkedList();

	void clear();
	void push_back(LogNode* node);
	void push_front(LogNode* node);
	LogNode* remove_at(int index); // returns removed node (caller should delete)
	int size() const;
	LogNode* head() const;
	LogNode* tail() const;

	// helpers
	static long long parseTimeKey(const std::string& ts);
	static std::string formatFullLine(const LogNode* node);

private:
	LogNode* head_;
	LogNode* tail_;
	int size_;
};

#endif