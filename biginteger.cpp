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
        m_digits[i] = mod_by_pow_of_2(digit, BASE_POW);
        carry = div_by_pow_of_2(digit, BASE_POW);

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
    if (is_zero()) m_is_positive = true;
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
    const char *const temp_number_begin = s.c_str() + (m_is_positive ? 0 : 1);
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

std::string to_string(const BigInteger &n) {
    BigInteger num = n; // We create a copy of a number to divide it by 10 for translation;
    std::string result; // vector for storing result digits (they will be stored here in reverse order)

    const uint32_t res_base = 10;
    while (num.m_digits.size() > 1 || num.m_digits.front() >= res_base) {
        uint32_t remainder = num.divide_by_short_number(10);
        result.push_back('0' + (char) remainder);
    }
    result.push_back('0' + (char) num.m_digits.front());

    if (!n.m_is_positive) {
        result.push_back('-');
    }

    return {result.rbegin(), result.rend()};
}

BigInteger &BigInteger::operator*=(const BigInteger &b) {
    if (is_zero() || b.is_zero()) {
        *this = 0;
    }

    BigInteger result;
    result.m_is_positive = m_is_positive == b.m_is_positive;
    result.m_digits.reserve(m_digits.size() + b.m_digits.size() + 1);
    for (size_t i = 1; i < m_digits.size() + b.m_digits.size(); ++i) {
        result.m_digits.push_back(0);
    }

    uint64_t digit, carry;
    for (size_t j = 0; j < b.m_digits.size(); ++j) {
        if (b.m_digits[j] == 0) {
            result.m_digits[j + m_digits.size()] = 0;
        } else {
            carry = 0;
            for (size_t k = 0; k < m_digits.size(); ++k) {
                digit = (uint64_t) m_digits[k] * b.m_digits[j] + (uint64_t) result.m_digits[k + j] + carry;
                result.m_digits[k + j] = mod_by_pow_of_2(digit, BASE_POW);
                carry = div_by_pow_of_2(digit, BASE_POW);
            }
            result.m_digits[j + m_digits.size()] = (uint32_t) carry;
        }
    }

    result.remove_high_order_zeros();

    *this = std::move(result);
    return *this;
}

BigInteger &BigInteger::operator/=(const BigInteger &a) {
    if (a.is_zero()) {
        throw std::runtime_error("Division by zero.");
    }
    if (a.is_one()) {
        return *this;
    }
    if (a.is_negative_one()) {
        change_sign();
        return *this;
    }
    if (a.m_digits.size() == 1) {
        divide_by_short_number(a.m_digits.back());
        m_is_positive = m_is_positive == a.m_is_positive;
        return *this;
    }
    size_t cnt = m_digits.size();
    BigInteger result;
    result.m_digits.resize(m_digits.size() - a.m_digits.size() + 1);
    BigInteger copy2 = a;
    uint64_t d = base / (a.m_digits.back() + 1);

    *this *= d;
    if (m_digits.size() <= cnt) {
        m_digits.push_back(0);
    }
    copy2 *= d;
    int carry;
    uint64_t possible_q, possible_r, digit, product;
    for (long j = result.m_digits.size() - 1; j >= 0; --j) {
        const uint64_t divisible =
                mult_by_pow_of_2(m_digits[j + a.m_digits.size()], BASE_POW) + m_digits[j + a.m_digits.size() - 1];
        possible_q = divisible / copy2.m_digits[a.m_digits.size() - 1];
        possible_r = divisible % copy2.m_digits[a.m_digits.size() - 1];
        do {
            if (possible_q == base || (possible_q * (copy2.m_digits[a.m_digits.size() - 2]) >
                                       (mult_by_pow_of_2(possible_r, BASE_POW) +
                                        m_digits[j + a.m_digits.size() - 2]))) {
                --possible_q;
                possible_r += copy2.m_digits[a.m_digits.size() - 1];
            } else {
                break;
            }
        } while (possible_r < base);
        carry = 0;
        for (size_t i = 0; i < a.m_digits.size(); ++i) {
            product = possible_q * copy2.m_digits[i];
            digit = m_digits[i + j] - mod_by_pow_of_2(product, BASE_POW) - carry;
            m_digits[i + j] = digit;
            carry = (int) (div_by_pow_of_2(product, BASE_POW) - div_by_pow_of_2(digit, BASE_POW));
        }
        digit = m_digits[j + a.m_digits.size()] - carry;
        m_digits[j + a.m_digits.size()] = digit;
        result.m_digits[j] = possible_q;
        if (digit < 0) {
            --result.m_digits[j];
            carry = 0;
            for (size_t i = 0; i < a.m_digits.size(); ++i) {
                digit = m_digits[i + j] + copy2.m_digits[i] + carry;
                carry = (int) div_by_pow_of_2(digit, BASE_POW);
                m_digits[i + j] = digit;
            }
            m_digits[j + a.m_digits.size()] += carry;
        }
    }
    result.m_is_positive = m_is_positive == a.m_is_positive;
    result.remove_high_order_zeros();
    *this = std::move(result);
    return *this;
}

BigInteger &BigInteger::operator%=(const BigInteger &b) {
    // TODO: write more efficient code
    *this -= (*this / b) * b;
    return *this;
}

BigInteger &BigInteger::operator++() {
    // TODO: write more efficient code
    *this += 1;
    return *this;
}

BigInteger &BigInteger::operator--() {
    // TODO: write more efficient code
    *this -= 1;
    return *this;
}

