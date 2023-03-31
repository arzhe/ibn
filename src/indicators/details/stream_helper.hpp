#ifndef INDICATORS_STREAM_HELPER_HPP
#define INDICATORS_STREAM_HELPER_HPP

#include <algorithm>
#include <chrono>
#include <iomanip>
#include <ostream>
#include <string>
#include <vector>

#include <cassert>
#include <cmath>

#include "indicators/display_width.hpp"
#include "indicators/setting.hpp"
#include "indicators/termcolor.hpp"

namespace indicators {

namespace details {

inline void SetStreamColor(std::ostream& os, Color color) {
    switch(color) {
    case Color::Grey:
        os << termcolor::Grey;
        break;
    case Color::Red:
        os << termcolor::Red;
        break;
    case Color::Green:
        os << termcolor::Green;
        break;
    case Color::Yellow:
        os << termcolor::Yellow;
        break;
    case Color::Blue:
        os << termcolor::Blue;
        break;
    case Color::Magenta:
        os << termcolor::Magenta;
        break;
    case Color::Cyan:
        os << termcolor::Cyan;
        break;
    case Color::White:
        os << termcolor::White;
        break;
    default:
        assert(false);
    }
}

inline void SetFontStyle(std::ostream& os, FontStyle style) {
    switch(style) {
    case FontStyle::Bold:
        os << termcolor::Bold;
        break;
    case FontStyle::Dark:
        os << termcolor::Dark;
        break;
    case FontStyle::Italic:
        os << termcolor::Italic;
        break;
    case FontStyle::Underline:
        os << termcolor::Underline;
        break;
    case FontStyle::Blink:
        os << termcolor::Blink;
        break;
    case FontStyle::Reverse:
        os << termcolor::Reverse;
        break;
    case FontStyle::Concealed:
        os << termcolor::Concealed;
        break;
    case FontStyle::Crossed:
        os << termcolor::Crossed;
        break;
    default:
        break;
    }
}

inline std::ostream& WriteDuration(std::ostream& os, std::chrono::nanoseconds ns) {
    using namespace std;
    using namespace std::chrono;
    using days = duration<int, ratio<86400>>;
    // By default, cout fills the unused parts of a field with 'Spaces', which can be
    // changed with the fill().
    char fill = os.fill();
    os.fill('0');

    auto d = duration_cast<days>(ns);
    ns -= d;
    auto h = duration_cast<hours>(ns);
    ns -= h;
    auto m = duration_cast<minutes>(ns);
    ns -= m;
    auto s = duration_cast<seconds>(ns);

    if(d.count() > 0) {
        os << setw(2) << d.count() << "d:";
    }
    if(h.count() > 0) {
        os << setw(2) << h.count() << "h:";
    }
    os << setw(2) << m.count() << "m:" << setw(2) << s.count() << 's';
    os.fill(fill);
    return os;
}

class BlockProgressScaleWriter {
public:
    BlockProgressScaleWriter(std::ostream& os, size_t bar_width) 
        : os_(os), bar_width_(bar_width) {}
    
    std::ostream& Write(float progress) {
        std::string fill_text{"█"};
        std::vector<std::string> lead_characters{" ", "▏", "▎", "▍", "▌", "▋", "▊", "▉"};
        
        auto value = (std::min)(1.0f, (std::max)(0.0f, progress / 100.0f));
        auto whole_width = std::floor(value * bar_width_);
        auto remainder_width = fmod((value * bar_width_), 1.0f);
        auto part_width = std::floor(remainder_width * lead_characters.size());
        std::string lead_text = lead_characters[size_t(part_width)];

        if((bar_width_ - whole_width - 1) < 0) {
            lead_text = "";
        }
        for(size_t i = 0; i < whole_width; ++i) {
            os_ << fill_text;
        }
        os_ << lead_text;
        for(size_t i = 0; i < (bar_width_ - whole_width - 1); ++i) {
            os_ << " ";
        }
        return os_;
    }

private:
    std::ostream& os_;
    size_t bar_width_ = 0;
};

class ProgressScaleWriter {
public:
    ProgressScaleWriter(std::ostream& os, size_t bar_width, const std::string& fill,
                        const std::string& lead, const std::string& remainder)
        : os_(os), bar_width_(bar_width), fill_(fill), lead_(lead), remainder_(remainder) {}

    std::ostream& Write(float progress) {
        auto pos = static_cast<size_t>(progress * bar_width_ / 100.0);
        for(size_t i = 0, current_display_width = 0; i < bar_width_;) {
            std::string next;

            if(i < pos) {
                next = fill_;
                current_display_width = unicode::DisplayWidth(fill_);
            }
            else if(i == pos) {
                next = lead_;
                current_display_width = unicode::DisplayWidth(lead_);
            }
            else {
                next = remainder_;
                current_display_width = unicode::DisplayWidth(remainder_);
            }

            i += current_display_width;

            if(i > bar_width_) {
                os_ << std::string((bar_width_ - (i - current_display_width)), ' ');
                break;
            }

            os_ << next;
        }
        return os_;
    }

private:
    std::ostream& os_;
    size_t bar_width_ = 0;
    std::string fill_;
    std::string lead_;
    std::string remainder_;
};

class IndeterminateProgressScaleWriter {
public:
    IndeterminateProgressScaleWriter(std::ostream& os, size_t bar_width, const std::string& fill,
                                     const std::string& lead)
        : os_(os), bar_width_(bar_width), fill_(fill), lead_(lead) {}

    std::ostream& Write(size_t progress) {
        for(size_t i = 0; i < bar_width_;) {
            std::string next;
            size_t current_display_width = 0;

            if(i < progress) {
                next = fill_;
                current_display_width = unicode::DisplayWidth(fill_);
            }
            else if(i == progress) {
                next = lead_;
                current_display_width = unicode::DisplayWidth(lead_);
            }
            else {
                next = fill_;
                current_display_width = unicode::DisplayWidth(fill_);
            }

            i += current_display_width;

            if(i > bar_width_) {
                os_ << std::string((bar_width_ - (i - current_display_width)), ' ');
                break;
            }

            os_ << next;
        }
        return os_;
    }

private:
    std::ostream& os_;
    size_t bar_width_ = 0;
    std::string fill_;
    std::string lead_;
};

} // !namespace details

} // !namespace indicators

#endif // !INDICATORS_STREAM_HELPER_HPP
