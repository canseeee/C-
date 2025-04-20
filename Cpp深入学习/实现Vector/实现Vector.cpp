// 实现Vector
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <stdio.h>


struct Vector
{
    int *m_data;
    size_t m_size;

    Vector()
    {
        m_size = 0;
        m_data = nullptr;
    }
    
    explicit Vector(size_t n)  // 防止隐式转换，explicit关键字，只能显示调用
    {
        m_data = new int[n]{}; // 初始化所有元素为0,如果不加{}，则会是随机值
        m_size = n;
    }

    void clear()
    {
        resize(0);
    }
    
    void resize(size_t n)  
    {
        auto old_data = m_data;
        auto old_size = m_size;

        if (n == 0)
        {
            m_data = nullptr;
            m_size = 0;
        }
        else{
            m_data = new int[n]{}; 
            m_size = n;
            if (old_data != nullptr){
                memcpy(m_data, old_data, std::min(old_size,n * sizeof(int));  // 拷贝旧数据到新数据
                delete[] old_data;
            }
        }
    }

    size_t size() const  // size(Vector *this)默认会有，所以要加const
    {
        return m_size;
    }

    // 重载[]运算符，实现下标访问元素,返回const引用，防止拷贝
    int const &operator[](size_t i) const
    {
        return m_data[i];
    }
    // 返回引用，方便修改值
    int &operator[](size_t i)
    {
        return m_data[i];
    }

    Vector(const Vector& other)
    {
        m_size = other.m_size;  
        m_data = new int[m_size];
        memcpy(m_data, other.m_data, m_size*sizeof(int));
    }
    
    ~Vector()
    {
        delete[] m_data;
    }
};

int main(int argc, char* argv[])
{
    Vector v;
    Vector arr(3);
    for (size_t i = 0; i < arr.size(); i++)
    {
        printf("%d\n", arr[i]);
    }
    system("pause");
    return 0;
}
 