BigInteger &BigInteger::operator>>=(const BigInteger &b) {
    if (!b.m_is_positive) {
        throw std::invalid_argument("Сan't bitshift to a negative number");
    }
    if (!m_is_positive) {
        *this = ~(*this);
        *this >>= b;
        *this = ~(*this);
        return *this;
    }
    if (b.is_zero()) {
        return *this;
    }
    long long b_ll = b.to_long_long();
    if (!fits_in_size_t(b_ll)) {
        throw std::range_error("Argument is too big for bitshift");
    }

    size_t digit_shift = b_ll / BASE_POW;
    uint32_t rem_shift = b_ll % BASE_POW;
    long long new_size = m_digits.size() - digit_shift;
    if (new_size <= 0) {
        *this = 0;
        return *this;
    }
    Vector<uint32_t> tmp(new_size, 0);
    size_t shift = BASE_POW - rem_shift, j = digit_shift, i;
    uint64_t accum = m_digits[j++] >> rem_shift;
    for (i = 0; j < m_digits.size(); ++i, ++j) {
        accum |= (uint64_t) m_digits[j] << shift;
        tmp[i] = accum;
        accum >>= BASE_POW;
    }
    tmp[i] = accum;
    m_digits = std::move(tmp);

    check_zero_sign();
    remove_high_order_zeros();
    return *this;
}

BigInteger &BigInteger::operator<<=(const BigInteger &b) {
    if (!b.m_is_positive) {
        throw std::invalid_argument("Сan't bitshift to a negative number");
    }
    if (b.is_zero()) {
        return *this;
    }
    long long b_ll = b.to_long_long();
    if (!fits_in_size_t(b_ll)) {
        throw std::range_error("Argument is too big for bitshift");
    }

    size_t digit_shift = b_ll / BASE_POW;
    uint32_t rem_shift = b_ll % BASE_POW;
    Vector<uint32_t> tmp(m_digits.size() + digit_shift + (rem_shift > 0 ? 1 : 0), 0);
    uint64_t accum = 0;
    for (size_t i = digit_shift, j = 0; j < m_digits.size(); ++i, ++j) {
        accum |= (uint64_t) m_digits[j] << rem_shift;
        tmp[i] = accum;
        accum >>= BASE_POW;
    }
    if (rem_shift) {
        tmp[tmp.size() - 1] = accum;
    }
    m_digits = std::move(tmp);

    check_zero_sign();
    remove_high_order_zeros();
    return *this;
}

BigInteger operator~(BigInteger a) {
    a.change_sign();
    --a;
    return a;
}

BigInteger &BigInteger::multiply_by_short_number(uint32_t number) {
    uint64_t carry = 0;
    for (auto & m_digit : m_digits) {
        uint64_t digit = m_digit * (uint64_t) number + carry;
        m_digit = mod_by_pow_of_2(digit, BASE_POW);
        carry = div_by_pow_of_2(digit, BASE_POW);
    }
    if (carry != 0) {
        m_digits.push_back(carry);
    }

    return *this;
}

uint32_t BigInteger::divide_by_short_number(uint32_t number) {
    // returns the remainder of the division

    if (number == 0) {
        throw std::runtime_error("Division by zero");
    }
    if (number < 0) {
        change_sign();
        number = -number;
    }
    uint64_t carry = 0, digit;
    for (long i = m_digits.size() - 1; i >= 0; --i) {
        digit = m_digits[i] + mult_by_pow_of_2(carry, BASE_POW);
        m_digits[i] = digit / number;
        carry = digit % number;
    }
    remove_high_order_zeros();
    return carry;
}

void BigInteger::make_twos_complement_form() {
    if (!m_is_positive) {
        for (size_t i = 0; i < m_digits.size(); ++i) {
            m_digits[i] = ~m_digits[i];
        }
        --(*this);
    }
}

BigInteger &BigInteger::bitwise_binary_operator(BigInteger b, char operation) {
    size_t digits_to_handle = max(m_digits.size(), b.m_digits.size());
    m_digits.resize(digits_to_handle, 0);
    make_twos_complement_form();
    b.m_digits.resize(digits_to_handle, 0);
    b.make_twos_complement_form();

    switch (operation) {
        case '&':
            for (size_t i = 0; i < digits_to_handle; ++i) {
                m_digits[i] = m_digits[i] & b.m_digits[i];
            }
            m_is_positive = m_is_positive || b.m_is_positive;
            break;
        case '|':
            for (size_t i = 0; i < digits_to_handle; ++i) {
                m_digits[i] = m_digits[i] | b.m_digits[i];
            }
            m_is_positive = m_is_positive && b.m_is_positive;
            break;
        case '^':
            for (size_t i = 0; i < digits_to_handle; ++i) {
                m_digits[i] = m_digits[i] ^ b.m_digits[i];
            }
            m_is_positive = m_is_positive == b.m_is_positive;
            break;
        default:
            throw std::invalid_argument("Invalid operation");
    }
    make_twos_complement_form();

    remove_high_order_zeros();
    check_zero_sign();
    return *this;
}

long long BigInteger::to_long_long() const {
    switch (m_digits.size()) {
        case 1:
            return m_digits[0];
            break;
        case 2:
            return ((long long) m_digits[1] << 32) | (long long) m_digits[0];
        default:
            throw std::range_error("This BigInteger can't be represented as unsigned long long");
    }
}

bool BigInteger::is_negative_one() const {
    return !m_is_positive && m_digits.size() == 1 && m_digits.back() == 1;
}

bool BigInteger::is_zero() const {
    return m_digits.size() == 1 && m_digits.back() == 0;
}

bool BigInteger::is_one() const {
    return m_is_positive && m_digits.size() == 1 && m_digits.back() == 1;
}
