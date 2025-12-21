/******************************************************************************
The MIT License(MIT)

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
******************************************************************************/
/**
 * @file        embed_function.hpp
 * 
 * @brief       A very tiny C++ wrapper for callable objects.
 * 
 * @version     1.0.3
 * 
 * @date        2025-12-6
 * 
 * @author      Kim-J-Smith
 * 
 * Wrapper for any callable object, including functor, normal function, lambda, etc.
 * 
 * Design intent is to do what `std::function` do but without
 * allocation of heap memory, virtual function. Whats more, user
 * can even disable the c++ exception to decrease ROM consumption.
 * 
 * The usage method of `embed::function` is almost exactly the same
 * as that of `std::function`. If users are familiar with the operation
 * of std::function, they can quickly get familiar with `embed::function`
 * as well. embed::function ensures that no heap memory is used. 
 * 
 * After careful consideration, `embed::function` does not plan to implement
 * the `target_type()` and `target()` member functions. The main reason for 
 * the former is that it relies on RTTI, which is often disabled in the embedded 
 * domain. The latter is due to the reason of thread-safe access isolation
 * (more details: https://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4159.pdf)
 * 
 * By default, the space occupied by a single instance is only the size of
 * 2 pointers. For larger lambda function objects, `embed::function`
 * allows users to specify the second template parameter, that is, the
 * buffer size to accommodate it (but such behavior is not recommended.
 * It is a better choice to wrap it with a lambda function and then pass it in).
 * 
 * If the user does not disable the C++ exception system, they will receive
 * a warning during the compilation process (this is merely a reminder and
 * will not actually affect the compilation). The user needs to choose to disable
 * C++ exceptions or to ignore this warning by defining a macro (the specific
 * warning content will include more detailed instructions on how to handle it).
 * 
 */

/// @c C++11 "embed_function.hpp"
#ifndef EMBED_FUNCTION_HPP_
#define EMBED_FUNCTION_HPP_

////////////////////////////////////////////////////////////////

/// @note User can customize following configs

// need fast call or not (fast call consume more RAM)
#define EMBED_FN_NEED_FAST_CALL     false

// assert nothrow callable function
#define EMBED_FN_NOTHROW_CALLABLE   false

namespace embed
{

  // the default buffer size for `embed::Fn`.
  constexpr decltype(sizeof(int)) _FnDefaultBufSize = (1 * sizeof(void*));

  // the callback function to handle the `bad_function_call`
  // only when the C++ exception is disabled.
  static inline void _bad_function_call_handler()
  {
    /// Your can deal with the `bad_function_call` here.
    /// Or you can just ignore this function, and use
    /// @e `std::set_terminate` instead.

    std::terminate();
  }

}

////////////////////////////////////////////////////////////////

/// @c EMBED_CXX_VERSION
#ifndef EMBED_CXX_VERSION
# if defined(_MSC_VER)
#  define EMBED_CXX_VERSION _MSVC_LANG
# else
#  define EMBED_CXX_VERSION __cplusplus
# endif
#endif

/// @c EMBED_ALIAS
#ifndef EMBED_ALIAS
# if defined(__GNUC__) || defined(__clang__) || defined(__INTEL_COMPILER)
#  define EMBED_ALIAS __attribute__((may_alias))
# else
#  define EMBED_ALIAS
# endif
#endif

/// @c EMBED_HAS_BUILTIN
#ifndef EMBED_HAS_BUILTIN
# if defined(__has_builtin) && defined(__is_identifier)
#  define EMBED_HAS_BUILTIN(x) (__has_builtin(x) || !__is_identifier(x))
# else
#  define EMBED_HAS_BUILTIN(x) 0
# endif
#endif

/// @c EMBED_CXX_ENABLE_EXCEPTION
#ifndef EMBED_CXX_ENABLE_EXCEPTION
# if defined(__cpp_exceptions)
#  define EMBED_CXX_ENABLE_EXCEPTION (__cpp_exceptions != 0)
# elif defined(_MSC_VER) && defined(_HAS_EXCEPTIONS)
#  define EMBED_CXX_ENABLE_EXCEPTION (_HAS_EXCEPTIONS != 0)
# elif (defined(__EXCEPTIONS) && __EXCEPTIONS == 1)
#  define EMBED_CXX_ENABLE_EXCEPTION 1
# else
#  define EMBED_CXX_ENABLE_EXCEPTION 0
# endif
#endif

/// @brief warning if exception is enabled.
#if !EMBED_NO_WARNING
# if ( EMBED_CXX_ENABLE_EXCEPTION != 0 )
#  warning You are using c++ exception, which may consume more ROM.\
 Try use `-fno-exceptions` to disable the exception. Or if you exactly\
 want to enable the exception, then please use `-DEMBED_NO_WARNING=1` to ignore this warning.
# endif
#endif

/// @c EMBED_ABI_VISIBILITY
#ifndef EMBED_ABI_VISIBILITY
# if defined(__GNUC__) || defined(__clang__)
#  define EMBED_ABI_VISIBILITY(x) __attribute__((visibility(#x)))
# else
#  define EMBED_ABI_VISIBILITY(x) 
# endif
#endif

/// @c EMBED_CXX17_NOEXCEPT
#ifndef EMBED_CXX17_NOEXCEPT
# if EMBED_CXX_VERSION >= 201703L
#  define EMBED_CXX17_NOEXCEPT noexcept
# else
#  define EMBED_CXX17_NOEXCEPT
# endif
#endif

/// @c EMBED_CXX14_CONSTEXPR
#ifndef EMBED_CXX14_CONSTEXPR
# if EMBED_CXX_VERSION >= 201402L
#  define EMBED_CXX14_CONSTEXPR constexpr
# else
#  define EMBED_CXX14_CONSTEXPR
# endif
#endif

