# 纵横

ZongHeng is opposite theories of vertical and horizontal alliance of states with the state of Qin as pivot in Chinese
history.

# General Usage

```c++
#include "Qin.h"
#include <iostream>

using namespace std;

int main() {
    auto hello    = new Qin<string> { "Hello " };
    auto name     = new Qin<string> { "" };
    auto mark     = new Qin<string> { " !" };
    // 联合`变量`
    auto sentence = *hello + *name + *mark;

    auto name_tmp = std::string("World");
    do {
        // 一呼百应
        *name = name_tmp;
        cout << sentence->get() << endl;
    } while (cin >> name_tmp);

    return 0;
}
```

# Run Example

```shell
mkdir -p build && cd build
cmake .. && make
./example
```

# Commit Information

| means    | emoji |
|----------|-------|
| feature  | ❤️    |
| fix      | 🤡    |
| format   | 🧾    |
| refactor | ⚙️    |
| update   | 🆕    |
