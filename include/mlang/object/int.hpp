#pragma once

#include <string>

#include "mlang/object/internal_object.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {
namespace object {

class Int : public InternalObject {
private:
    int m_value { 0 };
public:
    Int () = default;
    Int (const int value);
    ~Int () = default;
    
    const static inline std::string type_name { "Int" };

    const ObjectFactory& get_factory () const override;

    int get () const;

    void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void assign (const std::shared_ptr<InternalObject> param) override;

    bool is_true () const override;

    std::shared_ptr<InternalObject> operator_binary_add (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_binary_sub (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_binary_mul (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_binary_div (const std::shared_ptr<InternalObject> param) override;

    void operator_add_equal (const std::shared_ptr<InternalObject> param) override;
    void operator_sub_equal (const std::shared_ptr<InternalObject> param) override;
    void operator_mul_equal (const std::shared_ptr<InternalObject> param) override;
    void operator_div_equal (const std::shared_ptr<InternalObject> param) override;

    std::shared_ptr<InternalObject> operator_comparison_equal (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_greater (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_less (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_greater_equal (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_less_equal (const std::shared_ptr<InternalObject> param) override;

    std::shared_ptr<InternalObject> unary_minus () override;
    std::shared_ptr<InternalObject> unary_not () override;

    virtual void increment () override;
    virtual void decrement () override;
    
    std::shared_ptr<InternalObject> to_string ();
    std::shared_ptr<InternalObject> to_float ();
    std::shared_ptr<InternalObject> to_int ();

    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override;
    std::shared_ptr<InternalObject> access (const std::string& member) override;

    std::string get_string () const override;
    std::string get_typename () const override;
    int get_int () const override;
    double get_float () const override;
};

class IntFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override;
};

} /* namespace object */
} /* namespace mlang */