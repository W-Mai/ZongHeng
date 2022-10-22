#include "Qin.h"
#include <vector>

int main() {

    for (auto* _ : std::vector<QinBase*> {
             new Qin<int>(114514),
             new Qin<std::string>("Hello, "),
             new Qin<const char*>("Qin"),
             new Qin<double>(2333) }) {
        _->print();
    }

    return 0;
}
