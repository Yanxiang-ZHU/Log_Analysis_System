#include "KMP.h"
#include <cstring>

void KMP::computeLPS(const std::string& pat, int* lps) {
    int n = (int)pat.size();
    if (n==0) return;
    lps[0] = 0;
    int len = 0;
    int i = 1;
    while (i < n) {
        if (pat[i] == pat[len]) {
            ++len; lps[i] = len; ++i;
        } else {
            if (len != 0) len = lps[len-1];
            else { lps[i] = 0; ++i; }
        }
    }
}

bool KMP::contains(const std::string& text, const std::string& pattern) {
    int n = (int)text.size();
    int m = (int)pattern.size();
    if (m == 0) return true;
    if (n < m) return false;
    int* lpsArr = new int[m];
    computeLPS(pattern, lpsArr);
    int i = 0, j = 0;
    while (i < n) {
        if (text[i] == pattern[j]) { ++i; ++j; if (j==m) { delete[] lpsArr; return true; } }
        else {
            if (j != 0) j = lpsArr[j-1]; else ++i;
        }
    }
    delete[] lpsArr;
    return false;
}
