#pragma once

#include <string>

#include "mlang/object/internal_object.hpp"

namespace mlang {

class None : public InternalObject {
public:
    None () = default;
    ~None () = default;
    
    const static inline std::string type_name { "None" };

    /* construct */
    //void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override { }
    /* assign */
    //void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override { }
    /* destruct */
   // void destruct () override { }
   // void assign (const std::shared_ptr<InternalObject> param) override { }

    bool is_true () const override { return false; }

    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override {
        throw RuntimeError { "object of type '" + type_name + "' has no member functions" };
    }

    std::string get_string () const override { return type_name; }
    std::string get_typename () const override { return type_name; }
};

class NoneFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override {
        return std::make_shared<None>();
    }
};

} /* namespace mlang */