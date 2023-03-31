#ifndef INDICATORS_PROGRESS_BAR_HPP
#define INDICATORS_PROGRESS_BAR_HPP

#include <algorithm>
#include <atomic>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <mutex>
#include <string>
#include <thread>
#include <tuple>
#include <type_traits>
#include <utility>

#include "indicators/details/stream_helper.hpp"
#include "indicators/color.hpp"
#include "indicators/setting.hpp"
#include "indicators/terminal_size.hpp"

namespace indicators {

class ProgressBar {
    using Settings = 
        std::tuple<option::BarWidth, option::PrefixText, option::PostfixText,
                   option::Start, option::End, option::Fill, option::Lead,
                   option::Remainder, option::MaxPostfixTextLen,
                   option::Completed, option::ShowPercentage,
                   option::ShowElapsedTime, option::ShowRemainingTime,
                   option::SavedStartTime, option::ForegroundColor,
                   option::FontStyle, option::MinProgress, option::MaxProgress,
                   option::ProgressType, option::Stream>;

public:
    template<typename... Args,
             typename std::enable_if<
                 details::are_settings_from_tuple<
                    Settings, typename std::decay<Args>::type...>::value,
                 void*>::type = nullptr>
    explicit ProgressBar(Args&&... args)
    : settings_(
        details::Get<details::ProgressOption::BarWidth>(
            option::BarWidth{100}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::PrefixText>(
            option::PrefixText{}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::PostfixText>(
            option::PostfixText{}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::Start>(
            option::Start{"["}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::End>(
            option::End{"]"}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::Fill>(
            option::Fill{"="}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::Lead>(
            option::Lead{">"}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::Remainder>(
            option::Remainder{" "}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::MaxPostfixTextLen>(
            option::MaxPostfixTextLen{0}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::Completed>(
            option::Completed{false}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::ShowPercentage>(
            option::ShowPercentage{false}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::ShowElapsedTime>(
            option::ShowElapsedTime{false}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::ShowRemainingTime>(
            option::ShowRemainingTime{false}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::SavedStartTime>(
            option::SavedStartTime{false}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::ForegroundColor>(
            option::ForegroundColor{Color::Unspecified}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::FontStyle>(
            option::FontStyle{std::vector<FontStyle>{}}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::MinProgress>(
            option::MinProgress{0}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::MaxProgress>(
            option::MaxProgress{100}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::ProgressType>(
            option::ProgressType{ProgressType::Incremental}, std::forward<Args>(args)...),

        details::Get<details::ProgressOption::Stream>(
            option::Stream{std::cout}, std::forward<Args>(args)...)) {
 
        // If ProgressType is Incremental, start from MinProgress,
        // else start from MaxProgress.
        const auto type = GetValue<details::ProgressOption::ProgressType>();
        if(type == ProgressType::Incremental) {
            progress_ = GetValue<details::ProgressOption::MinProgress>();
        }
        else {
            progress_ = GetValue<details::ProgressOption::MaxProgress>();
        }
    }

    template<typename T, details::ProgressBarOption id>
    void SetOption(details::Setting<T, id>&& setting) {
        static_assert(
            !std::is_same<T, typename std::decay<decltype(details::GetValue<id>(
                std::declval<Settings>()))>::type>::value,
            "Setting has wrong type!");

        std::lock_guard<std::mutex> lock(mutex_);
        GetValue<id>() = std::move(setting).value;
    }

    template<typename T, details::ProgressBarOption id> 
    void SetOption(const details::Setting<T, id>& setting) {
        static_assert(
            !std::is_same<T, typename std::decay<decltype(details::GetValue<id>(
                std::declval<Setting>()))>::type>::value,
            "Setting has wrong type!");

        std::lock_guard<std::mutex> lock(mutex_);
        GetValue<id>() = setting.value;
    }

    void SetOption(const details::Setting<
                   std::string, details::ProgressBarOption::PostfixText>& setting) {
        std::lock_guard<std::mutex> lock(mutex_);
        GetValue<details::ProgressBarOption::PostfixText>() = setting.value;
        if(setting.value.length() >
           GetValue<details::ProgressBarOption::MaxPostfixTextLen>()) {
            GetValue<details::ProgressBarOption::MaxPostfixTextLen>() = setting.value.length();
        }
    }


    void SetOption(details::Setting<
                   std::string, details::ProgressBarOption::PostfixText>&& setting) {
        std::lock_guard<std::mutex> lock(mutex_);
        GetValue<details::ProgressBarOption::PostfixText>() = std::move(setting).value;
        auto& new_value = GetValue<details::ProgressBarOption::PostfixText>();
        if(new_value.length() >
           GetValue<details::ProgressBarOption::MaxPostfixTextLen>()) {
            GetValue<details::ProgressBarOption::MaxPostfixTextLen>() = new_value.length();
        }
    }

    void SetProgress(size_t new_progress) {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            progress_ = new_progress;
        }

        SavedStartTime();
        PrintProgress();
    }

    void Tick() {
        {
            std::lock_guard<std::mutex> lock(mutex_);
            const auto type = GetValue<details::ProgressBarOption::ProgressType>();
            if(type == ProgressType::Incremental) {
                progress_ += 1;
            }
            else {
                progress_ -= 1;
            }
        }

        SavedStartTime();
        PrintProgress();
    }

    size_t Current() {
        std::lock_guard<std::mutex> lock(mutex_);
        return (std::min)(
                progress_, 
                static_cast<size_t>(GetValue<details::ProgressBarOption::MaxProgress>()));
    }

    bool IsCompleted() const {
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
    Settings settings_;
    std::chrono::nanoseconds elapsed_;
    std::chrono::time_point<std::chrono::high_resolution_clock> start_time_point_;
    std::mutex mutex_;

    template<typename Indicator, size_t count> friend class MultiProgress;
    template<typename Indicator> friend class DynamicProgress;
    std::atomic<bool> multi_progress_mode_{false};

    void SavedStartTime() {
        auto& ShowElapsedTime = GetValue<details::ProgressBarOption::ShowElapsedTime>();
        auto& SavedStartTime = GetValue<details::ProgressBarOption::SavedStartTime>();
        auto& ShowRemainingTime = GetValue<details::ProgressBarOption::ShowRemainingTime>();
        
        if((ShowElapsedTime || ShowRemainingTime) && !SavedStartTime) {
            start_time_point_ = std::chrono::high_resolution_clock::now();
            SavedStartTime = true;
        }
    }

    std::pair<std::string, size_t> GetPrefixText() {
        std::stringstream os;
        os << GetValue<details::ProgressBarOption::PrefixText>();
        const auto result = os.str();
        const auto result_size = unicode::DisplayWidth(result);
        return { result, result_size };
    }

    std::pair<std::string, size_t> GetPostfixText() {
        std::stringstream os;
        const auto max_progress = GetValue<details::ProgressBarOption::MaxProgress>();

        if(GetValue<details::ProgressBarOption::ShowPercentage>()) {
            os << " " 
               << (std::min)(static_cast<size_t>(static_cast<float>(progress_) / max_progress * 100), 
                             size_t(100)) 
               << "%";
        }

        auto& saved_start_time = GetValue<details::ProgressBarOption::SavedStartTime>();

        if(GetValue<details::ProgressBarOption::ShowElapsedTime>()) {
            os << " [";
            if(saved_start_time) {
                details::WriteDuration(os, elapsed_);
            }
            else {
                os << "00:00s";
            }
        }

        if(GetValue<details::ProgressBarOption::ShowRemainingTime>()) {
            if(GetValue<details::ProgressBarOption::ShowElapsedTime>()) {
                os << "<";
            }
            else {
                os << " [";
            }

            if(saved_start_time) {
                auto eta = std::chrono::nanoseconds(
                    progress_ > 0 
                        ? static_cast<long long>(std::ceil(float(elapsed_.count()) *
                                                           max_progress / progress_)) 
                        : 0);
                auto remaining = eta > elapsed_ ? (eta - elapsed_) : (elapsed_ - eta);
                details::WriteDuration(os, remaining);
            }
            else {
                os << "00:00s";
            }

            os << "]";
        }
        else {
            if(GetValue<details::ProgressBarOption::ShowElapsedTime>()) {
                os << "]";
            }
        }

        os << " " << GetValue<details::ProgressBarOption::PostfixText>();

        const auto result = os.str();
        const auto result_size = unicode::DisplayWidth(result);
        return { result, result_size };
    }

public:
    void PrintProgress(bool from_multi_progress = false) {
        std::lock_guard<std::mutex> lock{mutex_};

        auto& os = GetValue<details::ProgressBarOption::Stream>();

        const auto type = GetValue<details::ProgressBarOption::ProgressType>();
        const auto min_progress = GetValue<details::ProgressBarOption::MinProgress>();
        const auto max_progress = GetValue<details::ProgressBarOption::MaxProgress>();

        if(multi_progress_mode_ && !from_multi_progress) {
            if((type == ProgressType::Incremental && progress_ >= max_progress) ||
               (type == ProgressType::Decremental && progress_ <= min_progress)) {
                GetValue<details::ProgressBarOption::Completed>() = true;
            }
            return;
        }

        auto now = std::chrono::high_resolution_clock::now();
        if(!GetValue<details::ProgressBarOption::Completed>()) {
            elapsed_ = std::chrono::duration_cast<std::chrono::nanoseconds>(
                now - start_time_point_);
        }
        
        if(GetValue<details::ProgressBarOption::ForegroundColor>() !=
           Color::Unspecified) {
            details::SetStreamColor(os, GetValue<details::ProgressBarOption::ForegroundColor>());
        }

        for(auto& style : GetValue<details::ProgressBarOption::FontStyle>()) {
            details::SetFontStyle(os, style);
        }

        const auto prefix_pair = GetPrefixText();
        const auto prefix_text = prefix_pair.first;
        const auto prefix_length = prefix_pair.second;
        os << "\r" << prefix_text;

        os << GetValue<details::ProgressBarOption::Start>();

        details::ProgressScaleWriter writer{
            os, GetValue<details::ProgressBarOption::BarWidth>(),
            GetValue<details::ProgressBarOption::Fill>(),
            GetValue<details::ProgressBarOption::Lead>(),
            GetValue<details::ProgressBarOption::Remainder>()
        };
        writer.Write(double(progress_) / double(max_progress) * 100.0f);

        os << GetValue<details::ProgressBarOption::End>();

        const auto postfix_pair = GetPostfixText();
        const auto postfix_text = postfix_pair.first;
        const auto postfix_length = postfix_pair.second;
        os << postfix_text;

        // Get length of the prefix text and postfix text.
        const auto start_length = GetValue<details::ProgressBarOption::Start>().size();
        const auto bar_width = GetValue<details::ProgressBarOption::BarWidth>();
        const auto end_length = GetValue<details::ProgressBarOption::End>().size();
        const auto terminal_width = TerminalSize().second();
        // prefix + bar_width + postfix should be <= terminal_width.
        const int remaining = terminal_width - (prefix_length + start_length + bar_width + end_length + postfix_length);
        if(remaining > 0) {
            os << std::string(remaining, ' ') << "\r";
        }
        else if(remaining < 0) {
            // Do nothing. Maybe in the future truncate postfix with ...
        }
        os.flush();

        if((type == ProgressType::Incremental && progress_ >= max_progress) ||
           (type == ProgressType::Decremental && progress_ <= min_progress)) {
            GetValue<details::ProgressBarOption::Completed>() = true;
        }
        if(GetValue<details::ProgressBarOption::Completed>() &&
           !from_multi_progress) {
            os << termcolor::Reset << std::endl;
        }
    }
};
    
} // !namespace indicators

#endif // !INDICATORS_PROGRESS_BAR_HPP
