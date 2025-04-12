#include <tuple>
#include <type_traits>
#include <string_view>
#include <utility>
#include <iostream>
#include <algorithm>
#include <string>

// 基础模板声明
template<typename T> struct TypeInfo;

// 函数特征萃取增强版
template<typename T> struct function_traits;

// 普通成员函数
template<typename Ret, typename Class, typename... Args>
struct function_traits<Ret(Class::*)(Args...)> {
    using return_type = Ret;
    using class_type = Class;
    using args_type = std::tuple<Args...>;
    static constexpr bool is_const = false;
    static constexpr bool is_static = false;
};

// const成员函数
template<typename Ret, typename Class, typename... Args>
struct function_traits<Ret(Class::*)(Args...) const> {
    using return_type = Ret;
    using class_type = Class;
    using args_type = std::tuple<Args...>;
    static constexpr bool is_const = true;
    static constexpr bool is_static = false;
};

// 静态成员函数
template<typename Ret, typename... Args>
struct function_traits<Ret(*)(Args...)> {
    using return_type = Ret;
    using args_type = std::tuple<Args...>;
    static constexpr bool is_static = true;
};

// 成员变量特征
template<typename T, typename Class>
struct member_traits {
    using value_type = T;
    using class_type = Class;
};

// 宏定义增强版
#define BEGIN_CLASS(T, ...) \
template<> struct TypeInfo<T> { \
    using type = T; \
    using bases = std::tuple<__VA_ARGS__>; \
    static constexpr std::string_view name = #T;

#define FUNCTIONS(...) \
    using functions = std::tuple<__VA_ARGS__>;

#define PROPERTIES(...) \
    using properties = std::tuple<__VA_ARGS__>;

#define STATIC_FUNCTIONS(...) \
    using static_functions = std::tuple<__VA_ARGS__>;

#define END_CLASS() };

// 示例类
struct Animal {
    virtual ~Animal() = default;
    virtual void speak() const = 0;
};

struct Person : Animal {
    // 成员函数
    bool IsFemale() { return true; }
    bool IsFemaleConst() const { return true; }
    void speak() const override { std::cout << "Hello"; }
    
    // 静态函数
    static int GetCount() { return 0; }
    
    // 成员变量
    float height;
    std::string name;
};

// 生成类型信息
BEGIN_CLASS(Person, Animal)  // 记录父类信息
    FUNCTIONS(
        function_traits<decltype(&Person::IsFemale)>,
        function_traits<decltype(&Person::IsFemaleConst)>,
        function_traits<decltype(&Person::speak)>
    )
    PROPERTIES(
        member_traits<decltype(Person::height), Person>,
        member_traits<decltype(Person::name), Person>
    )
    STATIC_FUNCTIONS(
        function_traits<decltype(&Person::GetCount)>
    )
END_CLASS()

// 使用示例
int main() {
    // 获取类型名称
    std::cout << TypeInfo<Person>::name << "\n";  // 输出 "Person"
    
    // 检查父类
    static_assert(std::is_same_v<
        std::tuple_element_t<0, TypeInfo<Person>::bases>, 
        Animal>);
    
    // 遍历成员函数
    using FirstFunc = std::tuple_element_t<0, TypeInfo<Person>::functions>;
    static_assert(FirstFunc::is_const == false);
    static_assert(std::is_same_v<FirstFunc::return_type, bool>);
    
    // 访问成员变量信息
    using FirstProp = std::tuple_element_t<0, TypeInfo<Person>::properties>;
    static_assert(std::is_same_v<FirstProp::value_type, float>);
    
    return 0;
}