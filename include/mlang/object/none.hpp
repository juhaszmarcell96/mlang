#pragma once

#include <string>

#include "mlang/object/internal_object.hpp"

namespace mlang {
namespace object {

class None : public InternalObject {
public:
    None () = default;
    ~None () = default;
    
    const static inline std::string type_name { "None" };

    const ObjectFactory& get_factory () const override;

    /*
    void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override;
    void destruct () override;
    */
    void assign (const std::shared_ptr<InternalObject> param) override;

    bool is_true () const override;

    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override;

    std::string get_string () const override;
    std::string get_typename () const override;
};

class NoneFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override;
};

} /* namespace object */
} /* namespace mlang */