/// @c EMBED_FN_CASE_NOEXCEPT
/// @brief Only when C++ version is greater than C++17,
/// c++ exception is disabled, and user need nothrow callable
/// functor, @b EMBED_FN_CASE_NOEXCEPT will be defined as noexcept.
#ifndef EMBED_FN_CASE_NOEXCEPT
# if EMBED_CXX_VERSION >= 201703L && EMBED_FN_NOTHROW_CALLABLE && !EMBED_CXX_ENABLE_EXCEPTION
#  define EMBED_FN_CASE_NOEXCEPT noexcept
# else
#  define EMBED_FN_CASE_NOEXCEPT
# endif
#endif

/// @c EMBED_INLINE
#ifndef EMBED_INLINE
# if defined(__GNUC__) || defined(__clang__)
#  define EMBED_INLINE __attribute__((always_inline))
# elif defined(_MSC_VER)
#  define EMBED_INLINE __forceinline
# else
#  define EMBED_INLINE inline
# endif
#endif

/// @c EMBED_NODISCARD
#ifndef EMBED_NODISCARD
# if EMBED_CXX_VERSION >= 201703L
#  define EMBED_NODISCARD [[nodiscard]]
# elif defined(__GNUC__) || defined(__clang__)
#  define EMBED_NODISCARD __attribute__((warn_unused_result))
# else
#  define EMBED_NODISCARD
# endif
#endif

// Header files
#if EMBED_CXX_VERSION >= 201103L
# include <cstddef> // std::size_t
# include <utility> // std::move, std::forward, std::addressof
# include <type_traits>
# include <exception>
#else
# error embed_func need C++11 or greater version, try use '-std=c++11'.
#endif

namespace embed EMBED_ABI_VISIBILITY(default)
{
  // declare ahead
  template <typename Signature, std::size_t BufSize>
  class Fn;

  /**
   *  @brief Exception class thrown when class template function's
   *  operator() is called with an empty target.
   *  @ingroup exceptions
   */
  struct bad_function_call : public std::exception
  {
    bad_function_call()                                     = default;
    bad_function_call(const bad_function_call&)             = default;
    bad_function_call& operator=(const bad_function_call&)  = default;

    virtual ~bad_function_call() noexcept override {};
    const char* what() const noexcept override
    {
      return "embed::Fn::operator() is called with an empty target";
    };
  };

  /// @c _throw_bad_function_call
  // For private use only.
  [[noreturn]] static inline void _throw_bad_function_call()
  {
#if ( EMBED_CXX_ENABLE_EXCEPTION == true )
    throw bad_function_call();
#else
    _bad_function_call_handler();
#endif
  }

  /// @c _FnBufType
  /// @brief Anchor the size of the objects that the embed::Fn can store.
  template <std::size_t BufSize>
  union _FnBufType
  {
    void*       vPtr;
    const void* cvPtr;
    void (* fPtr) (void);

    char        buf[BufSize];
  };

  /// @c _FnFunctor
  template <std::size_t BufSize>
  union EMBED_ALIAS _FnFunctor
  {
    void* M_access() noexcept { return &M_pod_data[0]; }
    const void* M_access() const noexcept { return &M_pod_data[0]; }

    template <typename T>
    T& M_access() noexcept { return *static_cast<T*>(M_access()); }
    template <typename T>
    const T& M_access() const noexcept
    { return *static_cast<const T*>(M_access()); }

    _FnBufType<BufSize>   M_unused;
    char                  M_pod_data[sizeof(_FnBufType<BufSize>)];
  };

  /// @c _FnToolBox
  /// @brief Tool Box for embed::Fn
  class _FnToolBox
  {
  public:

    // cooperate with M_manager in FnManager
    enum manOpcode {
      OP_clone_functor,
      OP_move_functor,
      OP_destroy_functor,

#if ( EMBED_FN_NEED_FAST_CALL == false )
      OP_get_invoker,
#endif
    };

    // embed::Fn need some type traits that don't exist in C++11.
    /// @c FnTraits is aimed to provide these traits.
    struct FnTraits;

    // embed::Fn will forget the type of Functor
    /// @c FnManager is aimed to remember the type, help Fn manager functor.
    template <typename Signature, typename Functor, std::size_t BufSize>
    struct FnManager;

#if (EMBED_FN_NEED_FAST_CALL == true)

    /// @c FnInvoker is aimed to help Fn call the functor.
    /// When @b EMBED_FN_NEED_FAST_CALL is false, FnManager
    /// will helper Fn call the functor instead of FnInvoker,
    /// which will save the RAM, but is more slower than FnInvoker.
    template <typename Signature, typename Functor, std::size_t BufSize>
    struct FnInvoker;

#endif

  };


  /**
   * @c _FnToolBox::FnTraits
   * @brief Provide necessary type traits.
   */
  struct _FnToolBox::FnTraits
  {
    /// @e aligned_buf_size
    template <std::size_t BufSize>
    struct aligned_buf_size
    {
      // round up to an integer
      static constexpr std::size_t value = (BufSize == 0) ? sizeof(void*) :
        ((BufSize - 1) / sizeof(void*) + 1) * sizeof(void*);
    };

    template <typename T> using void_t = void;

    template <typename Func, typename... ArgsT>
    struct invoke_result
    {
      using type = decltype(std::declval<Func>() (std::declval<ArgsT>()...));
    };

    template <typename To, typename From>
    struct reference_converts_from_temporary
    {
      using ncvref_To = typename std::remove_cv<
        typename std::remove_reference<To>::type
      >::type;

      // prvalue(pure right value) is being bound to const lvalue(left value)
      static constexpr bool pr_to_l = std::is_lvalue_reference<To>::value
        && !std::is_reference<From>::value
        && std::is_const<typename std::remove_reference<To>::type>::value
        && std::is_convertible<From, ncvref_To>::value;

      // prvalue is being bound to [const] xvalue(right reference)
      static constexpr bool pr_to_x = std::is_rvalue_reference<To>::value
        && !std::is_reference<From>::value
        && std::is_convertible<From, ncvref_To>::value;

