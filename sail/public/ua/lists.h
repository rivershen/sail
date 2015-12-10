/*
 * lists.h
 *
 *  Created on: Aug 25, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_LISTS_H_
#define PUBLIC_UA_LISTS_H_
#include "ua_ref.h"
#include "metas.h"
namespace sail{
struct lists{
    template<typename T>
    static list_ref make_ref(const T& v){
        return list_ref(metas::get_list_accessor<T>(), &v);
    }
};

}//namespace

#endif /* PUBLIC_UA_LISTS_H_ */
