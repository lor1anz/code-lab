#include <initializer_list>

// With only one exception, auto type deduction is template type deduction
// auto = T, const auto& = ParamType
// See template_type_deduction.cpp

template<typename T>
void f(T param) {}

template<typename T>
void f_init(std::initializer_list<T> param) {}

// auto return type employ template type deduction
template<typename T>
auto f_auto(T param) { return param; }

// Curious exception
int main()
{
    auto x1 = 42;   // auto is int
    auto x2(42);    // ditto
    auto y1 = {42}; // auto is std::initializer_list<int>
    auto y2{42};    // ditto

    // auto err = {1, 2, 3.0};  // error: cannot deduce T for std::initializer_list<T>

    // f({1, 2, 3});    // error: cannot deduce type for T

    f_init({1, 2, 3});  // T is int

    // Why auto type deduction has a special rule for braced inits?
    // There are no convincing explanations. This is the rule.

    return 0;
}
