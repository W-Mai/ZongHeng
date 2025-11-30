//
// Created by Claude Code
//

#include "ZongHeng.h"
#include "test_utils.h"
#include <algorithm>
#include <string>
#include <vector>

// Test Qin::lian with custom effect function
int test_qin_lian_custom_effect() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);

    // Create a derived node with custom effect (a * b + 5)
    auto derived = a->lian(b, [a, b]() -> int {
        return a->get() * b->get() + 5;
    });

    ASSERT_I(derived->get(), 10 * 20 + 5); // 205

    // Update a, derived should update automatically
    *a = 5;
    ASSERT_I(derived->get(), 5 * 20 + 5); // 105

    // Update b, derived should update automatically
    *b = 3;
    ASSERT_I(derived->get(), 5 * 3 + 5); // 20

    return 0;
}

// Test Qin::lian with template functors (similar to + and *)
int test_qin_lian_functors() {
    auto x = Qin<double>::make(3.0);
    auto y = Qin<double>::make(4.0);

    // Using std::minus
    auto diff = x->lian<std::minus<double>>(y);
    ASSERT_F(diff->get(), 3.0 - 4.0); // -1.0

    *x = 10.0;
    ASSERT_F(diff->get(), 10.0 - 4.0); // 6.0

    return 0;
}

// Test multiple lian chains
int test_qin_lian_chains() {
    auto a = Qin<int>::make(2);
    auto b = Qin<int>::make(3);
    auto c = Qin<int>::make(4);

    // Create chain: (a + b) * c
    auto sum = a + b;
    auto result = sum->lian(c, [sum, c]() -> int {
        return sum->get() * c->get();
    });

    ASSERT_I(result->get(), (2 + 3) * 4); // 20

    *a = 5;
    ASSERT_I(result->get(), (5 + 3) * 4); // 32

    *c = 2;
    ASSERT_I(result->get(), (5 + 3) * 2); // 16

    return 0;
}

// Test Yi::lian with getter/setter transformations
int test_yi_lian_type_transform() {
    auto str1 = Yi<std::string, std::string>::make("Hello");
    auto str2 = Yi<std::string, std::string>::make("World");

    // Set getter to transform to uppercase on first string
    str1->getter([](const std::string& s) {
        std::string result = s;
        for (auto& c : result) c = std::toupper(c);
        return result;
    });

    // Create derived node that combines both strings
    auto derived = str1->lian(str2, [str1, str2]() -> std::string {
        return str1->get() + " " + str2->get();
    });

    ASSERT_S(derived->get(), std::string("HELLO World"));

    // Update the first string
    str1->set_inner("Good");
    ASSERT_S(derived->get(), std::string("GOOD World"));

    // Update the second string
    str2->set_inner("Morning");
    ASSERT_S(derived->get(), std::string("GOOD Morning"));

    return 0;
}

// Test lian with string concatenation
int test_qin_lian_string_operations() {
    auto prefix = Qin<std::string>::make("Hello");
    auto suffix = Qin<std::string>::make("World");

    // Custom separator effect
    auto sentence = prefix->lian(suffix, [prefix, suffix]() -> std::string {
        return prefix->get() + " " + suffix->get() + "!";
    });

    ASSERT_S(sentence->get(), std::string("Hello World!"));

    *prefix = std::string("Good");
    ASSERT_S(sentence->get(), std::string("Good World!"));

    *suffix = std::string("Morning");
    ASSERT_S(sentence->get(), std::string("Good Morning!"));

    return 0;
}

// Test lian with getter on source node (note: effect takes priority over getter)
int test_lian_with_getter_setter() {
    auto a = Qin<int>::make(5);
    auto b = Qin<int>::make(10);

    // Add getter to source node to scale its value
    a->getter([](const int& val) {
        return val * 2; // Double a's value
    });

    auto sum = a + b;

    // sum's effect uses a->get() which applies the getter
    ASSERT_I(sum->get(), (5 * 2) + 10); // 20

    *a = 20;
    ASSERT_I(sum->get(), (20 * 2) + 10); // 50

    // Note: If we set getter on sum (which has effect), effect takes priority
    // This demonstrates the priority: effect > getter > raw value

    return 0;
}

// Test multiple derived nodes from same sources
int test_multiple_lian_from_same_sources() {
    auto x = Qin<int>::make(10);
    auto y = Qin<int>::make(5);

    auto sum = x + y;
    auto product = x * y;
    auto custom = x->lian(y, [x, y]() -> int {
        return x->get() - y->get();
    });

    ASSERT_I(sum->get(), 15);
    ASSERT_I(product->get(), 50);
    ASSERT_I(custom->get(), 5);

    *x = 20;
    ASSERT_I(sum->get(), 25);
    ASSERT_I(product->get(), 100);
    ASSERT_I(custom->get(), 15);

    return 0;
}

int main() {
    auto tests = {
        test_qin_lian_custom_effect(),
        test_qin_lian_functors(),
        test_qin_lian_chains(),
        test_yi_lian_type_transform(),
        test_qin_lian_string_operations(),
        test_lian_with_getter_setter(),
        test_multiple_lian_from_same_sources()
    };

    return !std::all_of(tests.begin(), tests.end(), [](int val) {
        return !val;
    });
}
