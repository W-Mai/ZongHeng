//
// Created by W-Mai on 2022/10/30.
//

#include "ZongHeng.h"
#include "test_utils.h"
#include <string>
#include <vector>

int test_binary_string_ops() {
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

int test_string_vec() {
    auto list = {
        std::string { "Hello " },
        std::string { "World " },
        std::string { "!" }
    };

    auto p = Qin<std::vector<std::string>>::make(list);

    p->getter([](const std::vector<std::string>& vec) -> std::vector<std::string> {
        std::string ret;
        for (const auto& s : vec) {
            ret += s;
        }
        return std::vector<std::string> { ret };
    });

    ASSERT_S(p->get()[0], std::string("Hello World !"));

    return 0;
}

int main() {
    auto tests = {
        test_binary_string_ops(),
        test_string_vec()
    };

    return !std::all_of(tests.begin(), tests.end(), [](int val) {
        return !val;
    });
}
