#pragma once

#include <string>

#include "mlang/object/internal_object.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {
namespace object {

class Number : public InternalObject {
private:
    double m_value { 0.0 };
public:
    Number () = default;
    Number (const double value);
    Number (const int value);
    ~Number () = default;
    
    const static inline std::string type_name { "Number" };

    const ObjectFactory& get_factory () const override;

    /*
    void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void destruct () override;
    */
    void assign (const std::shared_ptr<InternalObject> param) override;

    bool is_true () const override;

    std::shared_ptr<InternalObject> operator_binary_add (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_binary_sub (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_binary_mul (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_binary_div (const InternalObject* param) override;

    void operator_add_equal (const InternalObject* param) override;
    void operator_sub_equal (const InternalObject* param) override;
    void operator_mul_equal (const InternalObject* param) override;
    void operator_div_equal (const InternalObject* param) override;

    std::shared_ptr<InternalObject> operator_comparison_equal (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_comparison_not_equal (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_greater (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_less (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_greater_equal (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_less_equal (const InternalObject* param) override;

    std::shared_ptr<InternalObject> unary_minus () override;
    std::shared_ptr<InternalObject> unary_not () override;


    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override;

    std::string get_string () const override;
    std::string get_typename () const override;
    double get_number () const override;
};

class NumberFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override;
};

} /* namespace object */
} /* namespace mlang */