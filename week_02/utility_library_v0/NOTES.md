# Funcion Templates

1. A function template is a compiler blueprint which allows us to write function for various data types by using a placeholder ( ```T add(T a,T b){}``` ).
2. Now we do not have to manually overload the function for int double float separately because at compile time the compiler automatically deduces the variable type of T from arguments and geneate appropriate speialization

***T cannot be determined from runtime input because template type must be resolved at compile time.***
```cpp 
#include<iostream>

// Declare the template
template <typename T>

T add(T a, T b){
    return a+b; //Generic Addition
}

int main(){
    //compiler autimaically deduces T = int -> Instantiates add<int>(int,int)
    std::cout<<add(3,4)<<"\n";

    //compiler automatically deduces T = double -> Instantiates add<double>(double,double)
    std::cout<<add(3.4,4.3)<<"\n";

    return 0;
}
```

### Why do we need Function templates?

1. **The Code Duplication** : Without generics, for writing same function for various data types we have to manually write(overload) same function multiple tims.
2. **Maintenance** : If we want to update the function we have to change every overloaded function. If we miss any function it may lead to uncertain behaviour.
3. **Violates DRY Principle** : Manually overloading violates *DRY(Don't Repeat Yourself)* principle by repeating identical logic across types.

***Function templates replace $N$ redundant function overloads with 1 universal blueprint.***

### How Function templates Insantiation works?

1. **Phasing :** When compiler encounters templates only basic syntax is checked. No machine code is generated.
2. **Instantiation on Demand :** Machine code is generated only when the functon template is called. When compiler sees ```add(3,4)```: It deduces that type of T is int and Instantiate ```add<int>(int,int)```
3. **Handling Duplicate Instantiations :** If two or more files call ```add<int>(3,4)``` , both object file contain compiled version of ```add<int>```. To preent *"duplicate symbol"* linker error: Compiler marks instantiated template function as weak symbol(or COMDAT). The linker atomatically discards weak copies, keeping exactly one unified copy of ```add<int>```.

***If a template is defined but never called anywhere in your code, **zero machine code** is emitted for it!***

<br>
<hr>

# Class Templates

Class Template is a compiler blueprint used to generate user defined tpes(classes or structs) where one or more data type or constants are parameterized using palceholder types (declared via tempalte <typename T>) 

```cpp
#include<iostream>

template <typename T>

class Box{
    private:
    T m_value{};

    public:
    explicit Box(T value):m_value(value){}

    T get_value() const{
        return m_value;
    }
};

int main(){
    Box<int> int_box(42);

    Box<double> double_box(42.2);

    std::cout<<"Int Box : "<<int_box.get_value()<<"\n";
    std::cout<<"Double Box : "<<double_box.get_value()<<"\n";

    return 0;
}
```

*To avoid duplication, older C-style code used raw ```void*``` pointers to hold any pointer* 
**Flaw :** ```void*``` completely erases type information, causing Type ambiguity you can push integer in double stack, compiler canno catch this which leads to crashes at runtime error

***That's why we use Class Templates***

Unlike normal C++ classes where every member function is compiled into machine code upfont Class templates member function are compiled lazily(on Demand)

This allows us to instantiate a class with type T and even if T doesn't support every operation required by all member functions, as long as we don't call those specific function.

Class template must live in header file because if we define them in .cpp file they will throw a linker error becase if we make a .cpp for definations the linker will expect to provide definitions for T=int or T=double or other types

***Templates are generally instantiated on demand, so the compiler must see their definitions at the point of instantiation. That's why template definitions are usually placed in headers; otherwise, the required specialization may not be generated, causing a linker error.***

### Critical Edge Cases

1. Vector of boolean -> Vector is desgned to optimize space and bool is a variable type that needs only one bit so vector stores 8 boolean values(8-bit) in one single byte. As the least addressable size in C++ is 1 byte so we cannot have address to those 8 bit stored in 1 byte. Individual bits don't have separate addresses, so ```vector<bool>``` cannot provide a normal bool& to each element and instead uses a proxy reference.

2. Instantiating different data types like add<int>, add<double> or add<string> and more forces the compiler to stamp out different sets of machine code for every member function. So overusing templates across dozens of types can lead to bloated binary sizes.

3. Prior to C++20,non-type template parameters were restricted to integers, enums or pointer/references. Double or complex user defined objects cannot be used as non-type template parameters

