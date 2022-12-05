#ifndef IBN_BASE_UTIL_UTIL_H
#define IBN_BASE_UTIL_UTIL_H

#include <string>
#include <vector>
#include <algorithm>

typedef std::string String;
typedef std::vector<String> StringList;

namespace ibn {

/* namespace util { */

/* const char digits[] = "9876543210123456789"; */
/* const char* zero = digits + 9; */
/* static_assert(sizeof(digits) == 20, "wrong number of digits"); */

/* // Efficient Integer to String Conversions, by Matthew Wilson. */
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

/* #define BLACKSTRING(STR)  std::string("\033[30;1m" STR "\033[0m") */
/* #define REDSTRING(STR)    std::string("\033[31;1m" STR "\033[0m") */
/* #define GREENSTRING(STR)  std::string("\033[32;1m" STR "\033[0m") */
/* #define YELLOWSTRING(STR) std::string("\033[33;1m" STR "\033[0m") */
/* #define BLUESTRING(STR)   std::string("\033[34;1m" STR "\033[0m") */
/* #define PURPLESTRING(STR) std::string("\033[35;1m" STR "\033[0m") */
/* #define CYANSTRING(STR)   std::string("\033[36;1m" STR "\033[0m") */
/* #define WHITESTRING(STR)  std::string("\033[31;1m" STR "\033[0m") */

#define BLACK  0
#define RED    1
#define GREEN  2
#define YELLOW 3
#define BLUE   4
#define PURPLE 5
#define CYAN   6
#define WHITE  7

// Render font color.
template<typename T>
std::string ColorString(const std::string& str) { return ""; }

template<int N>
std::string ColorString(const std::string& str) {
    switch(N) {
        case 0:
            return "\033[30;1m" + str + "\033[0m";
        case 1:
            return "\033[31;1m" + str + "\033[0m";
        case 2:
            return "\033[32;1m" + str + "\033[0m";
        case 3:
            return "\033[33;1m" + str + "\033[0m";
        case 4:
            return "\033[34;1m" + str + "\033[0m";
        case 5:
            return "\033[35;1m" + str + "\033[0m";
        case 6:
            return "\033[36;1m" + str + "\033[0m";
        case 7:
            return "\033[37;1m" + str + "\033[0m";
        default:
            return str;
    }
}


class IbnUtil {
public:
    // Alias for algorithm of distance.
    using DIS = double (*)(String, String);

    /*--------------------------------------------------------------------------------
     * Cropp the WHITESPACE or OTHER DELIM on both sides of the string.
     */ 
    static String RTrim(const String& str, const String& delim = IbnUtil::WHITESPACE);

    static String LTrim(const String& str, const String& delim = IbnUtil::WHITESPACE);

    static String Trim(const String& str, const String& delim = IbnUtil::WHITESPACE);

    /*--------------------------------------------------------------------------------
     * Ignore case-sensitive comparisons between two strings.
     */ 
    static bool IgnoreCaseCompare(const String& a, const String& b);

    /*--------------------------------------------------------------------------------
     * Transfer the string to lower-case string.
     */ 
    static String ToLowerString(String str);

    /*--------------------------------------------------------------------------------
     * The algorithm of distance.
     * And the "Similarity" of two characters is calculated according to the distance algorithm.
     * By default, the JaroWinklerDistance algorithm is used.
     */ 
    static double JaroWinklerDistance(String a, String b);
    
    static double DamerauLevenshteinDistance(String a, String b);

    static double Similarity(String a, String b, DIS dis = &IbnUtil::JaroWinklerDistance);
 
    /*--------------------------------------------------------------------------------
     * Divides the string into "limit" substrings based on "delim".
     * By default , the "limit" number is set to zero.
     */ 
    static StringList Split(String str, const String& delim, int limit = 0);

    /*--------------------------------------------------------------------------------
     * Determines if the string is logically true or false.
     * As for, "yes", "NO" and etc.
     */
    static bool LogicTrue(String str);
    static bool LogicFalse(String str);

    /*--------------------------------------------------------------------------------
     * Determines if the string is a number.
     */ 
    static bool IsNum(String str);

private:
    static const String WHITESPACE;
    static const int LARGE;
};

} // !namespace ibn

#endif // !IBN_BASE_UTIL_UTIL_H
