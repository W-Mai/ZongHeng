#include "Qin.h"
#include <iostream>
#include <vector>

template<typename T>
void display(std::vector<QinBase*>& vec) {
    std::for_each(vec.begin(), vec.end(), [](QinBase* item) {
        std::cout << as<T>(item).get() << " ";
    });
    std::cout << std::endl;
}

int main() {
    auto vec = std::vector<QinBase*> {};

    for (int i = 0; i < 10; i++) {
        vec.push_back(new Qin<int>(i));
    }
    display<int>(vec);

    for (int i = 1; i < vec.size(); ++i) {
        vec[i - 1] << *vec[i];
    }
    display<int>(vec);

    int a               = 4;
    as<int>(vec.back()) = a;
    display<int>(vec);

    as<int>(vec.back()) = 114514;
    display<int>(vec);

    vec.back()->into<int>() = 2333;
    display<int>(vec);

    auto& q = vec.back()->into<int>();
    q       = 42;
    display<int>(vec);

    return 0;
}
