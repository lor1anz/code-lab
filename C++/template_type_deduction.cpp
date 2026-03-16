#include <cstddef>  // for std::size_t

// -------ptr/ref---------- //

// If expr's type is a reference, ignore the reference part
// Then pattern-match expr's type against ParamType to determine T

template<typename T>
void f_ref(T& param) {}

template<typename T>
void f_const_ref(const T& param) {}

template<typename T>
void f_ptr(T* param) {}

template<typename T>
void f_const_ptr(const T* param) {}

void ref_ptr_template_type_deduction() {
    int x = 42;
    const int cx = x;
    const int& rx = x;

    f_ref(x);   // T is int, param's type is int&
    f_ref(cx);  // T is const int, param's type is const int&
    f_ref(rx);  // T is const int, param's type is const int&

    f_const_ref(x);     // T is int, param's type is const int& 
    f_const_ref(cx);    // T is int, param's type is const int&
    f_const_ref(rx);    // T is int, param's type is const int&

    int* px = &x;
    const int* cpx = px;

    f_ptr(px);  // T is int, param's type is int*
    f_ptr(cpx); // T is const int, param's type is const int*

    f_const_ptr(px);    // T is int, param's type is const int*
    f_const_ptr(cpx);   // T is int, param's type is const int*
}

// ------------------------ //


// -----universal-ref------ //

// If expr is an lvalue, both T and ParamType are deduced to be lvalue refernce
// If expr is an rvalue, the rules from ptr/ref deduction apply

template<typename T>
void f_uni_ref(T&& param) {}

void uni_ref_template_type_deduction() {
    int x = 42;
    const int cx = x;
    const int& rx = x;

    f_uni_ref(x);   // T is int&, param's type is int&
    f_uni_ref(cx);  // T is const int&, param's type is const int&
    f_uni_ref(rx);  // T is const int&, param's type is const int&

    f_uni_ref(42);  // T is int, param's type is int&&   
}

// ------------------------ //


// -----pass-by-value------ //

// If expr's type is a reference, ignore the reference part
// If expr is also const or volatile, ignore that too

template<typename T>
void f(T param) {}

template<typename T>
void f_const(const T param) {}

void pass_by_value_template_type_deduction() {
    int x = 42;
    const int cx = x;
    const int& rx = x;

    f(x);   // T is int, param's type is int
    f(cx);  // T is int, param's type is int
    f(rx);  // T is int, param's type is int

    f_const(x);     // T is int, param's type is const int 
    f_const(cx);    // T is int, param's type is const int
    f_const(rx);    // T is int, param's type is const int
}

// ------------------------ //


// ----------array--------- //

// If expr's type is a reference, ignore the reference part
// If expr is also const or volatile, ignore that too

void array_template_type_deduction() {
    const char name[] = "Mr. Bean"; // const char[9]
    const char* ptr_to_name = name; // const char*

    // const char* and const char[9] are not the same
    
    f(name);        // T is const char*
    f_ref(name);    // T is const char[9], param's type is const char(&)[9]
}

// We can deduce the number of elements that an array contains
template<typename T, std::size_t N>
constexpr std::size_t array_size(T (&)[N]) noexcept { return N; }

// ------------------------ //


// -----function-args------ //

void some_func(int, double) {}

void function_template_type_deduction() {
    f(some_func);       // param is void (*)(int, double)
    f_ref(some_func);   // param is void (&)(int, double)
}

// ------------------------ //


int main()
{
    ref_ptr_template_type_deduction();
    uni_ref_template_type_deduction();
    pass_by_value_template_type_deduction();
    array_template_type_deduction();
    function_template_type_deduction();
    return 0;
}
