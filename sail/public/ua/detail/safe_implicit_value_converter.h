/*
 * safe_implicit_value_converter.h
 *
 *  Created on: Aug 12, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_SAFE_IMPLICIT_VALUE_CONVERTER_H_
#define PUBLIC_UA_DETAIL_SAFE_IMPLICIT_VALUE_CONVERTER_H_
#include <type_traits>
namespace sail{ namespace detail{
/**
 * \brief A type converter that follows C/C++ implicit conversion rule (With some exceptions).
 *
 * Exceptions to the C/C++ implicit conversion rule:
 * - Arithmetic value conversion is based on the actual value not by its static type. For example,
 *   trying to convert an int to short int will succeed if the int value falls into the valid
 *   range of short int.
 * conversion will occur).
 */
struct safe_implicit_value_converter{
    template<typename T>
    static bool convert(const T& from, T* to){
        if(to){
            *to = from;
        }
        return true;
    }
    template<typename T1, typename T2>
    static bool convert(const T1& from, T2* to){
        return _convert(from,to);
    }
    static bool convert(const std::string& from, std::wstring* to);
    static bool convert(const std::wstring& from, std::string* to);
private:
    template<typename T>
    static typename std::enable_if< std::is_integral<T>::value, bool>::type
    _convert(const bool& from, T* to){
          if(to){
              *to = (T)from;
          }
          return true;
    }
    template<typename T>
    static typename std::enable_if< std::is_integral<T>::value, bool>::type
    _convert(const T& from, bool* to){
        if(to){
            *to = (from != 0);
        }
        return true;
    }
    template<typename T1, typename T2>
    static typename std::enable_if<
        std::is_integral<T1>::value && std::is_integral<T2>::value && std::is_convertible<T1, T2>::value
    , bool>::type
    _convert(const T1& from, T2* to){
        T2 tmp = (T2) from;
        if((T1)tmp == from && !((from < 0) ^ (tmp < 0))){
            if(to){
                *to = tmp;
            }
            return true;
        }
        return false;
    }
    template<typename T1, typename T2>
    static typename std::enable_if<!std::is_integral<T2>::value&& std::is_convertible<T1,T2>::value,
    bool>::type _convert(const T1& from, T2* to){
            if(to){
                *to = (T2)from;
            }
            return true;
    }
    template<typename T1, typename T2>
    static typename std::enable_if<std::is_floating_point<T1>::value && std::is_integral<T2>::value,
    bool>::type _convert(const T1& from, T2* to){
           if(from >=0){
               return _convert((uint64_t)from, to);
           }else{
               return _convert((int64_t)from, to);
           }
    }
    template<typename ... args>
    static bool _convert(args ...){
        return false;
    }
};
struct implicit_value_converter{
    template<typename T>
    static bool convert(const T& from, T* to){
        if(to){
            *to = from;
        }
        return true;
    }
    template<typename T1, typename T2>
    static typename std::enable_if<std::is_convertible<T1,T2>::value,bool>::type
    convert(const T1& from, T2* to){
        return _convert(from, to);
    }
    static bool convert(const std::string& from, std::wstring* to);
    static bool convert(const std::wstring& from, std::string* to);
    template<typename ... args>
    static bool convert(args ...){
        return false;
    }
private:
    template<typename T1, typename T2>
    static bool _convert(const T1&from, T2* to){
        if (to){
            *to = (T2)from;
        }
        return true;
    }
    template<typename T>
    static bool _convert(const bool& from, T*to){
        if (to){
            *to = from ? (T)1 : (T)0;
        }
        return true;
    }
    template<typename T>
    static bool _convert(const T& from, bool*to){
        if (to){
            *to = (0 != from);
        }
        return true;
    }

};

}}//namespace
#endif /* PUBLIC_UA_DETAIL_SAFE_IMPLICIT_VALUE_CONVERTER_H_ */
