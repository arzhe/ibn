#ifndef INDICATORS_SETTINGS_HPP
#define INDICATORS_SETTINGS_HPP

#include <cstddef>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

#include "indicators/color.hpp"
#include "indicators/font_style.hpp"
#include "indicators/progress_type.hpp"

namespace indicators {

namespace details {
/*-----------------------------------------------------------------------------------
 * Compile-time conditional judgement.
 */
template<bool condition> struct if_else;

template<> struct if_else<true>  { using type = std::true_type; };
template<> struct if_else<false> { using type = std::false_type; };

/*-----------------------------------------------------------------------------------
 * Compile-time conditional judgement.
 * If condition is true, then "typename True" will be used as "type", and 
 * "typename False" will be ignored.
 */
template<bool condition, typename True, typename False> struct if_else_type;

template<typename True, typename False> struct if_else_type<true, True, False> {
    using type = True;
};
template<typename True, typename False> struct if_else_type<false, True, False> {
    using type = False;
};

/*-----------------------------------------------------------------------------------
 * Compile-time conditional judgement.
 * Conjuncted judgement. It means that all operations will be judged if they are true.
 * But once one of the conditions is false, the judgement will be stopped.
 * 
 * eg:
 *      +--------------------------------------------+
 *      |               stopped!                     |
 *      |                  ^                         |
 *      |                  |                         |
 *      |True -> True -> False -> True -> True -> ...|
 *      +--------------------------------------------+
 */
template<typename... Ops> struct conjunction;

// Recursion termination for generic derivation.
template<> struct conjunction<> : std::true_type {};

template<typename Op, typename... TailOps>
struct conjunction<Op, TailOps...>
    : if_else_type<!Op::value, std::false_type, conjunction<TailOps...>>::type {};

/*-----------------------------------------------------------------------------------
 * Compile-time conditional judgement.
 * Disjuncted judgement. It means that all operations will be judged if they are false.
 * But once one of the conditions is true, the judgement will be stopped.
 * 
 * eg:
 *      +-----------------------------------------------+
 *      |                stopped!                       |
 *      |                   ^                           |
 *      |                   |                           |
 *      |False -> False -> True -> False -> False -> ...|
 *      +-----------------------------------------------+
 */
template<typename... Ops> struct disjunction;

// Recursion termination for generic derivation.
template<> struct disjunction<> : std::false_type {};

template<typename Op, typename... TailOps>
struct disjunction<Op, TailOps...>
    : if_else_type<Op::value, std::true_type, disjunction<TailOps...>>::type {};

/*-----------------------------------------------------------------------------------
 * Progress bar options.
 */
enum class ProgressBarOption {
    BarWidth = 0,
    PrefixText,
    PostfixText,
    Start,
    End,
    Fill,
    Lead,
    Remainder,
    MaxPostfixTextLen,
    Completed,
    ShowPercentage,
    ShowElapsedTime,
    ShowRemainingTime,
    SavedStartTime,
    ForegroundColor,
    SpinnerShow,
    SpinnerStates,
    FontStyles,
    HideBarWhenComplete,
    MinProgress,
    MaxProgress,
    ProgressType,
    Stream
};

/*-----------------------------------------------------------------------------------
 * The setting type of ProgressBarOption.
 */
template<typename T, ProgressBarOption Id> struct Setting {
    template<typename... Args,
             typename = typename std::enable_if<std::is_constructible<T, Args...>::value>::type>
    explicit Setting(Args&&... args) : value(std::forward<Args>(args)...) {}
    Setting(const Setting&) = default;
    Setting(Setting&&) = default;

    static constexpr auto id = Id;
    using type = T;

    T value{};
};

/*-----------------------------------------------------------------------------------
 * Determines whether a type is the Setting-type.
 */
template<typename T> struct is_setting : std::false_type {};

template<ProgressBarOption Id, typename T> struct is_setting<Setting<T, Id>> : std::true_type {};

/*-----------------------------------------------------------------------------------
 * Determines whether certain types are Setting.
 */
template<typename... Args>
struct are_settings : if_else<conjunction<is_setting<Args>...>::value>::type {};

// Recursion termination for generic derivation.
template<> struct are_settings<> : std::true_type {};

/*-----------------------------------------------------------------------------------
 * Determines whether the Setting-type is equal to the tuple-type.
 */
template<typename Setting, typename Tuple> struct is_setting_from_tuple;

template<typename Setting> struct is_setting_from_tuple<Setting, std::tuple<>> : std::true_type {};

/*-----------------------------------------------------------------------------------
 * Determines whether the Setting-type is equal to one of the Tuple-type's members(TupleTypes).
 * As long as one of the TupleTypes-types is equal to the Setting-type, returns std::true_type.
 */
template<typename Setting, typename... TupleTypes>
struct is_setting_from_tuple<Setting, std::tuple<TupleTypes...>>
    : if_else<disjunction<std::is_same<Setting, TupleTypes>...>::value>::type {};

/*-----------------------------------------------------------------------------------
 * Determines whether all Setting-types are included by the Tuple-type.
 * Tuple-type may contain lots of Setting-types.
 * As long as one of the Setting-types is not included by the Tuple-type, returns std::false_type.
 */
template<typename Tuple, typename... Settings>
struct are_settings_from_tuple
    : if_else<conjunction<is_setting_from_tuple<Settings, Tuple>...>::value>::type {};

/*-----------------------------------------------------------------------------------
 * Always true for ProgressBarOption.
 */
template<ProgressBarOption Id> struct always_true { static constexpr auto value = true; };

/*-----------------------------------------------------------------------------------
 * 'get' will find the Setting-type in the {Args...}, and 
 * the Setting-type's id must be the same as ProgressBarOption Id. If not found,
 * the 'Default' value will be returned. 
 * The {Args...} have to be all of Setting-type.
 */

// If none of {Args...} meet the requirement that the type::id 
// be the same as ProgressBarOption Id, return the Default value.
template<ProgressBarOption Id, typename Default> Default&& GetImpl(Default&& def) {
    return std::forward<Default>(def);
}

// If T::type::id is the same as ProgressBarOption Id, return T-type value.
template<ProgressBarOption Id, typename Default, typename T, typename... Args>
auto GetImpl(Default&& /*def*/, T&& first, Args&&... /*tail*/)
    -> typename std::enable_if<(std::decay<T>::type::id == Id),
                                decltype(std::forward<T>(first))>::type {
    return std::forward<T>(first);
}

// If T::type::id is not the same as ProgressBarOption Id, go next iteration 'GetImpl'.
template<ProgressBarOption Id, typename Default, typename T, typename... Args>
auto GetImpl(Default&& def, T&& /*first*/, Args&&... tail)
    -> typename std::enable_if<(std::decay<T>::type::id != Id),
                                decltype(GetImpl<Id>(std::forward<Default>(def),
                                                     std::forward<Args>(tail)...))>::type {
    return GetImpl<Id>(std::forward<Default>(def), std::forward<Args>(tail)...);       
}

// First determine whether the {Args...} are Setting-type. The actual will be left to 'GetImpl'.
template<ProgressBarOption Id, typename Default, typename... Args,
         typename = typename std::enable_if<are_settings<Args...>::value, void>::type>
auto Get(Default&& def, Args&&... args)
    -> decltype(details::GetImpl<Id>(std::forward<Default>(def), std::forward<Args>(args)...)) {
    return details::GetImpl<Id>(std::forward<Default>(def), std::forward<Args>(args)...);
}

/*-----------------------------------------------------------------------------------
 * Aliases of some Setting-types.
 */
template<ProgressBarOption Id> using StringSetting = Setting<std::string, Id>;

template<ProgressBarOption Id> using IntegerSetting = Setting<std::size_t, Id>;

template<ProgressBarOption Id> using BooleanSetting = Setting<bool, Id>;

/*-----------------------------------------------------------------------------------
 * Find the index of the first specified ProgressBarOption Id that appears in a list of Setting-types.
 * Start with the index 0 by default.
 */
template<ProgressBarOption Id, typename Tuple, std::size_t counter = 0> struct option_idx;

template<ProgressBarOption Id, typename T, typename... Settings, std::size_t counter>
struct option_idx<Id, std::tuple<T, Settings...>, counter>
    : if_else_type<(Id == T::id), std::integral_constant<std::size_t, counter>,
                    option_idx<Id, std::tuple<Settings...>, counter + 1>>::type {};

template<ProgressBarOption Id, std::size_t counter> struct option_idx<Id, std::tuple<>, counter> {
    static_assert(always_true<(ProgressBarOption)Id>::value, "No such option was found");
};

/*-----------------------------------------------------------------------------------
 * Get the first specified ProgressBarOption Id that appears in the Tuple-type(Settings-type).
 */
template<ProgressBarOption Id, typename Settings>
auto GetValue(Settings&& settings)
    -> decltype((std::get<option_idx<Id, typename std::decay<Settings>::type>::value>(
                 std::declval<Settings&&>()))) {
    return std::get<option_idx<Id, typename std::decay<Settings>::type>::value>(
           std::forward<Settings>(settings));
}

} // !namespace details

/*-----------------------------------------------------------------------------------
 * Aliases of all Setting-types.
 */
namespace option {

using BarWidth            = details::IntegerSetting<details::ProgressBarOption::BarWidth>;
using PrefixText          = details::StringSetting<details::ProgressBarOption::PrefixText>;
using PostfixText         = details::StringSetting<details::ProgressBarOption::PostfixText>;
using Start               = details::StringSetting<details::ProgressBarOption::Start>;
using End                 = details::StringSetting<details::ProgressBarOption::End>;
using Fill                = details::StringSetting<details::ProgressBarOption::Fill>;
using Lead                = details::StringSetting<details::ProgressBarOption::Lead>;
using Remainder           = details::StringSetting<details::ProgressBarOption::Remainder>;
using MaxPostfixTextLen   = details::IntegerSetting<details::ProgressBarOption::MaxPostfixTextLen>;
using Completed           = details::BooleanSetting<details::ProgressBarOption::Completed>;
using ShowPercentage      = details::BooleanSetting<details::ProgressBarOption::ShowPercentage>;
using ShowElapsedTime     = details::BooleanSetting<details::ProgressBarOption::ShowElapsedTime>;
using ShowRemainingTime   = details::BooleanSetting<details::ProgressBarOption::ShowRemainingTime>;
using SavedStartTime      = details::BooleanSetting<details::ProgressBarOption::SavedStartTime>;
using ForegroundColor     = details::Setting<Color, details::ProgressBarOption::ForegroundColor>;
using ShowSpinner         = details::BooleanSetting<details::ProgressBarOption::SpinnerShow>;
using SpinnerStates       = details::Setting<std::vector<std::string>, details::ProgressBarOption::SpinnerStates>;
using HideBarWhenComplete = details::BooleanSetting<details::ProgressBarOption::HideBarWhenComplete>;
using FontStyles          = details::Setting<std::vector<FontStyle>, details::ProgressBarOption::FontStyles>;
using MinProgress         = details::IntegerSetting<details::ProgressBarOption::MinProgress>;
using MaxProgress         = details::IntegerSetting<details::ProgressBarOption::MaxProgress>;
using ProgressType        = details::Setting<ProgressType, details::ProgressBarOption::ProgressType>;
using Stream              = details::Setting<std::ostream&, details::ProgressBarOption::Stream>;

} // !namespace option

} // !namespace indicators

#endif // !INDICATORS_SETTINGS_HPP
