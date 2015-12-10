/*
 * static_map_accessor.h
 *
 *  Created on: Aug 13, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_MAP_ACCESSOR_H_
#define PUBLIC_UA_DETAIL_STATIC_MAP_ACCESSOR_H_
namespace sail{

struct map_accessor;

struct static_map_accessor{
    template<typename T>
    static const map_accessor& obtain(T* = 0);

    template<typename T>
    static const map_accessor* sniff(T* = 0);
};
}




#endif /* PUBLIC_UA_DETAIL_STATIC_MAP_ACCESSOR_H_ */
