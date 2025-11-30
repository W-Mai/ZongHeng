//
// Created by Claude Code for v2.0 refactoring
// Type Safety Tests - Verify runtime type checking and error handling
//

#include "Qin.h"
#include "test_utils.h"
#include <stdexcept>
#include <string>

// Test 1: into() with correct type should succeed
int test_into_correct_type() {
    auto yi = Yi<int, int>::make(42);

    // Correct type conversion should work
    auto converted = yi->into<int, int>();
    ASSERT_I(converted->get(), 42);

    return 0;
}

// Test 2: into() with incorrect type should throw exception
int test_into_incorrect_type() {
    auto yi_int = Yi<int, int>::make(42);

    try {
        // Attempting to convert Yi<int,int> to Yi<string,string> should fail
        auto converted = yi_int->into<std::string, std::string>();
        // Should not reach here
        return 1;
    } catch (const std::runtime_error& e) {
        // Expected exception - test passes
        return 0;
    }
}

// Test 3: into() with heterogeneous types
int test_into_heterogeneous_type() {
    auto yi = Yi<std::string, int>::make("Hello");
    yi->getter([](const std::string& s) {
        return static_cast<int>(s.length());
    });

    // Correct type should work
    auto converted = yi->into<std::string, int>();
    ASSERT_I(converted->get(), 5);

    // Incorrect type should throw
    try {
        auto wrong = yi->into<int, std::string>();
        return 1; // Should not reach
    } catch (const std::runtime_error&) {
        return 0; // Expected
    }
}

// Test 4: convert() with convertible types should succeed
int test_convert_valid() {
    // int -> double is convertible
    int input = 42;
    double result = convert<int, double>(input);
    ASSERT_F(result, 42.0);

    // double -> int is convertible (with truncation)
    double input2 = 3.14;
    int result2 = convert<double, int>(input2);
    ASSERT_I(result2, 3);

    return 0;
}

// Test 5: convert() with non-convertible types should throw
int test_convert_invalid() {
    try {
        // vector<int> -> int is not convertible
        std::vector<int> vec = {1, 2, 3};
        int result = convert<std::vector<int>, int>(vec);
        return 1; // Should not reach
    } catch (const std::runtime_error&) {
        return 0; // Expected
    }
}

// Test 6: set() type propagation with mixed types in Zong/Heng
int test_set_mixed_type_propagation() {
    // Create Yi<int,int> and Yi<string,string>
    auto yi_int = Yi<int, int>::make(10);
    auto yi_str = Yi<std::string, std::string>::make("Hello");

    // Manually add cross-type dependency (would happen in complex graph)
    // Note: This simulates what could happen in a real mixed-type graph
    yi_int->bind(yi_str);

    // Update yi_int - should not crash even though yi_str has different type
    *yi_int = 20;
    ASSERT_I(yi_int->get(), 20);

    // yi_str should be unchanged (incompatible type was skipped)
    ASSERT_S(yi_str->get(), std::string("Hello"));

    return 0;
}

// Test 7: Polymorphic QinBase with virtual destructor
int test_polymorphic_base() {
    auto yi = Yi<int, int>::make(42);

    // Can use as QinBase pointer (polymorphic)
    std::shared_ptr<QinBase> base_ptr = yi;

    // dynamic_pointer_cast should work (requires polymorphic class)
    auto derived = std::dynamic_pointer_cast<Yi<int, int>>(base_ptr);
    if (!derived) {
        return 1; // Cast failed
    }

    ASSERT_I(derived->get(), 42);
    return 0;
}

// Test 8: into() error message contains type information
int test_into_error_message() {
    auto yi = Yi<int, double>::make(42);

    try {
        auto wrong = yi->into<std::string, std::string>();
        return 1; // Should not reach
    } catch (const std::runtime_error& e) {
        std::string msg = e.what();
        // Error message should mention "Type mismatch in into()"
        if (msg.find("Type mismatch") == std::string::npos) {
            return 1; // Error message doesn't contain expected text
        }
        return 0;
    }
}

// Test 9: Qin<T> type safety (Qin = Yi<T,T>)
int test_qin_type_safety() {
    auto qin_int = Qin<int>::make(42);
    auto qin_str = Qin<std::string>::make("Hello");

    // Correct type conversions
    auto int_converted = qin_int->into<int, int>();
    ASSERT_I(int_converted->get(), 42);

    auto str_converted = qin_str->into<std::string, std::string>();
    ASSERT_S(str_converted->get(), std::string("Hello"));

    // Cross-type conversion should fail
    try {
        auto wrong = qin_int->into<std::string, std::string>();
        return 1;
    } catch (const std::runtime_error&) {
        // Expected
    }

    return 0;
}

// Test 10: Mixed-type lian should work when types match
int test_lian_type_matching() {
    auto a = Yi<int, int>::make(10);
    auto b = Yi<int, int>::make(20);

    // Same types - should work
    auto sum = a->lian(b, [a, b]() -> int {
        return a->get() + b->get();
    });

    ASSERT_I(sum->get(), 30);

    return 0;
}

// Test 11: lian with type mismatch should throw clear error
int test_lian_type_mismatch() {
    auto a = Yi<int, int>::make(10);
    auto b = Yi<std::string, std::string>::make("Hello");

    try {
        // Type mismatch in lian - should throw when accessing b as Yi<int,int>
        auto result = a->lian(b, [a, b]() -> int {
            // This will try to convert b (which is Yi<string,string>) to Yi<int,int>
            return a->get() + b->into<int, int>()->get();
        });

        // Trigger the effect to cause the error
        result->get();

        return 1; // Should not reach
    } catch (const std::runtime_error&) {
        return 0; // Expected
    }
}

// Test 12: Virtual destructor ensures proper cleanup
int test_virtual_destructor() {
    {
        std::shared_ptr<QinBase> base = Yi<int, int>::make(42);
        // When base goes out of scope, virtual destructor should be called
        // This test mainly checks compilation - actual cleanup is verified by ASAN
    }

    return 0;
}

int main() {
    auto tests = {
        test_into_correct_type(),
        test_into_incorrect_type(),
        test_into_heterogeneous_type(),
        test_convert_valid(),
        test_convert_invalid(),
        test_set_mixed_type_propagation(),
        test_polymorphic_base(),
        test_into_error_message(),
        test_qin_type_safety(),
        test_lian_type_matching(),
        test_lian_type_mismatch(),
        test_virtual_destructor()
    };

    return !std::all_of(tests.begin(), tests.end(), [](int val) {
        return !val;
    });
}