      static constexpr bool value = pr_to_l || pr_to_x;
    };

    // check Result::type --> RetT
    template <typename Result, typename RetT,
      bool = std::is_void<RetT>::value, typename = void>
    struct is_invocable_impl : public std::false_type
    {
      using noThrow = std::false_type;
    };

    template <typename Result, typename RetT>
    struct is_invocable_impl<Result, RetT,
      /* is_void<RetT> = */ true, void_t<typename Result::type> >
    : public std::true_type
    {
      using noThrow = std::true_type;
    };

#if defined(__GNUC__)
# pragma GCC diagnostic push
# pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
# pragma GCC diagnostic ignored "-Wreturn-type"
#endif
    template <typename Result, typename RetT>
    struct is_invocable_impl<Result, RetT,
      /* is_void<RetT> = */ false, void_t<typename Result::type> >
    {
      // INVOKE expression
      using Res_T = typename Result::type;

      // Unlike declval, this doesn't add_rvalue_reference, so it respects
      // guaranteed copy elision.
      static Res_T S_get() noexcept { return std::declval<Res_T>(); };

      // Used to check if Res_T can implicitly convert to T.
      template <typename T>
      static void S_conv(T) noexcept {};

      // Viable only when INVOKE(f, Args...) can convert to `RetT`.
      template <typename T,
        bool NoThrow_conv = noexcept(S_conv<T>(S_get())),
        typename = decltype(S_conv<T>(S_get())),
#if EMBED_HAS_BUILTIN(__reference_converts_from_temporary)
        bool Dangle = __reference_converts_from_temporary(T, Res_T)
#else
        bool Dangle = reference_converts_from_temporary<T, Res_T>::value
#endif
      >
      static typename std::integral_constant<
        bool, NoThrow_conv && !Dangle
      >::type
      S_test(int) noexcept
      {
        return std::integral_constant<bool, NoThrow_conv && !Dangle>();
      }

      template <typename T, bool = false>
      static std::false_type S_test(...) noexcept
      { return std::false_type(); };

      using type = decltype(S_test<RetT, true>(1));
      using noThrow = decltype(S_test<RetT>(1));
    };
#if defined(__GNUC__)
# pragma GCC diagnostic pop
#endif

    /// @e Callable
    template <typename RetT, typename Functor, typename... ArgsT>
    struct Callable
    : public is_invocable_impl<
      invoke_result<typename std::decay<Functor>::type&, ArgsT...>, RetT
    >::type
    {
      using Caller = typename std::decay<Functor>::type&;
      using Result = invoke_result<Caller, ArgsT...>;

#if ( EMBED_CXX_VERSION >= 201703L ) && (!EMBED_CXX_ENABLE_EXCEPTION)
      // The noexcept-specification is a part of the function type 
      // and may appear as part of any function declarator.
      // And only when `EMBED_CXX_ENABLE_EXCEPTION` is false,
      // embed::Fn::operator() can be `noexcept`.
      using NoThrow_call = typename std::integral_constant<
        bool, noexcept(std::declval<Caller>()(std::declval<ArgsT>()...))
      >::type;
#else
      using NoThrow_call = std::true_type;
#endif
      using NoThrow_conv = typename is_invocable_impl<Result, RetT>::noThrow;

      static constexpr bool NoThrow_v = NoThrow_call::value && NoThrow_conv::value;

#if ( EMBED_FN_NOTHROW_CALLABLE == true )

      static_assert(NoThrow_v, "embed::Fn requires no-throw callable functor");

#endif
    };

    /// @e results_are_same
    template <typename RetFrom, typename RetTo>
    struct results_are_same
    {
      using nc_From = typename std::remove_const<RetFrom>::type;
      using nc_To = typename std::remove_const<RetTo>::type;

      static constexpr bool value = std::is_same<nc_From, nc_To>::value;
    };

    /// @e args_package
    template <typename... ArgsT>
    struct args_package
    { };

    template <std::size_t Index, typename Package>
    struct unwrap_package
    {
      // always error if this struct is being used.
      static_assert(Index != 0 && false, "unwrap_package out of range");
    };

    template <typename T, typename... Res>
    struct unwrap_package<0, args_package<T, Res...>>
    {
      using type = T;
    };

    template <std::size_t Index, typename T, typename... Res>
    struct unwrap_package<Index, args_package<T, Res...>>
    {
      using type = typename unwrap_package<Index-1, args_package<Res...>>::type;
    };

    /// @e arguments_are_same_impl
    template <typename ArgsPackageFrom, typename ArgsPackageTo, std::size_t Idx>
    struct arguments_are_same_impl
    {
      using FromType = typename std::remove_const<typename unwrap_package<Idx, ArgsPackageFrom>::type>::type;
      using ToType = typename std::remove_const<typename unwrap_package<Idx, ArgsPackageTo>::type>::type;

      static constexpr bool value = std::is_same<
        FromType, ToType  
      >::value && arguments_are_same_impl<ArgsPackageFrom, ArgsPackageTo, Idx-1>::value;
    };

    template <typename ArgsPackageFrom, typename ArgsPackageTo>
    struct arguments_are_same_impl<ArgsPackageFrom, ArgsPackageTo, 0>
    {
      using FromType = typename std::remove_const<typename unwrap_package<0, ArgsPackageFrom>::type>::type;
      using ToType = typename std::remove_const<typename unwrap_package<0, ArgsPackageTo>::type>::type;

      static constexpr bool value = std::is_same<
        FromType, ToType
      >::value;
    };

    template <typename ArgsPackageFrom, typename ArgsPackageTo>
    struct arguments_are_same_impl<ArgsPackageFrom, ArgsPackageTo, (std::size_t)-1>
    { static constexpr bool value = false; };

    /// @e arguments_are_same
    template <typename ArgsPackageFrom, typename ArgsPackageTo, std::size_t ArgNum>
    struct arguments_are_same : public std::conditional<
      ArgNum == 0, std::true_type,
      arguments_are_same_impl<ArgsPackageFrom, ArgsPackageTo, ArgNum-1>
    >::type { };

