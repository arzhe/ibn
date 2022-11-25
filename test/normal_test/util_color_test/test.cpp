#include <iostream>
#include <string>

#include "./util.h"

using namespace std;
using namespace ibn;

class Sayings {
public:
    static const std::string SUCCESS;
};

const std::string Sayings::SUCCESS = ColorString<RED>("<IBN>");

int main()
{
    cout << Sayings::SUCCESS << endl;

    const std::string str1 = "adfkjashgfwjfjqwefdsajf";
    const std::string str2 = "sadfhewabfdsfasdjflkdah";
    cout << IbnUtil::Similarity(str1, str2) << endl;
    
    const std::string str3 = "wuhan";
    const std::string str4 = "whn";
    cout << IbnUtil::Similarity(str4, str3) << endl;

    cout << ColorString<BLACK>("hello") << endl;
    cout << ColorString<RED>("hello") << endl;
    cout << ColorString<BLUE>("hello") << endl;
    cout << ColorString<GREEN>("hello") << endl;
    return 0;
}

