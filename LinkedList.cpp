#include "LinkedList.h"
#include <sstream>
#include <iomanip>
#include <cstdlib>

LinkedList::LinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}

LinkedList::~LinkedList() { clear(); }

void LinkedList::clear() {
	LogNode* cur = head_;
	while (cur) {
		LogNode* nxt = cur->next;
		delete cur;
		cur = nxt;
	}
	head_ = tail_ = nullptr;
	size_ = 0;
}

void LinkedList::push_back(LogNode* node) {
	if (!node) return;
	node->prev = node->next = nullptr;
	if (!tail_) {
		head_ = tail_ = node;
	} else {
		tail_->next = node;
		node->prev = tail_;
		tail_ = node;
	}
	++size_;
}

void LinkedList::push_front(LogNode* node) {
	if (!node) return;
	node->prev = node->next = nullptr;
	if (!head_) {
		head_ = tail_ = node;
	} else {
		head_->prev = node;
		node->next = head_;
		head_ = node;
	}
	++size_;
}

LogNode* LinkedList::remove_at(int index) {
	if (index < 1 || index > size_) return nullptr;
	LogNode* cur = head_;
	int i = 1;
	while (i < index) { cur = cur->next; ++i; }
	if (cur->prev) cur->prev->next = cur->next; else head_ = cur->next;
	if (cur->next) cur->next->prev = cur->prev; else tail_ = cur->prev;
	cur->prev = cur->next = nullptr;
	--size_;
	return cur;
}

int LinkedList::size() const { return size_; }
LogNode* LinkedList::head() const { return head_; }
LogNode* LinkedList::tail() const { return tail_; }

// Convert timestamp "YYYY-MM-DD HH:MM:SS" to long long YYYYMMDDHHMMSS
long long LinkedList::parseTimeKey(const std::string& ts) {
	// expect length >= 19
	if (ts.size() < 19) return 0;
	std::string s;
	for (char c : ts) if (std::isdigit((unsigned char)c)) s.push_back(c);
	if (s.size() < 14) return 0;
	return std::atoll(s.c_str());
}

std::string LinkedList::formatFullLine(const LogNode* node) {
	if (!node) return std::string();
	std::string out = "[" + node->timestamp + "] " + node->level + " " + node->module + " " + node->message;
	return out;
}
