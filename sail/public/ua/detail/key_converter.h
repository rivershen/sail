/*
 * key_converter.h
 *
 *  Created on: Aug 10, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_KEY_CONVERTER_H_
#define PUBLIC_UA_DETAIL_KEY_CONVERTER_H_
#include "utf_converter.h"
#include "static_meta_value.h"
#include <cassert>
namespace sail{
template<typename T, typename = void>
struct key_converter{
};

struct generic_key_receiver:public key_receiver{
    using key_receiver::set;
    template<typename T>
    bool set(const T& v){
        return (set(static_meta_value::obtain<T>(), &v) || key_converter<T>::copy_to(&v, *this));
    }
};

// specialization for std::string
template<>
struct key_converter<std::string,void> : public generic_key_receiver{
    using generic_key_receiver::set;
    key_converter():_result(nullptr){}
    bool set(const std::string&v)override{
        _result = &v;
        _lastError = bind_status::SUCCESS;
        return true;
    }
    bool set(const std::wstring& v) override{
        _result = &_string;
        _lastError = detail::utf_converter::ws_to_u8(v, _string);
        return _lastError == bind_status::SUCCESS;
    }
    bool set(const char* v)override{
        assert(v);
        _result = &_string;
        _string.assign(v);
        _lastError = bind_status::SUCCESS;
        return true;
    }
    bool set(const wchar_t* v)override{
        assert(v);
        _result = &_string;
        _lastError = detail::utf_converter::ws_to_u8(v, _string);
        return _lastError == bind_status::SUCCESS;
    }
    const std::string& get()const{
        assert(_lastError == bind_status::SUCCESS);
        assert(_result);
        return *_result;
    }
    bool set_copy(std::string v) override{
        _string.swap(v);
        _result = &_string;
       _lastError = bind_status::SUCCESS;
       return true;
    }
private:
   std::string _string;
   const std::string* _result;
};

// specialization for std::wstring
template<>
struct key_converter<std::wstring, void> : public generic_key_receiver{
    using generic_key_receiver::set;
    key_converter():_result(nullptr){}
    bool set(const std::string&v)override{
        _result = &_string;
        _lastError = detail::utf_converter::u8_to_ws(v, _string);
        return _lastError == bind_status::SUCCESS;
    }
    bool set(const std::wstring& v) override{
        _result = &v;
        _lastError = bind_status::SUCCESS;
        return true;
    }
    bool set(const char* v)override{
        assert(v);
        _result = &_string;
        _lastError = detail::utf_converter::u8_to_ws(v, _string);
        return _lastError == bind_status::SUCCESS;
    }
    bool set(const wchar_t* v)override{
        assert(v);
        _result = &_string;
        _string.assign(v);
        _lastError = bind_status::SUCCESS;
        return true;
    }
    const std::wstring& get()const{
        assert(_lastError == bind_status::SUCCESS);
        assert(_result);
        return *_result;
    }
    bool set_copy(std::wstring v) override{
        _string.swap(v);
        _result = &_string;
       _lastError = bind_status::SUCCESS;
       return true;
    }
private:
   std::wstring _string;
   const std::wstring* _result;
};

// specialization for const char*
template<>
struct key_converter<const char*, void> : public generic_key_receiver{
    using generic_key_receiver::set;
    key_converter():_result(nullptr){}
    bool set(const std::string&v)override{
        _result = v.c_str();
        _lastError = bind_status::SUCCESS;
        return true;
    }
    bool set(const std::wstring& v) override{
        _lastError = detail::utf_converter::ws_to_u8(v, _string);
        _result = _string.c_str();
        return _lastError == bind_status::SUCCESS;
    }
    bool set(const char* v)override{
        assert(v);
        _result = v;
        _lastError = bind_status::SUCCESS;
        return true;
    }
    bool set(const wchar_t* v)override{
        assert(v);
        _lastError = detail::utf_converter::ws_to_u8(v, _string);
        _result = _string.c_str();
        return _lastError == bind_status::SUCCESS;
    }
    const char* get()const{
        assert(_lastError == bind_status::SUCCESS);
        assert(_result);
        return _result;
    }
    bool set_copy(std::string v) override{
        _string.swap(v);
        _result = _string.c_str();
       _lastError = bind_status::SUCCESS;
       return true;
    }
private:
    std::string _string;
    const char* _result;
};

//speicalization for const wchar*
template<>
struct key_converter<const wchar_t*, void> : public generic_key_receiver{
    using generic_key_receiver::set;
    key_converter():_result(nullptr){}
     bool set(const std::string&v)override{
           _lastError = detail::utf_converter::u8_to_ws(v, _string);
           _result = _string.c_str();
           return _lastError == bind_status::SUCCESS;
     }
     bool set(const std::wstring& v) override{
         _lastError = bind_status::SUCCESS;
         _result = v.c_str();
         return true;
     }
     bool set(const char* v)override{
         assert(v);
         _lastError = detail::utf_converter::u8_to_ws(v, _string);
         _result = _string.c_str();
         return _lastError == bind_status::SUCCESS;
     }
     bool set(const wchar_t* v)override{
         assert(v);
         _lastError = bind_status::SUCCESS;
         _result = v;
         return true;
     }
     const wchar_t* get()const{
         assert(_lastError == bind_status::SUCCESS);
         assert(_result);
         return _result;
     }
    bool set_copy(std::wstring v) override{
        _string.swap(v);
        _result = _string.c_str();
       _lastError = bind_status::SUCCESS;
       return true;
    }
private:
     std::wstring _string;
     const wchar_t* _result;
};

// speicalization for enum
template<typename T>
struct key_converter<T,
typename std::enable_if<is_native_enum<T>::value && sizeof(T)<=4>::type> : public generic_key_receiver{
    using generic_key_receiver::set;
    typedef typename std::add_const<T>::type const_t;

     bool set(const std::string&v)override{
         _result = enums::from_string<T>(v, &_lastError);
         return _lastError == bind_status::SUCCESS;
     }
     bool set(const std::wstring& v) override{
         _string.assign(v.begin(), v.end());
         _result = enums::from_string<T>(_string, &_lastError);
         return _lastError == bind_status::SUCCESS;
     }
     bool set(const char* v)override{
         assert(v);
         _string.assign(v);
         _result = enums::from_string<T>(_string, &_lastError);
         return _lastError == bind_status::SUCCESS;
     }
     bool set(const wchar_t* v)override{
         _string.assign(v, v+::wcslen(v));
         _result = enums::from_string<T>(_string, &_lastError);
         return _lastError == bind_status::SUCCESS;
     }
     bool set(const meta_value& meta, const void* v)override{
         if(&meta == &static_meta_value::obtain<T>()){
             _result = *reinterpret_cast<const_t*>(v);
             _lastError = bind_status::SUCCESS;
         }else{
             key_receiver::set(meta, v);
         }
         return _lastError == bind_status::SUCCESS;
     }
     T get()const{
         assert(_lastError == bind_status::SUCCESS);
         return _result;
     }
     static bool copy_to(const void* from, key_receiver& to){
         T v = *reinterpret_cast<const_t*>(from);
         bind_status status;
         const std::string& name = enums::to_string(v, &status);
         if(status == bind_status::SUCCESS){
             return to.set(name);
         }else{
             return false;
         }
     }
private:
    std::string _string;
    T _result;
};

}//namespace universal_access;




#endif /* PUBLIC_UA_DETAIL_KEY_CONVERTER_H_ */
