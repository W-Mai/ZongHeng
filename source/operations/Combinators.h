//
// Combinators - Functional programming utilities for ZongHeng
//

#ifndef ZONGHENG_OPERATIONS_COMBINATORS_H
#define ZONGHENG_OPERATIONS_COMBINATORS_H

#include "../nodes/Qin.h"
#include <vector>

namespace ZongHeng {

// ============================================================================
// map - Transform values through a function
// ============================================================================

/**
 * @brief Map a Qin node through a transformation function
 *
 * Free function version - calls member function internally.
 *
 * @param source Source Qin node
 * @param fn Transformation function (IN -> OUT)
 * @return New Qin node with transformed value
 *
 * @example
 * auto nums = Qin<int>::make(10);
 * auto doubled = map(nums, [](int x) { return x * 2; });  // 20
 */
template<class IN, class Fn>
auto map(std::shared_ptr<Qin<IN>> source, Fn fn)
    -> std::shared_ptr<Qin<decltype(fn(std::declval<IN>()))>> {
    return source->map(fn);
}

// ============================================================================
// filter - Conditionally filter values
// ============================================================================

/**
 * @brief Filter a Qin node based on a predicate
 *
 * Free function version - calls member function internally.
 *
 * @param source Source Qin node
 * @param defaultVal Default value to use when predicate fails
 * @param predicate Boolean function (T -> bool)
 * @return New Qin node with filtered value
 *
 * @example
 * auto value = Qin<int>::make(15);
 * auto fallback = Qin<int>::make(0);
 * auto positive = filter(value, fallback, [](int x) { return x > 0; });
 */
template<class T, class Pred>
std::shared_ptr<Qin<T>> filter(std::shared_ptr<Qin<T>> source,
                                std::shared_ptr<Qin<T>> defaultVal,
                                Pred predicate) {
    return source->filter(defaultVal, predicate);
}

// ============================================================================
// fold - Reduce multiple nodes to a single value
// ============================================================================

/**
 * @brief Fold (reduce) multiple Qin nodes into a single value
 *
 * Creates a new reactive node that combines multiple source nodes using a
 * binary function and initial value. Updates automatically when any source
 * node changes.
 *
 * @param sources Vector of source Qin nodes
 * @param initial Initial accumulator value
 * @param combine Binary function (acc, value) -> acc
 * @return New Qin node with folded value
 *
 * @example
 * auto a = Qin<int>::make(10);
 * auto b = Qin<int>::make(20);
 * auto c = Qin<int>::make(30);
 * auto sum = fold({a, b, c}, 0, [](int acc, int x) { return acc + x; });
 */
template<class T, class Fn>
std::shared_ptr<Qin<T>> fold(const std::vector<std::shared_ptr<Qin<T>>>& sources,
                              T initial, Fn combine) {
    auto result = Qin<T>::make(initial);

    // Connect all source nodes to result (friend access to protected method)
    for (const auto& src : sources) {
        src->addDerivedNode(result);
    }

    result->setEff([sources, initial, combine]() -> T {
        T acc = initial;
        for (const auto& src : sources) {
            acc = combine(acc, src->get());
        }
        return acc;
    });
    return result;
}

// ============================================================================
// when - Conditional branch selection
// ============================================================================

/**
 * @brief Select between two values based on a boolean condition
 *
 * Free function version - calls member function internally.
 *
 * @param condition Boolean Qin node
 * @param trueVal Value to return when condition is true
 * @param falseVal Value to return when condition is false
 * @return New Qin node with selected value
 *
 * @example
 * auto flag = Qin<bool>::make(true);
 * auto optionA = Qin<int>::make(100);
 * auto optionB = Qin<int>::make(200);
 * auto selected = when(flag, optionA, optionB);  // 100
 */
template<class T>
std::shared_ptr<Qin<T>> when(std::shared_ptr<Qin<bool>> condition,
                              std::shared_ptr<Qin<T>> trueVal,
                              std::shared_ptr<Qin<T>> falseVal) {
    return trueVal->when(condition, falseVal);
}

} // namespace ZongHeng

#endif // ZONGHENG_OPERATIONS_COMBINATORS_H
