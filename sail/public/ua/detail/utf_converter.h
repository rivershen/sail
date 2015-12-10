/*
 * utf_converter.h
 *
 *  Created on: Aug 11, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_UTF_CONVERTER_H_
#define PUBLIC_UA_DETAIL_UTF_CONVERTER_H_
#include <string>
namespace sail{ namespace detail{
struct utf_converter{
    static bind_status u8_to_ws(const std::string& src, std::wstring& dst);
    static bind_status u8_to_ws(const char* src, std::wstring& dst);
    static bind_status ws_to_u8(const std::wstring& src, std::string& dst);
    static bind_status ws_to_u8(const wchar_t* src, std::string& dst);
};

}} // namespace



#endif /* PUBLIC_UA_DETAIL_UTF_CONVERTER_H_ */
