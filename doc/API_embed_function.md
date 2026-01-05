# embed::function

Defined in header file `"embed/embed_function.hpp"`.

**`embed::function` is alias of `embed::Fn`.**

The template class `embed::Fn` is designed to act as a wrapper for callable entities such as free functions, member functions, and function objects. It encapsulates the actual call within a wrapper function. It stores the pointer to the function or the object to be called or a copy of the object.

The `embed::Fn` class requires sufficient type information to define how to perform the call, namely the return type and parameters of the call. Other type information, such as the stored function name and the type of the function (free function, member function, function object), is invisible to the caller. 

Its main intended use is as a class similar to `std::function`. We guarantee that it occupies a relatively small amount of memory (defaulting to only two pointers), does not perform heap allocation, and does not throw exceptions when exceptions are disabled.
`embed::Fn` stores the object to be called, so the user does not need to ensure its lifetime.

The usage method of `embed::Fn` is very similar to that of `std::function`. The main difference lies in that `embed::Fn` has two template parameters. The first parameter is the same as that of `std::function`, which is the signature of the callable object. The second parameter is the size of the internal buffer. `embed::function` is an alias for `embed::Fn`, and it gives a default value to the second template parameter. When the user specifies the second template parameter, it automatically aligns the buffer size.

---

```cpp
namespace embed {
    template <typename Signature, std::size_t BufSize>
    class Fn; // undefined
    template <typename RetType, std::size_t BufSize, typename... ArgsType>
    class Fn<RetType(ArgsType...), BufSize>;

    template <typename Signature, std::size_t BufSize = detail::FnDefaultBufSize>
    using function = Fn<Signature, detail::FnToolBox::FnTraits::aligned_buf_size<BufSize>::value>;
} // end namespace embed
```

| Type parameters | Description
| --- | ---
| **Signature** | Signature for function call. Contain return type and argument types, similar to `std::function`.
| **RetType** | Return type from the called function.
| **ArgsType** | Parameter pack with parameters for the function call.

### Member types

| Type |  Definition | Description
| --- | --- | ---
| `result_type` | `RetType` | The return type of the `Signature`.

### Member constants

| Member name | Type | Description
| --- | --- | ---
| buffer_size | `std::size_t` | The buffer size of the `embed::Fn` object.


### Member functions

| Member functions | Description
| --- | ---
| [(constructor)](detail/constructor.md) | constructs a new `embed::Fn` instance
| [(destructor)](detail/destructor.md) | destroys a `embed::Fn` instance
| operator= | Assigns a new target to *embed::Fn*.
| swap | void swap( Fn& other ) noexcept;<br>Exchanges the stored callable objects of *this and other.
| is_empty | bool is_empty() const noexcept;<br>`true` if the instance is empty.
| [operator bool](detail/operator%20bool.md) | checks if a target is contained
| [operator ()](detail/operator().md) | invokes the target


### Non-member functions

| Non-member functions | Description
| --- | ---
| std::swap(`embed::Fn`) | specializes the std::swap algorithm
| operator== | compares a std::function with nullptr
| operator!= | compares a std::function with nullptr
| [make_function]() | Factory function, automatically deduces type signatures and the required buffer sizes and constructs an `embed::Fn` instance.
