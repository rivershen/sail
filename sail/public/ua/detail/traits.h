/*
 * value_traits.h
 *
 *  Created on: Jul 22, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_TRAITS_H_
#define PUBLIC_UA_TRAITS_H_
#include "member_detector.h"
#include "container_trait.h"
#include <type_traits>
namespace sail{

namespace detail{
    UA_DECLARE_METHOD_DETECTOR(_has_clear, void, clear,());
    UA_DECLARE_METHOD_DETECTOR(_has_size, size_t, size, () const);
    UA_DECLARE_METHOD_DETECTOR(_has_max_size, size_t, max_size, ()const);
}

template<typename T>
struct is_container : std::integral_constant<bool, detail::container_trait<T>::is_container>{
};

template<typename T>
struct is_map : std::integral_constant<bool, detail::container_trait<T>::is_map>{
};

template<typename T>
struct is_set : std::integral_constant<bool, detail::container_trait<T>::is_set>{
};

template<typename T>
struct is_list : std::integral_constant<bool, detail::container_trait<T>::is_list>{
};
template<typename T, typename V>
void* _ua_visit_native_object(T*, V*);

template<typename T, typename=char>
struct is_native_object : std::integral_constant<bool,
    !std::is_same<
    void*,
    decltype(_ua_visit_native_object((T*)0,(int*)0))>::value>{
};

template<typename T, typename V>
void* _ua_visit_native_enum(T, V*);

template<typename T>
struct is_native_enum : std::integral_constant<bool,
    !std::is_same<
    void*,
    decltype(_ua_visit_native_enum((T)0,(int*)0))>::value>{
};

template<typename T>
struct can_get_size : std::integral_constant<bool, detail::_has_size<T>::exists>{
};

template<typename T>
struct can_clear : std::integral_constant<bool, detail::_has_clear<T>::exists>{
};
template<typename T>
struct can_call_array_subscription:
        std::integral_constant<bool, detail::container_trait<T>::has_array_subscription>{
};
template<typename T>
struct can_find_by_key: std::integral_constant<bool, detail::container_trait<T>::has_find>{
};

template<typename T, typename = void>
struct key_trait{
    static const bool can_declare = false;
    static const bool can_input = false;
    static const bool can_output = false;
};
template<>
struct key_trait<std::string,void> {
    static const bool can_declare = true;
    static const bool can_input = true;
    static const bool can_output = true;
};
template<>
struct key_trait<std::wstring, void>{
    static const bool can_declare = true;
    static const bool can_input = true;
    static const bool can_output = true;
};
template<>
struct key_trait<const char*, void> {
    static const bool can_declare = false;
    static const bool can_input = true;
    static const bool can_output = false;
};
template<>
struct key_trait<const wchar_t*, void> {
    static const bool can_declare = false;
    static const bool can_input = true;
    static const bool can_output = false;
};
template<typename T>
struct key_trait<T,
typename std::enable_if<is_native_enum<T>::value>::type>{
    static const bool can_declare = true;
    static const bool can_input = true;
    static const bool can_output = false;
};


} //namespace universal_access




#endif /* PUBLIC_UA_TRAITS_H_ */
