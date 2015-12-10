/*
 * refs.h
 *
 *  Created on: Aug 14, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_VALUES_H_
#define PUBLIC_UA_VALUES_H_
#include "ua_ref.h"
#include "metas.h"
namespace sail{
struct values{
    template<typename T>
    static value_ref make_ref(const T& v){
        return value_ref(metas::get_meta_value<T>(), &v);
    }
    static value_ref make_ref(const meta_value& meta, const void* address);
};
} //namespace
#endif /* PUBLIC_UA_VALUES_H_ */
