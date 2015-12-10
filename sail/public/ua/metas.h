/*
 * meta.h
 *
 *  Created on: Aug 7, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_METAS_H_
#define PUBLIC_UA_METAS_H_

#include <ua/ua_meta.h>
#include "enums.h"


#include "detail/static_meta_value.h"
#include "detail/static_meta_class.h"
#include "detail/static_list_accessor.h"
#include "detail/static_meta_enum.h"
#include "detail/static_map_accessor.h"

#include "detail/static_meta_value_impl.h"
#include "detail/static_meta_class_impl.h"
#include "detail/static_list_accessor_impl.h"
#include "detail/static_meta_enum_impl.h"
#include "detail/static_map_accessor_impl.h"

//! \cond DEV
namespace sail{
struct metas{
    template<typename T>
    static const meta_value& get_meta_value(T* v= 0){
        return static_meta_value::obtain(v);
    }
    template<typename T>
    static const meta_class& get_meta_class(T* v= 0){
        return static_meta_class::obtain(v);
    }
    template<typename T>
    static const list_accessor& get_list_accessor(T* v= 0){
       return static_list_accessor::obtain(v);
    }
    template<typename T>
    static const list_accessor* sniff_list_accessor(T* v= 0){
        return static_list_accessor::sniff(v);
    }
    template<typename T>
     static const meta_enum& get_meta_enum(T* v=0){
        return static_meta_enum::obtain(v);
    }
    template<typename T>
    static const map_accessor& get_map_accessor(T* v= 0){
        return static_map_accessor::obtain(v);
    }

    template<typename T>
    static const map_accessor* sniff_map_accessor(T* v= 0){
        return static_map_accessor::sniff(v);
    }

};
}
//! \endcond
#endif /* PUBLIC_UA_METAS_H_ */
