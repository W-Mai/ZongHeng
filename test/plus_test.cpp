//
// Created by W-Mai on 2022/10/30.
//

#include "Qin.h"
#include "test_utils.h"
#include <vector>

int test_binary_int_ops() {
    auto p = new Qin<int> { 10 };
    auto q = new Qin<int> { 30 };
    auto r = new Qin<int> { 37 };

    auto zh = *q + *p + *r;

    ASSERT_I(zh->get(), 10 + 30 + 37);
    *q = 3;
    ASSERT_I(zh->get(), 10 + 3 + 37);

    return 0;
}

int test_binary_double_ops() {
    auto p = new Qin<double> { 3.14 };
    auto q = new Qin<double> { 1 };
    auto r = new Qin<double> { -10 };

    auto zh = *q + *p + *r;

    ASSERT_F(zh->get(), 3.14 + 1 - 10);
    *p = 2.5;
    *q = -10;
    *r = 2.1;
    ASSERT_F(zh->get(), 2.5 - 10 + 2.1);

    return 0;
}

int test_binary_many_int_ops() {
    const int              NUM = 100;
    std::vector<Qin<int>*> operands;
    for (int i = 0; i < NUM; ++i) {
        operands.push_back(new Qin<int> { i });
    }

    auto zh = *operands.begin();
    for (int i = 1; i < operands.size(); ++i) {
        zh = zh + *operands[i];
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
