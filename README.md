# embed-function

![Version - 0.1.0](https://img.shields.io/badge/Version-1.0.0rc-green?style=flat&logo=github) ![License - MIT](https://img.shields.io/badge/license-MIT-orange?style=flat)


A very tiny C++ wrapper for callable object without any heap memory.

---

## Usage

```cpp
#include "embed_function.hpp"
#include <cstdio>

void print_num(int num) { printf("hello %d", num); }

int main()
{
    embed::function<void(int)> fn = print_num;
    fn(123);
    return 0;
}

```

```cpp
#include "embed_function.hpp"
#include <iostream>

int main()
{
    embed::function<void()> fn = []() { std::cout << "hello world" << std::endl; };
    fn();
    return 0;
}

```

```cpp
#include "embed_function.hpp"
#include <iostream>
#include <string>

struct Test
{
    std::string str;
    void print() const
    {
        std::cout << this->str << std::endl;
    }
};

int main()
{
    Test t{"hello world"};
    embed::function<void()> fn = [&t]() { t.print(); };
    fn();
    return 0;
}

```
