#ifndef INDICATORS_TERMCOLOR_HPP
#define INDICATORS_TERMCOLOR_HPP

#include <iostream>
#include <cstdio>
#include <unistd.h>

namespace termcolor {



/*-----------------------------------------------------------------------------------
 * Forward delaration of the 'inter' namespace.
 */
namespace inter {

/*-----------------------------------------------------------------------------------
 * Base tool.
 */
inline int ColorizeIndex();
inline FILE* GetStandardStream(const std::ostream& stream);
inline bool IsColorized(std::ostream& stream);
inline bool IsAtty(const std::ostream& stream);

} // !namespace inter



/*-----------------------------------------------------------------------------------
 * Determine whether the stream is colorized.
 */
inline std::ostream& Colorize(std::ostream& stream) {
    stream.iword(inter::ColorizeIndex()) = 1L;
    return stream;
}

inline std::ostream& NoColorize(std::ostream& stream) {
    stream.iword(inter::ColorizeIndex()) = 0L;
    return stream;
}

/*-----------------------------------------------------------------------------------
 * Indicators for the stream.
 */
inline std::ostream& Reset(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[00m";
    }
    return stream;
}

inline std::ostream& Bold(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[1m";
    }
    return stream;
}

inline std::ostream& Dark(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[2m";
    }
    return stream;
}

inline std::ostream& Italic(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[3m";
    }
    return stream;
}

inline std::ostream& Underline(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[4m";
    }
    return stream;
}

inline std::ostream& Blink(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[5m";
    }
    return stream;
}

inline std::ostream& Reverse(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[7m";
    }
    return stream;
}

inline std::ostream& Concealed(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[8m";
    }
    return stream;
}

inline std::ostream& Crossed(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[9m";
    }
    return stream;
}

template<uint8_t code>
inline std::ostream& Color(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        char command[12];
        std::snprintf(command, sizeof(command), "\033[38;5;%dm", code);
        stream << command;
    }
    return stream;
}

template<uint8_t code>
inline std::ostream& OnColor(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        char command[12];
        std::snprintf(command, sizeof(command), "\033[48;5;%dm", code);
        stream << command;
    }
    return stream;
}

template<uint8_t r, uint8_t g, uint8_t b>
inline std::ostream& Color(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        char command[20];
        std::snprintf(command, sizeof(command), "\033[38;2;%d;%d;%dm", r, g, b);
        stream << command;
    }
    return stream;
}

template<uint8_t r, uint8_t g, uint8_t b>
inline std::ostream& OnColor(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        char command[20];
        std::snprintf(command, sizeof(command), "\033[48;2;%d;%d;%dm", r, g, b);
        stream << command;
    }
    return stream;
}

inline std::ostream& Grey(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[30m";
    }
    return stream;
}

inline std::ostream& Red(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[31m";
    }
    return stream;
}

inline std::ostream& Green(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[32m";
    }
    return stream;
}

inline std::ostream& Yellow(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[33m";
    }
    return stream;
}

inline std::ostream& Blue(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[34m";
    }
    return stream;
}

inline std::ostream& Magenta(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[35m";
    }
    return stream;
}

inline std::ostream& Cyan(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[36m";
    }
    return stream;
}

inline std::ostream& White(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[37m";
    }
    return stream;
}

inline std::ostream& BrightGrey(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[90m";
    }
    return stream;
}

inline std::ostream& BrightRed(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[91m";
    }
    return stream;
}

inline std::ostream& BrightGreen(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[92m";
    }
    return stream;
}

inline std::ostream& BrightYellow(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[93m";
    }
    return stream;
}

inline std::ostream& BrightBlue(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[94m";
    }
    return stream;
}

inline std::ostream& BrightMagenta(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[95m";
    }
    return stream;
}

inline std::ostream& BrightCyan(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[96m";
    }
    return stream;
}

inline std::ostream& BrightWhite(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[97m";
    }
    return stream;
}

inline std::ostream& OnGrey(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[40m";
    }
    return stream;
}

inline std::ostream& OnRed(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[41m";
    }
    return stream;
}

inline std::ostream& OnGreen(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[42m";
    }
    return stream;
}

inline std::ostream& OnYellow(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[43m";
    }
    return stream;
}

inline std::ostream& OnBlue(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[44m";
    }
    return stream;
}

inline std::ostream& OnMagenta(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[45m";
    }
    return stream;
}

inline std::ostream& OnCyan(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[46m";
    }
    return stream;
}

inline std::ostream& OnWhite(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[47m";
    }
    return stream;
}

inline std::ostream& OnBrightGrey(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[100m";
    }
    return stream;
}

inline std::ostream& OnBrightRed(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[101m";
    }
    return stream;
}

inline std::ostream& OnBrightGreen(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[102m";
    }
    return stream;
}

inline std::ostream& OnBrightYellow(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[103m";
    }
    return stream;
}

inline std::ostream& OnBrightBlue(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[104m";
    }
    return stream;
}

inline std::ostream& OnBrightMagenta(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[105m";
    }
    return stream;
}

inline std::ostream& OnBrightCyan(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[106m";
    }
    return stream;
}

inline std::ostream& OnBrightWhite(std::ostream& stream) {
    if(inter::IsColorized(stream)) {
        stream << "\033[107m";
    }
    return stream;
}


/*-----------------------------------------------------------------------------------
 * Since C++ hasn't a way to hide something in the header from the outer access, 
 * I have to introduce this namespace which is used for internal purpose and should
 * not be access from the user code.
 */
namespace inter {

/*-----------------------------------------------------------------------------------
 * An index to be used to access a private storage of I/O streams. Due to the fact
 * that static variables ain't shared between translation units, inline function with
 * local static variable is used to do the trick and share the variable value between
 * translation units.
 */
inline int ColorizeIndex() {
    static int colorize_index = std::ios_base::xalloc();
    return colorize_index;
}

/*-----------------------------------------------------------------------------------
 * An index to be used to access a private storage of I/O streams. Due to the fact
 * that static variables ain't shared between translation units, inline function with
 * local static variable is used to do the trick and share the variable value between
 * translation units.
 */
inline FILE* GetStandardStream(const std::ostream& stream) {
    if(&stream == &std::cout) {
        return stdout;
    }
    else if((&stream == &std::cerr) || (&stream == &std::clog)) {
        return stderr;
    }

    return nullptr;
}

/*-----------------------------------------------------------------------------------
 * Say whether a given stream should be colorized or not. It's always true for ATTY
 * streams and may be true for streams marked with colorized flag.
 */
inline bool IsColorized(std::ostream& stream) {
    return IsAtty(stream) || static_cast<bool>(stream.iword(ColorizeIndex()));
}

/*-----------------------------------------------------------------------------------
 * Test whether a given 'std::ostream' object refers to a terminal.
 */
inline bool IsAtty(const std::ostream& stream) {
    FILE* std_stream = GetStandardStream(stream);

    // Unfortunately, fileno() ends with segmentation fault if invalid file
    // descriptor is passed. So we need to handle this case gracefully and
    // assume it's not a tty if standard stream is not detected, and 0 is returned.
    if(!std_stream) {
        return false;
    }

    return ::isatty(fileno(std_stream));
}

} // !namespace inter

} // !namespace termcolor


#endif // !INDICATORS_TERMCOLOR_HPP
