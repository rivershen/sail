/*
 * static_list_ops.h
 *
 *  Created on: Aug 7, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_LIST_ACCESSOR_IMPL_H_
#define PUBLIC_UA_DETAIL_STATIC_LIST_ACCESSOR_IMPL_H_
#include "ua/ua_ref.h"
#include "static_list_accessor.h"
#include "std_utils.h"

namespace sail{
namespace detail{
template<typename T>
struct static_std_list_accessor:public list_accessor{
    virtual bool is_valid(const void*) const override{
        return true;
    }
    virtual const meta_value& get_meta_type() const override{
        return std_utils::get_meta_type<T>();
    }
    virtual const meta_value* get_value_meta_type(const void*) const override{
        return &std_utils::get_value_meta_type<T>();
    }
    virtual size_t get_size_hint(const void* v) const override{
        return size_of(v);
    }
    virtual bool is_empty(const void* v) const override{
        return std_utils::is_empty<T>(v);
    }
    virtual size_t size_of(const void* v) const override{
        return std_utils::size_of<T>(v);
    }
    virtual int for_each_element_of(const void* v, for_each_element_cb callback, void* context) const override{
        value_ref ref;
        int result = 0;
        for(auto& item : *static_cast<const T*>(v)){
            static_meta_value::reset_ref(ref,std_utils::get_value_meta_type<T>(),&item);
            result = callback(ref,context);
            if(result){
                break;
            }
        }
        return result;
    }
    virtual value_ref get_element_at(const void* v, size_t index) const override{
        const void* result = std_utils::get_element_at<T>(v, index);
        if(nullptr != result){
            return static_meta_value::make_ref(std_utils::get_value_meta_type<T>(),result);
        }else{
            return value_ref();
        }
    }
    virtual bind_status load_from(const value_ref&from, void* to) const override{
        if(load_from_fast(from, to, (std::is_copy_assignable<T>*)0)){
             return bind_status::SUCCESS;
        }else if(from.is_list()){
            return std_utils::load_list<T>(from.get_meta(),from.get_address(), to);
        }else{
            return bind_status::INVALID_CONVERSION;
        }
    }
    static list_accessor& get(){
        static static_std_list_accessor ops;
        return ops;
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
struct default_list_type_router{
    template<typename T>
    static list_accessor& get(){
        static_assert(is_list<T>::value || is_set<T>::value, "incompatible list.");
        return static_std_list_accessor<T>::get();
    }
    template<typename T>
    static typename std::enable_if< is_list<T>::value || is_set<T>::value, list_accessor*>::type
    get_optional(T*){
        return &static_std_list_accessor<T>::get();
    }
    static list_accessor* get_optional(...){
        return 0;
    }

};
}// namespace detail

template<typename T>
struct static_list_accessor_impl{
    static list_accessor& get(){
        return detail::default_list_type_router::get<T>();
    }
    static list_accessor* get_optional(){
        return detail::default_list_type_router::get_optional(static_cast<T*>(0));
    }
    friend struct list_accessor;
    friend struct static_meta;
};

//specialization for basic_string<T>, it should not be treated as a list
template<typename T>
struct static_list_accessor_impl<std::basic_string<T>>{
    static list_accessor& get() = delete;
    static list_accessor* get_optional(){
        return 0;
    }

};

template<typename T>
inline const list_accessor& static_list_accessor::obtain(T*){
    return static_list_accessor_impl<typename std::decay<T>::type>::get();
}

template<typename T>
inline const list_accessor* static_list_accessor::sniff(T* ){
    return static_list_accessor_impl<typename std::decay<T>::type>::get_optional();
}

}




#endif /* PUBLIC_UA_DETAIL_STATIC_LIST_ACCESSOR_IMPL_H_ */
