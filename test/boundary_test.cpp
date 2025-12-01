//
// Created by Claude Code
// Boundary Value Tests - Testing extreme values and limits
//

#include "ZongHeng.h"
#include "test_utils.h"
#include <iostream>
#include <limits>
#include <cmath>

using namespace ZongHeng;

// ============================================================================
// Integer Boundary Tests
// ============================================================================

int test_integer_overflow() {
    auto max_val = Qin<int>::make(std::numeric_limits<int>::max());
    auto one = Qin<int>::make(1);

    // Overflow: max + 1 (undefined behavior in C++, but framework should handle)
    auto overflow = max_val + one;

    // Document the behavior (will wrap around in most implementations)
    int result = overflow->get();
    std::cout << "  max_int + 1 = " << result << " (overflow wrapped)" << std::endl;

    return 0;
}

int test_integer_underflow() {
    auto min_val = Qin<int>::make(std::numeric_limits<int>::min());
    auto one = Qin<int>::make(1);

    // Underflow: min - 1
    auto underflow = min_val - one;

    int result = underflow->get();
    std::cout << "  min_int - 1 = " << result << " (underflow wrapped)" << std::endl;

    return 0;
}

int test_large_multiplication() {
    auto large1 = Qin<int>::make(1000000);
    auto large2 = Qin<int>::make(1000000);

    // 1000000 * 1000000 = 1000000000000 (overflows int32)
    auto product = large1 * large2;

    int result = product->get();
    std::cout << "  1000000 * 1000000 = " << result << " (overflow)" << std::endl;

    return 0;
}

// ============================================================================
// Floating Point Boundary Tests
// ============================================================================

int test_float_infinity() {
    auto finite = Qin<double>::make(std::numeric_limits<double>::max());
    auto two = Qin<double>::make(2.0);

    auto infinity = finite * two;

    double result = infinity->get();
    if (std::isinf(result)) {
        std::cout << "  ✓ max_double * 2 = infinity" << std::endl;
    } else {
        std::cout << "  Result: " << result << std::endl;
    }

    return 0;
}

int test_float_nan() {
    auto zero = Qin<double>::make(0.0);
    auto result_node = zero / zero;

    double result = result_node->get();
    if (std::isnan(result)) {
        std::cout << "  ✓ 0.0 / 0.0 = NaN" << std::endl;
    } else {
        std::cout << "  Unexpected result: " << result << std::endl;
    }

    return 0;
}

int test_float_precision() {
    auto small = Qin<double>::make(0.1);
    auto ten = Qin<double>::make(10.0);

    // 0.1 + 0.1 + ... (10 times) vs 1.0
    auto sum = small;
    for (int i = 1; i < 10; i++) {
        sum = sum + small;
    }

    double result = sum->get();
    double expected = 1.0;
    double diff = std::abs(result - expected);

    std::cout << "  0.1 * 10 = " << result
              << " (diff from 1.0: " << diff << ")" << std::endl;

    return 0;
}

// ============================================================================
// String Boundary Tests
// ============================================================================

int test_empty_string() {
    auto empty1 = Qin<std::string>::make("");
    auto empty2 = Qin<std::string>::make("");

    auto concat = empty1 + empty2;

    ASSERT_S(concat->get(), std::string(""));

    std::cout << "  ✓ Empty string concatenation works" << std::endl;

    return 0;
}

int test_large_string() {
    // Create a large string (1MB)
    std::string large_str(1024 * 1024, 'A');
    auto large = Qin<std::string>::make(large_str);
    auto suffix = Qin<std::string>::make("_END");

    auto concat = large + suffix;

    std::string result = concat->get();
    if (result.size() == 1024 * 1024 + 4 &&
        result.substr(result.size() - 4) == "_END") {
        std::cout << "  ✓ Large string (1MB) concatenation works" << std::endl;
    } else {
        std::cout << "  ERROR: Large string concatenation failed" << std::endl;
        return -1;
    }

    return 0;
}

// ============================================================================
// Chain Depth Tests
// ============================================================================

int test_deep_chain() {
    auto value = Qin<int>::make(0);
    auto current = value;

    // Create a chain of 100 map operations
    constexpr int CHAIN_DEPTH = 100;
    for (int i = 0; i < CHAIN_DEPTH; i++) {
        current = current->map([](int x) { return x + 1; });
    }

    ASSERT_I(current->get(), CHAIN_DEPTH);

    std::cout << "  ✓ Deep chain (100 levels) works correctly" << std::endl;

    return 0;
}

int test_wide_fold() {
    // Create 1000 nodes and fold them
    constexpr int NUM_NODES = 1000;
    std::vector<std::shared_ptr<Qin<int>>> nodes;

    for (int i = 0; i < NUM_NODES; i++) {
        nodes.push_back(Qin<int>::make(1));
    }

    auto sum = fold(nodes, 0, [](int acc, int x) {
        return acc + x;
    });

    ASSERT_I(sum->get(), NUM_NODES);

    std::cout << "  ✓ Wide fold (1000 nodes) works correctly" << std::endl;

    return 0;
}

// ============================================================================
// Boolean Edge Cases
// ============================================================================

int test_boolean_chains() {
    auto true_val = Qin<bool>::make(true);
    auto false_val = Qin<bool>::make(false);

    // Test all boolean combinations
    auto and_result = true_val & false_val;
    auto or_result = true_val | false_val;
    auto xor_result = true_val ^ false_val;

    if (and_result->get() == false &&
        or_result->get() == true &&
        xor_result->get() == true) {
        std::cout << "  ✓ Boolean operations work correctly" << std::endl;
    } else {
        std::cout << "  ERROR: Boolean operations failed" << std::endl;
        return -1;
    }

    return 0;
}

// ============================================================================
// Comparison Edge Cases
// ============================================================================

int test_comparison_chains() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto c = Qin<int>::make(10);

    auto eq = a == c;
    auto ne = a != b;
    auto lt = a < b;
    auto gt = b > a;
    auto le = a <= c;
    auto ge = a >= c;

    if (eq->get() && ne->get() && lt->get() &&
        gt->get() && le->get() && ge->get()) {
        std::cout << "  ✓ All comparison operators work correctly" << std::endl;
    } else {
        std::cout << "  ERROR: Comparison operators failed" << std::endl;
        return -1;
    }

    return 0;
}

// ============================================================================
// Test Runner
// ============================================================================

int main() {
    std::cout << "=== Boundary Value Tests ===" << std::endl;

    std::cout << "\n--- Integer Boundaries ---" << std::endl;
    if (test_integer_overflow() != 0) return -1;
    if (test_integer_underflow() != 0) return -1;
    if (test_large_multiplication() != 0) return -1;

    std::cout << "\n--- Floating Point Boundaries ---" << std::endl;
    if (test_float_infinity() != 0) return -1;
    if (test_float_nan() != 0) return -1;
    if (test_float_precision() != 0) return -1;

    std::cout << "\n--- String Boundaries ---" << std::endl;
    if (test_empty_string() != 0) return -1;
    if (test_large_string() != 0) return -1;

    std::cout << "\n--- Chain Depth & Width ---" << std::endl;
    if (test_deep_chain() != 0) return -1;
    if (test_wide_fold() != 0) return -1;

    std::cout << "\n--- Boolean & Comparison ---" << std::endl;
    if (test_boolean_chains() != 0) return -1;
    if (test_comparison_chains() != 0) return -1;

    std::cout << "\n✅ All boundary tests completed!" << std::endl;
    return 0;
}
