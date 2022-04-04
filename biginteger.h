#pragma once

#include <iostream>
#include <string>
#include "Vector.h"
#include "helpers.h"

#define BASE_POW 32

//--------------------------------
// BigInteger
//--------------------------------
class BigInteger {
    static constexpr uint64_t base = 1ull << BASE_POW;

    bool m_is_positive;
    Vector<uint32_t> m_digits;

public:

    //--------------------------------
    // Constructors
    //--------------------------------
    // constructors from integer types
    BigInteger(short num) : m_is_positive(num >= 0), m_digits(1) {
        num = abs(num);
        m_digits.push_back(num);
    }

    BigInteger(unsigned short num) : m_is_positive(true), m_digits(1) {
        m_digits.push_back(num);
    }

    BigInteger(int num = 0) : m_is_positive(num >= 0), m_digits(1) {
        num = abs(num);
        m_digits.push_back(num);
    }

    BigInteger(unsigned int num) : m_is_positive(true), m_digits(1) {
        m_digits.push_back(num);
    }

    BigInteger(long num) : m_is_positive(num >= 0), m_digits(1) {
        num = abs(num);
        m_digits.push_back(num);
    }

    BigInteger(unsigned long num) : m_is_positive(true), m_digits(1) {
        m_digits.push_back(num);
    }

    BigInteger(long long num) : m_is_positive(num >= 0), m_digits(2) {
        num = abs(num);
        m_digits.push_back(mod_by_pow_of_2(num, BASE_POW));
        auto high_order_digit = (long long) div_by_pow_of_2(num, BASE_POW);
        if (high_order_digit != 0) {
            m_digits.push_back(high_order_digit);
        }
    }

    BigInteger(unsigned long long num) : m_is_positive(true), m_digits(2) {
        m_digits.push_back(mod_by_pow_of_2(num, BASE_POW));
        unsigned long long high_order_digit = div_by_pow_of_2(num, BASE_POW);
        if (high_order_digit != 0) {
            m_digits.push_back(high_order_digit);
        }
    }

    // constructor from string
    explicit BigInteger(const std::string &s);

    // copy and move constructors
    BigInteger(const BigInteger &num);

    BigInteger(BigInteger &&num) noexcept;

    // copy and move assignment
    BigInteger &operator=(const BigInteger &num) = default;

    BigInteger &operator=(BigInteger &&num) noexcept;

    //--------------------------------
    // Arithmetic Operators
    //--------------------------------
    // binary operators
    BigInteger &operator+=(const BigInteger &b);

    friend BigInteger operator+(BigInteger a, const BigInteger &b) {
        a += b;
        return a;
    }

    BigInteger &operator-=(const BigInteger &b);

    friend BigInteger operator-(BigInteger a, const BigInteger &b) {
        a -= b;
        return a;
    }

    BigInteger &operator*=(const BigInteger &b);

    friend BigInteger operator*(BigInteger a, const BigInteger &b) {
        a -= b;
        return a;
    };

    BigInteger &operator/=(const BigInteger &b);

    friend BigInteger operator/(BigInteger a, const BigInteger &b) {
        a /= b;
        return a;
    };

    BigInteger &operator%=(const BigInteger &b);

    friend BigInteger operator%(BigInteger a, const BigInteger &b) {
        a /= b;
        return a;
    };

    // unary operators
    friend BigInteger operator-(const BigInteger &a);

    friend BigInteger operator+(const BigInteger &a);

    // increment
    BigInteger &operator++();

    BigInteger operator++(int) {
        BigInteger old(*this);
        operator++();
        return old;
    };

    // decrement
    BigInteger &operator--();

    BigInteger operator--(int) {
        BigInteger old(*this);
        operator--();
        return old;
    };

    //--------------------------------
    // Comparison operators
    //--------------------------------
    friend bool operator<(const BigInteger &a, const BigInteger &b);

    friend inline bool operator>(const BigInteger &a, const BigInteger &b) { return b < a; }

    friend inline bool operator<=(const BigInteger &a, const BigInteger &b) { return !(a > b); }

    friend inline bool operator>=(const BigInteger &a, const BigInteger &b) { return !(a < b); }

    friend bool operator==(const BigInteger &a, const BigInteger &b);

    friend inline bool operator!=(const BigInteger &a, const BigInteger &b) { return !(a == b); }

    //--------------------------------
    // Bitwise operators
    //--------------------------------
    // binary operators
    BigInteger &operator&=(const BigInteger &b);

    friend BigInteger operator&(BigInteger a, const BigInteger &b) {
        a &= b;
        return a;
    };

    BigInteger &operator|=(const BigInteger &b);

    friend BigInteger operator|(BigInteger a, const BigInteger &b) {
        a |= b;
        return a;
    };

    BigInteger &operator^=(const BigInteger &b);

    friend BigInteger operator^(BigInteger a, const BigInteger &b) {
        a ^= b;
        return a;
    };

    BigInteger &operator>>=(const BigInteger &b);

    friend BigInteger operator>>(BigInteger a, const BigInteger &b) {
        a >>= b;
        return a;
    };

    BigInteger &operator<<=(const BigInteger &b);

    friend BigInteger operator<<(BigInteger a, const BigInteger &b) {
        a <<= b;
        return a;
    };

    // unary operators
    friend BigInteger operator~(const BigInteger &a);

private:

    //--------------------------------
    // Private methods
    //--------------------------------
    BigInteger &add_number_with_same_sign(const BigInteger &b);

    BigInteger &subtract_lesser_number_with_same_sign(const BigInteger &b);

    void check_zero_sign();

    void remove_high_order_zeros();

    inline void change_sign() { m_is_positive = !m_is_positive; }

    //--------------------------------
    // Non-member functions
    //--------------------------------
//    friend std::ostream &operator<<(std::ostream &out, const BigInteger &num);

    friend std::string to_string(const BigInteger &num);

};
