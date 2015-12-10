/*
 * static_meta_enum.h
 *
 *  Created on: Aug 10, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_META_ENUM_IMPL_H_
#define PUBLIC_UA_DETAIL_STATIC_META_ENUM_IMPL_H_
#include "literal_utils.h"
#include "static_meta_enum.h"
#include "traits.h"
namespace sail{
namespace detail{
struct meta_enum_impl:public meta_enum{
    virtual size_t size() const override;
    virtual const std::string& lookup_name(int v, bind_status* status = nullptr)const override;
    virtual int lookup_value(const std::string& name, bind_status* status = nullptr)const override;
    virtual const field* begin() const override;
    virtual const field* end() const override;

    typedef std::vector<field>values_type;
    values_type _values;
    void sort();
};

}//namespace detail

template<typename T>
struct static_meta_enum_impl{
    static_assert(is_native_enum<T>::value,
                "The enum type is not annotated with UA_NATIVE_ENUM() or"
                " UA_MAKE_NATIVE_ENUM().");
    static const meta_enum& get(){
        static static_meta_enum_impl s;
        return s._meta;
    }
private:
    static_meta_enum_impl(){
        visitor v(_meta._values);
        _ua_visit_native_enum((T)0, &v);
        _meta.sort();
    }
    struct visitor{
        explicit visitor(detail::meta_enum_impl::values_type& values)
        :_values(values){
        }
        visitor& operator<<(const char* s){
            detail::literal_utils::parse_enum_name(s, _name);
            return *this;
        }
        visitor& operator<<(T v){
            _values.emplace_back(_name, (int)v);
            return *this;
        }
        template<typename U>
        void operator = ( U ){
        }
        detail::meta_enum_impl::values_type& _values;
        std::string _name;
        visitor& operator=(const visitor&) = delete;
    };
    detail::meta_enum_impl  _meta;
};

template<typename T>
inline const meta_enum& static_meta_enum::obtain(T*){
    return static_meta_enum_impl<typename std::decay<T>::type>::get();
}
}//namespace universal_access




#endif /* PUBLIC_UA_DETAIL_STATIC_META_ENUM_IMPL_H_ */
