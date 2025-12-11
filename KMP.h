#ifndef KMP_H
#define KMP_H

#include <string>

class KMP {
public:
    // returns true if pattern exists in text
    static bool contains(const std::string& text, const std::string& pattern);
    // compute lps
    static void computeLPS(const std::string& pat, int* lps);
};

#endif