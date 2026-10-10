# ✎ Exercise 13. #
Add a `Delete{std::string key}` alternative and observe the compile error from describe. Fix it.

# ✎ Exercise 14. #
Replace overloaded + lambdas with a visitor struct having operator() overloads. Which do you prefer?

_Answer_: the original, I want a compile error if I forget one option.  
A missing overload might not (always?) generate it (IDK, IDC).

# Exercise 13 error: #

Note line `variant_exercises.cpp:41:22:   required from here`

```bash
pablo@pablo-dell-ubuntu:~/projects/modern_cpp/cpp17$ g17 variant_exercises.cpp 
In file included from /usr/include/c++/13/bits/move.h:37,
                 from /usr/include/c++/13/bits/exception_ptr.h:41,
                 from /usr/include/c++/13/exception:164,
                 from /usr/include/c++/13/ios:41,
                 from /usr/include/c++/13/ostream:40,
                 from /usr/include/c++/13/iostream:41,
                 from variant_exercises.cpp:4:
/usr/include/c++/13/type_traits: In substitution of ‘template<class _Fn, class ... _Args> using std::invoke_result_t = typename std::invoke_result::type [with _Fn = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(constFlush&)> >; _Args = {const Delete&}]’:
/usr/include/c++/13/variant:1131:14:   required from ‘constexpr bool std::__detail::__variant::__check_visitor_results(std::index_sequence<_Idx ...>)[with _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >; _Variant = const std::variant<Read, Write, Flush, Delete>&; long unsigned int ..._Idxs = {0, 1, 2, 3}; std::index_sequence<_Idx ...> = std::integer_sequence<long unsigned int, 0, 1, 2, 3>]’
/usr/include/c++/13/variant:1868:44:   required from ‘constexpr std::__detail::__variant::__visit_result_t<_Visitor, _Variants ...> std::visit(_Visitor&&, _Variants&& ...) [with _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >; _Variants = {const variant<Read, Write, Flush, Delete>&}; __detail::__variant::__visit_result_t<_Visitor, _Variants ...> = __cxx11::basic_string<char>]’
variant_exercises.cpp:41:22:   required from here
/usr/include/c++/13/type_traits:3073:11: error: no type named ‘type’ in ‘struct std::invoke_result<overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >, const Delete&>’
 3073 |     using invoke_result_t = typename invoke_result<_Fn, _Args...>::type;
      |           ^~~~~~~~~~~~~~~
In file included from variant_exercises.cpp:6:
/usr/include/c++/13/variant: In instantiation of ‘constexpr std::__detail::__variant::__visit_result_t<_Visitor, _Variants ...> std::visit(_Visitor&&, _Variants&& ...) [with _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(constOp&)::<lambda(const Flush&)> >; _Variants = {const variant<Read, Write, Flush, Delete>&}; __detail::__variant::__visit_result_t<_Visitor, _Variants ...> = __cxx11::basic_string<char>]’:
variant_exercises.cpp:41:22:   required from here
/usr/include/c++/13/variant:1868:44:   in ‘constexpr’ expansion of ‘std::__detail::__variant::__check_visitor_results<overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >, const std::variant<Read, Write, Flush, Delete>&, 0, 1, 2, 3>((std::make_index_sequence<4>(), std::make_index_sequence<4>()))’
/usr/include/c++/13/variant:1867:26: error: ‘constexpr’ call flows off the end of the function
 1867 |           constexpr bool __visit_rettypes_match = __detail::__variant::
      |                          ^~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/13/variant:1872:29: error: non-constant condition for static assertion
 1872 |               static_assert(__visit_rettypes_match,
      |                             ^~~~~~~~~~~~~~~~~~~~~~
/usr/include/c++/13/variant:1875:15: error: return-statement with no value, in function returning ‘std::__detail::__variant::__visit_result_t<overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >, const std::variant<Read, Write, Flush, Delete>&>’ {aka ‘std::__cxx11::basic_string<char>’} [-fpermissive]
 1875 |               return;
      |               ^~~~~~
/usr/include/c++/13/variant: In instantiation of ‘static constexpr decltype(auto) std::__detail::__variant::__gen_vtable_impl<std::__detail::__variant::_Multi_array<_Result_type (*)(_Visitor, _Variants ...)>, std::integer_sequence<long unsigned int, __indices ...> >::__visit_invoke(_Visitor&&, _Variants ...) [with _Result_type = std::__detail::__variant::__deduce_visit_result<std::__cxx11::basic_string<char> >; _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >&&; _Variants = {const std::variant<Read, Write, Flush, Delete>&}; long unsigned int ...__indices = {3}]’:
/usr/include/c++/13/variant:1818:5:   required from ‘constexpr decltype(auto) std::__do_visit(_Visitor&&, _Variants&& ...) [with _Result_type = __detail::__variant::__deduce_visit_result<__cxx11::basic_string<char> >; _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >; _Variants = {const variant<Read, Write, Flush, Delete>&}]’
/usr/include/c++/13/variant:1878:34:   required from ‘constexpr std::__detail::__variant::__visit_result_t<_Visitor, _Variants ...> std::visit(_Visitor&&, _Variants&& ...) [with _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >; _Variants = {const variant<Read, Write, Flush, Delete>&}; __detail::__variant::__visit_result_t<_Visitor, _Variants ...> = __cxx11::basic_string<char>]’
variant_exercises.cpp:41:22:   required from here
/usr/include/c++/13/variant:1060:31: error: no matching function for call to ‘__invoke(overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >, const Delete&)’
 1060 |           return std::__invoke(std::forward<_Visitor>(__visitor),
      |                  ~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 1061 |               __element_by_index_or_cookie<__indices>(
      |               ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 1062 |                 std::forward<_Variants>(__vars))...);
      |                 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In file included from /usr/include/c++/13/bits/refwrap.h:38,
                 from /usr/include/c++/13/string:52,
                 from /usr/include/c++/13/bits/locale_classes.h:40,
                 from /usr/include/c++/13/bits/ios_base.h:41,
                 from /usr/include/c++/13/ios:44:
/usr/include/c++/13/bits/invoke.h:90:5: note: candidate: ‘template<class _Callable, class ... _Args> constexpr typename std::__invoke_result<_Functor, _ArgTypes>::type std::__invoke(_Callable&&, _Args&& ...)’
   90 |     __invoke(_Callable&& __fn, _Args&&... __args)
      |     ^~~~~~~~
/usr/include/c++/13/bits/invoke.h:90:5: note:   template argument deduction/substitution failed:
/usr/include/c++/13/bits/invoke.h: In substitution of ‘template<class _Callable, class ... _Args> constexpr typename std::__invoke_result<_Functor, _ArgTypes>::type std::__invoke(_Callable&&, _Args&& ...) [with _Callable = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >; _Args = {const Delete&}]’:
/usr/include/c++/13/variant:1060:24:   required from ‘static constexpr decltype(auto) std::__detail::__variant::__gen_vtable_impl<std::__detail::__variant::_Multi_array<_Result_type (*)(_Visitor, _Variants ...)>, std::integer_sequence<long unsigned int, __indices ...> >::__visit_invoke(_Visitor&&, _Variants ...) [with _Result_type = std::__detail::__variant::__deduce_visit_result<std::__cxx11::basic_string<char> >; _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >&&; _Variants = {const std::variant<Read, Write, Flush, Delete>&}; long unsigned int ...__indices = {3}]’
/usr/include/c++/13/variant:1818:5:   required from ‘constexpr decltype(auto) std::__do_visit(_Visitor&&, _Variants&& ...) [with _Result_type = __detail::__variant::__deduce_visit_result<__cxx11::basic_string<char> >; _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >; _Variants = {const variant<Read, Write, Flush, Delete>&}]’
/usr/include/c++/13/variant:1878:34:   required from ‘constexpr std::__detail::__variant::__visit_result_t<_Visitor, _Variants ...> std::visit(_Visitor&&, _Variants&& ...) [with _Visitor = overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >; _Variants = {const variant<Read, Write, Flush, Delete>&}; __detail::__variant::__visit_result_t<_Visitor, _Variants ...> = __cxx11::basic_string<char>]’
variant_exercises.cpp:41:22:   required from here
/usr/include/c++/13/bits/invoke.h:90:5: error: no type named ‘type’ in ‘struct std::__invoke_result<overloaded<describe(const Op&)::<lambda(const Read&)>, describe(const Op&)::<lambda(const Write&)>, describe(const Op&)::<lambda(const Flush&)> >, const Delete&>’
 1 pablo@pablo-dell-ubuntu:~/projects/modern_cpp/cpp17$ 
```