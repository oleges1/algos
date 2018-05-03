#include <utility>
#include <memory>
#include <iostream>

template <typename T>
struct VectorBase {
    T * data;
    std::size_t cp;

    VectorBase():
        data(nullptr),
        cp(0) {
        }

    VectorBase(std::size_t n):
        data(static_cast<T*>(operator new(n * sizeof(T)))), cp(n) {
        }

    // Копии объекта этого класса нам не нужны, поэтому запретим конструктор копирования
    // и оператор присваивания.
    VectorBase(const VectorBase&) = delete;
    VectorBase& operator =(const VectorBase&) = delete;

    // А вот конструктор перемещения определим:
    VectorBase(VectorBase&& other):
        data(other.data),
        cp(other.cp) {
            other.data = nullptr;
            other.cp = 0;
        }
    // Оператор присваивания с move-семинтикой:
    VectorBase& operator = (VectorBase&& other) {
        // С помощью std::swap обменяемся содержимым наших полей
        std::swap(data, other.data);
        std::swap(cp, other.cp);
        return *this;
    }

    ~VectorBase() {
        operator delete (data);
    }

    // Здесь должна быть реализация swap:
    // void swap(VectorBase other) {}
    void swap(VectorBase& other) {
        std::swap(data, other.data);
        std::swap(cp, other.cp);
    }
};


// Конечная реализация Vector (точнее, конструктора копирования)
template <typename T>
class Vector {
 private:
    VectorBase<T> base;
    std::size_t sz;

 public:
    Vector() {
        sz = 0;
    }

    Vector(std::size_t n): base(n), sz(n) {
    }

     // Впрочем, эта функция уже есть в стандартной библиотеке.
    Vector(const Vector& other):
        base(other.sz), sz(other.sz) {
        std::uninitialized_copy(
                other.base.data, other.base.data + sz, base.data);
    }

    std::size_t size() const {
        return sz;
    }

    std::size_t capacity() const {
        return base.cp;
    }

    T* begin() {
        return (base.data);
    }

    T* end() {
        return (base.data + sz);
    }

    T& operator[] (size_t i) {
        return (base.data[i]);
    }

    T& operator[] (size_t i) const {
        return (base.data[i]);
    }

    void pop_back() {
        sz--;
        base.data[sz].~T();
    }

    void destroy() {
        for (std::size_t i = 0; i != sz; ++i)
            base.data[i].~T();
    }

    void reserve(std::size_t n) {
        if (n <= capacity())
            return;

        VectorBase<T> vb_new(n);
        std::uninitialized_copy(base.data, base.data + size(), vb_new.data);
        destroy();
        base = std::move(vb_new);
    }

    void resize(std::size_t n) {
        if (n < size()) {
            for (std::size_t i = n; i != sz; ++i)
                base.data[i].~T();
            sz = n;
            return;
        }
        if (n > capacity()) {
            VectorBase<T> vb_new(n);
            std::uninitialized_copy(base.data, base.data + size(), vb_new.data);
            for (size_t i = sz; i != n; ++i)
                new(vb_new.data + i) T();
            destroy();
            base = std::move(vb_new);
            sz = n;
            return;
        }
        for (std::size_t i = sz; i != n; ++i)
            new (base.data + i) T();
        sz = n;
        return;
    }

    Vector& operator = (const Vector& other) {
        Vector tmp(other);
        swap(tmp);
        return *this;
    }

    void swap(Vector& other) {
        base.swap(other.base);
        std::swap(sz, other.sz);
    }

    void push_back(T&& val) {
        if (capacity() == 0) {
            VectorBase<T> vb_new(1);
            base = std::move(vb_new);
            sz++;
            new (base.data) T(std::move(val));
            return;
        }

        if (capacity() == size()) {
            VectorBase<T> vb_new(2 * size());
            std::uninitialized_copy(base.data, base.data + size(), vb_new.data);
            destroy();
            base = std::move(vb_new);
            new (base.data + size()) T(std::move(val));
            sz++;
            return;
        }

        new (base.data + size()) T(std::move(val));
        sz++;
        return;
    }

    void push_back(const T& val) {
        if (capacity() == 0) {
            VectorBase<T> vb_new(1);
            base = std::move(vb_new);
            sz++;
            new (base.data) T(val);
            return;
        }

        if (capacity() == size()) {
            VectorBase<T> vb_new(2 * size());
            std::uninitialized_copy(base.data, base.data + size(), vb_new.data);
            destroy();
            base = std::move(vb_new);
            new (base.data + size()) T(val);
            sz++;
            return;
        }

        new (base.data + size()) T(val);
        sz++;
        return;
    }

    ~Vector() {
        destroy();
    }
};
