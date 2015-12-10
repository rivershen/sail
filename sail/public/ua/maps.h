/*
 * maps.h
 *
 *  Created on: Aug 20, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_MAPS_H_
#define PUBLIC_UA_MAPS_H_
#include "ua_ref.h"
#include "metas.h"
namespace sail{

struct maps{
    template<typename T>
    static map_ref make_ref(const T& v){
        return map_ref(metas::get_map_accessor<T>(), &v);
    }
};
} //namespace




#endif /* PUBLIC_UA_MAPS_H_ */