    /// @e invoke_r
    template <typename RetType, typename Callee, typename... ArgsType>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR
    typename std::enable_if<!std::is_void<RetType>::value, RetType>::type
    invoke_r(Callee&& fn, ArgsType&&... args)
    {
      // The return type is not `void`.
      return std::forward<Callee>(fn)(std::forward<ArgsType>(args)...);
    }

    template <typename RetType, typename Callee, typename... ArgsType>
    static EMBED_INLINE EMBED_CXX14_CONSTEXPR
    typename std::enable_if<std::is_void<RetType>::value>::type
    invoke_r(Callee&& fn, ArgsType&&... args)
    {
      // The return type is `void`.
      std::forward<Callee>(fn)(std::forward<ArgsType>(args)...);
    }

    /// @e is_similar_Fn
    template <
      typename SelfRet, typename SelfArgs_package, std::size_t SelfArgNum, std::size_t SelfBuf,
      typename OtherRet, typename OtherArgs_package, std::size_t OtherArgNum, std::size_t OtherBuf>
    struct is_similar_Fn
    {
      static constexpr bool value = 
        ( aligned_buf_size<SelfBuf>::value >= aligned_buf_size<OtherBuf>::value )
        && results_are_same<OtherRet, SelfRet>::value
        && ( SelfArgNum == OtherArgNum )
        && arguments_are_same<SelfArgs_package, OtherArgs_package, SelfArgNum>::value;
    };

    /// @e get_unique_call_signature_impl
    template <typename Signature>
    struct get_unique_call_signature_impl;

    template <typename Ret, typename Functor, typename... ArgsType>
    struct get_unique_call_signature_impl<Ret (Functor::*) (ArgsType...)>
    { using type = Ret(ArgsType...); };

    template <typename Ret, typename Functor, typename... ArgsType>
    struct get_unique_call_signature_impl<Ret (Functor::*) (ArgsType...) const>
    { using type = Ret(ArgsType...); };

#if EMBED_CXX_VERSION >= 201703L

    // See https://en.cppreference.com/w/cpp/language/noexcept_spec
    // The noexcept-specification is a part of the function type and 
    // may appear as part of any function declarator. (Since C++17)

    template <typename Ret, typename Functor, typename... ArgsType>
    struct get_unique_call_signature_impl<Ret (Functor::*) (ArgsType...) noexcept>
    { using type = Ret(ArgsType...); };

    template <typename Ret, typename Functor, typename... ArgsType>
    struct get_unique_call_signature_impl<Ret (Functor::*) (ArgsType...) const noexcept>
    { using type = Ret(ArgsType...); };

#endif

    /// @e is_unique_callable
    template <typename Functor, bool = true, typename = void>
    struct is_unique_callable
    : public std::false_type
    { };

    template <typename Functor>
    struct is_unique_callable<Functor, true, void_t<decltype(&Functor::operator())>>
    : public std::true_type
    { };

    /// @e get_unique_call_signature
    template <typename Functor, bool = is_unique_callable<Functor>::value>
    struct get_unique_call_signature
    {
      using type = void; // unused
    };

    template <typename Functor>
    struct get_unique_call_signature<Functor, true>
    {
      using type = typename
        get_unique_call_signature_impl<decltype(&Functor::operator())>::type;
    };

    /// @e unwrap_signature
    template <typename Signature> struct unwrap_signature;

    template <typename RetType, typename... ArgsType>
    struct unwrap_signature<RetType(ArgsType...)>
    {
      using ret = RetType;
      using args = args_package<ArgsType...>;
      static constexpr std::size_t arg_num = sizeof... (ArgsType);
    };

    /// @e is_Fn_and_similar
    template <typename Signature, typename Functor>
    struct is_Fn_and_similar
    : public std::false_type { };

    template <typename Signature,
      typename OtherRet, std::size_t BufSize, typename... OtherArgs>
    struct is_Fn_and_similar<Signature, Fn<OtherRet(OtherArgs...), BufSize>>
    {
      static constexpr bool value = is_similar_Fn<
          typename unwrap_signature<Signature>::ret,
          typename unwrap_signature<Signature>::args,
          unwrap_signature<Signature>::arg_num,
          BufSize, OtherRet, args_package<OtherArgs...>,
          sizeof... (OtherArgs), BufSize
        >::value;
    };

  }; // end FnTraits


  /**
   * @c _FnToolBox::FnManager
   * @brief Manage the functor for embed::Fn.
   */
  template <typename RetType, typename Functor, std::size_t BufSize, typename... ArgsType>
  struct _FnToolBox::FnManager<RetType(ArgsType...), Functor, BufSize>
  {
  public:  
    constexpr static std::size_t M_max_size = sizeof(_FnBufType<BufSize>);
    constexpr static std::size_t M_max_align = alignof(_FnBufType<BufSize>);

    /// @e Invoker_Type (same as `Invoker_Type` in embed::Fn)
    using Invoker_Type = RetType (*) (const _FnFunctor<BufSize>&, ArgsType&&...);

    /// @e Local_Storage
    /// @brief Judge the functor is small or big.

    static constexpr bool noThrowExcept =
      std::is_nothrow_copy_constructible<Functor>::value
      && std::is_nothrow_destructible<Functor>::value;

    static constexpr bool smallAndAligned =
      sizeof(Functor) <= M_max_size
      && alignof(Functor) <= M_max_align
      && (sizeof(Functor) % alignof(Functor) == 0);

    using Local_Storage = typename
    std::integral_constant<bool,
      noThrowExcept && smallAndAligned
    >::type; // end Local_Storage

    // MUST small and nothrow
    static_assert(noThrowExcept,
      "embed::Fn requires the functor to be nothrow copy-constructible"
      " and nothrow destructible");
    static_assert(smallAndAligned,
      "embed::Fn requires the functor to fit in `BufSize` and"
      " have valid alignment (adjust `BufSize` if needed)");

