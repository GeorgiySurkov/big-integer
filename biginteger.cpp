#include "biginteger.h"

BigInteger &BigInteger::operator+=(const BigInteger &b) {
    // Handle different signs
    if (m_is_positive && !b.m_is_positive) {
        return this->operator-=(-b);
    } else if (!m_is_positive && b.m_is_positive) {
        change_sign();
        this->operator-=(b);
        change_sign();
        check_zero_sign();
        return *this;
    }

    return add_number_with_same_sign(b);
}

BigInteger &BigInteger::add_number_with_same_sign(const BigInteger &b) {
    // Make sure we have enough space to sum carry
    m_digits.reserve(max(m_digits.size(), b.m_digits.size()) + 1);
    m_digits.push_back(0);

    // Sum each digit in numbers
    uint64_t carry = 0;
    for (size_t i = 0; i < max(m_digits.size(), b.m_digits.size()) || carry != 0; ++i) {
        if (i == m_digits.size()) {
            m_digits.push_back(0);
        }
        uint64_t digit = (uint64_t) m_digits[i] + (i < b.m_digits.size() ? b.m_digits[i] : 0) + carry;
        if (digit >= base) {
            carry = 1;
            digit -= base;
        } else {
            carry = 0;
        }
        m_digits[i] = digit;
    }

    remove_high_order_zeros();

    check_zero_sign();
    return *this;
}

BigInteger &BigInteger::operator-=(const BigInteger &b) {
    // Handle different signs and check that abs(*this) is not less than abs(b)
    if (m_is_positive == !b.m_is_positive) {
        return add_number_with_same_sign(b);
    } else if ((m_is_positive && *this < b) || (!m_is_positive && *this > b)) {

        // swap *this and b
        auto temp = b;
        temp.subtract_lesser_number_with_same_sign(*this);
        temp.change_sign();
        temp.check_zero_sign();

        *this = std::move(temp);
        return *this;
    }

    return subtract_lesser_number_with_same_sign(b);
}

BigInteger &BigInteger::subtract_lesser_number_with_same_sign(const BigInteger &b) {
    // Subtract digits
    int64_t carry = 0;
    for (size_t i = 0; i < b.m_digits.size() || carry != 0; ++i) {
        if (i == m_digits.size()) {
            m_digits.push_back(0);
        }
        int64_t digit = (int64_t) m_digits[i] - (carry + (int64_t) (i < b.m_digits.size() ? b.m_digits[i] : 0));
        if (digit < 0) {
            carry = 1;
            digit += base;
        } else {
            carry = 0;
        }
        m_digits[i] = digit;
    }

    remove_high_order_zeros();

    check_zero_sign();
    return *this;
}

bool operator==(const BigInteger &a, const BigInteger &b) {
    // If order of numbers or signs differ then they are not equal
    if (a.m_digits.size() != b.m_digits.size() || a.m_is_positive != b.m_is_positive) return false;

    // Compare all digits until we find different digits
    for (size_t i = 0; i < a.m_digits.size(); ++i) {
        if (a.m_digits[i] != b.m_digits[i]) {
            return false;
        }
    }

    // Otherwise, numbers are equal
    return true;
}

bool operator<(const BigInteger &a, const BigInteger &b) {
    // We need to compare all digits only if a and b have same signs and same order.
    // All other cases are handled here.
    if (a.m_is_positive != b.m_is_positive) {
        return b.m_is_positive;
    }
    if (a.m_digits.size() != b.m_digits.size()) {
        return a.m_is_positive ? a.m_digits.size() < b.m_digits.size() : a.m_digits.size() > b.m_digits.size();
    }

    // Compare all digits
    for (long i = a.m_digits.size() - 1; i >= 0; --i) {
        if (a.m_digits[i] > b.m_digits[i]) {
            return !a.m_is_positive;
        } else if (a.m_digits[i] < b.m_digits[i]) {
            return a.m_is_positive;
        }
    }

    // Otherwise, numbers are equal
    return false;
}

void BigInteger::check_zero_sign() {
    // Make sure zero is always positive
    if (m_digits.size() == 1 && m_digits.back() == 0) m_is_positive = true;
}

