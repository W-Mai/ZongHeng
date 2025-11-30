//
// Created by Claude Code for v2.0 refactoring
// Test extended operators functionality
//

#include "ZongHeng.h"
#include "test_utils.h"

// ============================================================================
// Arithmetic Operators Tests
// ============================================================================

int test_arithmetic_subtraction() {
    auto a = Qin<int>::make(50);
    auto b = Qin<int>::make(20);
    auto result = a - b;

    ASSERT_I(result->get(), 30);

    *a = 100;
    ASSERT_I(result->get(), 80);

    return 0;
}

int test_arithmetic_division() {
    auto a = Qin<double>::make(100.0);
    auto b = Qin<double>::make(4.0);
    auto result = a / b;

    ASSERT_F(result->get(), 25.0);

    *b = 5.0;
    ASSERT_F(result->get(), 20.0);

    return 0;
}

int test_arithmetic_modulus() {
    auto a = Qin<int>::make(17);
    auto b = Qin<int>::make(5);
    auto result = a % b;

    ASSERT_I(result->get(), 2);

    *a = 23;
    ASSERT_I(result->get(), 3);

    return 0;
}

// ============================================================================
// Comparison Operators Tests
// ============================================================================

int test_comparison_equal() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(10);
    auto result = (a == b);

    ASSERT_I(result->get(), 1);  // true

    *b = 20;
    ASSERT_I(result->get(), 0);  // false

    return 0;
}

int test_comparison_not_equal() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto result = (a != b);

    ASSERT_I(result->get(), 1);  // true

    *b = 10;
    ASSERT_I(result->get(), 0);  // false

    return 0;
}

int test_comparison_less_than() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto result = (a < b);

    ASSERT_I(result->get(), 1);  // true

    *a = 30;
    ASSERT_I(result->get(), 0);  // false

    return 0;
}

int test_comparison_greater_than() {
    auto a = Qin<int>::make(30);
    auto b = Qin<int>::make(20);
    auto result = (a > b);

    ASSERT_I(result->get(), 1);  // true

    *a = 10;
    ASSERT_I(result->get(), 0);  // false

    return 0;
}

int test_comparison_less_equal() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto result = (a <= b);

    ASSERT_I(result->get(), 1);  // true

    *a = 20;
    ASSERT_I(result->get(), 1);  // still true

    *a = 30;
    ASSERT_I(result->get(), 0);  // false

    return 0;
}

int test_comparison_greater_equal() {
    auto a = Qin<int>::make(30);
    auto b = Qin<int>::make(20);
    auto result = (a >= b);

    ASSERT_I(result->get(), 1);  // true

    *a = 20;
    ASSERT_I(result->get(), 1);  // still true

    *a = 10;
    ASSERT_I(result->get(), 0);  // false

    return 0;
}

// ============================================================================
// Bitwise Operators Tests
// ============================================================================

int test_bitwise_and() {
    auto a = Qin<int>::make(0b1100);  // 12
    auto b = Qin<int>::make(0b1010);  // 10
    auto result = a & b;

    ASSERT_I(result->get(), 0b1000);  // 8

    *a = 0b1111;
    ASSERT_I(result->get(), 0b1010);  // 10

    return 0;
}

int test_bitwise_or() {
    auto a = Qin<int>::make(0b1100);  // 12
    auto b = Qin<int>::make(0b1010);  // 10
    auto result = a | b;

    ASSERT_I(result->get(), 0b1110);  // 14

    *b = 0b0011;
    ASSERT_I(result->get(), 0b1111);  // 15

    return 0;
}

int test_bitwise_xor() {
    auto a = Qin<int>::make(0b1100);  // 12
    auto b = Qin<int>::make(0b1010);  // 10
    auto result = a ^ b;

    ASSERT_I(result->get(), 0b0110);  // 6

    *a = 0b1111;
    ASSERT_I(result->get(), 0b0101);  // 5

    return 0;
}

// ============================================================================
// Unary Operators Tests
// ============================================================================

int test_unary_negation() {
    auto a = Qin<int>::make(42);
    auto result = -a;

    ASSERT_I(result->get(), -42);

    *a = -10;
    ASSERT_I(result->get(), 10);

    return 0;
}

int test_unary_bitwise_not() {
    auto a = Qin<int>::make(0);
    auto result = ~a;

    ASSERT_I(result->get(), -1);  // All bits flipped

    *a = -1;
    ASSERT_I(result->get(), 0);

    return 0;
}

int test_unary_logical_not() {
    auto a = Qin<int>::make(42);
    auto result = (!a);

    ASSERT_I(result->get(), 0);  // false

    *a = 0;
    ASSERT_I(result->get(), 1);  // true

    return 0;
}

// ============================================================================
// Complex Expression Tests
// ============================================================================

int test_complex_arithmetic_expression() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(5);
    auto c = Qin<int>::make(2);

    // (a - b) * c
    auto result = (a - b) * c;

    ASSERT_I(result->get(), 10);  // (10 - 5) * 2 = 10

    *b = 3;
    ASSERT_I(result->get(), 14);  // (10 - 3) * 2 = 14

    return 0;
}

int test_comparison_chain() {
    auto a = Qin<int>::make(5);
    auto b = Qin<int>::make(10);
    auto c = Qin<int>::make(15);

    auto less = (a < b);
    auto greater = (c > b);

    ASSERT_I(less->get(), 1);     // true
    ASSERT_I(greater->get(), 1);  // true

    *b = 20;
    ASSERT_I(less->get(), 1);     // still true
    ASSERT_I(greater->get(), 0);  // now false

    return 0;
}

// ============================================================================
// Zong/Heng API Tests
// ============================================================================

int test_zong_heng_accessors() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto sum = a + b;

    // Verify dependency graph structure
    ASSERT_I(sum->getZongCount(), 0);  // sum is not bidirectionally bound
    ASSERT_I(sum->getHengCount(), 0);  // nothing is derived from sum yet

    // a and b should have sum in their Heng (sum recomputes when a or b change)
    ASSERT_I(a->getHengCount(), 1);
    ASSERT_I(b->getHengCount(), 1);

    return 0;
}

int main() {
    auto tests = {
        // Arithmetic
        test_arithmetic_subtraction(),
        test_arithmetic_division(),
        test_arithmetic_modulus(),
        // Comparison
        test_comparison_equal(),
        test_comparison_not_equal(),
        test_comparison_less_than(),
        test_comparison_greater_than(),
        test_comparison_less_equal(),
        test_comparison_greater_equal(),
        // Bitwise
        test_bitwise_and(),
        test_bitwise_or(),
        test_bitwise_xor(),
        // Unary
        test_unary_negation(),
        test_unary_bitwise_not(),
        test_unary_logical_not(),
        // Complex
        test_complex_arithmetic_expression(),
        test_comparison_chain(),
        // API
        test_zong_heng_accessors()
    };

    return !std::all_of(tests.begin(), tests.end(), [](int val) {
        return !val;
    });
}
