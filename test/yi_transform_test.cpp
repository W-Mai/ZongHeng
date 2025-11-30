//
// Created by Claude Code
// Test Yi with different INPUT_TYPE and OUTPUT_TYPE
//

#include "ZongHeng.h"
#include "test_utils.h"
#include <algorithm>
#include <string>
#include <vector>

// Test Yi<string, int> with getter/setter transformations
int test_yi_string_to_int() {
    auto str_to_len = Yi<std::string, int>::make("Hello");

    // Getter: string -> int (length)
    str_to_len->getter([](const std::string& s) {
        return static_cast<int>(s.length());
    });

    // Setter: int -> string (repeat '*' n times)
    str_to_len->setter([](const int& len) {
        return std::string(len, '*');
    });

    // Initial value
    ASSERT_I(str_to_len->get(), 5); // "Hello".length()

    // Set using OUTPUT type (int)
    *str_to_len = 10;
    ASSERT_I(str_to_len->get(), 10); // "**********".length()

    // Set using internal type directly
    str_to_len->set_inner("ZongHeng");
    ASSERT_I(str_to_len->get(), 8);

    return 0;
}

// Test Yi<int, double> numeric type conversion
int test_yi_int_to_double() {
    auto int_to_double = Yi<int, double>::make(100);

    // Getter: int -> double (divide by 100)
    int_to_double->getter([](const int& val) {
        return val / 100.0;
    });

    // Setter: double -> int (multiply by 100)
    int_to_double->setter([](const double& val) {
        return static_cast<int>(val * 100);
    });

    ASSERT_F(int_to_double->get(), 1.0); // 100 / 100.0

    *int_to_double = 3.14;
    ASSERT_F(int_to_double->get(), 3.14); // 314 / 100.0

    return 0;
}

// Test Yi<vector<string>, string> collection to single value
int test_yi_vector_to_string() {
    std::vector<std::string> words      = { "Hello", "World", "!" };
    auto                     vec_to_str = Yi<std::vector<std::string>, std::string>::make(words);

    // Getter: join vector with spaces
    vec_to_str->getter([](const std::vector<std::string>& vec) {
        std::string result;
        for (size_t i = 0; i < vec.size(); ++i) {
            if (i > 0) result += " ";
            result += vec[i];
        }
        return result;
    });

    // Setter: split string by spaces
    vec_to_str->setter([](const std::string& s) {
        std::vector<std::string> result;
        std::string              word;
        for (char c : s) {
            if (c == ' ') {
                if (!word.empty()) {
                    result.push_back(word);
                    word.clear();
                }
            } else {
                word += c;
            }
        }
        if (!word.empty()) result.push_back(word);
        return result;
    });

    ASSERT_S(vec_to_str->get(), std::string("Hello World !"));

    // Set using OUTPUT type (string)
    *vec_to_str = std::string("Good Morning");
    ASSERT_S(vec_to_str->get(), std::string("Good Morning"));

    return 0;
}

// Test Yi<string, int> with unidirectional transform (no setter)
int test_yi_readonly_transform() {
    auto str = Yi<std::string, int>::make("Hello World");

    // Only getter, no setter (read-only transform)
    str->getter([](const std::string& s) {
        return static_cast<int>(s.length());
    });

    ASSERT_I(str->get(), 11);

    // Update internal value directly
    str->set_inner("ZongHeng");
    ASSERT_I(str->get(), 8);

    return 0;
}

// Test Yi lian with same heterogeneous type
int test_yi_hetero_lian_safe() {
    // Both are Yi<string, int>, safe to lian
    auto a = Yi<std::string, int>::make("Hello");
    auto b = Yi<std::string, int>::make("World");

    a->getter([](const std::string& s) {
        return static_cast<int>(s.length());
    });

    b->getter([](const std::string& s) {
        return static_cast<int>(s.length());
    });

    // Create derived node: sum of lengths
    auto sum = a->lian(b, [a, b]() -> int {
        return a->get() + b->get();
    });

    ASSERT_I(sum->get(), 5 + 5); // "Hello" + "World"

    // Update a
    a->set_inner("Hi");
    ASSERT_I(sum->get(), 2 + 5); // "Hi" + "World"

    return 0;
}

// Test Yi with effect (derived computation)
int test_yi_hetero_with_effect() {
    auto source = Yi<std::string, int>::make("Test");

    source->getter([](const std::string& s) {
        return static_cast<int>(s.length());
    });

    // Create another Yi that derives from source using effect
    auto doubled = Yi<std::string, int>::make("");

    // Set effect to double the source value
    doubled->setEff([source]() -> int {
        return source->get() * 2;
    });

    ASSERT_I(doubled->get(), 4 * 2); // "Test".length() * 2 = 8

    source->set_inner("Hello");
    ASSERT_I(doubled->get(), 5 * 2); // "Hello".length() * 2 = 10

    return 0;
}

// Test Yi<int, string> reverse conversion
int test_yi_int_to_string() {
    auto num_to_str = Yi<int, std::string>::make(42);

    // Getter: int -> string representation
    num_to_str->getter([](const int& val) {
        return std::to_string(val) + " units";
    });

    // Setter: string -> int (parse number)
    num_to_str->setter([](const std::string& s) {
        // Simple parsing: extract first number
        int result = 0;
        for (char c : s) {
            if (c >= '0' && c <= '9') {
                result = result * 10 + (c - '0');
            }
        }
        return result;
    });

    ASSERT_S(num_to_str->get(), std::string("42 units"));

    *num_to_str = std::string("123");
    ASSERT_S(num_to_str->get(), std::string("123 units"));

    return 0;
}

// Test Yi<double, bool> conditional conversion
int test_yi_double_to_bool() {
    auto threshold = Yi<double, bool>::make(0.5);

    // Getter: > 0.5 -> true, else false
    threshold->getter([](const double& val) {
        return val > 0.5;
    });

    // Setter: bool -> double (1.0 or 0.0)
    threshold->setter([](const bool& b) {
        return b ? 1.0 : 0.0;
    });

    ASSERT_I(threshold->get(), 0); // 0.5 is not > 0.5, false

    threshold->set_inner(0.8);
    ASSERT_I(threshold->get(), 1); // 0.8 > 0.5, true

    *threshold = true;
    ASSERT_I(threshold->get(), 1); // 1.0 > 0.5, true

    *threshold = false;
    ASSERT_I(threshold->get(), 0); // 0.0 is not > 0.5, false

    return 0;
}

// Test multiple Yi transformations in a pipeline
int test_yi_transform_pipeline() {
    // Yi<string, int>: string -> length
    auto str_to_len = Yi<std::string, int>::make("Hello");
    str_to_len->getter([](const std::string& s) {
        return static_cast<int>(s.length());
    });

    // Yi<int, int>: can be used with previous result
    auto len_doubled = Qin<int>::make(0);

    // Manually update based on str_to_len (no binding due to type safety)
    len_doubled->setter([str_to_len](const int&) {
        return str_to_len->get() * 2;
    });

    *len_doubled = 0;                    // Trigger setter
    ASSERT_I(len_doubled->get(), 5 * 2); // "Hello".length() * 2

    return 0;
}

int main() {
    auto tests = {
        test_yi_string_to_int(),
        test_yi_int_to_double(),
        test_yi_vector_to_string(),
        test_yi_readonly_transform(),
        test_yi_hetero_lian_safe(),
        test_yi_hetero_with_effect(),
        test_yi_int_to_string(),
        test_yi_double_to_bool(),
        test_yi_transform_pipeline()
    };

    return !std::all_of(tests.begin(), tests.end(), [](int val) {
        return !val;
    });
}