  private:
    /// @e M_create
    /// Q: Why not use Functor, but use the `Func`?
    /// A: Because this is perfect forward, need compiler deduce `Func` instead of specify it.
    template <typename Func>
    static void M_create(_FnFunctor<BufSize>& dest, Func&& functor) noexcept
    {
      ::new (dest.M_access()) Functor(std::forward<Func>(functor));
    }

    /// @e M_destroy
    static void M_destroy(_FnFunctor<BufSize>& victim) noexcept
    {
      victim.template M_access<Functor>().~Functor();
    }

  public:
    /// @e M_get_pointer
    // break the `const` promises.
    static Functor*
    M_get_pointer(const _FnFunctor<BufSize>& src) noexcept
    {
      const Functor& fn = src.template M_access<Functor>();
      return const_cast<Functor*>(std::addressof(fn));
    }

    /// @e M_not_empty_function
    template <typename Signature, std::size_t Size>
    static bool M_not_empty_function(const Fn<Signature, Size>& f) noexcept
    { return static_cast<bool>(f); }

    template <typename T>
    static bool M_not_empty_function(T* fp) noexcept
    { return fp != nullptr; }

    template <typename Class, typename T>
    static bool M_not_empty_function(T Class::* mp) noexcept
    { return mp != nullptr; }

    template <typename T>
    static bool M_not_empty_function(const T&) noexcept
    { return true; }

    /// @e M_init_functor
    // init functor by using M_create (perfect forward)
    template <typename Func>
    static void M_init_functor(_FnFunctor<BufSize>& dest, Func&& functor) noexcept
    {
      M_create(dest, std::forward<Func>(functor));
    }

#if ( EMBED_FN_NEED_FAST_CALL == false )

    /// @e M_invoke
    // This function only used when we expect FnManager to help Fn
    // call the functor instead of FnInvoker.
    static RetType M_invoke(const _FnFunctor<BufSize>& functor, ArgsType&&... args)
    noexcept(FnTraits::Callable<RetType, Functor, ArgsType...>::NoThrow_v)
    {
      return FnTraits::invoke_r<RetType>(*M_get_pointer(functor),
        std::forward<ArgsType>(args)...);
    }

#endif

    /// @e M_manager
    // Core func to manager functor.
    static Invoker_Type
    M_manager(_FnFunctor<BufSize>& dest, const _FnFunctor<BufSize>& src, manOpcode op) noexcept
    {
      Invoker_Type invoker = nullptr;

      switch (op)
      {
      case OP_clone_functor:
        M_init_functor(dest,
          *const_cast<const Functor*>(M_get_pointer(src)));
        break;

      case OP_move_functor:
        M_init_functor(dest, std::move( *(M_get_pointer(src)) ));
        break;

      case OP_destroy_functor:
        M_destroy(dest);
        break;

#if ( EMBED_FN_NEED_FAST_CALL == false )
      case OP_get_invoker:
        invoker = &M_invoke;
        break;
#endif
      } // end switch

      return invoker;
    }

  };

#if (EMBED_FN_NEED_FAST_CALL == true)

  /**
   * @c _FnToolBox::FnInvoker
   * @brief Invoke the functor for Fn.
   */
  template <typename RetType, typename Functor, std::size_t BufSize, typename... ArgsType>
  struct _FnToolBox::FnInvoker<RetType(ArgsType...), Functor, BufSize>
  : private _FnToolBox::FnManager<RetType(ArgsType...), Functor, BufSize>
  {
  private:
    using Base = _FnToolBox::FnManager<RetType(ArgsType...), Functor, BufSize>;
  public:

    static RetType M_invoke(const _FnFunctor<BufSize>& functor, ArgsType&&... args)
    noexcept(FnTraits::Callable<RetType, Functor, ArgsType...>::NoThrow_v)
    {
      return FnTraits::invoke_r<RetType>(*Base::M_get_pointer(functor),
        std::forward<ArgsType>(args)...);
    }

  };

#endif

  /**
   * @brief   A light polymorphic wrapper for callable object.
   * @note    Only use stack memory. NO HEAP MEMORY!
   */
  template <typename RetType, std::size_t BufSize, typename... ArgsType>
  class Fn<RetType(ArgsType...), BufSize> : private _FnToolBox
  {
  private:
    template <typename Functor>
    using DecayFunc_t = typename std::enable_if<
      !std::is_same<Fn,
        typename std::remove_cv<
          typename std::remove_reference<Functor>::type
        >::type
      >::value,
      typename std::decay<Functor>::type
    >::type;

    template <typename Functor>
    using MyManager = FnManager<RetType(ArgsType...), Functor, BufSize>;

#if ( EMBED_FN_NEED_FAST_CALL == true )
    template <typename Functor>
    using MyInvoker = FnInvoker<RetType(ArgsType...), Functor, BufSize>;
#endif

    template <typename Functor>
    using Callable = FnTraits::Callable<RetType, Functor, ArgsType...>;

    using Invoker_Type = RetType (*) (const _FnFunctor<BufSize>&, ArgsType&&...);

    using Manager_Type = 
      Invoker_Type (*) (_FnFunctor<BufSize>&, const _FnFunctor<BufSize>&, manOpcode) EMBED_CXX17_NOEXCEPT;

  private:

    // The `M_functor` store the callable object.
    _FnFunctor<BufSize>   M_functor{};

    // The `M_manager` describes how to move / copy / destroy the `M_functor`,
    // and even describes how to invoke `M_functor` when not using `M_invoker`.
    Manager_Type          M_manager{};

#if ( EMBED_FN_NEED_FAST_CALL == true )

    // invoker for the functor (func pointer)
    /// IF @b EMBED_FN_NEED_FAST_CALL is not true, `M_manager` 
    /// will help Fn invoke functor instead of `M_invoker`.
    Invoker_Type          M_invoker{};

#endif

