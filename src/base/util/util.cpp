#include "base/util/util.h"
#include "log/logging.h"

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
const int IbnUtil::LARGE = 2147483647;

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

double IbnUtil::JaroWinklerDistance(String a, String b) {
    std::transform(a.begin(), a.end(), a.begin(), ::tolower);
    std::transform(b.begin(), b.end(), b.begin(), ::tolower);

    size_t len1 = a.size();
    size_t len2 = b.size();
    // Make sure that len1 is no less than len2.
    if(len1 < len2) {
        std::swap(a, b);
        std::swap(len1, len2);
    }
    /* Since len2 is 0, len1 must be no less than 0, 
     * so len1 could be either 0 or greater than 0.
     * In this case, if len1 is 0, the jaro-winkler distance
     * is 0(a is the same as b), otherwise ,
     * it's 1(a an b are completely different).
     */
    if(len2 == 0) {
        return len1 == 0 ? 0.0 : 1.0;
    }
    
    // Make sure that match-window size is a positive number.
    size_t delta = std::max(size_t(1), len1 / 2) - 1;
    
    // Initialize the flag, which determine if the char in a and b is matched.
    std::vector<bool> flag(len2, false);
    // Restore the char in a that matched with the char in b. 
    std::vector<char> ch1_match;
    ch1_match.reserve(len1);

    for(size_t idx1 = 0; idx1 < len1; ++idx1) {
        char ch1 = a[idx1];
        for(size_t idx2 = 0; idx2 < len2; ++idx2) {
            char ch2 = b[idx2];
            if(idx2 <= idx1 + delta && idx2 + delta >= idx1 &&
               ch1 == ch2 && !flag[idx2]) {
                flag[idx2] = true;
                ch1_match.push_back(ch1);
                break;
            }
        }
    }
    
    // m
    size_t matches = ch1_match.size();
    
    if(matches == 0) {
        return 1.0;
    }

    size_t transpositions = 0;
    for(size_t idx1 = 0, idx2 = 0; idx2 < len2; ++idx2) {
        if(flag[idx2]) {
            if(b[idx2] != ch1_match[idx1]) {
                ++transpositions;
            }
            ++idx1;
        }
    }

    // m
    double m = matches;
    double jaro = (m/len1 + m/len2 + (m - transpositions/2.0)/m) / 3.0;
    // l
    size_t common_prefix = 0;
    // l is less than 4.
    len2 = std::min(size_t(4), len2);
    for(size_t i = 0; i < len2; ++i) {
        if(a[i] == b[i]) {
            ++common_prefix;
        }
    }

    return (jaro + common_prefix * 0.1 * (1.0 - jaro)) * 100;
}

double IbnUtil::DamerauLevenshteinDistance(String a, String b) {
    std::transform(a.begin(), a.end(), a.begin(), ::tolower);
    std::transform(b.begin(), b.end(), b.begin(), ::tolower);
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
            if(i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1]) {
                dp[i][j] = std::min(dp[i][j], dp[i - 2][j - 2] + 1);
            }
        }
    }

    double levenshtein_distance = static_cast<double>(dp[I - 1][J - 1]);

    int i = static_cast<int>(b.size());
    
    auto ratio = (1 - (levenshtein_distance / i)) * 100;
    return ratio;
}

double IbnUtil::Similarity(String a, String b, DIS dis) {
    return dis(a, b);
}

StringList IbnUtil::Split(String str, const String& delim, int limit) {
    StringList arr;
    if(limit <= 0)
        limit = LARGE;

    for(int i = 0; i < limit; ++i) {
        size_t pos = str.find(delim);
        if(pos != String::npos) {
            arr.push_back(str.substr(0, pos));
            str = str.substr(pos + delim.length());
        }
        else break;
    }

    arr.push_back(str);
    return arr;
}

bool IbnUtil::LogicTrue(String str) {
    if(IbnUtil::IgnoreCaseCompare(str, "y") || IbnUtil::IgnoreCaseCompare(str, "yes")) {
        return true;
    }
    return false;
}

bool IbnUtil::LogicFalse(String str) {
    if(IbnUtil::IgnoreCaseCompare(str, "n") || IbnUtil::IgnoreCaseCompare(str, "no")) {
        return true;
    }
    return false;
}

bool IbnUtil::IsNum(String str) {
    return str.find_first_not_of("0123456789") == std::string::npos;
}

} // !namespace ibn
