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
    BigInteger(short num) : BigInteger((long long) num) {}

    BigInteger(unsigned short num) : BigInteger((unsigned long long) num) {}

    BigInteger(int num = 0) : BigInteger((long long) num) {}

    BigInteger(unsigned int num) : BigInteger((unsigned long long) num) {}

    BigInteger(long num) : BigInteger((long long) num) {}

    BigInteger(unsigned long num) : BigInteger((unsigned long long) num) {}

    BigInteger(long long num) : m_is_positive(num >= 0), m_digits(2) {
        num = abs(num);
        m_digits.push_back(mod_by_pow_of_2(num, BASE_POW));
        m_digits.push_back(div_by_pow_of_2(num, BASE_POW));
        remove_high_order_zeros();
    }

    BigInteger(unsigned long long num) : m_is_positive(true), m_digits(2) {
        m_digits.push_back(mod_by_pow_of_2(num, BASE_POW));
        m_digits.push_back(div_by_pow_of_2(num, BASE_POW));
        remove_high_order_zeros();
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
        a *= b;
        return a;
    };

    BigInteger &operator/=(const BigInteger &b);

    friend BigInteger operator/(BigInteger a, const BigInteger &b) {
        a /= b;
        return a;
    };

    BigInteger &operator%=(const BigInteger &b);

    friend BigInteger operator%(BigInteger a, const BigInteger &b) {
        a %= b;
        return a;
    };

    // unary operators
    friend BigInteger operator+(const BigInteger &a);

    friend BigInteger operator-(const BigInteger &a);

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
    BigInteger &operator&=(const BigInteger &b) {
        return bitwise_binary_operator(b, '&');
    }

    friend BigInteger operator&(BigInteger a, const BigInteger &b) {
        a &= b;
        return a;
    }

    BigInteger &operator|=(const BigInteger &b) {
        return bitwise_binary_operator(b, '|');
    }

    friend BigInteger operator|(BigInteger a, const BigInteger &b) {
        a |= b;
        return a;
    }

    BigInteger &operator^=(const BigInteger &b) {
        return bitwise_binary_operator(b, '^');
    }

    friend BigInteger operator^(BigInteger a, const BigInteger &b) {
        a ^= b;
        return a;
    }

    BigInteger &operator>>=(const BigInteger &b);

    friend BigInteger operator>>(BigInteger a, const BigInteger &b) {
        a >>= b;
        return a;
    }

    BigInteger &operator<<=(const BigInteger &b);

    friend BigInteger operator<<(BigInteger a, const BigInteger &b) {
        a <<= b;
        return a;
    }

    // unary operators
    friend BigInteger operator~(BigInteger a);

private:

    //--------------------------------
    // Private methods
    //--------------------------------
    BigInteger &add_number_with_same_sign(const BigInteger &b);

    BigInteger &subtract_lesser_number_with_same_sign(const BigInteger &b);

    BigInteger &bitwise_binary_operator(BigInteger b, char operation);

    bool is_zero() const;

    bool is_one() const;

    bool is_negative_one() const;

    BigInteger &multiply_by_short_number(uint32_t number);

    uint32_t divide_by_short_number(uint32_t number);

    void make_twos_complement_form();

    void check_zero_sign();

    void remove_high_order_zeros();

    [[nodiscard]] long long to_long_long() const;

    inline void change_sign() { m_is_positive = !m_is_positive; }

    //--------------------------------
    // Non-member functions
    //--------------------------------
    friend std::ostream &operator<<(std::ostream &out, const BigInteger &num) {
        out << to_string(num);
        return out;
    };

    friend std::string to_string(const BigInteger &num);

};
