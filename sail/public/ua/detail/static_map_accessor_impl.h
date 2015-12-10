/*
 * static_map_ops.h
 *
 *  Created on: Aug 7, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_MAP_ACCESSOR_IMPL_H_
#define PUBLIC_UA_DETAIL_STATIC_MAP_ACCESSOR_IMPL_H_
#include "ua/ua_ref.h"
#include "traits.h"
#include "static_meta_class.h"
#include "static_meta_value.h"
#include "static_map_accessor.h"
#include "key_converter.h"
#include "std_utils.h"

namespace sail{
namespace detail{
template<typename T>
struct static_native_map_accessor:public map_accessor{
    virtual bool is_valid(const void*) const override{
        return false;
    }
    virtual bool is_empty(const void* /*v*/) const override{
        return false;
    }
    virtual const meta_value& get_meta_type() const override{
        return std_utils::get_meta_type<T>();
    }
    virtual const meta_value* get_value_meta_type(const void*) const override{
        if(_meta.is_homogenious()){
            return _meta.begin()->meta;
        }else{
            return nullptr;
        }
    }
    virtual size_t size_of(const void* /*v*/) const override{
        return _meta.size();
    }
    virtual int for_each_field_of(const void* v, for_each_field_cb cb, key_receiver& receiver, void* context) const override{
        value_ref ref;
        int result = 0;
        for(auto& field: _meta){
           receiver.set(field.name);
           static_meta_value::reset_ref(ref, *field.meta, (const char*)v + field.offset);
           result = cb(ref, context);
           if(result){
               break;
           }
        }
        return result;
    }
    virtual void run_with_key_receiver(const void* /*item*/, with_key_cb cb, void* context) const override{
        key_converter<std::string> converter;
        cb(converter, context);
    }
    virtual bool exists(const void*, key_receiver& receiver) const override{
        if(receiver.get_last_error() == bind_status::SUCCESS){
            key_converter<std::string>& converter = static_cast<key_converter<std::string>&>(receiver);
            return _meta.exists(converter.get());
        }else{
            return false;
        }
    }
    virtual value_ref get_field_at(const void* v, key_receiver& receiver) const override{
        value_ref ref;
        if(receiver.get_last_error() == bind_status::SUCCESS){
           key_converter<std::string>& converter = static_cast<key_converter<std::string>&>(receiver);
           bind_status status;
           const meta_class::field& field = _meta.get_field(converter.get(), &status);
           if(bind_status::SUCCESS == status){
               static_meta_value::reset_ref(ref, *field.meta, (const char*)v + field.offset);
           }
        }
        return ref;
    }
    static map_accessor& get(){
        static static_native_map_accessor ops;
        return ops;
    }
private:
    static_native_map_accessor():_meta(static_meta_class::obtain<T>()){
    }
    const meta_class& _meta;
    static_native_map_accessor& operator=(const static_native_map_accessor&) = delete;
};
template<typename T>
struct static_std_map_accessor:public map_accessor{
    virtual bool is_valid(const void*) const override{
        return true;
    }
    virtual const meta_value& get_meta_type() const override{
        return std_utils::get_meta_type<T>();
    }
    virtual const meta_value* get_value_meta_type(const void*) const override{
        return &std_utils::get_mapped_meta_type<T>();
    }
    virtual bool is_empty(const void* v) const override{
        return std_utils::is_empty<T>(v);
    }
    virtual size_t size_of(const void* v) const override{
        return std_utils::size_of<T>(v);
    }
    virtual int for_each_field_of(const void* v, for_each_field_cb cb, key_receiver& receiver, void* context) const override{
        value_ref ref;
        int result = 0;
        for(auto& kv_pair: *static_cast<const T*>(v)){
            generic_key_receiver& generic_receiver = static_cast<generic_key_receiver&>(receiver);
            generic_receiver.set(kv_pair.first);
            static_meta_value::reset_ref(ref, std_utils::get_mapped_meta_type<T>(), &kv_pair.second);
            result = cb(ref, context);
            if(result){
                break;
            }
        }
        return result;
    }
    virtual void run_with_key_receiver(const void* /*item*/, with_key_cb cb, void* context) const override{
        static_assert(key_trait<typename T::key_type>::can_input, "type do not supported to be inputed as key");
        key_converter<typename T::key_type> converter;
        cb(converter, context);
    }

    virtual bool exists(const void* v, key_receiver& receiver) const override{
        if(receiver.get_last_error() == bind_status::SUCCESS){
            key_converter<typename T::key_type>& converter =
                    static_cast<key_converter<typename T::key_type>&>(receiver);

            return std_utils::field_exists<T>(v, converter.get());
        }
        return false;

    }
    virtual value_ref get_field_at(const void* v, key_receiver& receiver) const override{
        if(receiver.get_last_error() == bind_status::SUCCESS){
            key_converter<typename T::key_type>& converter =
                               static_cast<key_converter<typename T::key_type>&>(receiver);
            const void* field = std_utils::find_field<T>(v, converter.get());
            if(nullptr != field){
                return static_meta_value::make_ref(std_utils::get_mapped_meta_type<T>(),field);
            }
        }
        return value_ref();
    }
    static map_accessor& get(){
        static static_std_map_accessor ops;
        return ops;
    }
};
struct default_map_type_router{
    template<typename T>
    static map_accessor& get_accessor(){
        return _get<T>(is_native_object<T>());
    }
    template<typename T>
    static map_accessor& _get(std::true_type /*is_native*/){
        return static_native_map_accessor<T>::get();
    }
    template<typename T>
    static map_accessor& _get(std::false_type /*is_std*/){
        static_assert(is_map<T>::value,"incompatible map.");
        static_assert(key_trait<typename T::key_type>::can_declare, "type do not supported to declare as key");
        return static_std_map_accessor<T>::get();
    }
    template<typename T>
    static typename std::enable_if<
       is_map<T>::value && key_trait<typename T::key_type>::can_declare, map_accessor*>::type
    get_optional_accessor(T*){
        return &static_std_map_accessor<T>::get();
    }
    template<typename T>
    static typename std::enable_if<is_native_object<T>::value, map_accessor*>::type
    get_optional_accessor(T*){
        return &static_native_map_accessor<T>::get();
    }
    static map_accessor* get_optional_accessor(...){
        return 0;
    }
};
}// namespace detail

template<typename T>
struct static_map_accessor_impl{
    static map_accessor& get(){
        return detail::default_map_type_router::get_accessor<T>();
    }
    static map_accessor* get_optional(){
        return detail::default_map_type_router::get_optional_accessor(static_cast<T*>(0));
    }
};

template<typename T>
inline const map_accessor& static_map_accessor::obtain(T* ){
    return static_map_accessor_impl<typename std::decay<T>::type>::get();
}

template<typename T>
inline const map_accessor* static_map_accessor::sniff(T*){
    return static_map_accessor_impl<typename std::decay<T>::type>::get_optional();
}

}




#endif /* PUBLIC_UA_DETAIL_STATIC_MAP_ACCESSOR_IMPL_H_ */
