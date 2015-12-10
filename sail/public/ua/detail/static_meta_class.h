/*
 * static_meta_class.h
 *
 *  Created on: Aug 13, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_META_CLASS_H_
#define PUBLIC_UA_DETAIL_STATIC_META_CLASS_H_

namespace sail{
struct meta_class;
struct static_meta_class{
    template<typename T>
    static const meta_class& obtain(T* = 0);
};
}



#endif /* PUBLIC_UA_DETAIL_STATIC_META_CLASS_H_ */
