#pragma once

#include <string>

#include "mlang/object/object.hpp"

namespace mlang {

class None : public Object {
public:
    None () : Object(false) {}
    ~None () = default;

    /* construct */
    void construct (const std::vector<Object*>& params) override { }
    /* assign */
    Object* assign (const std::vector<Object*>& params) override { return nullptr; }
    /* destruct */
    void destruct () override { }
    void assign (const Object* param) override { }


    void call (const std::string& func, const std::vector<Object*>& params, std::shared_ptr<Object>& ret_val) override {
        throw RuntimeError { "object of type 'none' has no member functions" };
    }

    std::string get_string () const override { return "none"; }
    std::string get_typename () const override { return "none"; }
};

class NoneFactory : public ObjectFactory {
public:
    std::shared_ptr<Object> create () override {
        return std::make_shared<None>();
    }
};

} /* namespace mlang */