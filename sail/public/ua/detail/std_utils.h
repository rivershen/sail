/*
 * ops_utils.h
 *
 *  Created on: Aug 4, 2015
 *      Author: chushen
 */
#ifndef PUBLIC_UA_STD_OPS_H
#define PUBLIC_UA_STD_OPS_H
#include "ua/ua_meta.h"
#include "static_meta_value.h"
#include "traits.h"
#include <stdexcept>
namespace sail{

struct meta_value;

namespace detail{

template<size_t P>
struct merit : merit<P - 1>{
};
template<> struct merit<0>{
};

struct std_utils{
    template<typename T>
    static const meta_value& get_meta_type(){
       return  static_meta_value::obtain<T>();
    }
    template<typename T>
    static const meta_value& get_value_meta_type(){
       return static_meta_value::obtain<typename T::value_type>();
    }
   template<typename T>
    static const meta_value& get_mapped_meta_type(){
      return  static_meta_value::obtain<typename T::mapped_type>();
    }
    template<typename T>
    static bool is_empty(const void* v){
      return static_cast<const T*>(v)->empty();
    }
    template<typename T>
    static size_t size_of(const void* v){
       return _size_of(static_cast<const T*>(v), can_get_size<T>());
    }
    template<typename T>
    static const void* get_element_at(const void* v, size_t index){
        return _element_at(static_cast<const T*>(v), index, (merit<8>*)0);
    }
    template<typename T>
    static bool field_exists(const void* v, const typename T::key_type& key){
        return _field_exists(static_cast<const T*>(v), key, (merit<8>*)0);
    }
    template<typename T>
    static const void* find_field(const void* v, const typename T::key_type&key){
        return _find_field(static_cast<const T*>(v), key, (merit<8>*)0);
    }
    template<typename T>
    static bind_status load_list(const meta_value& meta, const void* from, void*to){
        size_t capacity_hint;
        const list_extractor* extractor = meta.to_forward_list(from);
        assert(extractor);
        const list_accessor* accessor = meta.to_list(from);
        if(accessor){
            capacity_hint = accessor->size_of(from);
        }else{
            capacity_hint = extractor->get_size_hint(from);
        }
        _try_call_reserve(*static_cast<T*>(to), capacity_hint);
        return _load_list(*extractor, from, *static_cast<T*>(to),(merit<8>*)0);
    }
private:
    ////////////////////////////////////////////////////////////////////////
    // _size_of
    ///////////////////////////////////////////////////////////////////////
    template<typename T>
   static size_t _size_of(const T* v, std::true_type /*has size()*/){
         return v->size();
     }
    template<typename T>
    static size_t _size_of(const T* v, std::false_type /*has size()*/){
         return std::distance(v->begin(),v->end());
    }
    /////////////////////////////////////////////////////////////////////////
    // _element_at
    /////////////////////////////////////////////////////////////////////////
    template<typename T>
    static typename std::enable_if<can_call_array_subscription<T>::value && can_get_size<T>::value,
    const typename T::value_type*>::type
    _element_at(const T*v, size_t index, merit<2>*){
         if(index < v->size()){
             return &(v->operator[](index));
         }else{
             return nullptr;
         }
    }
    template<typename T>
    static typename std::enable_if<can_get_size<T>::value, const typename T::value_type*>::type
    _element_at(const T* v, size_t index, merit<1>*){
        if(index < v->size()){
            typename T::const_iterator itr = v->begin();
            std::advance(itr, index);
            return &(*itr);
        }else{
            return nullptr;
        }
    }
    template<typename T>
    static const typename T::value_type* _element_at(const T* v, size_t index, merit<0>*){
        for(const auto& item: *v){
            if( 0 == index){
                return &item;
            }
            --index;
        }
        return nullptr;
    }
    //////////////////////////////////////////////////////////////////////
    // _field_exists
    //////////////////////////////////////////////////////////////////////
    template<typename T>
    static bool _field_exists(const T* v, const typename T::key_type& key,merit<0>*){
        return v->find(key) != v->end();
    }
    ///////////////////////////////////////////////////////////////////////
    // _find_field
    //////////////////////////////////////////////////////////////////////
    template<typename T>
    static const void* _find_field(const T* v, const typename T::key_type& key, merit<0>*){
        typename T::const_iterator itr = v->find(key);
        if(itr != v->end()){
            return &(itr->second);
        }else{
            return nullptr;
        }
    }
    //////////////////////////////////////////////////////////////////////
    //_try_call_reserve
    //////////////////////////////////////////////////////////////////////
    template<typename T>
    static void _try_call_reserve(T& /*v*/, size_t /*capacity*/){
        //TODO implement
    }

    /////////////////////////////////////////////////////////////////////
    //_load_list
    /////////////////////////////////////////////////////////////////////

    //fallback
    template<typename T>
    static bind_status _load_list(const list_extractor& /*extractor*/, const void* /*from*/, T& /*to*/,merit<0>*){
        return bind_status::INVALID_CONVERSION;
    }
};

}
}

#endif //PUBLIC_UA_STD_OPS_H
