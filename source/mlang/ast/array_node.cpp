#include "mlang/ast/array_node.hpp"
#include "mlang/object/array.hpp"

namespace mlang {
namespace ast {

ArrayNode::ArrayNode() : Node(ast_node_types::array) {}

object::Object ArrayNode::execute (script::EnvStack& env) const {
    std::vector<object::Object> array_elements;
    for (const auto& elem : m_elements) {
        array_elements.push_back(elem->execute(env));
    }
    return object::Object{std::make_shared<object::Array>(array_elements)};
}

void ArrayNode::add_element (node_ptr elem) {
    m_elements.push_back(std::move(elem));
}

void ArrayNode::print () const {
    std::cout << "{";
    for (std::size_t i = 0; i < m_elements.size(); ++i) {
        m_elements[i]->print();
        if (i < m_elements.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << "}";
}

} /* namespace ast */
} /* namespace mlang */