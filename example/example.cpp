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

void example_yi_vector_to_string() {
    cout << "=== Example 1: Yi vector<string> to string ===" << endl;

    auto init = {
        string { "Hello " }, string { "World" }
    };
    auto strings = Yi<vector<string>, string>::make(init);

    strings->hook(
        [](const vector<string>& vec) -> string {
            string ret;
            for (const auto& s : vec) {
                ret += s;
            }
            return ret;
        },
        [](const string& s) -> vector<string> {
            vector<string> ret;
            ret.reserve(s.length());
            for (size_t i = 0; i < s.length(); i += 2) {
                ret.emplace_back(1, s[i]);
            }
            return ret;
        });

    *strings = string("Hello World !");
    cout << strings->get() << endl;
    cout << endl;
}

void example_qin_composition() {
    cout << "=== Example 2: Qin composition with operators ===" << endl;

    auto hello_str = std::string("Hello ");
    auto hello     = Qin<string>::make(hello_str);
    auto name      = Qin<string>::make("World");
    auto mark      = Qin<string>::make(" !");

    // 联合`变量`
    auto sentence = hello + name + mark;

    sentence->getter([](const string& s) {
        return s + " 🤤";
    });

    cout << "Initial: " << sentence->get() << endl;

    // 一呼百应
    *name = string("China🇨🇳");
    cout << "After change: " << sentence->get() << endl;
    cout << endl;
}

void example_yi_type_transform() {
    cout << "=== Example 3: Yi type transformation (string to int) ===" << endl;

    auto hello_str = std::string("Hello World !");
    auto world     = Yi<string, int>::make(hello_str);

    // Yi<string, int> uses getter to transform string to int (length)
    world->getter([](const string& s) {
        cout << "  [getter] Converting string \"" << s << "\" to length" << endl;
        return static_cast<int>(s.length());
    });

    // Yi<string, int> uses setter to transform int back to string
    world->setter([](const int& len) {
        cout << "  [setter] Converting length " << len << " to string" << endl;
        return string(len, '*');
    });

    cout << "Getting value (triggers getter): " << world->get() << endl;

    // Set using the OUTPUT type (int)
    cout << "\nSetting value to 8 (triggers setter):" << endl;
    *world = 8;
    cout << "Getting new value: " << world->get() << endl;
    cout << endl;
}

int main() {
    example_yi_vector_to_string();
    example_qin_composition();
    example_yi_type_transform();

    return 0;
}
