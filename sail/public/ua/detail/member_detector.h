/*
 * member_detector.h
 *
 *  Created on: Jul 22, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_MEMBER_DETECTOR_H_
#define PUBLIC_UA_MEMBER_DETECTOR_H_
#include "ua/macros.h"

/// @cond HIDE
//NOTE: Visual studio 2015 and earlier does not support expression SFINAE
#if defined(_MSC_VER)
#define UA_HAS_EXPRESSION_SFINAE_SUPPORT     0
#else
#define UA_HAS_EXPRESSION_SFINAE_SUPPORT     1
#endif
#define UA_DECLARE_NESTED_TYPE_EXTRACTOR_DEFAULT(tag, nested)                   \
    template<typename UA_T>                                                     \
        struct tag{                                                             \
        typedef char yes[1];                                                    \
        typedef char no [2];                                                    \
        template<typename UA_V> struct holder{};                                \
        template<typename UA_U> static yes&                                     \
        chk(const holder<typename UA_U::nested>*);                              \
        template<typename UA_U> static no& chk(...);                            \
        static bool const exists = sizeof(chk<UA_T>(0)) == sizeof(yes);         \
        struct invalid_type;                                                    \
        template<bool UA_B, typename UA_U>                                      \
        struct extractor{typedef invalid_type result;};                         \
        template<typename UA_U>                                                 \
        struct extractor<true, UA_U>{                                           \
            typedef typename UA_U::nested result;                               \
        };                                                                      \
        typedef typename extractor<exists, UA_T>::result type;                  \
    }

#define UA_PARAMS(...) <__VA_ARGS__>

#define UA_DECLARE_METHOD_DETECTOR_DEFAULT(tag, ret, func, params)              \
template<typename UA_T>                                                         \
struct tag{                                                                     \
   typedef char yes[1];                                                         \
   typedef char no[2];                                                          \
   template<typename UA_C>                                                      \
   static yes& match(ret (UA_C::*) params);                                     \
   template<typename UA_C>                                                      \
   static decltype(match(&UA_C::func)) chk(const UA_C* m);                      \
   static no& chk(...);                                                         \
   static bool const exists = sizeof(chk((UA_T*)(0)))==sizeof(yes);             \
}

#define UA_DECLARE_TEMPLATE_METHOD_DETECTOR_DEFAULT(tag,ret,func,args, params)  \
template<typename UA_T>                                                         \
struct tag{                                                                     \
   typedef char yes[1];                                                         \
   typedef char no[2];                                                          \
   template<typename UA_C>                                                      \
   static yes& match(ret(UA_C::*)params);                                       \
   template<typename UA_C>                                                      \
   static decltype(match(&UA_C::template func UA_PARAMS args))                  \
   chk(const UA_C* m);                                                          \
   static no& chk(...);                                                         \
   static bool const exists = sizeof(chk((UA_T*)(0)))==sizeof(yes);             \
}

#define UA_DECLARE_STATIC_METHOD_DETECTOR_DEFAULT(tag,ret, func, params)        \
template<typename UA_T>                                                         \
struct tag{                                                                     \
   typedef char yes[1];                                                         \
   typedef char no[2];                                                          \
   static yes& match(ret(*)params);                                             \
   template<typename UA_C>                                                      \
   static decltype(match(&UA_C::func)) chk(const UA_C* m);                      \
   static no& chk(...);                                                         \
   static bool const exists = sizeof(chk((UA_T*)(0)))==sizeof(yes);             \
}
/*
#define UA_DECLARE_METHOD_DETECTOR_FALLBACK(tag,ret,func, params)               \
template<typename UA_T>                                                         \
class tag{                                                                      \
    static const bool is_class = std::is_class<UA_T>::value;                    \
    template<typename UA_U, bool UA_E>                                          \
    struct match{                                                               \
        static const bool result = false;                                       \
        };                                                                      \
    template<typename UA_U>                                                     \
    struct match<UA_U, true>{                                                   \
        struct base{ base* func;};                                              \
        struct derived:UA_U,base{};                                             \
        static int chk(ret(derived::*)params);                                  \
        static char chk(...);                                                   \
        static const bool result = sizeof(char) != sizeof(chk(&derived::func)); \
        };                                                                      \
public:                                                                         \
   static const bool exists = match<UA_T,is_class>::result;                     \
}
*/
#define UA_DECLARE_METHOD_DETECTOR_FALLBACK(tag, ret, func, params)             \
template<typename UA_T>                                                         \
class tag{                                                                      \
    typedef char yes[1];                                                        \
    typedef char no[2];                                                         \
    template<typename UA_U, UA_U> struct match;                                 \
    template<typename UA_U, bool UA_E>                                          \
    struct match_name{                                                          \
        static const bool result = false;                                       \
    };                                                                          \
    template<typename UA_U>                                                     \
    struct match_name<UA_U, true>{                                              \
    struct dummy;                                                               \
    struct fallback{ dummy& func(dummy&); };                                    \
    struct derived :public UA_T, public fallback{};                             \
    template<typename UA_C>                                                     \
    static no& chk(const match<dummy& (fallback::*)(dummy&), &UA_C::func>*);    \
    template<typename UA_C>                                                     \
    static yes& chk(...);                                                       \
    static const bool result = sizeof(yes) == sizeof(chk<derived>(0));          \
    };                                                                          \
    static const bool is_compound = std::is_class<UA_T>::value;                 \
    static const bool name_exists = match_name<UA_T, is_compound>::result;      \
    template<typename UA_U, bool UA_E>                                          \
    struct match_sig{                                                           \
        static const bool result = false;                                       \
    };                                                                          \
    template<typename UA_U>                                                     \
    struct match_sig < UA_U, true > {                                           \
        struct derived :public UA_U{                                            \
            using UA_U::func;                                                   \
        };                                                                      \
        template<typename UA_C>                                                 \
        static yes& chk(const match<ret (UA_C::*) params, &UA_C::func>*);       \
        template<typename UA_C>                                                 \
        static no& chk(...);                                                    \
        static const bool result = sizeof(yes) == sizeof(chk<derived>(0));      \
    };                                                                          \
    static const bool sig_exists = match_sig<UA_T, name_exists>::result;        \
    template<typename UA_C, bool UA_E>                                          \
    struct verify_sig{                                                          \
       static const bool result = false;                                        \
    };                                                                          \
    template<typename UA_C>                                                     \
    struct verify_sig < UA_C, true > {                                          \
        static int verify(ret (UA_C::*) params);                                \
        static const bool result = sizeof(verify(&UA_C::func))>0;               \
    };                                                                          \
public:                                                                         \
    static const bool exists = verify_sig<UA_T, sig_exists>::result;            \
}
/*
#define UA_DECLARE_TEMPLATE_METHOD_DETECTOR_FALLBACK(tag,ret,func,args, params) \
template<typename UA_T>                                                         \
class tag{                                                                      \
    static const bool is_class = std::is_class<UA_T>::value;                    \
    template<typename UA_U, bool UA_E>                                          \
    struct match{                                                               \
        static const bool result = false;                                       \
    };                                                                          \
    template<typename UA_U>                                                     \
    struct match<UA_U, true>{                                                   \
        struct base{ base* func;};                                              \
        struct derived:UA_U,base{};                                             \
        template<typename UA_C>                                                 \
        static decltype((ret(UA_C::*)params)(&UA_C::func)) chk(UA_C*);          \
        static char chk(...);                                                   \
        static const bool result = sizeof(char) != sizeof(chk((derived*)0));    \
    };                                                                          \
public:                                                                         \
   static const bool exists = match<UA_T,is_class>::result;                     \
}
*/
#define UA_DECLARE_TEMPLATE_METHOD_DETECTOR_FALLBACK(tag,ret,func,args, params) \
template<typename UA_T>                                                         \
class tag{                                                                      \
    typedef char yes[1];                                                        \
    typedef char no[2];                                                         \
    template<typename UA_U, UA_U> struct match;                                 \
    template<typename UA_C>                                                     \
    static yes& chk(const match<ret (UA_C::*)params, &UA_C::func>*);            \
    template<typename UA_C>                                                     \
    static no& chk(...);                                                        \
public:                                                                         \
    static const bool exists = sizeof(yes) == sizeof(chk<UA_T>(0));             \
}

