# embed::Fn<RetType(ArgsType...)>::**Fn**

| Constructor | Description
| --- | ---
| Fn() noexcept; | Creates an empty embed::Fn.
| Fn( std::nullptr_t ) noexcept; | Creates an empty embed::Fn.
| Fn( const Fn& other ) noexcept; | Copies the target of `other` to the target of `*this`.<br>If `other` is empty, `*this` will be empty right after the call too.
| Fn( Fn&& other ) noexcept; | Moves the target of `other` to the target of `*this`.<br>If `other` is empty, `*this` will be empty right after the call too.
| template<typename OtherRet, std::size_t OtherBuf, typename... OtherArgs><br>Fn( const Fn<OtherRet(OtherArgs...), OtherBuf>& other ) noexcept; | Copies the target of `other` to the target of `*this`.<br>The `other` has a similar<sup>[1](#similar_mean)</sup> signature to `*this`.
| template&lt;typename F&gt;<br>Fn( F&& f ) noexcept; | Initializes the target with `std::forward<F>(f)`. The target is of type [std::decay](https://www.cppreference.com/w/cpp/types/decay.html)&lt;F&gt;::type.<br>If `f` is a null pointer to function, a null pointer to member, or an empty value of some embed::Fn specialization, `*this` will be empty right after the call.

<a id="similar_mean">[1]</a>: "Similar" means: The return type and parameter types of the functions are the same, but with different buffer size. ( The buffer size of `other` should be smaller than `*this`. )


[Back](../API_embed_function.md)
