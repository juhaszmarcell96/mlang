#include "mlang/ast/constructor_node.hpp"

namespace mlang {
namespace ast {

ConstructorNode::ConstructorNode(const std::string& type_name) : Node(ast_node_types::constructor), m_type_name(type_name) {}

object::Object ConstructorNode::execute (script::EnvStack& env) const {
    std::vector<object::Object> arguments;
    for (const auto& arg : m_arguments) {
        arguments.push_back(arg->execute(env));
    }
    object::Object new_object { script::Environment::get_factory(m_type_name) };
    new_object.construct(arguments);
    return new_object;
}

void ConstructorNode::add_argument (node_ptr argument) {
    m_arguments.push_back(std::move(argument));
}

void ConstructorNode::print () const {
    std::cout << "new " << m_type_name << " (";
    for (std::size_t i = 0; i < m_arguments.size(); ++i) {
        m_arguments[i]->print();
        if (i < m_arguments.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << ")";
}

} /* namespace ast */
} /* namespace mlang */