#define UA_DECLARE_STATIC_METHOD_DETECTOR_FALLBACK(tag, ret, func, params)      \
template<typename UA_T>                                                         \
class tag{                                                                      \
    typedef char yes[1];                                                        \
    typedef char no[2];                                                         \
    template<typename UA_U, UA_U> struct match;                                 \
    template<typename UA_U, bool UA_E>                                          \
    struct match_name{                                                          \
        static const bool result = false;                                       \
    };                                                                          \
    template<typename UA_U>                                                     \
    struct match_name<UA_U, true>{                                              \
    struct dummy;                                                               \
    struct fallback{ static dummy& func(dummy&); };                             \
    struct derived :public UA_T, public fallback{};                             \
    template<typename UA_C>                                                     \
    static no& chk(const match<dummy& (*)(dummy&), &UA_C::func>*);              \
    template<typename UA_C>                                                     \
    static yes& chk(...);                                                       \
    static const bool result = sizeof(yes) == sizeof(chk<derived>(0));          \
    };                                                                          \
    static const bool is_compound = std::is_compound<UA_T>::value;              \
    static const bool name_exists = match_name<UA_T, is_compound>::result;      \
    template<typename UA_U, bool UA_E>                                          \
    struct match_sig{                                                           \
        static const bool result = false;                                       \
    };                                                                          \
    template<typename UA_U>                                                     \
    struct match_sig < UA_U, true > {                                           \
        struct derived :public UA_U{                                            \
            using UA_U::func;                                                   \
        };                                                                      \
        template<typename UA_C>                                                 \
        static yes& chk(const match<ret (*) params, &UA_C::func>*);             \
        template<typename UA_C>                                                 \
        static no& chk(...);                                                    \
        static const bool result = sizeof(yes) == sizeof(chk<derived>(0));      \
    };                                                                          \
    static const bool sig_exists = match_sig<UA_T, name_exists>::result;        \
    template<typename UA_C, bool UA_E>                                          \
    struct verify_sig{                                                          \
       static const bool result = false;                                        \
    };                                                                          \
    template<typename UA_C>                                                     \
    struct verify_sig < UA_C, true > {                                          \
        static int verify(ret (*) params);                                      \
        static const bool result = sizeof(verify(&UA_C::func))>0;               \
    };                                                                          \
public:                                                                         \
    static const bool exists = verify_sig<UA_T, sig_exists>::result;            \
}

