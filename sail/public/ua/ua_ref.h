/*
 * any_ref.h
 *
 *  Created on: Aug 5, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_REF_H_
#define PUBLIC_UA_REF_H_
#include <ua/ua_meta.h>
#include <typeinfo>
#include <cassert>
#include <functional>

namespace sail{
class forward_map_ref;
class map_ref;
class list_ref;
class none_type{
};
struct meta_value;
/**
 * \brief Provides a dynamic view to any C++ native type.
 *
 *  any_ref hold a reference to a variable of any type and provide runtime introspection and traverse
 *  capabilities to the variable. Like a raw pointer, it does not own or affect the life circle of
 *  bounded variable. The caller is responsible to ensure that the validity of the variable while
 *  accessing it through any_ref.
 *
 *  The size of an any_ref instance is always the size of two pointers; the extra pointer points to the
 *  hidden meta data that enables dynamic access to the referenced variable. The dynamic interface provide
 *  by any_ref is very similar a recursive variant type.

 * any_ref can replace variant types in many cases. It provides a variant like interface without
 * runtime memory or CPU overhead introduced by a real variant type. It simply adapt any native C++
 * type into a variant at any time without copying data around.
 *
 * an any_ref instance can be created from a variable of any value types using values::make_ref:
 * \snippet value_ref_examples.cpp make_value_ref_example
 *
 */
class value_ref{
public:
    //!the default constructor creates an empty value_ref.
    /**
     * An empty value_ref which does not bind to any variables will always be bound to an hidden
     * static variable whose type is none_type. The is_none() method can be used to check if
     * the value_ref is empty.
     *
     */
    value_ref();
    //!conversion constructor to create a value_ref from a map_ref.
    value_ref(const map_ref& rhs);
    //!conversion constructor to create a value_ref from a list_ref;
    value_ref(const list_ref& rhs);

    //!returns true if two instances point to the same variable.
    bool operator == (const value_ref& rhs) const;
    //!return the meta info of the referenced variable.
    const meta_value& get_meta()const;
    //!return the address of the referenced variable.
    const void* get_address() const;

    /*!\brief return true if the type of the referenced variable matches exactly
     * the type parameter T.
     *
     * This method returns true only if referenced variable has the type identical
     * to the given type parameter. Types are not considered identical even if one
     * type can be implicitly converted to another.
     *
     */
    template<typename T>
    bool is()const;
    /**
     * \brief return the reference to the underlying variable if its type is identical
     * to the type parameter; otherwise, std::bad_cast is thrown.
     */
    template<typename T>
    const T& get_typed_reference() const;
    /**
     * \brief return the strong typed pointer to the underlying variable if its
     * type is identical to the given type parameter; otherwise, nullptr is returned.
     * Types are not considered identical even if one type can be implicitly
     * converted to another.
     */
    template<typename T>
    const T* peek_typed_address() const;
    /**
     * \brief return true if there exists an implicit conversion from underlying type and
     * the given type argument.
     * If both the given type argument and the underlying type are arithmetic type
     * (boolean, integral or floating), this method returns true.
     * If both given type argument and the underlying type are std::string or
     * std::wstring, this method returns true.
     * Otherwise, it returns true only if the type argument is identical to the underlying
     * type.
     */
    template<typename T>
    bool can_cast_to() const;
    /**
     * \brief cast the reference to a given type or throw std::bad_cast() if such cast
     * is impossible.
     * For arithmetic types, this method will success if the source type can be
     * converted to the target type implicitly.

     * For convenience, conversion between std::string and std::wstring is also possible.
     * a utf-8 to utf-32 is made if the source is std::string and the target is
     * std::wstring on platforms where wchar_t is 32bit. utf-8 to utf-16 is made on
     * platforms where wchar_t is 16 bit.
     *
     * For other types, this method behave like get().
     */
    template<typename T>
    T as() const;