    // Regard all Fn<Signature, BufSize> as friend class.
    template <typename Signature, std::size_t BSize>
    friend class Fn;

  public:
    // See https://en.cppreference.com/w/cpp/utility/functional/function.html
    // Get the return type.
    using result_type = RetType;

    // The `BufSize` of this embed::Fn object.
    static constexpr std::size_t buffer_size = BufSize;

  public:

    // Default destructor for embed::Fn.
    // Destroy the functor, call functor's destructor.
    EMBED_INLINE ~Fn() noexcept
    {
      if (M_manager)
        (void)M_manager(M_functor, M_functor, OP_destroy_functor);
    }

    // Create an empty function wrapper.
    EMBED_INLINE Fn() noexcept {}

    // Create an empty function wrapper.
    EMBED_INLINE Fn(std::nullptr_t) noexcept {}

#if ( EMBED_FN_NEED_FAST_CALL == true )

    // Copy constructor for embed::Fn.
    // Use `placement new` to create new functor,
    // which will call functor's copy-constructor.
    Fn(const Fn& fn) noexcept
    {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_clone_functor);
        M_manager = fn.M_manager;
        M_invoker = fn.M_invoker;
      }
    }

    // Move constructor for embed::Fn.
    // Use `placement new` to create new functor,
    // which will call functor's move-constructor.
    Fn(Fn&& fn) noexcept
    {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        M_manager = fn.M_manager;
        M_invoker = fn.M_invoker;
        fn.M_manager = nullptr;
        fn.M_invoker = nullptr;
      }
    }

    // Construct Fn<Sig_A> with Fn<Sig_B>
    // restrictions: `Sig_B.ret` can convert to `Sig_A.ret`
    // `Sig_A.args` are same with `Sig_B.args`.
    template <typename OtherRet, std::size_t OtherSize, typename... OtherArgs>
    Fn(
      const Fn<OtherRet(OtherArgs...), OtherSize>& fn,
      typename std::enable_if<
        FnTraits::is_similar_Fn<
          RetType, FnTraits::args_package<ArgsType...>, sizeof...(ArgsType), BufSize,
          OtherRet, FnTraits::args_package<OtherArgs...>, sizeof...(OtherArgs), OtherSize
        >::value,
        bool
      >::type = true
    ) noexcept {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(
          *reinterpret_cast<_FnFunctor<OtherSize>*>(&M_functor),
          fn.M_functor, OP_clone_functor
        );
        M_manager = reinterpret_cast<Manager_Type>(fn.M_manager);
        M_invoker = reinterpret_cast<Invoker_Type>(fn.M_invoker);
      }
    }

    /**
     * @brief Builds a Fn that targets a copy of the incoming
     * function object.
     * @param func - A callable object with parameters of type
     * `ArgsType...` and returns a value convertible to `RetType`.
     * 
     * REQUIRE:
     * 1. `decltype(func)` must be Callable.
     * 2. `std::decay<decltype(func)>::type` must be nothrow copy constructible.
     * 3. `std::decay<decltype(func)>::type` must be nothrow constructible from
     * the type `decltype(func)` object.
     */
    template <typename Functor,
      typename DecayFunctor = Fn::DecayFunc_t<Functor> >
    Fn(Functor&& func) noexcept
    {
      static_assert(Fn::Callable<Functor>::value,
        "embed::Fn require the Functor is callable and the Signature match RetType");

      static_assert(std::is_copy_constructible<DecayFunctor>::value,
        "membed::Fn target must be copy-constructible");

      static_assert(std::is_nothrow_constructible<DecayFunctor, Functor>::value,
        "embed::Fn target must be NO-THROW constructible from the "
        "constructor argument");

      if (Fn::MyManager<DecayFunctor>::M_not_empty_function(func))
      {
        Fn::MyManager<DecayFunctor>::M_init_functor(M_functor, std::forward<Functor>(func));
        M_manager = &Fn::MyManager<DecayFunctor>::M_manager;
        M_invoker = &Fn::MyInvoker<DecayFunctor>::M_invoke;
      }
    }

    // Swap Fn but unkown the real type of `Functor`.
    // But only M_manager remember the `Functor`.
    void swap(Fn& fn) noexcept
    {
      _FnFunctor<BufSize> tmpFunc{};
      if (M_manager)
      {
        M_manager(tmpFunc, M_functor, OP_move_functor);
        M_manager(M_functor, M_functor, OP_destroy_functor);
      }
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        fn.M_manager(fn.M_functor, fn.M_functor, OP_destroy_functor);
      }
      if (M_manager)
      {
        M_manager(fn.M_functor, tmpFunc, OP_move_functor);
        M_manager(tmpFunc, tmpFunc, OP_destroy_functor);
      }
      std::swap(M_manager, fn.M_manager);
      std::swap(M_invoker, fn.M_invoker);
    }

    // Call the functor with type `ArgsType...` arguments.
    RetType operator() (ArgsType... args) const
    EMBED_FN_CASE_NOEXCEPT
    {
      if (M_invoker)
        return M_invoker(M_functor, std::forward<ArgsType>(args)...);
      else
        _throw_bad_function_call(); /* throw exception */
    }
