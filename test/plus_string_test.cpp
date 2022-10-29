//
// Created by W-Mai on 2022/10/30.
//

#include "Qin.h"
#include "test_utils.h"
#include <string>
#include <vector>

int test_binary_int_ops() {
    auto p = new Qin<std::string> { "Hello " };
    auto q = new Qin<std::string> { "World " };
    auto r = new Qin<std::string> { "!" };

    auto zh = *p + *q + *r;

    ASSERT_S(zh->get(), std::string("Hello World !"));

    auto s = std::string("China🇨🇳 ");
    *q     = s;

    ASSERT_S(zh->get(), std::string("Hello China🇨🇳 !"));
    return 0;
}

int main() {
    auto tests = {
        test_binary_int_ops(),
    };

    return !std::all_of(tests.begin(), tests.end(), [](int val) {
        return !val;
    });
}