    /*!
     * \brief same as as() except that the fall back value is returned in the case of
     * casting failure.
     */
    template<typename T>
    T safe_as(const T& fallback) const;
    /**
     * \brief Same as as() but the casted value is copied to the given variable.
     * It has some performance benefit to as() if the cost of creatng a new instance of
     * the target type is significant.
     * create.
     */
    template<typename T>
    T& cast_to(T& target) const;

    //! return true if the bound value is structurally equivalent to a map.
    bool is_map() const;
    /*!
     * \brief return a map_ref bound to the same variable or throw std:bad_cast if the
     *  value cannot be accessed as a map.
     */
    map_ref to_map() const;
    /*!
     *  \brief return true, if the value_ref is empty (does not bind to a user given variable).
     */
    bool is_none() const;

    //! return true if the bound value is structurally equivalent to a list.
    bool is_list() const;
    /*!
     * \brief return a list_ref bound to the same variable or throw std::bad_cast if the value
     * cannot be accessed as a list.
     */
    list_ref to_list() const;

    //! accept a custom value visitor.
    bind_status accept(value_visitor& visitor) const;

    //! print the content of the underlying variable as a JSON document.
    friend std::ostream& operator << (std::ostream& os, const value_ref& v);
protected:
    //! create a value_ref from given meta_value and an address.
    value_ref(const meta_value& meta, const void* address);
    void reset(const meta_value& meta, const void* value);
    friend struct static_meta_value;
    friend struct values;
protected:
    const meta_value* _meta;
    const void* _address;
};

//!  Provides a dynamic view to any list like types.
class list_ref{
public:
    //! return the associated map extractor
    const list_accessor& get_accessor() const;
     //! return the raw address.
    const void* get_address() const;
    /**
    * \brief call the given function for each elements.
    */
   void for_each(const std::function<void(const value_ref&)> cb) const;

   /**
    * \brief return true of the list is empty.
    *  If underlying type is a STL sequence like container, and it has a method named
    *  empty(), the return value of the method is returned. For any other types, this
    *  method always return false.
    */
   bool empty() const;
  /**
   * \brief return the number of element the underlying list contains.
   * Based on underlying type, it choose following strategies:
   * - If the underlying type is a STL sequence like container and it has a method named size(),
   * the return value of the method is returned.
   * - If the underlying type is a STL sequence like container and it does not have the size() method
   * defined, the value of the expression std::distance(_list.begin(), _list.end()) is returned.
   */
   size_t size() const;

   /**
    * \brief return a value_ref bound to the requested element by its index.
    *
    * A value_ref is returned if the given index is less than size() otherwise,
    * std::out_of_range is thrown.
    */
   value_ref operator[](size_t index) const;
protected:
    //~ construct a list_ref by given extractor and address.
    list_ref(const list_accessor& accessor, const void* address);

protected:
    const list_accessor* _accessor;
    const void* _address;
    friend struct lists;
    friend class value_ref;
};
/**
* \brief Provides a dynamic view to any map like types.
*/
class map_ref{
public:
    //! return the associated map extractor
     const map_accessor& get_extractor() const;
     //! return the raw address.
     const void* get_address() const;
     /**
      * \brief call the given function for each fields.
      */
     void for_each(const std::function<void(const std::string&, const value_ref&)>& cb) const;
     /**
      * \brief return true if the map is empty.
      * If underlying type is a STL map like container, and it has a method named empty(), the return
      * value of the method is returned. For any other types, this method always return false.
      */
     bool empty() const;
     //! return the number of fields the underlying map contains.
     /**
      * Based on underlying type, it choose following strategies:
      * - If the underlying type is a STL map like container and it has a method named size(),
      * the return value of the method is returned.
      * - If the underlying type is a STL map like container and it does not have the size() method
      * defined, the value of the expression std::distance(_map.begin(), _map.end()) is returned.
      * - If the underlying type is a native object ( annotated by the UA_NATIVE_OBJECT() family
      * of macros) the number of fields that are passed to the macro is returned.
      */
     size_t size() const;
     /**
      * \brief return a value_ref bound to the requested field by its key.
      * A value_ref is returned if the requested field exists, otherwise,
      * std::out_of_bound is thrown. If the user pass the key as const char*, they must
      * ensure that it points to a valid c style string, otherwise, the behavior is undefined.
      */
     value_ref operator[](const char* key) const;
     value_ref operator[](const std::string& key) const;

protected:
    //! construct a map_ref by given accessor and address.
    map_ref(const map_accessor& accessor, const void* address);

private:
    const map_accessor* _accessor;
    const void* _address;
    friend class value_ref;
    friend struct maps;
};

