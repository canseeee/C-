// 实现Vector
#include <cstdlib>
#include <stdio.h>


struct Vector
{
    int *m_data;
    size_t m_size;
    Vector(size_t n)
    {
        m_data = new int[n];
        m_size = n;
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

    
    ~Vector()
    {
        delete[] m_data;
    }
};

int main(int argc, char* argv[])
{
    Vector arr(3);
    for (size_t i = 0; i < arr.size(); i++)
    {
        printf("%d\n", arr[i]);
    }
    system("pause");
    return 0;
}
