#include <iostream>
using namespace std;

// 基础模板：用于移除指针类型，此处为通用版本，未做具体实现
template<typename  T>
struct remove_pointer
{
    using type = T;
};

// 模板特化：当传入的类型 T 是指针类型时，移除指针得到原始类型
template<typename T>
struct remove_pointer<T *>
{
    // 定义一个类型别名 type，其值为去掉指针后的原始类型
    using type = T;
};

// 模板特化：当传入的类型 T 是常量类型时，移除常量修饰符
template<typename T>
struct remove_const<const T>
{
    // 定义一个类型别名 type，其值为去掉常量修饰符后的原始类型
    using type =T;
};


bool Foo(int){ return true; }

struct Person
{
    bool IsFemale() {};
    bool IsFemaleConst() const {};

    float height;
};


//萃取成员函数的类型特征（返回类型、参数类型、是否const等）。
//这个模板接受一个函数指针类型作为模板参数，并通过decltype来获取函数的类型信息。
//然后，它使用std::conditional_t来判断函数是否是const成员函数，并提取返回类型和参数类型。
//最后，它将这些信息封装在一个结构体中，并通过using来定义别名。
template<typename T>
struct function_traits;

// 普通成员函数特化
template<typename Ret, typename Class, typename... Args>
struct function_traits<Ret(Class::*)(Args...)> {
    using return_type = Ret;       // 返回值类型
    using class_type = Class;      // 所属类类型
    static constexpr bool is_const = false; // 非const函数
};

// const成员函数特化
template<typename Ret, typename Class, typename... Args>
struct function_traits<Ret(Class::*)(Args...) const> {
    using return_type = Ret;
    using class_type = Class;
    static constexpr bool is_const = true; // const函数
};



//萃取类的类型特征（成员函数、成员变量等）。
//这个模板接受一个类类型作为模板参数，并通过decltype来获取类的类型信息。
//然后，它使用std::conditional_t来判断类是否是const成员函数，并提取返回类型和参数类型。
//最后，它将这些信息封装在一个结构体中，并通过using来定义别名。
template<typename T> struct TypeInfo;  // 前置声明

#define BEGIN_CLASS(T)                  \
 template<> struct TypeInfo<T>{          \
    using type = T; 

#define FUNCTIONS(...) using functions=tuple<__VA_ARGS__>;

#define END_CLASS() };

BEGIN_CLASS(Person)
    FUNCTIONS(function_traits<decltype(&Person::IsFemale)>,
    function_traits<decltype(&Person::IsFemaleConst)>)
END_CLASS()

/*宏展开后代码：
template<> struct TypeInfo<Person>{
    using type = Person;
    using functions=std::tuple<
        function_traits<decltype(&Person::IsFemale)>,
        function_traits<decltype(&Person::IsFemaleConst)>
    >;
};*/

int main(int argc, char* argv[])
{
    // 定义一个类型别名 type，使用 remove_pointer 模板移除 int* 的指针，得到 int 类型
    using type=remove_pointer<int*>::type;

    
    /*decltype(ptr)会在编译阶段分析ptr并返回。ptr是&Fo0的结果，也就是函数Fo0的指针*/
    auto ptr=&Foo;
    using function_info=function_traits<decltype(ptr)>;
    using param=function_info::param; //int
    using return_type=function_info::return_type; //bool

    using function_info2=function_traits<decltype(&Person::IsFemale)>;
    /*  这样子写是错误的，因为decltype(&Person::IsFemale)返回的是一个指向成员函数的指针，而不是一个指向成员函数的指针类型。
     *using param2=function_info2::param; //Person*
    using return_type2=function_info2::return_type; //bool
    using function_info3=function_traits<decltype(&Person::IsFemaleNonConst)>;*/
    bool (Person::*p)(void)=&Person::IsFemale; //定义了一个指向成员函数的指针p，该指针指向Person类的成员函数IsFemale，返回类型为bool，参数为空。
    bool (Person::*p1)(void) const=&Person::IsFemaleConst; //定义了一个指向成员函数的指针p1，该指针指向Person类的成员函数IsFemaleConst，返回类型为bool，参数为空。
    float (Person::*p2)(void)=&Person::height; //定义了一个指向成员变量的指针p2，该指针指向Person类的成员变量height，返回类型为float，参数为空。;


    using type=TypeInfo<Person>::type;

    system("pause");

    
    return 0;
}
