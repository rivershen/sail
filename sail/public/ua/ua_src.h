/*
 * src.h
 *
 *  Created on: Aug 7, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_SRC_H_
#define PUBLIC_UA_SRC_H_
#include "ua/metas.h"
#include "ua/values.h"
#include "utf8/unchecked.h"
#include "ua/detail/safe_implicit_value_converter.h"
#include <cstring>
#include <cwchar>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <limits>
#include <cmath>
#if defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable: 4100 4512 )
#endif
namespace sail{
/////////////////////////////////////////////////////////////////////////////////////////
//value_ref
/////////////////////////////////////////////////////////////////////////////////////////
static none_type none_value;
value_ref::value_ref()
:_meta(&metas::get_meta_value<none_type>()),_address(&none_value){
}
value_ref::value_ref(const map_ref& rhs)
:_meta(&rhs._accessor->get_meta_type()), _address(rhs._address){
}
value_ref::value_ref(const list_ref& rhs)
:_meta(&rhs._accessor->get_meta_type()), _address(rhs._address){
}
value_ref::value_ref(const meta_value& meta, const void* address)
:_meta(&meta),_address(address){
    assert(address);
}
bool value_ref::operator== (const value_ref& rhs) const{
    return (_meta == rhs._meta && _address == rhs._address);
}
void value_ref::reset(const meta_value& meta, const void* value){
    assert(value);
    _address = value;
    _meta = &meta;
}
const meta_value& value_ref::get_meta()const{
    return *_meta;
}
const void* value_ref::get_address() const{
    return _address;
}
bool value_ref::is_map() const{
    return (nullptr != _meta->to_map(_address));
}
map_ref value_ref::to_map() const{
    const map_accessor* accessor = _meta->to_map(_address);
    if(accessor){
        return map_ref(*accessor, _address);
    }
    throw type_mismatch_exception("to_map(): referenced value is not a map");
}
bool value_ref::is_none() const{
    return _meta == &metas::get_meta_value<none_type>();
}
bool value_ref::is_list()const{
    return(nullptr != _meta->to_list(_address));
}
list_ref value_ref::to_list() const{
    const list_accessor* accessor = _meta->to_list(_address);
    if(accessor){
        return list_ref(*accessor, _address);
    }
    throw type_mismatch_exception("to_list(): referenced value is not a list");
}
bind_status value_ref::accept(value_visitor& visitor) const{
    return _meta->accept(_address,visitor);
}
template<typename T>
inline void value_ref_print_integral_helper(std::ostream& os, const T& v, size_t size){
    os<<std::dec;
    if(size<4){
        os<<(int)v;
    }else{
        os<<v;
    }
 
}
template<typename T>
inline void value_ref_print_iec559_helper(std::ostream& os, const T&v){
    if (std::isnan(v)){
        os << "\"NaN\"";
    }
    else if (v == std::numeric_limits<T>::infinity()){
        os << "\"Infinity\"";
    }
    else{
        os << v;
    }
}
std::ostream& operator << (std::ostream& os, const value_ref& v){
    struct printer:value_visitor{
        explicit printer(std::ostream& os)
        :_os(os){
        }
        bool visit(const bool* value) override{
           _os << (value? "true":"false");
           return true;
        }
        void write_escaped(const char* s){
            _os << std::hex<<std::right << std::setfill('0');
            _os<<"\"";
            while(*s){
                switch(*s){
                case '"': _os << "\\\""; break;
                case '\\': _os << "\\\\"; break;
                case '/': _os << "\\/"; break;
                case '\b': _os << "\\b"; break;
                case '\f': _os << "\\f"; break;
                case '\n': _os << "\\n"; break;
                case '\r': _os << "\\r"; break;
                case '\t': _os << "\\t"; break;
                default:
                    if ((*s > 0 && *s < 32) || *s == 127){
                        _os << "\\u";
                        _os << std::setw(4) << static_cast<unsigned int>(*s);
                    }else
                    _os << *s;
                }
                ++s;
            }
            _os<<"\"";
        }
        bool visit(const std::string * value)override{
            write_escaped(value->c_str());
            return true;
        }
        bool visit(const std::wstring * value) override{
            key_converter<const char*> converter;
            converter.set(*value);
            write_escaped(converter.get());
            return true;
        }
        bool visit(const meta_value& meta, const void* value) override{
            const value_ref& ref = values::make_ref(meta, value);
            printer& self = *this;
            const char* seperator = "";
            if(ref.is_list()){
                _os<<"[";
                ref.to_list().for_each([&](const value_ref& item){
                    _os<< seperator;
                    seperator = ",";
                    item.accept(self);
                });
                _os<<"]";
            }else if(ref.is_map()){
                _os<<"{";
                ref.to_map().for_each([&](const std::string& key, const value_ref& field){
                    _os<< seperator;
                    seperator = ",";
                    write_escaped(key.c_str());
                    self._os << ":";
                    field.accept(self);
                });
                _os<<"}";
            }else if(&meta == &metas::get_meta_value<none_type>()){
                _os<<"null";
            }else{
                _os<<"\"[unsuported@0x"<<std::hex<<(size_t)value<<"]\"";
            }
            return true;
        }
        bool visit(const float* value) override{
            value_ref_print_iec559_helper(_os,*value);
            return true;
        }
        bool visit(const double* value) override{
            value_ref_print_iec559_helper(_os,*value);
            return true;
        }
        #define UA_DECLARE_PRINTER_METHOD(type)                                 \
              bool visit(const type* value)override{                            \
                    value_ref_print_integral_helper(_os,*value,sizeof(type));   \
                    return true;                                                \
                }
        UA_FOR_EACH_INTEGRAL(UA_DECLARE_PRINTER_METHOD)
        #undef UA_DECLARE_PRINTER_METHOD
        std::ostream& _os;
    };
    printer visitor(os);
    std::ios_base::fmtflags saved_flag = os.flags();
    v.accept(visitor);
    os.setf(saved_flag);
    return os;
}
//////////////////////////////////////////////////////////////////////////////////////////
//values
/////////////////////////////////////////////////////////////////////////////////////////
value_ref values::make_ref(const meta_value& meta, const void* address){
    return value_ref(meta, address);
}

////////////////////////////////////////////////////////////////////////////////////////
//list_ref
////////////////////////////////////////////////////////////////////////////////////////
list_ref::list_ref(const list_accessor& accessor, const void* address)
:_accessor(&accessor),_address(address){
}

const list_accessor& list_ref::get_accessor() const{
    return *_accessor;
}

const void* list_ref::get_address() const{
    return _address;
}
void list_ref::for_each(const std::function<void(const value_ref&)> cb) const{
    struct adaptor{
        adaptor(const std::function<void(const value_ref&)>& cb)
        :_cb(cb){
        }
        void process(const value_ref& item){
             _cb(item);
        }
        static int do_process(const value_ref& item, void* context){
            ((adaptor*)context)->process(item);
            return 0;
        }
        const std::function<void(const value_ref&)>& _cb;
    };
    adaptor adaptor(cb);
    _accessor->for_each_element_of(_address, adaptor.do_process, &adaptor);
}
bool list_ref::empty() const{
    return _accessor->is_empty(_address);
}
size_t list_ref::size() const{
    return _accessor->size_of(_address);
}

value_ref list_ref::operator[](size_t index) const{
    const value_ref& result =  _accessor->get_element_at(_address, index);
    if(result.is_none()){
        throw std::out_of_range("list_ref[size_t] : index out of bound");
    }
    return result;
}
/////////////////////////////////////////////////////////////////////////////////////////
//_map_ref
////////////////////////////////////////////////////////////////////////////////////////
const map_accessor& map_ref::get_extractor()const{
    return *_accessor;
}
const void* map_ref::get_address() const{
    return _address;
}

template<typename Converter, typename Callback>
static void map_for_each_impl(const void* address,const map_extractor& extractor,
                                  const Callback& cb, Converter& conv){
    struct adaptor{
        adaptor(const Callback& cb, Converter& conv)
        :_cb(cb),_conv(conv){
        }
        void process(const value_ref& item){
            _cb(_conv.get(), item);
        }
        static int do_process(const value_ref& item, void* context){
            ((adaptor*)context)->process(item);
            return 0;
        }
        const Callback& _cb;
        Converter& _conv;
    };
    adaptor adaptor(cb, conv);
    extractor.for_each_field_of(address, adaptor.do_process, conv, &adaptor);
}
void map_ref::for_each(const std::function<void(const std::string&, const value_ref&)>& cb) const{
    key_converter<std::string> converter;
    map_for_each_impl(_address,*_accessor, cb, converter);
}

bool map_ref::empty() const{
    return _accessor->is_empty(_address);
}
size_t map_ref::size() const{
    return _accessor->size_of(_address);
}
template<typename T>
static value_ref map_get_field_impl(const map_accessor& accessor, const void* address, const T& key){
    struct adaptor{
        adaptor(const map_accessor& accessor, const void* v, const T& key)
        :_accessor(accessor),_address(v),_key(key){
        }
        static void call_get_field(key_receiver& receiver, void* context){
              adaptor* self = (adaptor*)context;
              ((generic_key_receiver&) receiver).set(self->_key);
              self->_result = self->_accessor.get_field_at(self->_address,receiver);
         }
        const map_accessor& _accessor;
        const void* _address;
        const T& _key;
        value_ref _result;
//        adaptor* operator=(const adaptor&) = delete;
    };
    adaptor o(accessor, address, key);
    accessor.run_with_key_receiver(address, &adaptor::call_get_field, &o);
    if(o._result.is_none()){
        throw std::out_of_range("map_ref[key] key does not exist");
    }
    return o._result;

}
value_ref map_ref::operator[](const char* key) const{
    return map_get_field_impl<const char*>(*_accessor, _address, key);
}
value_ref map_ref::operator[](const std::string& key) const{
    return map_get_field_impl<std::string>(*_accessor, _address, key);
}
map_ref::map_ref(const map_accessor& accessor, const void* address)
:_accessor(&accessor), _address(address){
}

/////////////////////////////////////////////////////////////////////////////////////////
//default_type_converter
/////////////////////////////////////////////////////////////////////////////////////////
template<typename T>
struct default_value_visitor:value_visitor{
    explicit default_value_visitor(T* to)
    :_to(to){
    }

#define UA_IMPLEMENT_VISITER_METHOD(type)                                       \
   virtual bool visit(const type* from){                                        \
        if(detail::implicit_value_converter::convert(*from, _to)){              \
             _last_error = bind_status::SUCCESS;                                \
             return true;                                                       \
        }else{                                                                  \
            _last_error = bind_status::INVALID_CONVERSION;                      \
            return false;                                                       \
        }                                                                       \
    }
    UA_FOR_EACH_SCALARS(UA_IMPLEMENT_VISITER_METHOD)
#undef UA_IMPLEMENT_VISITER_METHOD
private:
    T* _to;
};
#define UA_IMPLEMENT_CONVERTER_METHOD(type)                                     \
bind_status default_type_converter::convert(const value_ref& from, type* to){   \
    default_value_visitor<type> visitor(to);                                    \
    return from.accept(visitor);                                                \
}
    UA_FOR_EACH_SCALARS(UA_IMPLEMENT_CONVERTER_METHOD)
#undef UA_IMPLEMENT_CONVERTER_METHOD

//////////////////////////////////////////////////////////////////////////////////////////
//key_receiver
/////////////////////////////////////////////////////////////////////////////////////////
bool key_receiver::set_copy(std::string v){
    return set(v);
}
bool key_receiver::set_copy(std::wstring v){
    return set(v);
}

bool key_receiver::set(const meta_value& meta, const void* v){
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////
//field_name_wrapper
//////////////////////////////////////////////////////////////////////////////////////////
struct field_name_wrapper{
    field_name_wrapper(const std::string& name):mName(name){}
    field_name_wrapper(const meta_class::field& field):mName(field.name){}
    bool operator <(const field_name_wrapper& rhs) const{
        return mName < rhs.mName;
    }
    const std::string& mName;
};
//////////////////////////////////////////////////////////////////////
// static_meta_value
//////////////////////////////////////////////////////////////////////
void static_meta_value::reset_ref(value_ref& ref, const meta_value& meta, const void* address){
    ref.reset(meta,address);
}
value_ref static_meta_value::make_ref(const meta_value& meta, const void* address){
    return value_ref(meta,address);
}
namespace detail{
////////////////////////////////////////////////////////////////////////////////////////////
//meta_class_impl
///////////////////////////////////////////////////////////////////////////////////////////

size_t meta_class_impl::size() const{
   return _fields.size();
}

const meta_class_impl::field* meta_class_impl::begin() const{
    assert(!_fields.empty());
    return &_fields.front();
}

const meta_class_impl::field* meta_class_impl::end() const{
    assert(!_fields.empty());
   return &_fields.back()+1;
}

bool meta_class_impl::is_homogenious() const{
   return _is_homogenious;
}

const meta_class_impl::field& meta_class_impl::get_field(const std::string& name, bind_status* status)const{
    static field sInvalidMetaClassField;
    const field* result = &sInvalidMetaClassField;
    bind_status search_status = bind_status::INVALID_ENUM_NAME;

    auto itr = std::lower_bound(_fields.begin(), _fields.end(), field_name_wrapper(name),
                                        [](const field_name_wrapper& left, const field_name_wrapper& right){
                                            return left < right;
                                        });

    if(itr != _fields.end() && itr->name == name){
       search_status = bind_status::SUCCESS;
       result = &(*itr);
   }

   if(status){
       *status = search_status;
   }else if(search_status != bind_status::SUCCESS){
       throw std::out_of_range("Invalid Field Name.");
   }

   return *result;
}
bool meta_class_impl::exists(const std::string& name) const{
    return std::binary_search(_fields.begin(), _fields.end(), field_name_wrapper(name),
                                        [](const field_name_wrapper& left, const field_name_wrapper& right){
                                            return left < right;
                                        });
}
///////////////////////////////////////////////////////////////////////////////
// meta_enum
///////////////////////////////////////////////////////////////////////////////
size_t meta_enum_impl::size() const{
    return _values.size();
}
const std::string& meta_enum_impl::lookup_name(int v, bind_status* status)const{
    static std::string sInvalidName("<invalid enum>");
    const std::string* result = &sInvalidName;
    bind_status search_status = bind_status::INVALID_ENUM_VALUE;
    for(auto& item: _values){
        if(item.value == v){
            result = & item.name;
            search_status = bind_status::SUCCESS;
            break;
        }
    }
    if(status){
        *status = search_status;
    }else if(search_status != bind_status::SUCCESS){
        throw std::out_of_range("Invalid Enum Value.");
    }
    return *result;
 }
struct enum_name_wrapper{
    enum_name_wrapper(const std::string& name):mName(name){}
    enum_name_wrapper(const meta_enum::field& record):mName(record.name){}
    bool operator <(const enum_name_wrapper& rhs) const{
        return mName < rhs.mName;
    }
    const std::string& mName;
};
int meta_enum_impl::lookup_value(const std::string& name, bind_status* status)const{
    int result = (int)0xBADBADEE;
    bind_status search_status = bind_status::INVALID_ENUM_NAME;

    auto it = std::lower_bound(_values.begin(), _values.end(),
            enum_name_wrapper(name),[](const enum_name_wrapper& left, const enum_name_wrapper& right){
        return left.mName < right.mName;
    });

    if(it != _values.end() && it->name == name){
        search_status = bind_status::SUCCESS;
        result = it->value;
    }

    if(status){
        *status = search_status;
    }else if(search_status != bind_status::SUCCESS){
        throw std::out_of_range("Invalid Enum Name.");
    }

    return result;
}
const meta_enum_impl::field* meta_enum_impl::begin() const{
    assert(!_values.empty());
    return &_values.front();
}
const meta_enum_impl::field* meta_enum_impl::end() const{
    assert(!_values.empty());
     return &_values.back()+1;
}
void meta_enum_impl::sort(){
    std::sort(_values.begin(), _values.end(),[](const field& left, const field& right){
        return left.name < right.name;
    });
}

///////////////////////////////////////////////////////////////////////////////////
// native_object_visitor
///////////////////////////////////////////////////////////////////////////////////
void native_object_visitor::add_field(const void* address, const meta_value& meta, bool isOptional){
    assert(mFieldName);
    size_t offset = (const char*)address - mBase;
    //fields from virtual base are not supported. the following assert() enforces this.
    assert(offset < mLimit);
    mFields.resize(mFields.size()+1);
    meta_class::field& r = mFields.back();
    literal_utils::parse_field_name(mFieldName, r.name);
    r.offset = offset;
    r.meta = &meta;
    r.is_optional = isOptional;
}
void native_object_visitor::sortFields(){
    std::sort(mFields.begin(), mFields.end(),[](const meta_class::field& left, const meta_class::field& right){
        return left.name < right.name;
    });
}
////////////////////////////////////////////////////////////////////////////////
// utf_converter
////////////////////////////////////////////////////////////////////////////////
template<int I> struct wchar_size_tag{};
struct utf_converter_impl{
    template<typename iterator_in, typename iterator_out>
    static iterator_out u8_to_ws(iterator_in start, iterator_in end, iterator_out result,
            wchar_size_tag<2>){
        return utf8::unchecked::utf8to16(start,end,result);
    }
    template<typename iterator_in, typename iterator_out>
    static iterator_out u8_to_ws(iterator_in start, iterator_in end, iterator_out result,
            wchar_size_tag<4>){
        return utf8::unchecked::utf8to32(start,end,result);
    }
    template<typename iterator_in, typename iterator_out>
    static iterator_out ws_to_u8(iterator_in start, iterator_in end, iterator_out result,
            wchar_size_tag<2>){
       return utf8::unchecked::utf16to8(start,end,result);
    }
    template<typename iterator_in, typename iterator_out>
    static iterator_out ws_to_u8(iterator_in start, iterator_in end, iterator_out result,
            wchar_size_tag<4>){
        return utf8::unchecked::utf32to8(start,end,result);
    }
};
bind_status utf_converter::u8_to_ws(const std::string& src, std::wstring& dst){
    if(src.empty()){
        dst.clear();
    }else{
        // just assume the worst cast scenario: the utf8 string contains only single
        // byte characters.
        if(dst.size() < src.size()){
           dst.resize(src.size());
        }
        auto end = utf_converter_impl::u8_to_ws(&src.front(), &src.back()+1, &dst.front(),
                   wchar_size_tag<sizeof(wchar_t)>());
        dst.resize(end - &dst.front());
    }
    return bind_status::SUCCESS;
}
bind_status utf_converter::u8_to_ws(const char* src, std::wstring& dst){
    size_t src_size = ::strlen(src);
    if(src_size == 0){
        dst.clear();
    }else{
        const char* src_end = src + src_size;
        if(dst.size() < src_size){
            dst.resize(src_size);
        }
        auto end = utf_converter_impl::u8_to_ws(src, src_end, &dst.front(),
                wchar_size_tag<sizeof(wchar_t)>());
        dst.resize(end - &dst.front());
    }
    return bind_status::SUCCESS;
}
bind_status utf_converter::ws_to_u8(const std::wstring& src, std::string& dst){
    if(src.empty()){
        dst.clear();
    }else{
        // just assume the worst case scenario, each character in the utf8 string uses 4 bytes
        size_t dst_size = 4 * src.size();
        if(dst.size() < dst_size){
            dst.resize(dst_size);
        }
        auto end = utf_converter_impl::ws_to_u8(&src.front(), &src.back()+1, &dst.front(),
                wchar_size_tag<sizeof(wchar_t)>());
        dst.resize(end - &dst.front());
    }
    return bind_status::SUCCESS;
}
bind_status utf_converter::ws_to_u8(const wchar_t* src, std::string& dst){
    size_t src_size = ::wcslen(src);
    if(src_size == 0){
        dst.clear();
    }else{
        const wchar_t* src_end = src+ src_size;
        size_t dst_size = 4 * src_size;
        if(dst.size() < dst_size){
            dst.resize(dst_size);
        }
        auto end = utf_converter_impl::ws_to_u8(src, src_end, &dst.front(),
                    wchar_size_tag<sizeof(wchar_t)>());
        dst.resize(end - &dst.front());
    }
    return bind_status::SUCCESS;
}
///////////////////////////////////////////////////////////////////////////
//literal_utils
///////////////////////////////////////////////////////////////////////////
void literal_utils::parse_enum_name(const char* from, std::string& to ){
   parse_field_name(from, to);
}
void literal_utils::parse_field_name(const char* from, std::string& to ){
    while(*from && !::isalpha(*from) && '_' != *from){
        ++from;
    }
    const char* end = from;
    while(*end && (::isalnum(*end) || '_' == *end)){
        ++end;
    }
    assert(from != end);
    to.assign(from,end);
}
////////////////////////////////////////////////////////////////////////////////////////
//safe_implicit_value_converter
////////////////////////////////////////////////////////////////////////////////////////
bool safe_implicit_value_converter::convert(const std::string& from, std::wstring* to){
    if(to){
        utf_converter::u8_to_ws(from,*to);
    }
    return true;
}
bool safe_implicit_value_converter::convert(const std::wstring& from, std::string* to){
    if(to){
        utf_converter::ws_to_u8(from,*to);
    }
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
//implicit_value_converter
////////////////////////////////////////////////////////////////////////////////////
bool implicit_value_converter::convert(const std::string& from, std::wstring* to){
    if(to){
        utf_converter::u8_to_ws(from,*to);
    }
    return true;
}
bool implicit_value_converter::convert(const std::wstring& from, std::string* to){
    if(to){
        utf_converter::ws_to_u8(from,*to);
    }
    return true;
}
}// namespace detail

}// namespace universal_access;
#if defined(_MSC_VER)
#pragma warning( pop )
#endif
#endif /* PUBLIC_UA_SRC_H_ */
