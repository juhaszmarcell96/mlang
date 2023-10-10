#pragma once

#include <string>
#include <algorithm>

#include "mlang/object/internal_object.hpp"
#include "mlang/object/object.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {
namespace object {

class Array : public InternalObject {
private:
    std::vector<Object> m_arr;
public:
    Array () = default;
    Array (const std::vector<Object>& arr);
    ~Array () = default;
    
    const static inline std::string type_name { "Array" };

    const ObjectFactory& get_factory () const override;

    const std::vector<Object>& get () const;

    /*
    void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void assign (const std::shared_ptr<InternalObject> param) override;
    void destruct () override;
    */

    std::shared_ptr<InternalObject> operator_binary_add (const InternalObject* param) override;

    void operator_add_equal (const InternalObject* param) override;

    std::shared_ptr<InternalObject> operator_comparison_equal (const InternalObject* param) override;

    std::shared_ptr<InternalObject> operator_comparison_not_equal (const InternalObject* param) override;

    std::shared_ptr<InternalObject> operator_subscript (const InternalObject* param) override;

    std::shared_ptr<InternalObject> reverse ();


    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override;

    std::string get_string () const override;
    std::string get_typename () const override;
};

class ArrayFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override;
};

} /* namespace object */
} /* namespace mlang */