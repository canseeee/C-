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

// 成员变量特征模板
template<typename T>
struct field_traits
{
    // constexpr :这个关键字表明该构造函数可以在编译时执行，意味着在编译阶段就能创建field_traits对象。这在元编程中非常有用，能让一些操作在编译时完成，提升运行时的性能!
    constexpr field_traits(T&& pointer):pointer{pointer} {}
    T pointer;  // 存储成员变量/函数指针
};

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
    auto field=field_traits(&Person::GetMarried); // 推导普通成员函数
    
    return 0;
}