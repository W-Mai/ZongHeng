//
// Created by W-Mai on 2022/10/30.
//

#include "Qin.h"
#include "test_utils.h"
#include <string>
#include <vector>

int test_binary_int_ops() {
    auto p = Qin<std::string>::make("Hello ");
    auto q = Qin<std::string>::make("World ");
    auto r = Qin<std::string>::make("!");

    auto zh = p + q + r;

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
