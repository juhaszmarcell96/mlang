#include <string>
#include <algorithm>

#include "mlang/object/array.hpp"
#include "mlang/object/assert.hpp"

namespace mlang {
namespace object {

class Array : public InternalObject {
private:
    std::vector<Object> m_arr;
public:
    Array () = default;
    Array (const std::vector<Object>& arr) : m_arr(arr) {}
    ~Array () = default;
    
    const static inline std::string type_name { "Array" };

    const std::vector<Object>& get () const { return m_arr; }

    /* construct */
    /*void construct (const std::vector<std::shared_ptr<InternalObject>>& params) override {
        if (params.size() == 0) { m_arr.clear(); return; }
        for (const auto param : params) {
            m_arr.push_back(Object{param});
        }
    }*/
    /* assign */
    /*void assign (const std::vector<std::shared_ptr<InternalObject>>& params) override {
        m_arr = params;
    }
    void assign (const std::shared_ptr<InternalObject> param) override {
        const std::shared_ptr<Array> arr_ptr = assert_cast<Array>(param, type_name);
        m_arr = arr_ptr->get();
    }*/
    /* destruct */
    /*void destruct () override {
        m_arr.clear();
    }*/

    std::shared_ptr<InternalObject> operator_binary_add (const std::shared_ptr<InternalObject> param) override {
        assert_parameter(param, type_name, "+");
        const Array* arr_ptr = assert_cast<const Array*>(param, type_name);
        std::vector<std::shared_ptr<Object>> new_arr = m_arr;
        for (const auto elem : arr_ptr->get()) {
            new_arr.push_back(elem);
        }
        return std::make_shared<Array>(new_arr, false);
    }

    void operator_add_equal (const std::shared_ptr<InternalObject> param) override {
        assert_parameter(param, type_name, "+=");
        const Array* arr_ptr = assert_cast<const Array*>(param, type_name);
        for (const auto elem : arr_ptr->get()) {
            m_arr.push_back(elem);
        }
    }

    std::shared_ptr<InternalObject> operator_comparison_equal (const std::shared_ptr<InternalObject> param) override {
        assert_parameter(param, type_name, "==");
        const Array* arr_ptr = assert_cast<const Array*>(param, type_name);
        const std::vector<std::shared_ptr<Object>>& other_arr = arr_ptr->get();
        if (other_arr.size() != m_arr.size()) {
            return std::make_shared<Boolean>(false, false);
        }
        for (std::size_t i = 0; i < other_arr.size(); ++i) {
            if (other_arr[i] != m_arr[i]) {
                return std::make_shared<Boolean>(false, false);
            }
        }
        return std::make_shared<Boolean>(true, false);
    }

    std::shared_ptr<InternalObject> operator_comparison_not_equal (const std::shared_ptr<InternalObject> param) override {
        assert_parameter(param, type_name, "!=");
        const Array* arr_ptr = assert_cast<const Array*>(param, type_name);
        const std::vector<std::shared_ptr<Object>>& other_arr = arr_ptr->get();
        if (other_arr.size() != m_arr.size()) {
            return std::make_shared<Boolean>(true, false);
        }
        for (std::size_t i = 0; i < other_arr.size(); ++i) {
            if (other_arr[i] != m_arr[i]) {
                return std::make_shared<Boolean>(true, false);
            }
        }
        return std::make_shared<Boolean>(false, false);
    }

    std::shared_ptr<InternalObject> operator_subscript (const std::shared_ptr<InternalObject> param) override {
        assert_parameter(param, type_name, "[]");
        std::size_t index = static_cast<std::size_t>(param->get_number());
        return m_arr[index];
    }

    std::shared_ptr<InternalObject> reverse () {
        std::vector<std::shared_ptr<Object>> reversed = m_arr;
        std::reverse(reversed.begin(), reversed.end());
        return std::make_shared<Array>(reversed, false);
    }


    std::shared_ptr<InternalObject> call (const std::string& func, const std::vector<std::shared_ptr<InternalObject>>& params) override {
        if (func.compare(operators::construct) == 0) {
            construct(params);
        }
        else if (func.compare(operators::assign) == 0) {
            assign(params);
        }
        else if (func.compare(operators::destruct) == 0) {
            destruct();
        }
        else if (func.compare("reverse") == 0) {
            return reverse();
        }
        else {
            throw RuntimeError { "string object has no " + func + " member function" };
        }
        return nullptr;
    }

    std::string get_string () const override {
        std::string str = type_name;
        str += " : { ";
        for (const auto& elem : m_arr) {
            str += elem->get_string();
            str += " ";
        }
        str += "}";
        return str;
    }
    std::string get_typename () const override { return type_name; }
};

class ArrayFactory : public ObjectFactory {
public:
    std::shared_ptr<InternalObject> create () const override {
        return std::make_shared<Array>();
    }
};

} /* namespace object */
} /* namespace mlang */