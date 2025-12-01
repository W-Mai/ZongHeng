# 纵横

ZongHeng（合纵连横）以"纵"为上游依赖、"横"为并列/派生组合，围绕秦（Qin）节点为枢纽，构建可组合的数据流关系图；Yi（易）表达输入到输出的强类型变换。

## 特性

- ✅ **类型安全**：C++17 运行时类型验证
- ✅ **完整运算符**：支持 `+, -, *, /, %, ==, <, >, &, |, ^` 等
- ✅ **链式 API**：`map()->filter()->when()` 函数式编程
- ✅ **高阶组合**：`map, filter, fold, when` 函数式工具
- ✅ **依赖图查询**：`getZong/getHeng` 完整的关系图 API

## 项目简介与命名
- Qin（秦）：同型节点封装，承载状态与组合；对应 `Qin<T>`。
- ZongHeng（纵横）：数据依赖关系图与组合方式（纵=来源，上游；横=并列/派生）。
- Yi（易）：变换节点，`Yi<IN,OUT>` 表达输入→输出映射，支持双向 `hook` 与派生 `effect`。

## 快速开始

### 编译器要求
- C++17 或更高（GCC 7+, Clang 5+, MSVC 2017+）

### 构建与运行
```bash
mkdir -p build && cd build
cmake .. && make -j

# 运行基础示例
./example

# 运行链式 API 示例
./chainable_example

# 运行所有测试
ctest
```

## 使用示例

### 基础用法

```cpp
#include "ZongHeng.h"
#include <iostream>

int main() {
    auto hello = Qin<std::string>::make("Hello ");
    auto name  = Qin<std::string>::make();
    auto mark  = Qin<std::string>::make(" !");

    // 联合"变量" - 响应式组合
    auto sentence = hello + name + mark;

    sentence->getter([](const std::string& s) {
        return s + " 🤤";
    });

    auto name_tmp = std::string("World");
    do {
        *name = name_tmp;  // 一呼百应 - 自动更新
        std::cout << sentence->get() << std::endl;
    } while (std::cin >> name_tmp);

    return 0;
}
```

### 链式 API

```cpp
#include "ZongHeng.h"
using namespace ZongHeng;

int main() {
    auto value = Qin<int>::make(100);
    auto fallback = Qin<int>::make(0);

    // 链式组合：map -> filter -> map
    auto result = value->map([](int x) { return x / 10; })               // 100 -> 10
                       ->filter(fallback, [](int x) { return x < 50; })  // 10 < 50 -> 10
                       ->map([](int x) { return x * 3; });               // 10 -> 30

    std::cout << result->get() << std::endl;  // 输出: 30

    *value = 600;  // 响应式更新
    std::cout << result->get() << std::endl;  // 输出: 0 (600->60, 失败过滤)

    return 0;
}
```

### 函数式组合

```cpp
#include "ZongHeng.h"
using namespace ZongHeng;

int main() {
    // fold: 聚合多个节点
    auto a = Qin<int>::make(10);
    auto b = Qin<int>::make(20);
    auto c = Qin<int>::make(30);

    auto sum = fold({a, b, c}, 0, [](int acc, int x) {
        return acc + x;
    });

    std::cout << sum->get() << std::endl;  // 60

    // when: 条件选择
    auto flag = Qin<bool>::make(true);
    auto optionA = Qin<int>::make(100);
    auto optionB = Qin<int>::make(200);

    auto selected = optionA->when(flag, optionB);
    std::cout << selected->get() << std::endl;  // 100

    *flag = false;
    std::cout << selected->get() << std::endl;  // 200

    return 0;
}
```

## 核心概念

### 节点类型
- **`Qin<T>`**：同型节点，输入输出类型相同
- **`Yi<IN,OUT>`**：异型节点，支持类型转换

### 依赖关系（纵横）
- **纵（Zong）**：双向绑定关系（`operator<<`）
- **横（Heng）**：派生依赖关系（响应式更新）
- 更新传播：从上游向下游（Heng）自动传播
- 查询 API：`getZong()`, `getHeng()`, `getZongCount()`, `getHengCount()`

### 运算符与组合
- **算术**：`+, -, *, /, %`
- **比较**：`==, !=, <, >, <=, >=`（返回 `Qin<bool>`）
- **位运算**：`&, |, ^, ~`
- **一元**：`-, !, ~`
- **链式组合**：`map()`, `filter()`, `when()`
- **聚合**：`fold()`

### 变换机制
- **setter**：入站变换（OUTPUT → INPUT）
- **getter**：出站变换（INPUT → OUTPUT）
- **hook**：同时配置 getter 和 setter
- **effect**：派生计算（从依赖节点计算值）
- 读取优先级：`effect()` → 当前值 → `getter()`

## API 文档

### 基础 API
```cpp
// 创建节点
auto node = Qin<int>::make(42);

// 读写值
int value = node->get();
*node = 100;

// 运算符组合
auto sum = node1 + node2;
auto doubled = node * Qin<int>::make(2);
```

### 链式 API
```cpp
// map: 转换值
auto doubled = nums->map([](int x) { return x * 2; });

// filter: 条件过滤
auto positive = value->filter(fallback, [](int x) { return x > 0; });

// when: 条件选择
auto selected = optionA->when(condition, optionB);

// 链式组合
auto result = value->map(f1)->filter(def, pred)->map(f2);
```

### 聚合 API
```cpp
// fold: 多节点归约
auto sum = fold({a, b, c}, 0, [](int acc, int x) {
    return acc + x;
});
```

### 依赖图查询
```cpp
// 查询依赖关系
size_t zong_count = node->getZongCount();  // 双向绑定数量
size_t heng_count = node->getHengCount();  // 派生节点数量
const auto& hengs = node->getHeng();       // 获取派生节点列表
```

## 示例与测试
- 基础示例：`example/example.cpp`
- 链式 API 示例：`example/chainable_example.cpp`
- 测试套件：
  - `test/plus_test.cpp` - 数值组合
  - `test/plus_string_test.cpp` - 字符串操作
  - `test/lian_test.cpp` - 依赖关系
  - `test/yi_transform_test.cpp` - 类型转换
  - `test/type_safety_test.cpp` - 类型安全
  - `test/operators_test.cpp` - 运算符
  - `test/combinators_test.cpp` - 函数式组合

## Commit 信息

| means    | emoji |
|----------|-------|
| feature  | ❤️    |
| fix      | 🤡    |
| format   | 🧾    |
| refactor | ⚙️    |
| update   | 🆕    |
| WIP      | ⌛️    |