void BigInteger::remove_high_order_zeros() {
    while (m_digits.size() > 1 && m_digits.back() == 0) {
        m_digits.pop_back();
    }
}

BigInteger operator+(const BigInteger &a) {
    BigInteger res = a;
    res.m_is_positive = true;
    return res;
}

BigInteger operator-(const BigInteger &a) {
    BigInteger res = a;
    res.change_sign();
    res.check_zero_sign();
    return res;
}

BigInteger &BigInteger::operator=(BigInteger &&num) noexcept {
    if (&num != this) {
        m_is_positive = num.m_is_positive;
        m_digits = std::move(num.m_digits);
    }
    return *this;
}

BigInteger::BigInteger(BigInteger &&num) noexcept {
    m_is_positive = num.m_is_positive;
    m_digits = std::move(num.m_digits);
}

BigInteger::BigInteger(const BigInteger &num) {
    m_is_positive = num.m_is_positive;
    m_digits = num.m_digits;
}

BigInteger::BigInteger(const std::string &s) {
    if (s.empty()) {
        throw std::invalid_argument("string can't be empty");
    }

    m_is_positive = s[0] != '-';

    // We temporary store number as a big integer with base 10^12
    const size_t temp_number_size = s.size() - (m_is_positive ? 0 : 1);

    if (temp_number_size == 0) {
        throw std::invalid_argument("string should have at least 1 digit");
    }

    const int temp_number_base_pow = 9;
    const uint64_t temp_number_base = pow((uint64_t) 10, temp_number_base_pow);
    const char * const temp_number_begin = s.c_str() + (m_is_positive ? 0 : 1);
    Vector<uint64_t> temp_number(
            div_with_rounding_up(
                    temp_number_size,
                    (size_t) temp_number_base_pow
            )
    );

    // parsing str to temp_number
    for (
            const char *digit_end = temp_number_begin + temp_number_size;
            digit_end > temp_number_begin;
            digit_end -= temp_number_base_pow
            ) {
        int digit_size = min(temp_number_base_pow, (int) (digit_end - temp_number_begin));
        uint64_t res = parse_n_char_str_to_unsigned_int(digit_end - digit_size, digit_size);
        temp_number.push_back(res);
    }

    // Translate temp_number to base 2 ^ 32 by dividing by 2 ^ 16 (otherwise uint64 can overflow) until it is less than 2 ^ 32
    uint64_t iteration = 0;
    const int divider_pow = (int) div_by_pow_of_2(BASE_POW, 1);
    const uint64_t divider = mult_by_pow_of_2(1, divider_pow);
    while (temp_number.size() > 1 || temp_number.front() >= divider) {

        uint64_t carry = 0;
        for (long i = temp_number.size() - 1; i >= 0; --i) {
            uint64_t cur = temp_number[i] + carry * temp_number_base;
            temp_number[i] = div_by_pow_of_2(cur, divider_pow);
            carry = mod_by_pow_of_2(cur, divider_pow);
        }

        // remove trailing zeros
        while (temp_number.size() > 1 && temp_number.back() == 0)
            temp_number.pop_back();

        if (is_odd(iteration)) {
            m_digits[m_digits.size() - 1] += mult_by_pow_of_2(carry, BASE_POW / 2);
        } else {
            m_digits.push_back((uint32_t) carry);
        }

        ++iteration;
    }
    if (is_odd(iteration)) {
        m_digits[m_digits.size() - 1] += mult_by_pow_of_2(temp_number.front(), BASE_POW / 2);
    } else {
        m_digits.push_back(temp_number.front());
    }

    check_zero_sign();
}

BigInteger &BigInteger::operator*=(const BigInteger &b) {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &b) {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &b) {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator++() {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator--() {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator&=(const BigInteger &b) {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator|=(const BigInteger &b) {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator^=(const BigInteger &b) {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator>>=(const BigInteger &b) {
    //TODO: implement
    return *this;
}

BigInteger &BigInteger::operator<<=(const BigInteger &b) {
    //TODO: implement
    return *this;
}

BigInteger operator~(const BigInteger &a) {
    //TODO: implement
    auto copy = a;
    return copy;
}

std::string to_string(const BigInteger &num) {
    return std::to_string(num.m_digits.back());
}

