# 纵横

ZongHeng（合纵连横）以“纵”为上游依赖、“横”为并列/派生组合，围绕秦（Qin）节点为枢纽，构建可组合的数据流关系图；Yi（易）表达输入到输出的强类型变换。

## 项目简介与命名
- Qin（秦）：同型节点封装，承载状态与组合；对应 `Qin<T>`。
- ZongHeng（纵横）：数据依赖关系图与组合方式（纵=来源，上游；横=并列/派生）。
- Yi（易）：变换节点，`Yi<IN,OUT>` 表达输入→输出映射，支持双向 `hook` 与派生 `effect`。

## 快速开始
- 构建与运行示例：
  - `mkdir -p build && cd build`
  - `cmake .. && make -j`
  - `./example`
- 运行测试：
  - `mkdir -p build && cd build`
  - `cmake .. && make -j`
  - `ctest`

## 使用示例

```c++
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

```

## 核心概念
- 节点：承载状态与变换的单元；`Qin<T>` 专注同型，`Yi<IN,OUT>` 专注异型。
- 纵横关系：通过组合/绑定建立依赖图；更新从上游向下游传播；读取遵循“`effect()`→当前值→`getter`”。
- 变换职责：`setter` 处理入站变换，`getter` 处理出站变换，`hook(getter, setter)` 同时配置，`effect` 表达派生计算，`convert` 兜底类型转换。

## 示例与测试引用
- 示例入口：`example/example.cpp:1-96`
- 数值组合测试：`test/plus_test.cpp`
- 字符串与向量映射测试：`test/plus_string_test.cpp`

## Commit 信息

| means    | emoji |
|----------|-------|
| feature  | ❤️    |
| fix      | 🤡    |
| format   | 🧾    |
| refactor | ⚙️    |
| update   | 🆕    |
| WIP      | ⌛️    |
