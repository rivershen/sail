/*
 * jsoncpp_meta.h
 *
 *  Created on: Sep 2, 2015
 *      Author: chushen
 */

#ifndef UNITTESTS_UA_SRC_JSONCPP_SUPPORT_H_
#define UNITTESTS_UA_SRC_JSONCPP_SUPPORT_H_
#include "ua/ua.h"
#include "json/json.h"
namespace sail{
template<>
struct static_meta_value_impl<Json::Value> :public meta_value,list_accessor, map_accessor{
    static const meta_value& get(){
        static static_meta_value_impl<Json::Value> instance;
        return instance;
    }
    virtual bind_status accept(const void* value, value_visitor& visitor) const override{
        using namespace Json;
        const Value* v = (const Value*)value;
        switch(v->type()){
            case intValue:{
                int64_t i = v->asInt64();
                visitor.visit(&i);
                break;
            }
            case uintValue:{
                uint64_t ui = v->asUInt64();
                visitor.visit(&ui);
                break;
            }
            case realValue:{
                double d = v->asDouble();
                visitor.visit(&d);
                break;
            }
            case stringValue:{
                std::string s = v->asString();
                visitor.visit(&s);
                break;
            }
            case booleanValue:{
                bool b = v->asBool();
                visitor.visit(&b);
                break;
            }
            case objectValue:
            case arrayValue:{
                visitor.visit(*this, value);
                break;
            }
            case nullValue:{
                value_ref empty_ref;
                visitor.visit(empty_ref.get_meta(), empty_ref.get_address());
                break;
            }
        }
        return visitor.get_last_error();
    }
    virtual const map_extractor* to_forward_map(const void* value) const override{
        return to_map(value);
    }
    virtual const map_accessor* to_map(const void* value) const override{
        const Json::Value* v = (const Json::Value*)value;
        if(!v->isNull()&& v->isObject()){
            return this;
        }
        return nullptr;
    }
    virtual const list_extractor* to_forward_list(const void* value) const override{
        return to_list(value);
    }
    virtual const list_accessor* to_list(const void* value) const override{
        const Json::Value* v = (const Json::Value*)value;
        if(!v->isNull()&& v->isArray()){
            return this;
        }
        return nullptr;
    }
    virtual const std::type_info& get_type_info(const void*) const override{
        return typeid(Json::Value);
    }
    virtual bool is_valid(const void*) const override{
        return true;
    }
    virtual bool is_empty(const void* value) const override{
        const Json::Value* v = (const Json::Value*)value;
        return v->empty();
    }
    virtual size_t get_size_hint(const void* v) const override{
        return size_of(v);
    }
    virtual const meta_value& get_meta_type() const override{
        return *this;
    }
    virtual const meta_value* get_value_meta_type(const void* /*v*/) const override{
        return nullptr;
    }
    virtual int for_each_element_of(const void* value, for_each_element_cb callback,
            void* context) const override{
        const Json::Value* v = (const Json::Value*)value;
        value_ref item;
        int result = 0;
        for(auto itr = v->begin(); itr != v->end(); ++itr){
           static_meta_value::reset_ref(item,*this,&(*itr));
           result = callback(item, context);
           if(result){
               break;
           }
        }
        return result;
    }
    virtual size_t size_of(const void* value) const override{
        const Json::Value* v = (const Json::Value*)value;
        return v->size();
    }
    virtual value_ref get_element_at(const void* value, size_t index) const override{
        const Json::Value* v = (const Json::Value*)value;
        if(index < v->size()){
            return static_meta_value::make_ref(*this, &(v->operator[]((int)index)));
        }
        return value_ref();
    }
    virtual int for_each_field_of(const void* value, for_each_field_cb cb,
            key_receiver& receiver, void* context) const override{
        const Json::Value* v = (const Json::Value*)value;
        value_ref item;
        int result = 0;
        for(auto itr = v->begin(); itr != v->end(); ++itr){
            static_meta_value::reset_ref(item,*this,&(*itr));
            receiver.set(itr.memberName());
            result = cb(item,context);
            if(result){
                break;
            }
        }
        return result;
    }
    virtual void run_with_key_receiver(const void* /*item*/, with_key_cb cb, void* context) const override{
        key_converter<std::string> converter;
        cb(converter, context);
    }
    virtual bool exists(const void* value, key_receiver& receiver) const override{
        const Json::Value* v = (const Json::Value*)value;
        key_converter<std::string>& converter = (key_converter<std::string>&)receiver;
        return v->isMember(converter.get());
    }
    virtual value_ref get_field_at(const void* value, key_receiver& receiver) const override{
        const Json::Value* v = (const Json::Value*)value;
        key_converter<std::string>& converter = (key_converter<std::string>&)receiver;
        if(v->isMember(converter.get())){
            return static_meta_value::make_ref(*this,&(v->operator[](converter.get())));
        }
        return value_ref();
    }
    virtual bind_status load_from(const value_ref&from, void* to) const override{
        const Json::Value* v = from.peek_typed_address<Json::Value>();
        if(v){
            *static_cast<Json::Value*>(to) = *v;
            return bind_status::SUCCESS;
        }
        return bind_status::OP_NOT_SUPPORTED;
    }
};

}



#endif /* UNITTESTS_UA_SRC_JSONCPP_SUPPORT_H_*/
