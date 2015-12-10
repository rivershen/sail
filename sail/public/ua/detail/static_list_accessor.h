/*
 * static_list_accessor.h
 *
 *  Created on: Aug 13, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_LIST_ACCESSOR_H_
#define PUBLIC_UA_DETAIL_STATIC_LIST_ACCESSOR_H_

namespace sail{
struct list_accessor;

struct static_list_accessor{
    template<typename T>
    static const list_accessor& obtain(T* = 0);

    template<typename T>
    static const list_accessor* sniff(T* = 0);
};
}



#endif /* PUBLIC_UA_DETAIL_STATIC_LIST_ACCESSOR_H_ */