#else

    // Copy constructor for embed::Fn.
    // Use `placement new` to create new functor,
    // which will call functor's copy-constructor.
    Fn(const Fn& fn) noexcept
    {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_clone_functor);
        M_manager = fn.M_manager;
      }
    }

    // Move constructor for embed::Fn.
    // Use `placement new` to create new functor,
    // which will call functor's move-constructor.
    Fn(Fn&& fn) noexcept
    {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        M_manager = fn.M_manager;
        fn.M_manager = nullptr;
      }
    }

    // Construct Fn<Sig_A> with Fn<Sig_B>
    // restrictions: `Sig_B.ret` can convert to `Sig_A.ret`
    // `Sig_A.args` are same with `Sig_B.args`.
    template <typename OtherRet, std::size_t OtherSize, typename... OtherArgs>
    Fn(
      const Fn<OtherRet(OtherArgs...), OtherSize>& fn,
      typename std::enable_if<
        FnTraits::is_similar_Fn<
          RetType, FnTraits::args_package<ArgsType...>, sizeof...(ArgsType), BufSize,
          OtherRet, FnTraits::args_package<OtherArgs...>, sizeof...(OtherArgs), OtherSize
        >::value,
        bool
      >::type = true
    ) noexcept {
      if (static_cast<bool>(fn))
      {
        fn.M_manager(
          *reinterpret_cast<_FnFunctor<OtherSize>*>(&M_functor),
          fn.M_functor, OP_clone_functor
        );
        M_manager = reinterpret_cast<Manager_Type>(fn.M_manager);
      }
    }

    /**
     * @brief Builds a Fn that targets a copy of the incoming
     * function object.
     * @param func - A callable object with parameters of type
     * `ArgsType...` and returns a value convertible to `RetType`.
     * 
     * REQUIRE:
     * 1. `decltype(func)` must be Callable.
     * 2. `std::decay<decltype(func)>::type` must be nothrow copy constructible.
     * 3. `std::decay<decltype(func)>::type` must be nothrow constructible from
     * the type `decltype(func)` object.
     */
    template <typename Functor,
      typename DecayFunctor = Fn::DecayFunc_t<Functor> >
    Fn(Functor&& func) noexcept
    {
      static_assert(Fn::Callable<Functor>::value,
        "embed::Fn require the Functor is callable and the Signature match RetType");

      static_assert(std::is_copy_constructible<DecayFunctor>::value,
        "membed::Fn target must be copy-constructible");

      static_assert(std::is_nothrow_constructible<DecayFunctor, Functor>::value,
        "embed::Fn target must be NO-THROW constructible from the "
        "constructor argument");

      if (Fn::MyManager<DecayFunctor>::M_not_empty_function(func))
      {
        Fn::MyManager<DecayFunctor>::M_init_functor(M_functor, std::forward<Functor>(func));
        M_manager = &Fn::MyManager<DecayFunctor>::M_manager;
      }
    }

    // Swap Fn but unkown the real type of `Functor`.
    // But only M_manager remember the `Functor`.
    void swap(Fn& fn) noexcept
    {
      _FnFunctor<BufSize> tmpFunc{};
      if (M_manager)
      {
        M_manager(tmpFunc, M_functor, OP_move_functor);
        M_manager(M_functor, M_functor, OP_destroy_functor);
      }
      if (static_cast<bool>(fn))
      {
        fn.M_manager(M_functor, fn.M_functor, OP_move_functor);
        fn.M_manager(fn.M_functor, fn.M_functor, OP_destroy_functor);
      }
      if (M_manager)
      {
        M_manager(fn.M_functor, tmpFunc, OP_move_functor);
        M_manager(tmpFunc, tmpFunc, OP_destroy_functor);
      }
      std::swap(M_manager, fn.M_manager);
    }

# if defined(__clang__)
#  pragma clang diagnostic push
#  pragma clang diagnostic ignored "-Wuninitialized"
# elif defined(__GNUC__)
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
# endif

    // Call the functor with type `ArgsType...` arguments.
    RetType operator() (ArgsType... args) const
    EMBED_FN_CASE_NOEXCEPT
    {
      if (M_manager) {
        _FnFunctor<BufSize> nil;
        Invoker_Type invoker = M_manager(nil, nil, OP_get_invoker);
        return invoker(M_functor, std::forward<ArgsType>(args)...);
      }
      else
        _throw_bad_function_call(); /* throw exception */
    }

# if defined(__clang__)
#  pragma clang diagnostic pop
# elif defined(__GNUC__)
#  pragma GCC diagnostic pop
# endif

