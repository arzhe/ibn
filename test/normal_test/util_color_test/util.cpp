#include "./util.h"

namespace ibn {

/* namespace util { */

/* const char digits[] = "9876543210123456789"; */
/* const char* zero = digits + 9; */
/* static_assert(sizeof(digits) == 20, "wrong number of digits"); */

/* template<typename T> */
/* size_t convert(char buf[], T value) { */
/*     T i = value; */
/*     char* p = buf; */

/*     do { */
/*         int lsd = static_cast<int>(i % 10); */
/*         i /= 10; */
/*         *p++ = zero[lsd]; */
/*     } while(i != 0); */

/*     if(value < 0) { */
/*         *p++ = '-'; */
/*     } */
/*     *p = '\0'; */
/*     std::reverse(buf, p); */

/*     return p - buf; */
/* } */

/* } // !namespace util */

const String IbnUtil::WHITESPACE = "\n\r\t\v\f";

String IbnUtil::RTrim(const String& str, const String& delim) {
    size_t pos = str.find_last_not_of(delim);
    return (pos == String::npos) ? "" : str.substr(0, pos + 1);
}

String IbnUtil::LTrim(const String& str, const String& delim) {
    size_t pos = str.find_first_not_of(delim);
    return (pos == String::npos) ? "" : str.substr(pos);
}

String IbnUtil::Trim(const String& str, const String& delim) {
    return RTrim(LTrim(str, delim), delim);
}

bool IbnUtil::IgnoreCaseCompare(const String& a, const String& b) {
    if(a.length() == b.length()) {
        return std::equal(a.begin(), a.end(), b.begin(),
                          [](char a, char b) {
                            return tolower(a) == tolower(b);
                          });
    }
    else {
        return false;
    }
}

String IbnUtil::ToLowerString(String str) {
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

double IbnUtil::Similarity(const String& a, const String& b) {
    int I = static_cast<int>(a.size()) + 1;
    int J = static_cast<int>(b.size()) + 1;
    std::vector<std::vector<int>> dp;

    // Initialize the border.
    for(int i = 0; i < I; ++i) {
        std::vector<int> tmp;

        for(int j = 0; j < J; ++j) {
            if(i == 0)      tmp.emplace_back(j);
            else if(j == 0) tmp.emplace_back(i);
            else            tmp.emplace_back(0);
        }
        dp.emplace_back(tmp);
    }
        
    for(int i = 1; i < I; ++i) {
        for(int j = 1; j < J; ++j) {
            if(a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            }
            else {
                dp[i][j] = std::min(std::min(dp[i - 1][j], dp[i][j - 1]),
                               dp[i - 1][j - 1]) + 1;
            }
        }
    }

    double levenshtein_distance = static_cast<double>(dp[I - 1][J - 1]);

    int i = static_cast<int>(b.size());
    return (1 - (levenshtein_distance / i)) * 100;
}

} // !namespace ibn
