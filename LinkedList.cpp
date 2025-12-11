#include "LinkedList.h"
#include <sstream>
#include <iomanip>
#include <cstdlib>

LinkedList::LinkedList() : head_(nullptr), tail_(nullptr), size_(0)
{
}

LinkedList::~LinkedList()
{
	clear();
}

void LinkedList::clear()
{
	LogNode* p = head_;
	while (p) {
		LogNode* nextNode = p->next; // save next
		delete p;
		p = nextNode;
	}
	head_ = tail_ = nullptr;
	size_ = 0;
}

void LinkedList::push_back(LogNode* node)
{
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



void LinkedList::push_front(LogNode* node)
{
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

LogNode* LinkedList::remove_at(int index)
{
	if (index < 1 || index > size_) return nullptr;
	LogNode* p = head_;
	int ii = 1;
	while (ii < index) { p = p->next; ++ii; }
	if (p->prev) p->prev->next = p->next; else head_ = p->next;
	if (p->next) p->next->prev = p->prev; else tail_ = p->prev;
	p->prev = p->next = nullptr;
	--size_;
	return p;
}

int LinkedList::size() const { return size_; }
LogNode* LinkedList::head() const { return head_; }
LogNode* LinkedList::tail() const { return tail_; }

// Convert  "YYYY-MM-DD HH:MM:SS" to long long YYYYMMDDHHMMSS
long long LinkedList::parseTimeKey(const std::string& ts)
{
	// expect length >= 19
	if (ts.size() < 19) return 0;
	std::string s;
	for (char c : ts) if (std::isdigit((unsigned char)c)) s.push_back(c);
	if (s.size() < 14) return 0;
	return std::atoll(s.c_str());
}

std::string LinkedList::formatFullLine(const LogNode* node)
{
	if (!node) return std::string();
	std::string out = "[" + node->timestamp + "] " + node->level + " " + node->module + " " + node->message;
	return out;
}
