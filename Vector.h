#pragma once

#include <iostream>

//--------------------------------
// Vector
//--------------------------------
template<typename T>
class Vector {

    size_t m_size;
    size_t m_capacity;
    T *m_data;

public:

    //--------------------------------
    // Getters
    //--------------------------------
    [[nodiscard]] unsigned int size() const { return m_size; }

    [[nodiscard]] unsigned int capacity() const { return m_capacity; }

    //--------------------------------
    // Constructors and destructor
    //--------------------------------
    explicit Vector(size_t N = 0) : m_size(0), m_capacity(N), m_data(nullptr) {
        if (m_capacity > 0) {
            m_data = new T[m_capacity];
            if (!m_data) throw "Out of memory";
        }
    }

    // copy and move constructors
    Vector(const Vector &);
    Vector(Vector &&) noexcept;

    // copy and move assignment
    Vector &operator=(const Vector &);
    Vector &operator=(Vector &&) noexcept;

    // destructor
    ~Vector() {
        clear();
    }

    //--------------------------------
    // Iterator
    //--------------------------------
    class iterator {
        T *current_;
    public:
        explicit iterator(T *pos = nullptr) : current_(pos) {}

        T &operator*() const { return *current_; }

        T &operator*() { return *current_; }

        bool operator==(const iterator &pos) const { return current_ == pos.current_; }

        bool operator!=(const iterator &pos) const { return current_ != pos.current_; }

        iterator &operator++() {
            ++current_;
            return *this;
        }

        iterator operator++(int) {
            T *tmp = current_;
            ++current_;
            return iterator(tmp);
        }

        friend class MyArray;
    };

    iterator begin() const { return iterator(m_data); }

    iterator end() const { return iterator(m_data + m_size); }

    //--------------------------------
    // Reverse iterator
    //--------------------------------
    class reverse_iterator {
        T *current_;
    public:
        explicit reverse_iterator(T *pos = nullptr) : current_(pos) {}

        T &operator*() const { return *current_; }

        T &operator*() { return *current_; }

        bool operator==(const reverse_iterator &pos) const { return current_ == pos.current_; }

        bool operator!=(const reverse_iterator &pos) const { return current_ != pos.current_; }

        reverse_iterator &operator++() {
            --current_;
            return *this;
        }

        reverse_iterator operator++(int) {
            T *tmp = current_;
            --current_;
            return iterator(tmp);
        }
    };

    reverse_iterator rbegin() const { return reverse_iterator(m_data + m_size - 1); }

    reverse_iterator rend() const { return reverse_iterator(m_data - 1); }

    //--------------------------------
    // Public methods
    //--------------------------------
    void push_back(const T &X);

    void pop_back();

    const T &back() const { return m_data[m_size - 1]; }

    void reserve(size_t new_cap);

    T &operator[](unsigned int i) const {
        if (i >= m_size) throw "Out of array's bounds";
        return m_data[i];
    }

    void empty() { m_size = 0; }

    void clear() {
        if (m_data) {
            delete[] m_data;
            m_size = 0;
            m_capacity = 0;
            m_data = nullptr;
        }
    }

    iterator find(const T &) const;

    void erase(const iterator &);

};

template<typename T>
void Vector<T>::push_back(const T &X) {
    if (m_size + 1 > m_capacity) {
        reserve((m_capacity + 1) * 2);
    }
    m_data[m_size] = X;
    ++m_size;
}

template<typename T>
void Vector<T>::pop_back() {
    --m_size;
}

template<typename T>
Vector<T>::Vector(const Vector<T> &X) {
    m_size = X.m_size;
    m_capacity = X.m_capacity;
    m_data = new T[m_capacity];
    for (size_t i = 0; i < m_size; ++i)
        m_data[i] = X.m_data[i];
}

template<typename T>
Vector<T>::Vector(Vector<T> &&X) noexcept {
    m_size = X.m_size;
    m_capacity = X.m_capacity;
    m_data = X.m_data;
    X.m_data = nullptr;
    X.m_size = 0;
    X.m_capacity = 0;
}

template<typename T>
Vector<T> &Vector<T>::operator=(const Vector<T> &X) {
    if (this != &X) {
        if (m_data) delete[] m_data;
        m_size = X.m_size;
        m_capacity = X.m_capacity; // m_capacity = X.m_size;
        m_data = new T[m_capacity];
        for (size_t i = 0; i < m_size; ++i)
            m_data[i] = X.m_data[i];
    }
    return *this;
}

template<typename T>
Vector<T> &Vector<T>::operator=(Vector<T> &&X) noexcept {
    if (this != &X) {
        if (m_data) delete[] m_data;
        m_size = X.m_size;
        m_capacity = X.m_capacity;
        m_data = X.m_data;
        X.m_data = nullptr;
        X.m_size = 0;
        X.m_capacity = 0;
    }
    return *this;
}

template<typename T>
typename Vector<T>::iterator Vector<T>::find(const T &X) const {
    for (size_t i = 0; i < m_size; ++i)
        if (m_data[i] == X) return iterator(m_data + i);
    return end();
}

template<typename T>
void Vector<T>::erase(const typename Vector<T>::iterator &pos) {
    if (pos == end()) return;
    T *cur = pos.current_;
    while (cur != (m_data + m_size - 1)) {
        *cur = std::move(*(cur + 1));
        ++cur;
    }
    --m_size;
}

template<typename T>
void Vector<T>::reserve(size_t new_cap) {
    if (new_cap <= m_capacity) return;
    T *tmp = new T[new_cap];
    if (!tmp) throw "Out of memory";
    for (size_t i = 0; i < m_capacity; ++i)
        tmp[i] = std::move(m_data[i]);
    delete[] m_data;
    m_data = tmp;
    m_capacity = new_cap;
}
