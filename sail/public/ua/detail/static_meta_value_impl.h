/*
 * static_meta_value.h
 *
 *  Created on: Aug 7, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_META_VALUE_IMPL_H_
#define PUBLIC_UA_DETAIL_STATIC_META_VALUE_IMPL_H_
#include "ua/ua_ref.h"
#include "ua/ua_meta.h"
#include "static_meta_value.h"
#include "static_list_accessor.h"
#include "static_map_accessor.h"
#include "std_utils.h"
namespace sail{
namespace detail{
struct generic_value_visitor : public value_visitor{
    using value_visitor::visit;
    template<typename T>
    bool visit(const T* v){
        return visit(static_meta_value::obtain<T>(),v);
    }
};
template<typename T>
struct meta_value_impl : public meta_value{
    virtual bind_status accept(const void* value, value_visitor& visitor) const override{
        generic_value_visitor& generic_visitor = static_cast<generic_value_visitor&>(visitor);
        generic_visitor.visit(static_cast<const T*>(value));
        return visitor.get_last_error();
    }
    virtual const map_extractor* to_forward_map(const void*) const override{
        return static_map_accessor::sniff<T>();
    }
    virtual const map_accessor* to_map(const void*) const override{
        return static_map_accessor::sniff<T>();
    }
    virtual const list_extractor* to_forward_list(const void*) const override{
        return static_list_accessor::sniff<T>();
    }
    virtual const list_accessor* to_list(const void*) const override{
        return static_list_accessor::sniff<T>();
    }
    virtual const std::type_info& get_type_info(const void*) const override{
        return typeid(T);
    }
    virtual bind_status load_from(const value_ref&from, void* to) const override{
        if(load_from_fast(from, to, (std::is_copy_assignable<T>*)0)){
            return bind_status::SUCCESS;
        }else{
            const list_accessor* list = static_list_accessor::sniff<T>();
            if(list){
                return list->load_from(from,to);
            }
        }
        return bind_status::INVALID_CONVERSION;
    }
    static const meta_value& instance(){
        static meta_value_impl s;
        return s;
    }
private:
    bool load_from_fast(const value_ref&from, void* to, std::true_type* /* is_copy_assinable*/) const{
        const T* v = from.peek_typed_address<T>();
        if(v){
            *reinterpret_cast<T*>(to) = *v;
            return true;
        }
        return false;
    }
    bool load_from_fast(const value_ref&from, void* to, std::false_type* /* is_copy_assinable*/)const{
        return false;
    }
};

}
template<typename T>
struct static_meta_value_impl{
    static const meta_value& get(){
        return detail::meta_value_impl<T>::instance();
    }
};

template<>
struct static_meta_value_impl<unsigned long long>{
    static const meta_value& get(){
           return detail::meta_value_impl<uint64_t>::instance();
       }
       friend struct static_meta;
};
template<>
struct static_meta_value_impl<long long>{
    static const meta_value& get(){
           return detail::meta_value_impl<int64_t>::instance();
       }
       friend struct static_meta;
};
template<>
struct static_meta_value_impl<char>{
    static const meta_value& get(){
      return _get((std::is_signed<char>::type*)0);
    }
    static const meta_value& _get(std::true_type* /*signed*/){
        return detail::meta_value_impl<int8_t>::instance();
    }
    static const meta_value& _get(std::false_type* /*unsigned*/){
        return detail::meta_value_impl<uint8_t>::instance();
    }
    friend struct static_meta;
};

template<typename T>
inline const meta_value& static_meta_value_obtain(detail::merit<0>*){
     return static_meta_value_impl<T>::get();
}
class value_ref;
class map_ref;
class list_ref;

//those specializations are disabled to prevent common programming errors.
template<typename T, typename = typename std::enable_if<std::is_pointer<T>::value>::type>
inline const meta_value& static_meta_value_obtain(detail::merit<1>*) = delete;
template<>
inline const meta_value& static_meta_value_obtain<value_ref>(detail::merit<0>*) = delete;
template<>
inline const meta_value& static_meta_value_obtain<map_ref>(detail::merit<0>*) = delete;
template<>
inline const meta_value& static_meta_value_obtain<list_ref>(detail::merit<0>*) = delete;



template<typename T>
inline const meta_value& static_meta_value::obtain(T*){
    return static_meta_value_obtain<typename std::decay<T>::type>((detail::merit<8>*)0);
}

}



#endif /* PUBLIC_UA_DETAIL_STATIC_META_VALUE_IMPL_H_ */
