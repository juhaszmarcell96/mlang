#pragma once

#include <string>
#include <algorithm>

#include "mlang/object/internal_object.hpp"

namespace mlang {
namespace object {

class Object;

class Array : public InternalObject {
private:
    std::vector<Object> m_arr;
public:
    Array () = default;
    Array (const std::vector<Object>& arr);
    ~Array () = default;
    
    std::string get_typename () const override;
    
    const static inline std::string type_name { "Array" };

    const ObjectFactory& get_factory () const override;

    /*
    void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void destruct () override;
    */
    void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void assign (const std::shared_ptr<InternalObject> param) override;

    std::shared_ptr<InternalObject> operator_binary_add (const std::shared_ptr<InternalObject> param) override;

    void operator_add_equal (const std::shared_ptr<InternalObject> param) override;

    std::shared_ptr<InternalObject> operator_comparison_equal (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) override;

    Object& operator_subscript (const std::shared_ptr<InternalObject> param) override;

    std::shared_ptr<InternalObject> reverse ();


    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override;
    std::shared_ptr<InternalObject> access (const std::string& member) override;

    std::string get_string () const override;
};

class ArrayFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override;
};

} /* namespace object */
} /* namespace mlang */