/*
 * container_trait.h
 *
 *  Created on: Aug 11, 2015
 *      Author: chushen
 */

#ifndef PUBLIC_UA_DETAIL_CONTAINER_TRAIT_H_
#define PUBLIC_UA_DETAIL_CONTAINER_TRAIT_H_

#include "ua/detail/member_detector.h"

namespace sail{ namespace detail{
template<typename T>
class container_trait {
    UA_DECLARE_NESTED_TYPE_EXTRACTOR(extract_value_type, value_type);
    typedef typename extract_value_type<T>::type value_type;

    UA_DECLARE_NESTED_TYPE_EXTRACTOR(extract_iterator, iterator);
    typedef typename extract_iterator<T>::type iterator;

    UA_DECLARE_NESTED_TYPE_EXTRACTOR(extract_const_iterator, const_iterator);
    typedef typename extract_const_iterator<T>::type const_iterator;

    UA_DECLARE_NESTED_TYPE_EXTRACTOR(extract_mapped_type, mapped_type);
    typedef typename extract_mapped_type<T>::type mapped_type;

    UA_DECLARE_NESTED_TYPE_EXTRACTOR(extract_key_type, key_type);
    typedef typename extract_key_type<T>::type key_type;

    UA_DECLARE_METHOD_DETECTOR(has_begin, iterator, begin, ());
    UA_DECLARE_METHOD_DETECTOR(has_begin_const, const_iterator, begin, ()const);
    UA_DECLARE_METHOD_DETECTOR(has_end, iterator, end, ());
    UA_DECLARE_METHOD_DETECTOR(has_end_const, const_iterator, end, ()const);
    UA_DECLARE_METHOD_DETECTOR(has_array_subscription_op_const, const value_type&, operator[], (size_t) const);
    UA_DECLARE_METHOD_DETECTOR(has_find_const, const_iterator, find, (const key_type&)const);
public:
    /**
     * \brief true, if the type argument T complies to the container concept.
     *
     * A type complies to the container concept if it has following member types and
     * functions declared:
     * \code
     *  concept container {
     *      type value_type;
     *      type iterator;
     *      type const_iterator;
     *      method iterator begin();
     *      method iterator end();
     *  }
     * \endcode
     */
    static const bool is_container = extract_value_type<T>::exists
    && extract_iterator<T>::exists
    && has_begin_const<T>::exists
    && has_end_const<T>::exists;

    /**
     * \brief true, if the type argument T complies to the map concept.
     *
     * A type complies to the map concept if it has following member types and functions
     * declared:
     * \code
     * concept map extends container{
     *      type mapped_type;
     *      type key_type;
     * }
     * \endcode
     */
    static const bool is_map = is_container
    && extract_mapped_type<T>::exists
    && extract_key_type<T>::exists;
    /**
     * \brief true, if the type argument T complies to the set concept.
     *
     * A type complies to the set concept if the has following member types and functions
     * declared:
     * \code
     * concept set extends container{
     *      type key_type == value_type;
     * }
     * \endcode
     */
    static const bool is_set = is_container
    && extract_key_type<T>::exists
    && std::is_same<value_type, key_type>::value;

    /**
     * \brief true, if T is a container but is not a map or list.
     */
    static const bool is_list = is_container
            && !extract_key_type<T>::exists
            && !extract_mapped_type<T>::exists;
    /**
     * \brief true, if T has operator[ size_t ] const defined.
     */
    static const bool has_array_subscription = has_array_subscription_op_const<T>::exists;

    /**
     * \brief true, if T has the find method with this signature: const_iterator find(const key_type&)const
     */
    static const bool has_find = has_find_const<T>::exists;
};

}} // namespace




#endif /* PUBLIC_UA_DETAIL_CONTAINER_TRAIT_H_ */
