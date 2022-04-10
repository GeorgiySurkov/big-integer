#pragma once

#include <cstdint>
#include <stdexcept>
#include <limits>

template<typename T>
inline T abs(T num) {
    return num > 0 ? num : -num;
}

template<typename T>
inline bool fits_in_size_t(T num) {
    return std::numeric_limits<size_t>::min() <= num && num <= std::numeric_limits<size_t>::max();
}

template<typename T>
inline bool is_odd(T num) {
    return num & 1; // equal to (num % 2 == 1)
}

template<typename T>
T pow(T base, long long power) {
    T result = 1;
    while(power > 0) {
        if(is_odd(power)) {
            result *= base;
        }
        base *= base;
        power >>= 1; // equal to (power /= 2)
    }
    return result;
}

template<typename T>
inline T max(T a, T b) {
    return a > b ? a : b;
}

template<typename T>
inline T min(T a, T b) {
    return a > b ? b : a;
}

inline uint64_t div_by_pow_of_2(uint64_t num, int pow) {
    return num >> pow;
}

inline uint64_t mult_by_pow_of_2(uint64_t num, int pow) {
    return num << pow;
}

inline uint64_t mod_by_pow_of_2(uint64_t num, int pow) {
    return num & (mult_by_pow_of_2(1, pow) - 1);
}

uint64_t parse_n_char_str_to_unsigned_int(const char *s, int n);

template<typename T>
inline T div_with_rounding_up(T divisible, T divider) {
    return (divisible + divider - 1) / divider;
}