//
// Created by W-Mai on 2022/10/30.
//

#include "Qin.h"
#include "test_utils.h"
#include <vector>

int test_binary_int_ops() {
    auto p = Qin<int>::make(10);
    auto q = Qin<int>::make(30);
    auto r = Qin<int>::make(37);

    auto zh = q + p + r;

    ASSERT_I(zh->get(), 10 + 30 + 37);
    *q = 3;
    ASSERT_I(zh->get(), 10 + 3 + 37);

    return 0;
}

int test_binary_double_ops() {
    auto p = Qin<double>::make(3.14);
    auto q = Qin<double>::make(1);
    auto r = Qin<double>::make(-10);

    auto zh = q + p + r;

    ASSERT_F(zh->get(), 3.14 + 1 - 10);
    *p = 2.5;
    *q = -10;
    *r = 2.1;
    ASSERT_F(zh->get(), 2.5 - 10 + 2.1);

    return 0;
}

int test_binary_many_int_ops() {
    const int                          NUM = 100;
    std::vector<Qin<int>::SharedQin_T> operands;
    for (int i = 0; i < NUM; ++i) {
        operands.push_back(Qin<int>::make(i));
    }

    auto zh = *operands.begin();
    for (int i = 1; i < operands.size(); ++i) {
        zh = zh + operands[i];
    }

    ASSERT_I(zh->get(), 4950);

    *operands[1] = 20;
    ASSERT_I(zh->get(), 4950 - 1 + 20);
    return 0;
}

int main() {
    auto tests = {
        test_binary_int_ops(),
        test_binary_double_ops(),
        test_binary_many_int_ops()
    };

    return !std::all_of(tests.begin(), tests.end(), [](int val) {
        return !val;
    });
}
