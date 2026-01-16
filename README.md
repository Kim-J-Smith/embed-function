# embed-function

![Version - 1.1.0](https://img.shields.io/badge/Version-1.1.0-green?style=flat&logo=github) ![License - MIT](https://img.shields.io/badge/License-MIT-orange?style=flat) ![c++ - 11/14/17/20](https://img.shields.io/badge/C++-11/14/17/20-blue?style=flat)

![gcc-c++11/14/17/20/23 - passing](https://img.shields.io/badge/GCC_C++11/14/17/20/23-passing-3215911?style=flat) ![clang-c++11/14/17/20/23 - passing](https://img.shields.io/badge/Clang_C++11/14/17/20/23-passing-3215911?style=flat) ![msvc-c++14/17/20/23 - passing](https://img.shields.io/badge/MSVC_C++14/17/20/23-passing-3215911?style=flat)


*Embedded friendly `std::function` alternative. The usage method is consistent with `std::function`.*

## Overview

- ✅ **Easy to use**
  
  The `embed::function` is used in the same way as `std::function`.

  We also provide `embed::make_function` to automatically deduce the template type.

- ✅ **No dynamic memory allocation**

  Never heap alloc. Ensure the real-time performance of the code.

- ✅ **Adjustable buffer size**

  Users can specify the buffer size used by each embed::function instance, which is used to wrap callable objects of different sizes.

- ✅ **Extremely minimal memory usage**

  By default, it occupies 2 pointer-sized amounts of RAM.

  If you need to package larger callable objects, you can manually adjust the buffer size of the `embed::function<Signature, BufSize>`.

- ✅ **Environmentally friendly for disabling exceptions**

  The `embed::function` itself supports both enabling and disabling of exceptions, but it is more recommended to use it in the mode where exceptions are disabled.

- ✅ **Support non-copyable objects**

  According to the C++ proposal [N4159](https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4159.pdf), the `embed::function` class, in addition to supporting the basic functions of `std::function`, also adds support for non-copyable objects.

---

## Quick start

- Clone the repo.
- Add include path <repo_root>/include
- In program `#include "embed/embed_function.hpp"`
- Use the `embed::function` template class.

```cpp
#include "embed/embed_function.hpp"

struct Example {
    static void staticFkn(int) {};
    void memberFkn(int) {};
    void operator()(int) {};
};

int main() 
{
    Example e;

    // The type of fn is embed::Fn<void(int), sizeof(void*)>.
    // And embed::function is the alias type of embed::Fn.
    embed::function<void(int)> fn;

    fn = [&e](int p) { e.memberFkn(p); };
    fn(123);

    fn = &Example::staticFkn;
    fn(456);

    fn = e;
    fn(789);
}

```

## Design goals driving the design

  - Should behave close to a normal function pointer. Small, efficient, no heap allocation.

  - Should be used in a manner similar to `std::function`, being able to truly "wrap" callable objects and manage their lifetimes.

  - Support the packaging of all callable objects in C++, including:
    - Free function.
    - Lambda function.
    - Functor.
    - Static member function.
    - Member function. (should be wrapped by lambda function)

  - Be usable with C++11 while offering more functionality for later editions.
    - C++11 offer most functionality.
    - C++17 introduces a template type deduction guide, allowing users to use the underlying type `embed::Fn` (where `embed::function` is an alias for `embed::Fn`) without having to specify the template parameters.

  - Be constexpr and exception friendly. As much as possible should be declared constexpr and noexcept.

## Detail APIs

[Click here to view the detailed APIs](doc/API_embed_function.md)

## License

```
MIT License

https://github.com/Kim-J-Smith/embed-function

Copyright (c) 2025 Kim-J-Smith

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
