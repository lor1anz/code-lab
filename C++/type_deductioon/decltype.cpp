

void authUser() {}
// C++11: primary use of decltype is declaring function templates
// where the function's return type depends on its parameter types
template<typename Container, typename Index>
auto authAndAccess1(Container& c, Index i) -> decltype(c[i]) {
    authUser();
    return c[i];
}

// C++14: we can omit the trailing return type
// but auto return type deduction will strip off the refernece
template<typename Container, typename Index>
auto authAndAccess2(Container& c, Index i) {
    authUser();
    return c[i];
}

// to get exactly the same type that the expr c[i] returns
// we should use this constructure
template<typename Container, typename Index>
decltype(auto) authAndAccess(Container& c, Index i) {
    authUser();
    return c[i];
}

// more general version of function using perfect forwarding
template<typename Container, typename Index>
decltype(auto) authAndAccess(Container&& c, Index i) {
    authUser();
    return std::forward<Container>(c)[i];
}

// C++11 variant
template<typename Container, typename Index>
auto authAndAccess(Container&& c, Index i) -> decltype(std::forward<Container>(c)[i]) {
    authUser();
    return std::forward<Container>(c)[i];
}

// we also can use decltype(auto) for vars
decltype(auto) dangerous_global_var = 42;

int main()
{
    // decltype tells you the name's or expr's type
    // no suprises here
    const int x = 42; // decltype(x) is const int
    struct Point { int x, y; }; // decltype(Point::x) is int
    Point p;    // decltype(p) is Point
    
    // curious moment
    // decltype defines the (expr) as lvalue
    int y = 42;
    // decltype(y) is int
    // decltype((y)) in int&

    return 0;
}
