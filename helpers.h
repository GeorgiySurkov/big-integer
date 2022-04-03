#pragma once

template<typename T>
inline T abs(T num) {
    return num > 0 ? num : -num;
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