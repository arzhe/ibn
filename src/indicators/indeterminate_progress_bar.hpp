#ifndef INDICATORS_INDETERMINATE_PROGRESS_BAR_HPP
#define INDICATORS_INDETERMINATE_PROGRESS_BAR_HPP

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <type_traits>
#include <sstream>
#include <utility>

#include "indicators/color.hpp"
#include "indicators/setting.hpp"
#include "indicators/terminal_size.hpp"
#include "indicators/details/stream_helper.hpp"

namespace indicators {

class IndeterminateProgressBar {
    using Settings =
        std::tuple<option::BarWidth, option::PrefixText, option::PostfixText,
                   option::Start, option::End, option::Fill, option::Lead,
                   option::MaxPostfixTextLen, option::Completed,
                   option::ForegroundColor, option::FontStyles, option::Stream>;

    enum class Direction { Forward, Backward };

    Direction direction_ { Direction::Forward };

public:
    template<typename... Args,
             typename std::enable_if<details::are_settings_from_tuple<
                                         Settings, typename std::decay<Args>::type...>::value, 
                                     void*>::type = nullptr>
    explicit IndeterminateProgressBar(Args&&... args)
        : settings_(
            details::Get<details::ProgressBarOption::BarWidth>(
                option::BarWidth{100}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::PrefixText>(
                option::PrefixText{}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::PostfixText>(
                option::PostfixText{}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::Start>(
                option::Start{"["}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::End>(
                option::End{"]"}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::Fill>(
                option::Fill{"."}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::Lead>(
                option::Lead{"<==>"}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::MaxPostfixTextLen>(
                option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::Completed>(
                option::Completed{false}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::ForegroundColor>(
                option::ForegroundColor{Color::Unspecified}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::FontStyles>(
                option::FontStyles{std::vector<FontStyle>{}}, std::forward<Args>(args)...),

            details::Get<details::ProgressBarOption::Stream>(
                option::Stream{std::cout}, std::forward<Args>(args)...)) {
        
        // Starts with [<==>...........]
        // progress_ = 0
        // 
        // Ends with [...........<==>]
        //            ^^^^^^^^^^^^^^^ bar_width
        //            ^^^^^^^^^^^ (bar_width - len(lead))
        // progress_ = bar_width - len(lead)
        progress_ = 0;
        max_progress_ = GetValue<details::ProgressBarOption::BarWidth>() -
                        GetValue<details::ProgressBarOption::Lead>().size();
    }

    template<typename T, details::ProgressBarOption id>
    void SetOption(details::Setting<T, id>&& setting) {
        static_assert(!std::is_same<T, typename std::decay<decltype(details::GetValue<id>(
                                    std::declval<Settings>()))>::type>::value, 
                      "Setting has wrong type!");
        std::lock_guard<std::mutex> lock(mutex_);
        GetValue<id>() = std::move(setting).value;
    }

    template<typename T, details::ProgressBarOption id>
    void SetOption(const details::Setting<T, id>& setting) {
        static_assert(!std::is_same<T, typename std::decay<decltype(details::GetValue<id>(
                                    std::declval<Settings>()))>::type>::value, 
                      "Setting has wrong type!");
        std::lock_guard<std::mutex> lock(mutex_);
        GetValue<id>() = setting.value;
    }

    void SetOption(const details::Setting<std::string, details::ProgressBarOption::PostfixText>& setting) {
        std::lock_guard<std::mutex> lock(mutex_);
        GetValue<details::ProgressBarOption::PostfixText>() = setting.value;
        if(setting.value.length() > GetValue<details::ProgressBarOption::MaxPostfixTextLen>()) {
            GetValue<details::ProgressBarOption::MaxPostfixTextLen>() = setting.value.length();
        }
    }

    void SetOption(details::Setting<std::string, details::ProgressBarOption::PostfixText>&& setting) {
        std::lock_guard<std::mutex> lock(mutex_);
        GetValue<details::ProgressBarOption::PostfixText>() = std::move(setting).value;
        auto& new_value = GetValue<details::ProgressBarOption::PrefixText>();
        if(new_value.length() > GetValue<details::ProgressBarOption::MaxPostfixTextLen>()) {
            GetValue<details::ProgressBarOption::MaxPostfixTextLen>() = new_value.length();
        }
    }

    void Tick() {
        {
            std::lock_guard<std::mutex> lock{mutex_};
            if(GetValue<details::ProgressBarOption::Completed>()) {
                return;
            }

            progress_ += (direction_ == Direction::Forward) ? 1 : -1;
            if(direction_ == Direction::Forward && progress_ == max_progress_) {
                // time to go back
                direction_ = Direction::Backward;
            }
            else if(direction_ == Direction::Backward && progress_ == 0) {
                direction_ = Direction::Forward;
            }
        }
        PrintProgress();
    }

    bool IsCompleted() {
        return GetValue<details::ProgressBarOption::Completed>();
    }

    void MarkAsCompleted() {
        GetValue<details::ProgressBarOption::Completed>() = true;
        PrintProgress();
    }

private:
    template<details::ProgressBarOption id>
    auto GetValue()
        -> decltype((details::GetValue<id>(std::declval<Settings&>()).value)) {
        return details::GetValue<id>(settings_).value;
    }

    template<details::ProgressBarOption id>
    auto GetValue() const
        -> decltype((details::GetValue<id>(std::declval<const Settings&>()).value)) {
        return details::GetValue<id>(settings_).value;
    }

    size_t progress_{0};
    size_t max_progress_;
    Settings settings_;
    std::chrono::nanoseconds elapsed_;
    std::mutex mutex_;

    template<typename Indicator, size_t count> friend class MultiProgress;
    template<typename Indicator> friend class DynamicProgress;
    std::atomic<bool> multi_progress_mode_{false};

    std::pair<std::string, size_t> GetPrefixText() {
        std::stringstream os;
        os << GetValue<details::ProgressBarOption::PrefixText>();
        const auto result = os.str();
        const auto result_size = unicode::DisplayWidth(result);
        return { result, result_size };
    }

    std::pair<std::string, size_t> GetPostfixText() {
        std::stringstream os;
        os << " " << GetValue<details::ProgressBarOption::PostfixText>();
        const auto result = os.str();
        const auto result_size = unicode::DisplayWidth(result);
        return { result, result_size };
    }

public:
    void PrintProgress(bool from_multi_progress = false) {
        std::lock_guard<std::mutex> lock{mutex_};

        auto& os = GetValue<details::ProgressBarOption::Stream>();

        if(multi_progress_mode_ && !from_multi_progress) {
            return;
        }

        if(GetValue<details::ProgressBarOption::ForegroundColor>() != Color::Unspecified) {
            details::SetStreamColor(os, GetValue<details::ProgressBarOption::ForegroundColor>());
        }

        for(auto& style : GetValue<details::ProgressBarOption::FontStyles>()) {
            details::SetFontStyle(os, style);
        }

        const auto prefix_pair = GetPrefixText();
        const auto prefix_text = prefix_pair.first;
        const auto prefix_length = prefix_pair.second;
        os << "\r" << prefix_text;

        os << GetValue<details::ProgressBarOption::Start>();

        details::IndeterminateProgressScaleWriter writer {
            os, GetValue<details::ProgressBarOption::BarWidth>(),
            GetValue<details::ProgressBarOption::Fill>(),
            GetValue<details::ProgressBarOption::Lead>()};

        writer.Write(progress_);

        os << GetValue<details::ProgressBarOption::End>();

        const auto postfix_pair = GetPostfixText();
        const auto postfix_text = postfix_pair.first;
        const auto postfix_length = postfix_pair.second;
        os << postfix_text;

        // Get length of prefix text and postfix text.
        const auto start_length = GetValue<details::ProgressBarOption::Start>().size();
        const auto bar_width = GetValue<details::ProgressBarOption::BarWidth>();
        const auto end_length = GetValue<details::ProgressBarOption::End>().size();
        const auto terminal_width = TerminalSize().second;

        // prefix + bar_width + postfix should be <= terminal_width.
        const int remaining = terminal_width - (prefix_length + start_length + bar_width + end_length + postfix_length);
        if(remaining > 0) {
            os << std::string(remaining, ' ') << "\r";
        }
        else if(remaining < 0) {

        }

        os.flush();

        if(GetValue<details::ProgressBarOption::Completed>() &&
           !from_multi_progress) {
            os << termcolor::Reset << std::endl;
        }
    }

};

} // !namespace indicators

#endif // !INDICATORS_INDETERMINATE_PROGRESS_BAR_HPP
