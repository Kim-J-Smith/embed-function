# embed-function

![Version - 1.0.6](https://img.shields.io/badge/Version-1.0.6-green?style=flat&logo=github) ![License - MIT](https://img.shields.io/badge/License-MIT-orange?style=flat) ![c++ - 11/14/17/20](https://img.shields.io/badge/C++-11/14/17/20-blue?style=flat)

![gcc-c++11/14/17/20/23 - passing](https://img.shields.io/badge/GCC_C++11/14/17/20/23-passing-3215911?style=flat) ![clang-c++11/14/17/20/23 - passing](https://img.shields.io/badge/Clang_C++11/14/17/20/23-passing-3215911?style=flat) ![msvc-c++14/17/20/23 - passing](https://img.shields.io/badge/MSVC_C++14/17/20/23-passing-3215911?style=flat)

A very tiny C++ wrapper for callable object **without any heap memory or exception**.
It is used in the same way as `std::function`.

---

## Basic Usage

- normal function

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

- lambda function

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

- callable object

```cpp
#include "embed_function.hpp"
#include <iostream>
#include <string>

struct Test
{
    std::string str;
    void operator()() const
    {
        std::cout << this->str << std::endl;
    }
};

int main()
{
    Test t{"hello world"};
    embed::function<void(), sizeof(std::string)> fn = t;
    fn();
    return 0;
}

```

## Use `embed::make_function`

- Normal function

```cpp
#include "embed_function.hpp"
#include <cstdio>

void print_num(int num) { printf("hello %d", num); }

int main()
{
    // The type of fn is embed::function<void(int)>
    auto fn = embed::make_function(print_num);
    fn(123);
    return 0;
}

```

- lambda function

```cpp
#include "embed_function.hpp"
#include <iostream>

int main()
{
    // The type of fn is embed::function<void()>
    auto fn = embed::make_function(
        []() { std::cout << "hello world" << std::endl; });
    fn();
    return 0;
}

```

- callable object

```cpp
#include "embed_function.hpp"
#include <iostream>
#include <string>

struct Test
{
    std::string str;
    void operator()() const
    {
        std::cout << this->str << std::endl;
    }
};

int main()
{
    Test t{"hello world"};
    // The type of fn is embed::function<void()>
    auto fn = embed::make_function(t);
    fn();
    return 0;
}

```

