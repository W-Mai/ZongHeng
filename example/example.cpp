#include "Qin.h"
#include <iostream>
#include <vector>

template<typename T>
void display(std::vector<QinBase*>& vec) {
    for (QinBase* item : vec) {
        std::cout << static_cast<Qin<T>*>(item)->get() << " ";
    }
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

    int a                    = 4;
    *(Qin<int>*)(vec.back()) = a;
    display<int>(vec);

    *(Qin<int>*)(vec.back()) = 114514;
    display<int>(vec);

    return 0;
}
