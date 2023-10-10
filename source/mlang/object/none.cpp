#include <string>

#include "mlang/object/none.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

/* construct */
//void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override { }
/* assign */
//void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override { }
/* destruct */
// void destruct () override { }
void None::assign (const std::shared_ptr<InternalObject> param) { }

bool None::is_true () const { return false; }

const ObjectFactory& None::get_factory () const {
    static NoneFactory factory{};
    return factory;
}

std::shared_ptr<InternalObject> None::call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) {
    throw RuntimeError { "object of type '" + type_name + "' has no member functions" };
}

std::string None::get_string () const { return type_name; }
std::string None::get_typename () const { return type_name; }

std::shared_ptr<InternalObject> NoneFactory::create () const {
    return std::make_shared<None>();
}

} /* namespace object */
} /* namespace mlang */