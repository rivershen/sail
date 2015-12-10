/*
 * meta.h
 *
 *  Created on: Aug 12, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_META_INTERFACE_H_
#define PUBLIC_UA_META_INTERFACE_H_
#include <ua/macros.h>
#include <string>
#include <vector>
#include <typeinfo>
#include <cstdint>
namespace sail{
//! \cond DEV
enum class UA_ENUM(bind_status,
    SUCCESS,
    INVALID_STATE,
    INVALID_CONVERSION,
    NUMERIC_RANGE_OVERFLOW,
    INVALID_FIELD_NAME,
    INVALID_ENUM_VALUE,
    INVALID_ENUM_NAME,
    INVALID_KEY_TYPE,
    OP_NOT_SUPPORTED
);

struct list_extractor;
struct list_accessor;
struct map_extractor;
struct map_accessor;
struct value_visitor;
struct meta_class;
struct key_receiver;
struct meta_value;
class value_ref;

#define UA_FOR_EACH_INTEGRAL(x) \
        x(uint8_t)              \
        x(int8_t)               \
        x(uint16_t)             \
        x(int16_t)              \
        x(uint32_t)             \
        x(int32_t)              \
        x(uint64_t)             \
        x(int64_t)

#define UA_FOR_EACH_NUMERIC(x)  \
        UA_FOR_EACH_INTEGRAL(x) \
        x(float)                \
        x(double)

#define UA_FOR_EACH_SCALARS(x) \
        UA_FOR_EACH_NUMERIC(x) \
        x(bool)                \
        x(std::string)         \
        x(std::wstring)

#define UA_DECLARE_VISITOR_METHOD(type) virtual bool visit(const type*){return false;}
struct value_visitor{
    UA_FOR_EACH_SCALARS(UA_DECLARE_VISITOR_METHOD)
    virtual bool visit(const meta_value& /*meta*/, const void* /*value*/){return false;}
    bind_status get_last_error(){
        return _last_error;
    }
    virtual ~value_visitor(){}
protected:
    bind_status _last_error = bind_status::INVALID_CONVERSION;
};
#undef UA_DECLARE_VISITOR_METHOD
struct key_receiver{
    virtual bool set(const std::string&) = 0;
    virtual bool set(const std::wstring&)= 0;
    virtual bool set(const char*)= 0;
    virtual bool set(const wchar_t*)= 0;
    virtual bool set_copy(std::string v);
    virtual bool set_copy(std::wstring v);
    bind_status get_last_error() const{
        return _lastError;
    }
protected:
    virtual bool set(const meta_value& meta, const void* v);
    virtual ~key_receiver(){}
    bind_status _lastError = bind_status::INVALID_STATE;
};

struct meta_value{
    virtual bind_status accept(const void* value, value_visitor& visitor) const = 0;
    virtual const map_extractor* to_forward_map(const void* value) const = 0;
    virtual const map_accessor* to_map(const void* value) const = 0;
    virtual const list_extractor* to_forward_list(const void* value) const = 0;
    virtual const list_accessor* to_list(const void* value) const = 0;
    virtual const std::type_info& get_type_info(const void* value) const = 0;
    virtual bind_status load_from(const value_ref&from, void* to) const = 0;
    virtual ~meta_value(){}
};

struct list_extractor{
    //return true if the data is ready to extract.
    virtual bool is_valid(const void* v) const = 0;
    virtual bool is_empty(const void* v) const=0;
    /*
     * This method returns the element count of the underlying list when possible. It should return
     * 0 if is_empty() returns true, otherwise, return a positive number equal to or close to the
     * actual element count. The returned value might be used as a hint to load_from() to calculate
     * the memory needed in the destination data structure to hold the map.
     *
     * The implementation is allowed to return a dummy value such as 1 if the cost of calculating the
     * accurate element count is more expensive than re-allocating the destination buffer.
     */
    virtual size_t get_size_hint(const void* v) const = 0;
    virtual const meta_value& get_meta_type() const=0;
    virtual const meta_value* get_value_meta_type(const void* v) const=0;
    typedef int (*for_each_element_cb)(const value_ref& item, void* context);
    virtual int for_each_element_of(const void* v, for_each_element_cb callback, void* context) const = 0;
    virtual ~list_extractor(){}
};

struct list_accessor: list_extractor{
    virtual size_t size_of(const void* v) const=0;
    virtual value_ref get_element_at(const void* v, size_t index) const = 0;
    virtual bind_status load_from(const value_ref&from, void* to) const = 0;
};


struct map_extractor{
    virtual bool is_valid(const void* v) const = 0;
    virtual bool is_empty(const void* v) const=0;
    virtual const meta_value& get_meta_type() const=0;
    virtual const meta_value* get_value_meta_type(const void* v) const=0;
    typedef int (*for_each_field_cb)(const value_ref& item, void* context);
    virtual int for_each_field_of(const void* v, for_each_field_cb cb, key_receiver& receiver, void* context) const = 0;
    virtual ~map_extractor(){}
};
struct map_accessor: map_extractor{
    virtual size_t size_of(const void* v) const=0;
    typedef void(*with_key_cb)(key_receiver& reciever, void* context);
    virtual void run_with_key_receiver(const void* item, with_key_cb cb, void* context) const = 0;
    virtual bool exists(const void* v, key_receiver& receiver) const = 0;
    virtual value_ref get_field_at(const void* v, key_receiver& receiver) const = 0;
};

struct meta_class{
    struct field{
        std::string name;
        const meta_value* meta;
        size_t offset;
        bool is_optional;
    };
    virtual size_t size() const =0;
    virtual const field* begin() const = 0;
    virtual const field* end() const = 0;
    virtual bool is_homogenious() const = 0;
    virtual const field& get_field(const std::string& name, bind_status* status=0)const = 0;
    virtual bool exists(const std::string& name) const = 0;
    virtual ~meta_class(){}
};

struct meta_enum{
    struct field{
        field(const std::string& n, int v)
        :name(n), value(v){
        }
        std::string name;
        int value;
    };
    virtual size_t size() const =0;
    virtual const std::string& lookup_name(int v, bind_status* status = nullptr)const=0;
    virtual int lookup_value(const std::string& name, bind_status* status = nullptr)const=0;
    virtual const field* begin() const=0;
    virtual const field* end() const=0;
    virtual ~meta_enum(){}
};
//! \endcond
}

#endif /* PUBLIC_UA_META_INTERFACE_H_ */
