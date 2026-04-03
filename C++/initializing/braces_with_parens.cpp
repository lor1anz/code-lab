#include <vector>
#include <atomic>

class Widget 
{
public:
    Widget() = default;
    Widget(int) {}
    Widget(int, bool) {}
    Widget(int, double) {}

    // Widget(std::initializer_list<long double>) {}

private:
    int x{0};   // ok, x = 0
    int y = 0;  // ok, y = 0
    // int z(0);   // error!
};

int main()
{
    // simple case
    {
        Widget w1;      // default ctor
        Widget w2 = w1; // copy ctor
        w2 = w1;        // operator=

        // uniform initialization with braces
        int x(0);
        int y = 0;
        int z{0};
        int t = {0};

        // both are the same
        std::vector<int> v1{1, 2, 3};    // v1 = [1, 2, 3]
        std::vector<int> v2 = {1, 2, 3}; // v2 = [1, 2, 3]

        // uncopyable objects may be initialized
        // only using braces ot parens
        std::atomic<int> ai1{0}; // ok
        std::atomic<int> ai2(0); // ok
        // std::atomic<int> ai3 = 0; // error!

        // braced initialization prohibits
        // implicit narrowing conversion
        double s1 = 0.;
        double s2 = 0.;
        double s3 = 0.;
        // int sum{s1 + s2 + s3}; // error!

        // initializing using parens or "="
        // doesn't check for narrowing conversion
        // cause that could break too much legacy code
        int sum1(s1 + s2 + s3);     // ok
        int sum2 = s1 + s2 + s3;    // ok

        Widget w3(10);  // ctor Widget(int)
        Widget w4();    // most vexing parse! declares a function
        Widget w5{};    // default ctor
    }
    
    // complex case
    {
        // if no Widget(std::initializer_list<long double>)
        Widget w1(10, true); // Widget(int, bool)
        Widget w2{10, true}; // Widget(int, bool)
        Widget w3(10, 5.0);  // Widget(int, double)
        Widget w4{10, 5.0};  // Widget(int, double)

        // if Widget(std::initializer_list<long double>)
        Widget w1(10, true); // Widget(int, bool)
        Widget w2{10, true}; // Widget(std::initializer_list<long double>)
        Widget w3(10, 5.0);  // Widget(int, double)
        Widget w4{10, 5.0};  // Widget(std::initializer_list<long double>)

        // compilers determination to math braced initialization 
        // with ctors, taking std::initializer_list is very strong
        // and almost completly leads to ignore other ctors
        
        // exception: no way to convert the type of arguments
        // in braced initialization to type int std::initializer_list
    
        Widget w5{}; // always calls default ctor
        Widget w6({}); // calls Widget(std::initializer_list<T>) with empty list

        // complex interface:
        std::vector<int> v1(5, 2); // v1 = [2, 2, 2, 2, 2]
        std::vector<int> v2{5, 2}; // v2 = [5, 2]
    }

    return 0;
}