#if UA_HAS_EXPRESSION_SFINAE_SUPPORT == 1
#define UA_DECLARE_NESTED_TYPE_EXTRACTOR_ UA_DECLARE_NESTED_TYPE_EXTRACTOR_DEFAULT
#define UA_DECLARE_METHOD_DETECTOR_ UA_DECLARE_METHOD_DETECTOR_DEFAULT
#define UA_DECLARE_TEMPLATE_METHOD_DETECTOR_ UA_DECLARE_TEMPLATE_METHOD_DETECTOR_DEFAULT
#define UA_DECLARE_STATIC_METHOD_DETECTOR_ UA_DECLARE_STATIC_METHOD_DETECTOR_DEFAULT
#else
#define UA_DECLARE_NESTED_TYPE_EXTRACTOR_ UA_DECLARE_NESTED_TYPE_EXTRACTOR_DEFAULT
#define UA_DECLARE_METHOD_DETECTOR_ UA_DECLARE_METHOD_DETECTOR_FALLBACK
#define UA_DECLARE_TEMPLATE_METHOD_DETECTOR_ UA_DECLARE_TEMPLATE_METHOD_DETECTOR_FALLBACK
#define UA_DECLARE_STATIC_METHOD_DETECTOR_ UA_DECLARE_STATIC_METHOD_DETECTOR_FALLBACK
#endif // UA_HAS_EXPRESSION_SFINAE_SUPPORT == 1
/// @endcond

/**
 * \def UA_DECLARE_NESTED_TYPE_EXTRACTOR(tag, nested_type)
 * \brief Declares a type extractor that detects the existence of a nested type of a given type.
 * \param tag The name of the extractor.
 * \param nested_type the name of the nested type.
 *
 * This macro expends to a declaration of a template class that detects and extracts a nested type of a
 * given type which is passed in as its template type argument.
 * Following line:
 * \code
 * UA_DECLARE_NESTED_TYPE_EXTRACTOR(has_value_type, value_type);
 * \endcode
 * expends to following declaration:
 * \code
 * template<typename T>
 * struct has_value_type{
 *  static const bool exists = ... ; //<- true if T::value_type exists and is public.
 *  typedef extracted type;
 *  private:
 *    ... implementation detail omitted ....
 * };
 * \endcode
 * has_value_type::exists will be true only if the given T has a public nested type named 'value_type'.
 * has_value_type::type will be the same type as T::value_type if T::value_type exists; If T::value_type
 * does not exist or it is not public, has_value_type::exists is set to false and has_value_type::type will
 * be declared as an unspecified type.
 *
 * Code Example:
 * \snippet tvs_examples.cpp extract_nested_type
 *
 *  <b>Known limitations</b>
 *
 *  Due to limitations of Visual Studio 2013, an compilation failures happens if the nested type exists, but
 *  is private or protected.
 *
 */
