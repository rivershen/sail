/*
 * ua_fwd.h
 *
 *  Created on: May 7, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_NATIVE_H_
#define PUBLIC_UA_NATIVE_H_
/////////////////////////////////////////////////////////////////////////////
//The UA_FOR_EACH()macro is implemented using the recursive macro tricks
//explained here:
// http://stackoverflow.com/questions/12447557/can-we-have-recursive-macros
//Some tweaks are made to workaround Visual C++ preprocessor limitations.
/////////////////////////////////////////////////////////////////////////////
#define UA_PRIMITIVE_CAT(a, ...) a ## __VA_ARGS__
#define UA_IIF(c) UA_PRIMITIVE_CAT(UA_IIF_, c)
#define UA_IIF_0(t, ...) __VA_ARGS__
#define UA_IIF_1(t, ...) t
#define UA_CHECK_N(x, n, ...) n
#define _UA_CHECK_N(args) UA_CHECK_N args
#define UA_CHECK(...) _UA_CHECK_N((__VA_ARGS__, 0,))
#define UA_PROBE(x) x, 1,
#define UA_IS_PAREN(x) UA_CHECK(UA_IS_PAREN_PROBE x)
#define UA_IS_PAREN_PROBE(...) UA_PROBE(~)
#define UA_EMPTY()
#define UA_DEFER(id) id UA_EMPTY()
#define UA_OBSTRUCT(...) __VA_ARGS__ UA_DEFER(UA_EMPTY)()

#define UA_EVAL(...)  UA_EVAL1(UA_EVAL1(UA_EVAL1(__VA_ARGS__)))
#define UA_EVAL1(...) UA_EVAL2(UA_EVAL2(UA_EVAL2(__VA_ARGS__)))
#define UA_EVAL2(...) UA_EVAL3(UA_EVAL3(UA_EVAL3(__VA_ARGS__)))
#define UA_EVAL3(...) UA_EVAL4(UA_EVAL4(UA_EVAL4(__VA_ARGS__)))
#define UA_EVAL4(...) UA_EVAL5(UA_EVAL5(UA_EVAL5(__VA_ARGS__)))
#define UA_EVAL5(...) __VA_ARGS__


#define UA_EACH(macro, p, a, b, c, d,e,f,g,h,i,j,k, ...)\
    UA_IIF(UA_IS_PAREN(a))(,macro(p,a)) UA_IIF(UA_IS_PAREN(b))(,macro(p,b))\
    UA_IIF(UA_IS_PAREN(c))(,macro(p,c)) UA_IIF(UA_IS_PAREN(d))(,macro(p,d))\
    UA_IIF(UA_IS_PAREN(e))(,macro(p,e)) UA_IIF(UA_IS_PAREN(f))(,macro(p,f))\
    UA_IIF(UA_IS_PAREN(g))(,macro(p,g)) UA_IIF(UA_IS_PAREN(h))(,macro(p,h))\
    UA_IIF(UA_IS_PAREN(i))(,macro(p,i)) UA_IIF(UA_IS_PAREN(j))(,macro(p,j))\
    UA_IIF(UA_IS_PAREN(k))(,macro(p,k) UA_OBSTRUCT(_UA_EACH)()(macro,p,__VA_ARGS__))
#define _UA_EACH() UA_EACH

#define UA_FOR_EACH(macro, ...) \
    UA_EVAL(UA_EACH(macro,__VA_ARGS__,(),(),(),(),(),(),(),(),(),(),()))
///////////////////////////////////////////////////////////////////////////////

#define UA_VISIT_FIELD(e, v) visitor.field(#v) e v;
#define UA_EMPTY_TOKEN

#define UA_OBJECT_COMMON(modifier, klass, ...)                          \
    template< typename UA_VISITOR>  modifier UA_VISITOR*                \
    _ua_visit_native_object(klass* o,UA_VISITOR* visitor){              \
        struct wrapper:public klass{                                    \
            void _ua_accept(UA_VISITOR& visitor)const{                  \
                UA_FOR_EACH(UA_VISIT_FIELD, UA_EMPTY_TOKEN,__VA_ARGS__) \
            }                                                           \
        };                                                              \
        ((wrapper*)o)->_ua_accept(*visitor);                            \
        return visitor;                                                 \
    }

#define UA_MAKE_OBJECT(klass, ...)                                      \
                      UA_OBJECT_COMMON(inline, klass, __VA_ARGS__)

#define UA_OBJECT(klass, ...)                                           \
                 UA_OBJECT_COMMON(friend,klass, __VA_ARGS__)

//////////////////////////////////////////////////////////////////////////////
#define UA_VISIT_ENUM(e, v) (*visitor) << #v << e::v;

#define UA_ENUM_COMMON(modifier, name, ...)                             \
    template<typename UA_VISITOR> modifier UA_VISITOR*                  \
    _ua_visit_native_enum(name n, UA_VISITOR* visitor){                 \
        (void)n;                                                        \
        UA_FOR_EACH(UA_VISIT_ENUM, name, __VA_ARGS__)                   \
        return visitor;                                                 \
    }
#define UA_MAKE_ENUM(name, ...)                                         \
        UA_ENUM_COMMON(inline, name, __VA_ARGS__)

#define UA_ENUM(name, ...)                                              \
    name {__VA_ARGS__};                                                 \
    UA_ENUM_COMMON(inline, name, __VA_ARGS__)

#define UA_NESTED_ENUM(name, ...)                                       \
    name {__VA_ARGS__};                                                 \
    UA_ENUM_COMMON(friend, name, __VA_ARGS__)

#endif /* PUBLIC_UA_NATIVE_H_ */
