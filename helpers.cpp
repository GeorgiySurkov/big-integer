#include "helpers.h"

uint64_t parse_n_char_str_to_unsigned_int(const char *s, int n) {
    uint64_t res = 0, order = 1;
    for (int i = n - 1; i >= 0; --i) {
        if (s[i] < '0' || '9' < s[i]) {
            throw std::invalid_argument("string contains non-digit char");
        }
        res += ((uint64_t) s[i] - '0') * order;
        order *= 10;
    }
    return res;
}
