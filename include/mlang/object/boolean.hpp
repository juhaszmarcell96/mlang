#pragma once

#include <string>

#include "mlang/object/internal_object.hpp"

namespace mlang {
namespace object {

class Boolean : public InternalObject {
private:
    bool m_value;
public:
    Boolean () = default;
    Boolean (bool value);
    ~Boolean () = default;

    const static inline std::string type_name { "Boolean" };

    const ObjectFactory& get_factory () const override;

    const bool get () const;

    /*
    void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void destruct () override;
    */
    void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void assign (const std::shared_ptr<InternalObject> param) override;

    bool is_true () const override;

    std::shared_ptr<InternalObject> operator_comparison_equal (const std::shared_ptr<InternalObject> param) override;
    std::shared_ptr<InternalObject> operator_comparison_not_equal (const std::shared_ptr<InternalObject> param);
    std::shared_ptr<InternalObject> unary_not () override;

    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override;
    std::shared_ptr<InternalObject> access (const std::string& member) override;

    std::string get_string () const override;
    std::string get_typename () const override;
    double get_number () const override;
};

class BooleanFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override;
};

} /* namespace object */
} /* namespace mlang */