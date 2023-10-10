#pragma once

#include <string>
#include <algorithm>

#include "mlang/object/internal_object.hpp"
#include "mlang/object/boolean.hpp"

namespace mlang {
namespace object {

class String : public InternalObject {
private:
    std::string m_value;
public:
    String () = default;
    String (const std::string& value);
    ~String () = default;
    
    const static inline std::string type_name { "String" };

    const std::string& get () const;

    const ObjectFactory& get_factory () const override;

    /*
    void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void destruct () override;
    */
    void assign (const std::shared_ptr<InternalObject> param) override;

    std::shared_ptr<InternalObject> operator_binary_add (const InternalObject* param) override;

    void operator_add_equal (const InternalObject* param) override;

    std::shared_ptr<InternalObject> operator_comparison_equal (const InternalObject* param) override;
    std::shared_ptr<InternalObject> operator_comparison_not_equal (const InternalObject* param) override;

    std::shared_ptr<InternalObject> reverse ();


    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override;

    std::string get_string () const override;
    std::string get_typename () const override;
};

class StringFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override;
};

} /* namespace object */
} /* namespace mlang */