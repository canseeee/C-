// 实现Vector
#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <stdio.h>


struct Vector
{
    int *m_data;
    size_t m_size;
    size_t m_cap;

    Vector()
    {
        m_size = 0;
        m_data = nullptr;
        m_cap = 0;
    }
    
    explicit Vector(size_t n)  // 防止隐式转换，explicit关键字，只能显示调用
    {
        m_data = new int[n]{}; // 初始化所有元素为0,如果不加{}，则会是随机值
        memset(m_data, 0, n * sizeof(int));
        m_size = n;
        m_cap = n;
    }

    void clear()
    {
        reserve(0);
    }
    
    void shrink_to_fit() noexcept
    {
        auto old_data = m_data;
        m_cap = m_size;
    }

    void reserve(size_t n)  
    {
        if (n <= m_cap) return;
        n=std::max(n,m_cap*2);
        auto old_data = m_data;

        if (n == 0)
        {
            m_data = nullptr;
            m_size = 0;
        }
        else{
            m_data = new int[n]{}; 
            m_cap = n;
        }
        if (old_data != nullptr){
            if (m_size != 0)
            {
                memcpy(m_data, old_data,m_size * sizeof(int));  // 拷贝旧数据到新数据
                delete[] old_data;
            }

        }
    }

    size_t size() const  // size(Vector *this)默认会有，所以要加const
    {
        return m_size;
    }

    size_t capacity() const
    {
        return m_cap;
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

    int const &at(size_t i) const
    {
        if (i>=m_size) throw std::out_of_range("Vector::at");
        return m_data[i];
    }

    int &at(size_t i)
    {
        if (i>=m_size) throw std::out_of_range("Vector::at");
        return m_data[i];
    }

    Vector(Vector &&other)
    {
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
    }

    Vector &operator=(Vector &&other)
    {
        m_data = other.m_data;
        m_size = other.m_size;
        other.m_data = nullptr;
        other.m_size = 0;
        return *this;
    }
    
    Vector(const Vector& other)
    {
        m_size = other.m_size;  
        m_data = new int[m_size];
        memcpy(m_data, other.m_data, m_size*sizeof(int));
    }

    Vector &operator=(Vector const& other)
    {
        clear();
        m_size=other.m_size;
        if (m_size != 0){
            m_data = new int[m_size];
            memcpy(m_data, other.m_data, m_size*sizeof(int));
        }
        return *this;
    }

    int const &front() const
    {
        return operator[](0);
    }

    int &front()
    {
        return operator[](0);
    }
    
    int const &back()
    {
        return operator[](size() - 1);
    }
    
    int &back()
    {
        return operator[](size() - 1);
    }
    
    void push_back(int value)
    {
        resize(size() + 1);
        back() = value;
    }

    void erase(size_t i,size_t iend)
    {
        size_t n = iend - i;
        for (size_t j=iend; j<m_size; j++)
        {
            m_data[j-n] = std::move(m_data[j]);
        }
        resize(m_size-n);
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
 