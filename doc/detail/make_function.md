# embed::make_function

| Overloaded functions | Description
| --- | ---
| template&lt;typename Signature&gt;<br>Fn<Signature, sizeof(void*)> make_function() noexcept; | Creates an empty embed::Fn with specified signature.
| template&lt;typename Signature&gt;<br>Fn<Signature, sizeof(void*)> make_function( std::nullptr_t ) noexcept; | Creates an empty embed::Fn with specified signature.
| template&lt;typename Signature, typename F&gt;<br>Fn<Signature, sizeof(F)> make_function( F&& f ) noexcept; | Automatically determine the buffer size and create an instance of embed::Fn with specified signature, and initializes the target with std::forward&lt;F&gt;(f).
| template&lt;typename Ret, typename... Args&gt;<br>Fn<Ret(Args...), sizeof(void*)> make_function( Ret (*func) (Args...) ) noexcept; | Based on the type of the passed-in free function pointer, the signature is automatically derived.
| template&lt;typename Signature, typename Ret, typename... Args&gt;<br>Fn<Signature, sizeof(void*)> make_function( Ret (*func) (Args...) ) noexcept; | Used for free function pointer. Use the signature specified by the user instead of deducing it automatically.
| template&lt;typename Lambda, typename Signature = /* auto deduce */&gt;<br>Fn<Signature, sizeof(Lambda)> make_function( Lambda&& la ) noexcept; | Perform signature derivation for objects of type lambda or those with a unique overloaded operator().
| template&lt;typename Signature, std::size_t BufSize&gt;<br>Fn<Signature, BufSize> make_function( const Fn<Signature, BufSize>& other ) noexcept; | Copy make.
| template&lt;typename Signature, std::size_t BufSize&gt;<br>Fn<Signature, BufSize> make_function( Fn<Signature, BufSize>&& other ) noexcept; | Move make.
| template&lt;typename Signature, typename OtherRet, std::size_t BufSize, typename... OtherArgs&gt;<br>Fn<Signature, BufSize> make_function( const Fn<OtherRet(OtherArgs...), BufSize>& other ) noexcept; | Copy from [similar](constructor.md/#similar_mean) embed::Fn instance.
| template&lt;typename Class, typename RetType, typename... ArgsType&gt;<br>auto make_function( RetType (Class::* member_func) (ArgsType...) [const] ) noexcept -> Fn<RetType( [const] Class&, ArgsType...), sizeof(member_func)>; | This overloaded function is for use by member function pointers. The signature is automatically derived.


## Example

```cpp
#include "embed/embed_function.hpp"

void example_free_function(int, float) {}

struct Example {
    static void staticFkn(char) {};
    void memberFkn(float) {};
    void memberFkn_const(double) const {};
    void operator()(int) {};
};

void example_overload_func(void) {}
int example_overload_func(int) {}

int main()
{
    Example e;

    // The type of `fn1` is embed::function<void(int,float)>
    auto fn1 = embed::make_function(example_free_function);

    // The type of `fn2` is embed::function<void(int)>
    auto fn2 = embed::make_function(e);

    // The type of `fn3` is embed::function<void(char)>
    auto fn3 = embed::make_function(&Example::staticFkn);

    // The type of `fn4` is embed::function<void(Example&,float)>
    auto fn4 = embed::make_function(&Example::memberFkn);

    // The type of `fn5` is embed::function<void(const Example&,double)>
    auto fn5 = embed::make_function(&Example::memberFkn_const);

    // The type of `fn6` is embed::function<void()>
    auto fn6 = embed::make_function(static_cast<void(*)()>(
        example_overload_func));

    // The type of `fn7` is embed::function<int(int)>
    auto fn7 = embed::make_function(static_cast<int(*)(int)>(
        example_overload_func));

    // The type of `fn7` is embed::function<void(int, int)>
    // The signature of fn8 is different with example_free_function.
    auto fn8 = embed::make_function<void(int, int)>(example_free_function);

    return 0;
}

```

[Back](../API_embed_function.md)
