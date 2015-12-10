/*
 * enums.h
 *
 *  Created on: Aug 10, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_ENUMS_H_
#define PUBLIC_UA_ENUMS_H_
#include <ua/ua_meta.h>
#include "detail/static_meta_enum.h"
#include <string>

namespace sail{
struct enums{
    template<typename T>
    static T from_string(const std::string& name, bind_status* status = nullptr);
    template<typename T>
    static const std::string& to_string(T value, bind_status* status = nullptr);
};

///////////////////////////////////////////////////////////////////
//IMPLEMENTATION
///////////////////////////////////////////////////////////////////
template<typename T>
inline T enums::from_string(const std::string& name, bind_status* status){
    return (T)(static_meta_enum::obtain<T>().lookup_value(name, status));
}
template<typename T>
inline const std::string& enums::to_string(T value, bind_status* status){
    return static_meta_enum::obtain<T>().lookup_name((int)value, status);
}
}//namespace universal_access;

#include <ua/detail/static_meta_enum_impl.h>

#endif /* PUBLIC_UA_ENUMS_H_ */
