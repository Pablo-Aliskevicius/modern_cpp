/*
✎ Exercise 31.  
Write a generic lambda `[]<typename T> requires std::integral<T> (T x)` 
and call it with `int` and `double`; 
read the error.

*/
#include <concepts>
#include <iostream>

int main() {
    auto f = []<typename T> requires std::integral<T> (T x) { return x + 1; };
    std::cout << f(42) << '\n';
    // std::cout << f(3.14) << '\n'; // compile error: no matching function for call to 'main()::<lambda>(double)'

    /*
error C3889: call to object of class type 'main::<lambda_1>': no matching call operator found    
note: could be 'auto main::<lambda_1>::operator ()(T) const'
note: the associated constraints are not satisfied
note: the concept 'std::integral<double>' evaluated to false 
      ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
note: the constraint was not satisfied
    */
}