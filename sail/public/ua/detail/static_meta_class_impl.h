/*
 * meta_class_initializer.h
 *
 *  Created on: Aug 7, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_STATIC_META_CLASS_IMPL_H_
#define PUBLIC_UA_DETAIL_STATIC_META_CLASS_IMPL_H_
//#include "ua/ua.h"
#include "static_meta_class.h"
#include "literal_utils.h"
#include <string>
#include <vector>
#include <cassert>

namespace sail{
namespace detail{
struct meta_class_impl:public meta_class{
    virtual size_t size() const override;
    virtual const field* begin() const override;
    virtual const field* end() const override;
    virtual bool is_homogenious() const override;
    virtual const field& get_field(const std::string& name, bind_status* status=0)const override;
    virtual bool exists(const std::string& name) const override;
    std::vector<field> _fields;
    bool _is_homogenious;
};
struct native_object_visitor{
    native_object_visitor(std::vector<meta_class::field>& fields)
    :mFields(fields){
    }
    template<typename U>
    void setObject(const U* obj){
        mBase = (char*)(obj);
        mLimit = sizeof(U);
    }
    native_object_visitor& field(const char* fieldName){
        mFieldName = fieldName;
        return *this;
    }
    template<typename U>
    void operator + (const U& field){
        add_field(&field, static_meta_value::obtain(&field),false);
    }

    template<typename U>
    native_object_visitor& operator - (const U& field){
        add_field(&field, static_meta_value::obtain(&field),true);
        return *this;
    }
    template<typename U>
    void operator = (const U& ){
        //ignored.
    }
    const std::vector<meta_class::field>& getFields() const{
        return mFields;
    }
    void sortFields();

    friend struct NativeObjectFieldDefaultValueBase;

private:

    void add_field(const void* address, const meta_value& meta, bool isOptional);
    std::vector<meta_class::field>& mFields;
    const char* mFieldName = nullptr;
    const char* mBase = nullptr;
    size_t mLimit = 0;

    native_object_visitor& operator=(const native_object_visitor&) = delete;
};
}

template<typename T>
struct static_meta_class_impl{
    static_assert(is_native_object<T>::value,
                    "The class/struct is not annotated with UA_NATIVE_OBJECT() or"
                    " UA_MAKE_NATIVE_OBJECT().");
    static const meta_class& get(){
        static static_meta_class_impl<T> instance;
        return instance._meta;
    }
private:
    static_meta_class_impl(){
        std::vector<meta_class::field>& fields = _meta._fields;
        detail::native_object_visitor visitor(fields);
        T* obj = nullptr;
        visitor.setObject(obj);
        _ua_visit_native_object(obj,&visitor);
        visitor.sortFields();
        _meta._is_homogenious = true;
        assert(!fields.empty());
        if(!fields.empty()) {
            const meta_value* firstMeta = fields.front().meta;
            for (auto &entry:fields) {
                if(entry.meta != firstMeta){
                    _meta._is_homogenious = false;
                    break;
                }
            }
        }
    }
    detail::meta_class_impl _meta;
};

template<typename T>
inline const meta_class& static_meta_class::obtain(T*){
    return static_meta_class_impl<typename std::decay<T>::type>::get();
}

}




#endif /* PUBLIC_UA_DETAIL_STATIC_META_CLASS_IMPL_H_ */
