//
// Chainable API Example - Demonstrating fluent interface
//

#include "ZongHeng.h"
#include <iostream>

using namespace ZongHeng;

int main() {
    std::cout << "=== Chainable Combinators Demo ===" << std::endl << std::endl;

    // Example 1: Simple map chain
    std::cout << "1. Map Chain:" << std::endl;
    auto value = Qin<int>::make(10);
    auto result = value->map([](int x) { return x * 2; })
                       ->map([](int x) { return x + 5; });

    std::cout << "   value: " << value->get() << std::endl;
    std::cout << "   result (x*2+5): " << result->get() << std::endl;

    *value = 20;
    std::cout << "   after update: " << result->get() << std::endl << std::endl;

    // Example 2: Map + Filter chain
    std::cout << "2. Map + Filter Chain:" << std::endl;
    auto nums = Qin<int>::make(15);
    auto fallback = Qin<int>::make(0);

    auto filtered = nums->map([](int x) { return x - 10; })
                        ->filter(fallback, [](int x) { return x > 0; });

    std::cout << "   nums: " << nums->get() << std::endl;
    std::cout << "   filtered (x-10, positive): " << filtered->get() << std::endl;

    *nums = 5;
    std::cout << "   after update (5-10=-5): " << filtered->get() << std::endl << std::endl;

    // Example 3: Type conversion with map
    std::cout << "3. Type Conversion:" << std::endl;
    auto count = Qin<int>::make(42);
    auto message = count->map([](int x) {
        return "Count: " + std::to_string(x);
    });

    std::cout << "   " << message->get() << std::endl;

    *count = 100;
    std::cout << "   " << message->get() << std::endl << std::endl;

    // Example 4: Conditional with when
    std::cout << "4. Conditional Selection:" << std::endl;
    auto flag = Qin<bool>::make(true);
    auto optionA = Qin<int>::make(100);
    auto optionB = Qin<int>::make(200);

    auto selected = optionA->when(flag, optionB);

    std::cout << "   flag: true, selected: " << selected->get() << std::endl;

    *flag = false;
    std::cout << "   flag: false, selected: " << selected->get() << std::endl << std::endl;

    // Example 5: Complex chain
    std::cout << "5. Complex Chain:" << std::endl;
    auto input = Qin<int>::make(100);
    auto zero = Qin<int>::make(0);

    auto complex = input->map([](int x) { return x / 10; })      // 100 -> 10
                        ->map([](int x) { return x * 3; })       // 10 -> 30
                        ->filter(zero, [](int x) { return x < 50; })  // 30 < 50 -> 30
                        ->map([](int x) { return x + 10; });     // 30 -> 40

    std::cout << "   input: " << input->get() << ", result: " << complex->get() << std::endl;

    *input = 200;  // 200 -> 20 -> 60 (fails filter) -> 0 -> 10
    std::cout << "   input: " << input->get() << ", result: " << complex->get() << std::endl << std::endl;

    // Example 6: fold (not chainable, operates on multiple nodes)
    std::cout << "6. Fold (multi-node operation):" << std::endl;
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto c = Qin<int>::make(30);

    auto sum = fold({a, b, c}, 0, [](int acc, int x) {
        return acc + x;
    });

    std::cout << "   sum: " << sum->get() << std::endl;

    *b = 50;
    std::cout << "   after update: " << sum->get() << std::endl << std::endl;

    std::cout << "=== All examples completed ===" << std::endl;

    return 0;
}
