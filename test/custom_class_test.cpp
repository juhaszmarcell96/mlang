#include <gtest/gtest.h>

#include <string>
#include <cmath>

#include "mlang/object/assert.hpp"
#include "mlang/object/number.hpp"
#include "mlang/script/script.hpp"

class Complex : public mlang::object::InternalObject {
private:
    double m_real { 0.0 };
    double m_imag { 0.0 };
public:
    Complex () = default;
    Complex (double real, double imag);
    ~Complex () = default;
    
    const static inline std::string type_name { "Complex" };

    const mlang::object::ObjectFactory& get_factory () const override;

    void construct (const std::vector<std::shared_ptr<mlang::object::InternalObject>>& params) override;
    void assign (const std::shared_ptr<mlang::object::InternalObject> param) override;

    bool is_true () const override;

    std::shared_ptr<mlang::object::InternalObject> unary_minus () override;

    std::shared_ptr<mlang::object::InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override;
    std::shared_ptr<mlang::object::InternalObject> access (const std::string& member) override;

    std::shared_ptr<mlang::object::InternalObject> abs () const;

    std::string get_string () const override;
    std::string get_typename () const override;
};

class ComplexFactory : public mlang::object::ObjectFactory {
public:
    std::shared_ptr<mlang::object::InternalObject> create () const override;
};





Complex::Complex (double real, double imag) : m_real(real), m_imag(imag) {}

const mlang::object::ObjectFactory& Complex::get_factory () const {
    static ComplexFactory factory{};
    return factory;
}

/* construct */
void Complex::construct (const std::vector<std::shared_ptr<mlang::object::InternalObject>>& params) {
    if (params.size() == 0) { m_real = 0.0; m_imag = 0.0; }
    mlang::object::assert_params(params, 2, type_name, "constructor");
    mlang::object::assert_parameter(params[0], type_name, "constructor");
    mlang::object::assert_parameter(params[1], type_name, "constructor");
    const std::shared_ptr<mlang::object::Number> real_ptr = mlang::object::assert_cast<mlang::object::Number>(params[0], type_name);
    const std::shared_ptr<mlang::object::Number> imag_ptr = mlang::object::assert_cast<mlang::object::Number>(params[1], type_name);
    m_real = real_ptr->get();
    m_imag = imag_ptr->get();
}

void Complex::assign (const std::shared_ptr<mlang::object::InternalObject> param) {
    const std::shared_ptr<Complex> num_ptr = assert_cast<Complex>(param, type_name);
    m_real = num_ptr->m_real;
    m_imag = num_ptr->m_imag;
}

bool Complex::is_true () const {
    return (m_real != 0) || (m_imag != 0);
}

std::shared_ptr<mlang::object::InternalObject> Complex::unary_minus () {
    return std::make_shared<Complex>(-m_real, -m_imag);
}

std::shared_ptr<mlang::object::InternalObject> Complex::call (const std::string& func, const std::vector<std::shared_ptr<mlang::object::InternalObject>>& params) {
    if (func.compare("abs") == 0) {
        return abs();
    }
    throw mlang::RuntimeError { "object of type '" + type_name + "' has no '" + func + "' member function" };
}

std::shared_ptr<mlang::object::InternalObject> Complex::access (const std::string& member) {
    if (member.compare("real") == 0) {
        return std::make_shared<mlang::object::Number>(m_real);
    }
    else if (member.compare("imag") == 0) {
        return std::make_shared<mlang::object::Number>(m_imag);
    }
    throw mlang::RuntimeError { "object of type '" + type_name + "' has no '" + member + "' member" };
}

std::shared_ptr<mlang::object::InternalObject> Complex::abs () const {
    return std::make_shared<mlang::object::Number>(std::sqrt(m_real * m_real + m_imag * m_imag));
}

std::string Complex::get_string () const { return "(" + std::to_string(m_real) + "+" + std::to_string(m_imag) + "j)"; }
std::string Complex::get_typename () const { return type_name; }


std::shared_ptr<mlang::object::InternalObject> ComplexFactory::create () const {
    return std::make_shared<Complex>();
}



TEST(CustomClassTest, Test1) {
    std::string script_text;
    script_text += "var a = new Complex(1, 2); \n";
    script_text += "var b = a.abs();\n";
    script_text += "var real = a.real;\n";
    script_text += "var imag = a.imag;";
    mlang::script::Script script { script_text };
    mlang::script::EnvStack env {};
    mlang::script::Environment::define_type("Complex", std::make_shared<ComplexFactory>());
    script.execute(env);

    ASSERT_EQ(env.has_variable("a"), true);
    ASSERT_EQ(env.get_variable("a").get_typename(), Complex::type_name);
    ASSERT_EQ(env.get_variable("a").get_string(), "(1.000000+2.000000j)");
    ASSERT_EQ(env.has_variable("b"), true);
    ASSERT_EQ(env.get_variable("b").get_typename(), mlang::object::Number::type_name);
    ASSERT_GT(env.get_variable("b").get_number(), 2.236);
    ASSERT_LT(env.get_variable("b").get_number(), 2.237);
    ASSERT_EQ(env.has_variable("real"), true);
    ASSERT_EQ(env.get_variable("real").get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(env.get_variable("real").get_number(), 1);
    ASSERT_EQ(env.has_variable("imag"), true);
    ASSERT_EQ(env.get_variable("imag").get_typename(), mlang::object::Number::type_name);
    ASSERT_EQ(env.get_variable("imag").get_number(), 2);
}