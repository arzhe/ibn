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
    static String RTrim(const String& str, const String& delim = IbnUtil::WHITESPACE);

    static String LTrim(const String& str, const String& delim = IbnUtil::WHITESPACE);

    static String Trim(const String& str, const String& delim = IbnUtil::WHITESPACE);

    static bool IgnoreCaseCompare(const String& a, const String& b);

    static String ToLowerString(String str);

    static double Similarity(const String& a, const String& b);
    
private:
    static const String WHITESPACE;
};

} // !namespace ibn

#endif // !IBN_BASE_UTIL_UTIL_H
