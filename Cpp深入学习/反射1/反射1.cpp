#include <iostream>
#include <ostream>
#include <string>
#include <tuple>
using namespace std;

// 用于提取成员函数的特征信息（返回类型、所属类、是否const等）
// 函数特征萃取模板（主模板声明）
template<typename T>
struct function_traits;

// 普通成员函数特化
template<typename Ret, typename Class, typename... Args>
struct function_traits<Ret(Class::*)(Args...)> {
    using return_type = Ret;       // 返回值类型
    using class_type = Class;      // 所属类类型
    static constexpr bool is_const = false; // 标记是否为const成员函数
};

// const成员函数特化
template<typename Ret, typename Class, typename... Args>
struct function_traits<Ret(Class::*)(Args...) const> {
    using return_type = Ret;
    using class_type = Class;
    static constexpr bool is_const = true; // 标记为const成员函数
};

// 示例类：Person
struct Person final
{
    string familyname;  // 姓氏成员变量
    float height;      // 身高成员变量
    bool isFemale;     // 性别标识成员变量

    // const成员函数：自我介绍
    void IntroduceMyself() const {}
    
    // const成员函数：获取性别
    bool IsFemale() const {return false;}
    
    // 普通成员函数：结婚操作
    bool GetMarried(Person& other)
    {
        bool success = other.isFemale != isFemale;  // 异性才能结婚
        if (isFemale)
        {
            familyname = "Mrs."+other.familyname;  // 女方改姓
        }
        else
        {
            familyname = "Mr."+familyname;        // 男方加前缀
        }
        return success;
    }
};

template<typename T, typename Class>
struct variable_traits {
    using value_type = T;
    using class_type = Class;
};

// 类型信息模板（前置声明）
template<typename T>
struct TypeInfo;

// 利用函数重载决议推导各种函数指针的确切类型
// 辅助函数：推导普通函数指针类型
template<typename RetT,typename... Params>
auto function_pointer_type(RetT(*)(Params...))->RetT(*)(Params...);

// 辅助函数：推导普通成员函数指针类型
template<typename RetT,typename Class,typename... Params>
auto function_pointer_type(RetT(Class::*)(Params...))->RetT(Class::*)(Params...);

// 辅助函数：推导const成员函数指针类型
template<typename RetT,typename Class,typename... Params>
auto function_pointer_type(RetT(Class::*)(Params...) const)->RetT(Class::*)(Params...) const;

// 类型别名：获取函数指针类型
template<auto F>
using function_pointer_type_t = decltype(function_pointer_type(F));

// 类型别名：获取函数特征类型
template<auto F>
using function_traits_t = function_traits<function_pointer_type_t<F>>;

template <typename T>
struct is_function
{
    static constexpr bool value = std::is_function_v<T> || std::is_member_function_pointer_v<T>;
};

template <typename T>
constexpr bool is_function_v = is_function<T>::value;


template<typename T>
struct basic_field_traits;

template<typename T>
struct basic_field_traits<T,true>:public function_traits<T>
{
    using traits = function_traits<T>;
    constexpr bool is_member() const {return traits::is_member;}
    constexpr bool is_const() const {return traits::is_const;}
    constexpr bool is_function() const {return traits::is_function;}
    constexpr bool is_variable() const {return false;}
};

template<typename T>
struct basic_field_traits<T,false>:public variable_traits<T>
{
    using traits = function_traits<T>;
    constexpr bool is_member() const {return traits::is_member;}
    constexpr bool is_const() const {return traits::is_const;}
    constexpr bool is_function() const {return false;}
    constexpr bool is_variable() const {return true;}
};

// 成员变量特征模板
template<typename T>
struct field_traits :public basic_field_traits<T,is_function<T>>
{
    // constexpr :这个关键字表明该构造函数可以在编译时执行，意味着在编译阶段就能创建field_traits对象。这在元编程中非常有用，能让一些操作在编译时完成，提升运行时的性能!
    constexpr field_traits(T&& pointer):pointer{pointer} {}
    T pointer;  // 存储成员变量/函数指针
};

#define BEGIN_CLASS(X) template<> struct TypeInfo<X>{

#define functions(...) static constexpr auto functions = std::make_tuple(__VA_ARGS__);
#define func(F) field_traits{F}

#define END_CLASS()     \
    }                   \
    ;

BEGIN_CLASS(Person)
    functions(
        func(&Person::GetMarried),
        func(&Person::IsFemale),
        func(&Person::IntroduceMyself)
    )
END_CLASS()

template<typename T>
constexpr auto type_info()
{
    return TypeInfo<T>{};
}

// Person类型的特化信息
template<>
struct TypeInfo<Person>
{
    // 收集成员函数信息
    using functions = std::tuple<function_traits_t<&Person::IntroduceMyself>>;
};



int main(int argc, char* argv[])
{
    
    // 获取成员变量类型信息（示例，实际代码中variable_traits未定义）
    using nameType = variable_traits<decltype(&Person::familyname)>;
    using funcType = variable_traits<decltype(&Person::GetMarried)>;

    // 测试函数指针类型推导
    using type1=function_pointer_type_t<&Person::GetMarried>;  // 推导普通成员函数
    using type2=function_pointer_type_t<&Person::IntroduceMyself>; // 推导const成员函数
    using type3=function_pointer_type_t<&Person::IsFemale>;    // 推导const成员函数

    // 验证类型推导是否正确
    static_assert(std::is_same_v<type1,bool (Person::*)(Person&)>);  // 应匹配GetMarried类型
    static_assert(std::is_same_v<type2,void (Person::*)() const>);   // 应匹配IntroduceMyself类型
    static_assert(std::is_same_v<type3,bool (Person::*)() const>);   // 应匹配IsFemale类型

    // 创建成员函数特征对象
    //auto field=field_traits(&Person::GetMarried); // 推导普通成员函数

    auto field=field_traits(&Person::GetMarried); // 推导const成员函数
    cout<<field.is_member<<endl;
    cout<<field.is_const<<endl;
    cout<<(field.pointer==&Person::GetMarried)<<endl;

    constexpr auto info=type_info<Person>();
    
    return 0;
}