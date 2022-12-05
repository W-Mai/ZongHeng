#include "Qin.h"
#include <iostream>

using namespace std;

int main() {
    auto hello_str = std::string("Hello ");
    auto hello     = Qin<string>::make(hello_str);
    auto name      = Qin<string>::make();
    auto mark      = Qin<string>::make(" !");
    // 联合`变量`
    auto sentence  = hello + name + mark;

    sentence->getter([](const string& s) {
        return s + " 🤤";
    });

    auto name_tmp = std::string("World");
    do {
        // 一呼百应
        *name = name_tmp;
        cout << sentence->get() << endl;
    } while (cin >> name_tmp);

    return 0;
}