#define UA_DECLARE_NESTED_TYPE_EXTRACTOR(tag, nested)\
        UA_DECLARE_NESTED_TYPE_EXTRACTOR_(tag, nested)
/**
 * \def UA_DECLARE_METHOD_DETECTOR(tag, func, sig)
 * \brief Declares a class that detects the existence of a member function of a given type.
 * \param tag the name of the detector class.
 * \param ret return type of the function.
 * \param func the name of the function.
 * \param params the parameter list of function (e.g, (param1, param2)).
 *
 * This macro expends to a declaration of a template class that detects the existence of a matching
 * public instance member function of a given type which is passed in as its template type argument.
 * The method can be overloaded and can be inherited from base classes.  Only none template none
 * static member functions can be detected.
 *  Following line:
 *  \code
 *  UA_DECLARE_METHOD_DETECTOR(has_push_back, void, push_back, (const int&));
 *  \endcode
 *  expends to following declaration:
 *  \code
 *  template<typename T>
 *  struct has_push_back{
 *      static const bool exists = ...; //<- true if void T::push_back(const int&) exists and is public.
 *  private:
 *     ... implementation details omitted ...
 *  };
 *  \endcode
 * has_push_back::exists will be true only if the given T has a public instance method called 'push_back'
 * whose signature is void(T::*)(const int&).
 *
 * Code Example:
 * \snippet tvs_examples.cpp detect_methods
 *
 * <b>Known limitations</b>
 * Due to limitations of Visual Studio 2013( 2015 as well), an compilation failure happens if the method
 * with matching signature is private or protected.
 *
 * \sa UA_DECLARE_TEMPLATE_METHOD_DETECTOR(tag, func, param, sig)
 * \sa UA_DECLARE_STATIC_METHOD_DETECTOR(tag, func, param, sig)
 */
#define UA_DECLARE_METHOD_DETECTOR(tag, ret, func, params)\
        UA_DECLARE_METHOD_DETECTOR_(tag, ret, func, params)
/**
 * \def UA_DECLARE_TEMPLATE_METHOD_DETECTOR(tag, ret, func, args, params)
 * \brief Declares a class that detects the existence of a template member function of a given type.
 * \param tag the name of the detector class
 * \param ret the return value of the template function
 * \param func the name of the template function
 * \param args the type argument list of the template function.
 * \param params the parameter list of the template function.
 *
 * Similar to UA_DECLARE_METHOD_DETECTOR(tag, ret, func, params) but works
 * on template member functions. You have to explicitly specify the template
 * argument list as the 4th parameter.
 *
 * Code Example:
 * \snippet tvs_examples.cpp detect_template_methods
 *
 * <b>Known limitations</b>
 *
 * Due to the limitations of the Visual Studio 2013 ( 2015 as well), only template methods declared directly
 * in T can be detected; template methods inherited from base classes of T will not be detected.
 */
#define UA_DECLARE_TEMPLATE_METHOD_DETECTOR(tag, ret, func, args, params)\
        UA_DECLARE_TEMPLATE_METHOD_DETECTOR_(tag, ret, func, args, params)
/**
 * \def UA_DECLARE_STATIC_METHOD_DETECTOR(tag, ret, func, params)
 * \brief Declares a class that detects the existence of a static member function of a given type.
 *
 * Has the same behavior and limitations as UA_DECLARE_METHOD_DETECTOR(tag, ret, func, params) but
 * works on static member functions.
 *
 */
#define UA_DECLARE_STATIC_METHOD_DETECTOR(tag, ret, func, params)\
        UA_DECLARE_STATIC_METHOD_DETECTOR_(tag, ret, func, params)


#endif /* PUBLIC_UA_MEMBER_DETECTOR_H_ */
