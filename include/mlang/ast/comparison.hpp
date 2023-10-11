#pragma once

#include "mlang/ast/node.hpp"

namespace mlang {
namespace ast {

enum class comparison_mode {
    equal,
    not_equal,
    greater,
    less,
    greater_equal,
    less_equal
};

class BinaryComparisonNode : public Node {
private:
    node_ptr m_left;
    node_ptr m_right;
    comparison_mode m_mode { comparison_mode::equal };
public:
    BinaryComparisonNode(node_ptr left, node_ptr right, comparison_mode mode);
    ~BinaryComparisonNode () = default;
    const Node* const get_left () const;
    const Node* const get_right () const;
    comparison_mode get_mode () const;
    object::Object execute (EnvStack& env) const override;
    void print () const override;
};

} /* namespace ast */
} /* namespace mlang */