//////////////////////////////////////////////////////
//IMPLEMENTATION
/////////////////////////////////////////////////////
//! \cond NO_DOC
//! the expected type does not match the type of the given variable.
class type_mismatch_exception:public std::bad_cast{
public:
    explicit type_mismatch_exception(std::string s)
    :_str(std::move(s)){
    }
    virtual const char* what() const throw() override{
        return _str.c_str();
    }
private:
    std::string _str;
};

#define UA_DECLARE_CONVERTER_METHOD(type) bind_status convert(const value_ref&, type*);
struct default_type_converter{
    UA_FOR_EACH_SCALARS(UA_DECLARE_CONVERTER_METHOD)
    template<typename T>
    bind_status convert(const value_ref& from, T* to){
        const T* v = from.peek_typed_address<T>();
        if(v){
            if(to){
                *to = *v;
            }
            return bind_status::SUCCESS;
        }else{
            return bind_status::INVALID_CONVERSION;
        }
    }
};
#undef UA_DECLARE_CONVERTER_METHOD

template<typename T>
inline bool value_ref::is() const{
    return (_meta->get_type_info(_address) == typeid(typename std::decay<T>::type));
}
template<>
inline bool value_ref::is<long long>() const{
    return (_meta->get_type_info(_address) == typeid(int64_t));
}
template<>
inline bool value_ref::is<unsigned long long>() const{
    return (_meta->get_type_info(_address) == typeid(uint64_t));
}
template<>
inline bool value_ref::is<char>() const{
    const std::type_info& info = _meta->get_type_info(_address);
    return (info == typeid(uint8_t) || info == typeid(int8_t));
}

template<typename T>
inline const T& value_ref::get_typed_reference() const{
    const T* result = peek_typed_address<T>();
    if(!result){
        throw type_mismatch_exception("any_ref::get() type mismatch");
    }
    return *result;
}
template<typename T>
inline const T* value_ref::peek_typed_address() const{
    if(is<T>()){
            return reinterpret_cast<const T*>(_address);
    }else{
        return nullptr;
    }
}
template<typename T>
inline bool value_ref::can_cast_to() const{
    if(!is<T>()){
        default_type_converter converter;
        return bind_status::SUCCESS == converter.convert(*this, (T*)0);
    }else{
        return true;
    }
}
template<typename T>
inline T value_ref::as() const{
    T result;
    return cast_to(result);
}
template<typename T>
inline T& value_ref::cast_to(T& target) const{
    if(is<T>()){
        target =  *reinterpret_cast<const T*>(_address);
    }else{
        default_type_converter converter;
        if(bind_status::SUCCESS != converter.convert(*this, &target)){
           throw type_mismatch_exception("any_ref::cast_to() cast impossible.");
        }
    }
    return target;
}
template<typename T>
inline T value_ref::safe_as(const T& fallback) const {
    T result;
    default_type_converter converter;
    if(bind_status::SUCCESS != converter.convert(*this, &result)){
       return fallback;
    }
    return result;
}

inline std::ostream& operator << (std::ostream& os, const map_ref& v){
    value_ref ref(v);
    return operator << (os, ref);
}

inline std::ostream& operator << (std::ostream& os, const list_ref& v){
    value_ref ref(v);
    return operator << (os, ref);
}
//! \endcond
} // namespace universal_access;



#endif /* PUBLIC_UA_REF_H_ */