#endif // End EMBED_FN_NEED_FAST_CALL == true or not

    /// @deprecated operator= may cunsume more resource,
    /// maybe copy/move constructor is better.
    EMBED_INLINE Fn& operator=(const Fn& fn) noexcept
    {
      Fn(fn).swap(*this);
      return *this;
    }

    /// @deprecated operator= may cunsume more resource,
    /// maybe copy/move constructor is better.
    EMBED_INLINE Fn& operator=(Fn&& fn) noexcept
    {
      if (this != std::addressof(fn))
        Fn(std::move(fn)).swap(*this);
      return *this;
    }

    /// @deprecated operator= may cunsume more resource,
    /// maybe copy/move constructor is better.
    template <typename RetT, std::size_t OtherBufSize, typename... ArgsT>
    EMBED_INLINE Fn& operator=(const Fn<RetT(ArgsT...), OtherBufSize>& fn)
    noexcept(
      std::enable_if<
        FnTraits::is_similar_Fn<
          RetType, FnTraits::args_package<ArgsType...>, sizeof...(ArgsType), BufSize,
          RetT, FnTraits::args_package<ArgsT...>, sizeof...(ArgsT), OtherBufSize
        >::value,
        std::true_type
      >::type::value
    ) {
      Fn(fn).swap(*this);
      return *this;
    }

    /// @deprecated operator= may cunsume more resource,
    /// maybe copy/move constructor is better.
    template <typename Functor,
      typename DecayFunc = Fn::DecayFunc_t<Functor> >
    EMBED_INLINE Fn& operator=(Functor&& func) noexcept
    {
      Fn(std::forward<Functor>(func)).swap(*this);
      return *this;
    }

    // check if the embed::Fn is empty.
    EMBED_INLINE bool is_empty() const noexcept
    {
      return static_cast<bool>( M_manager == nullptr );
    }

    // `true` if the embed::Fn is not empty.
    EMBED_INLINE explicit operator bool() const noexcept
    {
      return !is_empty();
    }


  }; // end Fn


  // `true` if the wrapper has no target, `false` otherwise. (noexcept)
  template <typename Signature, std::size_t BufSize>
  static inline bool
  operator==(const Fn<Signature, BufSize>& fn, std::nullptr_t) noexcept
  { return fn.is_empty(); }

  // `true` if the wrapper has no target, `false` otherwise. (noexcept)
  template <typename Signature, std::size_t BufSize>
  static inline bool
  operator==(std::nullptr_t, const Fn<Signature, BufSize>& fn) noexcept
  { return fn.is_empty(); }

  // `true` if the wrapper does have target, `false` otherwise. (noexcept)
  template <typename Signature, std::size_t BufSize>
  static inline bool
  operator!=(const Fn<Signature, BufSize>& fn, std::nullptr_t) noexcept
  { return !fn.is_empty(); }

  // `true` if the wrapper does have target, `false` otherwise. (noexcept)
  template <typename Signature, std::size_t BufSize>
  static inline bool
  operator!=(std::nullptr_t, const Fn<Signature, BufSize>& fn) noexcept
  { return !fn.is_empty(); }

  /**
   * @brief `embed::function` is an alias of `embed::Fn`.
   * @note It is encouraged to use `embed::function` instead of `embed::Fn`.
   * `embed::function` will automatically align the BufSize.
   */
  template <typename Signature, std::size_t BufSize = _FnDefaultBufSize>
  using function = Fn<Signature, _FnToolBox::FnTraits::aligned_buf_size<BufSize>::value>;

  /**
   * @brief Make a function and automatically calculate the required size.
   * @note `embed::make_function` has many kinds of override function.
   */
  template <typename Signature, typename Functor>
  EMBED_NODISCARD inline typename std::enable_if<
    !_FnToolBox::FnTraits::is_Fn_and_similar<
      Signature,
      typename std::remove_const<
        typename std::remove_reference<Functor>::type
      >::type
    >::value,
    function<Signature, sizeof(Functor)>
  >::type
  make_function(Functor&& func) noexcept
  {
    return function<Signature, sizeof(Functor)>(std::forward<Functor>(func));
  }

  // Overload for empty.
  template <typename Signature, std::size_t BufSize = _FnDefaultBufSize>
  EMBED_NODISCARD inline function<Signature, BufSize>
  make_function() noexcept
  { return function<Signature, BufSize>(); }

  // Overload for nullptr.
  template <typename Signature, std::size_t BufSize = _FnDefaultBufSize>
  EMBED_NODISCARD inline function<Signature, BufSize>
  make_function(std::nullptr_t) noexcept
  { return function<Signature>(nullptr); }

  // Overload for normal function.
  template <typename RetType, typename... ArgsType>
  EMBED_NODISCARD inline function<RetType(ArgsType...)>
  make_function(RetType (*func) (ArgsType...) EMBED_FN_CASE_NOEXCEPT) noexcept
  {
    return function<RetType(ArgsType...)>(func);
  }

  // Overload for normal function with specified signature.
  template <typename Signature, typename RetType, typename... ArgsType>
  EMBED_NODISCARD inline function<Signature>
  make_function(RetType (*func) (ArgsType...) EMBED_FN_CASE_NOEXCEPT) noexcept
  {
    return function<Signature>(func);
  }

  // Overload for lambda function or other object which
  // uniquely override the `operator()`.
  template <typename Lambda,
    typename ClassType = typename std::remove_const<
      typename std::remove_reference<Lambda>::type
    >::type,
    typename Signature = typename 
    _FnToolBox::FnTraits::get_unique_call_signature<ClassType>::type
  >
  EMBED_NODISCARD inline typename std::enable_if<
    _FnToolBox::FnTraits::is_unique_callable<ClassType>::value,
    function<Signature, sizeof(Lambda)>
  >::type
  make_function(Lambda&& la) noexcept
  {
    return function<Signature, sizeof(Lambda)>(std::forward<Lambda>(la));
  }

  // Overload for `embed::Fn`. (Copy)
  template <typename Signature, std::size_t BufSize>
  EMBED_NODISCARD inline function<Signature, BufSize>
  make_function(const Fn<Signature, BufSize>& fn) noexcept
  {
    return function<Signature, BufSize>(fn);
  }

  // Overload for `embed::Fn`. (Move)
  template <typename Signature, std::size_t BufSize>
  EMBED_NODISCARD inline function<Signature, BufSize>
  make_function(Fn<Signature, BufSize>&& fn) noexcept
  {
    return function<Signature, BufSize>(std::move(fn));
  }

  // Overload for `embed::Fn<Other, Size>`.
  template <typename Signature, typename OtherRet, std::size_t BufSize, typename... OtherArgs>
  EMBED_NODISCARD inline typename std::enable_if<
    _FnToolBox::FnTraits::is_similar_Fn<
      typename _FnToolBox::FnTraits::unwrap_signature<Signature>::ret,
      typename _FnToolBox::FnTraits::unwrap_signature<Signature>::args,
      _FnToolBox::FnTraits::unwrap_signature<Signature>::arg_num,
      BufSize, OtherRet, _FnToolBox::FnTraits::args_package<OtherArgs...>,
      sizeof... (OtherArgs), BufSize
    >::value,
    function<Signature, BufSize>
  >::type
  make_function(const Fn<OtherRet(OtherArgs...), BufSize>& fn) noexcept
  {
    return function<Signature, BufSize>(fn);
  }

} // end namespace embed

// std::swap
namespace std EMBED_ABI_VISIBILITY(default)
{

  template<typename Signature, size_t BufSize>
  inline void swap(
    embed::Fn<Signature, BufSize>& fn1,
    embed::Fn<Signature, BufSize>& fn2
  ) noexcept { fn1.swap(fn2); }

}


#undef EMBED_FN_NEED_FAST_CALL
#undef EMBED_FN_NOTHROW_CALLABLE
#undef EMBED_FN_CASE_NOEXCEPT

#endif // EMBED_FUNCTION_HPP_

