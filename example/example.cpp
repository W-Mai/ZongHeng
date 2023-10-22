#include "Qin.h"
#include <iostream>
#include <utility>

using namespace std;

class Text {
private:
    using _text_yi_t = Yi<string, string>;
    _text_yi_t::SharedYi_T _text;

public:
    explicit Text(const _text_yi_t::SharedYi_T& text) {
        _text = _text_yi_t::make();
        _text << text;

        _text->setter([](const string& s) {
            cout << "SETTER: " << s.length() << endl;
            return s;
        });
    }
};

int main() {
    auto hello_str = std::string("Hello ");
    auto hello     = Qin<string>::make(hello_str);
    auto name      = Qin<string>::make();
    auto mark      = Qin<string>::make(" !");

    auto init = {
        string { "Hello " }, string { "World" }
    };
    auto strings = Qin<vector<string>>::make(init);

    // 联合`变量`
    auto sentence = hello + name + mark;

    sentence->getter([](const string& s) {
        return s + " 🤤";
    });

    sentence->setter([](const string& s) {
        cout << "SETTER: " << s << endl;
        return s;
    });

    auto name_tmp = std::string("World");
    do {
        // 一呼百应
//        *name = name_tmp;
        cout << sentence->get() << endl;
    } while (cin >> name_tmp);

    //    auto hello = Qin<string>::make(hello_str);
    //    auto world = Yi<string, int>::make(hello_str);
    //
    //    Text text(hello);
    //
    //    *hello = string("Hello World !");
    //
    //    world << hello;
    //
    //    auto hello_world = world->lian(hello, [hello, world]() {
    //        return hello->get() + to_string(world->get());
    //    });
    //
    //    world->getter([](const string& s) {
    //        return s.length();
    //    });
    //
    //    hello_world->getter([](const string& s) {
    //        return s.length();
    //    });
    //
    //    cout << world->get() << endl;
    //    cout << hello_world->get() << endl;

    return 0;
}
