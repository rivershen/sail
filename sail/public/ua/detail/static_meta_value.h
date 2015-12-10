/*
 * static_meta_value.h
 *
 *  Created on: Aug 13, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_META_VALUE_H_
#define PUBLIC_UA_DETAIL_STATIC_META_VALUE_H_

namespace sail{
struct meta_value;
class value_ref;
struct static_meta_value{
    template<typename T>
    static const meta_value& obtain(T* =0);
    static void reset_ref(value_ref& ref, const meta_value& meta, const void* address);
    static value_ref make_ref(const meta_value& meta, const void* address);
};
}

#endif /* PUBLIC_UA_DETAIL_STATIC_META_VALUE_H_ */
