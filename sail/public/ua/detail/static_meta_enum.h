/*
 * static_meta_enum.h
 *
 *  Created on: Aug 13, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_META_ENUM_H_
#define PUBLIC_UA_DETAIL_STATIC_META_ENUM_H_

namespace sail{
struct meta_enum;

struct static_meta_enum{
    template<typename T>
    static const meta_enum& obtain(T* = 0);
};
}



#endif /* PUBLIC_UA_DETAIL_STATIC_META_ENUM_H_ */
