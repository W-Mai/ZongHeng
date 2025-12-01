//
// Created by Claude Code for v2.0 refactoring
// Test combinators (map, filter, fold, when) functionality
//

#include "ZongHeng.h"
#include "test_utils.h"
#include <string>

using namespace ZongHeng;

// ============================================================================
// map Tests
// ============================================================================

int test_map_transform() {
    auto nums = Qin<int>::make(10);
    auto doubled = map(nums, [](int x) { return x * 2; });

    ASSERT_I(doubled->get(), 20);

    return 0;
}

int test_map_reactive() {
    auto nums = Qin<int>::make(10);
    auto doubled = map(nums, [](int x) { return x * 2; });

    ASSERT_I(doubled->get(), 20);

    *nums = 15;
    ASSERT_I(doubled->get(), 30);

    return 0;
}

int test_map_type_conversion() {
    auto nums = Qin<int>::make(42);
    auto str = map(nums, [](int x) {
        return std::to_string(x);
    });

    ASSERT_S(str->get(), std::string("42"));

    *nums = 100;
    ASSERT_S(str->get(), std::string("100"));

    return 0;
}

// ============================================================================
// filter Tests
// ============================================================================

int test_filter_predicate_true() {
    auto value = Qin<int>::make(15);
    auto fallback = Qin<int>::make(0);
    auto positive = filter(value, fallback, [](int x) { return x > 0; });

    ASSERT_I(positive->get(), 15);

    return 0;
}

int test_filter_predicate_false() {
    auto value = Qin<int>::make(-5);
    auto fallback = Qin<int>::make(0);
    auto positive = filter(value, fallback, [](int x) { return x > 0; });

    ASSERT_I(positive->get(), 0);

    return 0;
}

int test_filter_reactive() {
    auto value = Qin<int>::make(15);
    auto fallback = Qin<int>::make(0);
    auto positive = filter(value, fallback, [](int x) { return x > 0; });

    ASSERT_I(positive->get(), 15);

    *value = -10;
    ASSERT_I(positive->get(), 0);

    *value = 20;
    ASSERT_I(positive->get(), 20);

    return 0;
}

// ============================================================================
// fold Tests
// ============================================================================

int test_fold_accumulate() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto c = Qin<int>::make(30);

    auto sum = fold({a, b, c}, 0, [](int acc, int x) {
        return acc + x;
    });

    ASSERT_I(sum->get(), 60);

    return 0;
}

int test_fold_reactive() {
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto c = Qin<int>::make(30);

    auto sum = fold({a, b, c}, 0, [](int acc, int x) {
        return acc + x;
    });

    ASSERT_I(sum->get(), 60);

    *a = 100;
    ASSERT_I(sum->get(), 150);  // 100 + 20 + 30

    *b = 5;
    ASSERT_I(sum->get(), 135);  // 100 + 5 + 30

    return 0;
}

int test_fold_multiply() {
    auto a = Qin<int>::make(2);
    auto b = Qin<int>::make(3);
    auto c = Qin<int>::make(4);

    auto product = fold({a, b, c}, 1, [](int acc, int x) {
        return acc * x;
    });

    ASSERT_I(product->get(), 24);  // 2 * 3 * 4

    return 0;
}

// ============================================================================
// when Tests
// ============================================================================

int test_when_condition_true() {
    auto flag = Qin<bool>::make(true);
    auto optionA = Qin<int>::make(100);
    auto optionB = Qin<int>::make(200);

    auto selected = when(flag, optionA, optionB);

    ASSERT_I(selected->get(), 100);

    return 0;
}

int test_when_condition_false() {
    auto flag = Qin<bool>::make(false);
    auto optionA = Qin<int>::make(100);
    auto optionB = Qin<int>::make(200);

    auto selected = when(flag, optionA, optionB);

    ASSERT_I(selected->get(), 200);

    return 0;
}

int test_when_reactive() {
    auto flag = Qin<bool>::make(true);
    auto optionA = Qin<int>::make(100);
    auto optionB = Qin<int>::make(200);

    auto selected = when(flag, optionA, optionB);

    ASSERT_I(selected->get(), 100);

    *flag = false;
    ASSERT_I(selected->get(), 200);

    *flag = true;
    ASSERT_I(selected->get(), 100);

    // Test value changes
    *optionA = 300;
    ASSERT_I(selected->get(), 300);

    *flag = false;
    *optionB = 400;
    ASSERT_I(selected->get(), 400);

    return 0;
}

// ============================================================================
// Chainable API Tests
// ============================================================================

int test_chainable_map() {
    auto nums = Qin<int>::make(10);
    auto doubled = nums->map([](int x) { return x * 2; });

    ASSERT_I(doubled->get(), 20);

    *nums = 15;
    ASSERT_I(doubled->get(), 30);

    return 0;
}

int test_chainable_map_filter() {
    auto nums = Qin<int>::make(10);
    auto fallback = Qin<int>::make(0);

    auto result = nums->map([](int x) { return x - 5; })
                      ->filter(fallback, [](int x) { return x > 0; });

    ASSERT_I(result->get(), 5);  // 10 - 5 = 5 (passes filter)

    *nums = 3;
    ASSERT_I(result->get(), 0);  // 3 - 5 = -2 (fails filter, use fallback)

    return 0;
}

int test_chainable_complex() {
    auto nums = Qin<int>::make(100);
    auto fallback = Qin<int>::make(1);

    auto result = nums->map([](int x) { return x / 10; })      // 100 -> 10
                      ->map([](int x) { return x * 2; })       // 10 -> 20
                      ->filter(fallback, [](int x) { return x < 50; });  // 20 < 50 -> 20

    ASSERT_I(result->get(), 20);

    *nums = 300;  // 300 -> 30 -> 60 (fails filter) -> 1
    ASSERT_I(result->get(), 1);

    return 0;
}

int test_chainable_when() {
    auto flag = Qin<bool>::make(true);
    auto optionA = Qin<int>::make(100);
    auto optionB = Qin<int>::make(200);

    auto selected = optionA->when(flag, optionB);

    ASSERT_I(selected->get(), 100);

    *flag = false;
    ASSERT_I(selected->get(), 200);

    return 0;
}

// ============================================================================
// Main
// ============================================================================

int main() {
    auto tests = {
        // map
        test_map_transform(),
        test_map_reactive(),
        test_map_type_conversion(),
        // filter
        test_filter_predicate_true(),
        test_filter_predicate_false(),
        test_filter_reactive(),
        // fold
        test_fold_accumulate(),
        test_fold_reactive(),
        test_fold_multiply(),
        // when
        test_when_condition_true(),
        test_when_condition_false(),
        test_when_reactive(),
        // chainable API
        test_chainable_map(),
        test_chainable_map_filter(),
        test_chainable_complex(),
        test_chainable_when()
    };

    return !std::all_of(tests.begin(), tests.end(), [](int val) {
        return !val;
    });
}
