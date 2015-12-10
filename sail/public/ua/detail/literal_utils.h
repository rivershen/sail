/*
 * literal_utils.h
 *
 *  Created on: Aug 11, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_LITERAL_UTILS_H_
#define PUBLIC_UA_DETAIL_LITERAL_UTILS_H_
#include <string>
namespace sail{ namespace detail{
struct literal_utils {
    static void parse_enum_name(const char* from, std::string& to );
    static void parse_field_name(const char* from, std::string& to );
};

}} //namespace



#endif /* PUBLIC_UA_DETAIL_LITERAL_UTILS_H_ */
