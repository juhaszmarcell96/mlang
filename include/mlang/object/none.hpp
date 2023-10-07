#pragma once

#include <string>

#include "mlang/object/object.hpp"

namespace mlang {

class None : public Object {
public:
    None () : Object(false) {}
    ~None () = default;
    
    const static inline std::string type_name { "None" };

    /* construct */
    void construct (const std::vector<Object*>& params) override { }
    /* assign */
    void assign (const std::vector<Object*>& params) override { }
    /* destruct */
    void destruct () override { }
    void assign (const Object* param) override { }

    bool is_true () const override {
        return false;
    }

    void call (const std::string& func, const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) override {
        throw RuntimeError { "object of type '" + type_name + "' has no member functions" };
    }

    std::string get_string () const override { return type_name; }
    std::string get_typename () const override { return type_name; }
};

class NoneFactory : public ObjectFactory {
public:
    std::shared_ptr<Object> create () override {
        return std::make_shared<None>();
    }
};

} /* namespace mlang */