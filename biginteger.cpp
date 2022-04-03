#include "biginteger.h"

BigInteger &BigInteger::operator+=(const BigInteger &b) {
    // Handle different signs
    if (m_is_positive && !b.m_is_positive) {
        return this->operator-=(-b);
    } else if (!m_is_positive && b.m_is_positive) {
        m_is_positive = true;
        this->operator-=(b);
        m_is_positive = !m_is_positive;
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
    size_t digits_to_sum = min(m_digits.size(), b.m_digits.size());
    for (size_t i = 0; i < digits_to_sum; ++i) {
        int64_t digit = m_digits[i] + b.m_digits[i];
        m_digits[i] = mod_by_pow_of_2(digit, BASE_POW);
        m_digits[i + 1] = div_by_pow_of_2(digit, BASE_POW);
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
        temp.m_is_positive = !temp.m_is_positive;
        temp.check_zero_sign();

        *this = std::move(temp);
        return *this;
    }

    return subtract_lesser_number_with_same_sign(b);
}

BigInteger &BigInteger::subtract_lesser_number_with_same_sign(const BigInteger &b) {
    // Subtract digits
    size_t digits_to_subtruct = min(m_digits.size(), b.m_digits.size());
    for (int i = 0; i < digits_to_subtruct; ++i) {
        if (m_digits[i] < b.m_digits[i]) {
            --m_digits[i + 1];
        }
        m_digits[i] -= b.m_digits[i];
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
    for (size_t i = 0; i < a.m_digits.size(); ++i) {
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
    res.m_is_positive = !res.m_is_positive;
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

