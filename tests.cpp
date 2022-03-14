#include <string>
#include <gtest/gtest.h>

#include "biginteger.h"

TEST(correctness, one_plus_one)
{
    EXPECT_EQ(BigInteger(2), BigInteger(1) + BigInteger(1));
    EXPECT_EQ(BigInteger(2), BigInteger(1) + 1); // implicit conversion from int must work
    EXPECT_EQ(BigInteger(2), 1 + BigInteger(1));
}

TEST(correctness, one_plus_zero)
{
    EXPECT_EQ(BigInteger(1), BigInteger(1) + BigInteger(0));
    EXPECT_EQ(BigInteger(1), BigInteger(1) + 0);
    EXPECT_EQ(BigInteger(1), 0 + BigInteger(1));
}

TEST(correctness, default_constructor)
{
    BigInteger x;
    BigInteger y = 0;
    EXPECT_EQ(BigInteger(0), x);
    EXPECT_EQ(y, x);
}

TEST(correctness, copy_constructor)
{
    BigInteger x = 2;
    BigInteger y = x;

    EXPECT_EQ(y, x);
    EXPECT_EQ(2, y);
}

TEST(correctness, copy_constructor_real_copy)
{
    BigInteger x = 2;
    BigInteger y = x;
    x = 4;

    EXPECT_EQ(2, y);
}

TEST(correctness, copy_constructor_real_copy_2)
{
    BigInteger x = 3;
    BigInteger y = x;
    y = 5;

    EXPECT_EQ(3, x);
}

TEST(correctness, constructor_invalid_string)
{
    EXPECT_THROW(BigInteger("abc"), std::invalid_argument);
    EXPECT_THROW(BigInteger("123x"), std::invalid_argument);
    EXPECT_THROW(BigInteger(""), std::invalid_argument);
    EXPECT_THROW(BigInteger("-"), std::invalid_argument);
    EXPECT_THROW(BigInteger("-x"), std::invalid_argument);
    EXPECT_THROW(BigInteger("123-456"), std::invalid_argument);
    EXPECT_THROW(BigInteger("--5"), std::invalid_argument);
    EXPECT_THROW(BigInteger("++5"), std::invalid_argument);
}

TEST(correctness, assignment_operator)
{
    BigInteger a = 4;
    BigInteger b = 7;
    b = a;

    EXPECT_TRUE(a == b);
}

TEST(correctness, self_assignment)
{
    BigInteger a = 5;
    a = a;

    EXPECT_TRUE(a == 5);
}

TEST(correctness, assignment_return_value)
{
    BigInteger a = 4;
    BigInteger b = 7;
    (a = b) = a;

    EXPECT_TRUE(a == 7);
    EXPECT_TRUE(b == 7);
}

TEST(correctness, comparisons)
{
    BigInteger a = 100;
    BigInteger b = 100;
    BigInteger c = 200;
    BigInteger d = -100;

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
    EXPECT_TRUE(a < c);
    EXPECT_TRUE(c > a);
    EXPECT_TRUE(a <= a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(a <= c);
    EXPECT_TRUE(c >= a);
    EXPECT_TRUE(d != 0);
    EXPECT_TRUE(d < a);
}

TEST(correctness, compare_with_sign)
{
    BigInteger a = 1;
    BigInteger b = -a;

    EXPECT_TRUE(a != b);
}

TEST(correctness, compare_zero_and_minus_zero)
{
    BigInteger a;
    BigInteger b = -a;

    EXPECT_TRUE(a == b);
}

TEST(correctness, operator_plus)
{
    BigInteger a = 5;
    BigInteger b = 20;

    EXPECT_TRUE(a + b == 25);

    a += b;
    EXPECT_TRUE(a == 25);
}

TEST(correctness, operator_plus_signed)
{
    BigInteger a = 5;
    BigInteger b = -20;
    EXPECT_TRUE(a + b == -15);

    a += b;
    EXPECT_TRUE(a == -15);
}

TEST(correctness, operator_pluseq_return_value)
{
    BigInteger a = 5;
    BigInteger b = 1;

    (a += b) += b;
    EXPECT_EQ(7, a);
}

TEST(correctness, operator_sub)
{
    BigInteger a = 20;
    BigInteger b = 5;

    EXPECT_TRUE(a - b == 15);

    a -= b;
    EXPECT_TRUE(a == 15);
}

TEST(correctness, operator_sub_signed)
{
    BigInteger a = 5;
    BigInteger b = 20;

    EXPECT_TRUE(a - b == -15);

    a -= b;
    EXPECT_TRUE(a == -15);

    a -= -100;
    EXPECT_TRUE(a == 85);
}

TEST(correctness, operator_subeq_return_value)
{
    BigInteger a = 5;
    BigInteger b = 1;

    (a -= b) -= b;
    EXPECT_EQ(3, a);
}

TEST(correctness, operator_mul)
{
    BigInteger a = 5;
    BigInteger b = 20;
    EXPECT_TRUE(a * b == 100);

    a *= b;
    EXPECT_TRUE(a == 100);
}

TEST(correctness, operator_mul_signed)
{
    BigInteger a = -5;
    BigInteger b = 20;

    EXPECT_TRUE(a * b == -100);

    a *= b;
    EXPECT_TRUE(a == -100);
}

TEST(correctness, operator_muleq_return_value)
{
    BigInteger a = 5;
    BigInteger b = 2;

    (a *= b) *= b;
    EXPECT_EQ(20, a);
}

TEST(correctness, operator_div)
{
    BigInteger a = 20;
    BigInteger b = 5;
    BigInteger c = 20;
    EXPECT_EQ(0, b / c);
    EXPECT_TRUE(a / b == 4);
    EXPECT_TRUE(a % b == 0);

    a /= b;
    EXPECT_TRUE(a == 4);

    c %= b;
    EXPECT_TRUE(c == 0);
}

TEST(correctness, operator_div_signed)
{
    BigInteger a = -20;
    BigInteger b = 5;

    EXPECT_TRUE(a / b == -4);
    EXPECT_TRUE(a % b == 0);
}

TEST(correctness, operator_div_rounding)
{
    BigInteger a = 23;
    BigInteger b = 5;

    EXPECT_TRUE(a / b == 4);
    EXPECT_TRUE(a % b == 3);
}

TEST(correctness, operator_div_rounding_negative)
{
    BigInteger a = 23;
    BigInteger b = -5;
    BigInteger c = -23;
    BigInteger d = 5;

    EXPECT_TRUE(a / b == -4);
    EXPECT_TRUE(c / d == -4);
    EXPECT_TRUE(a % b == 3);
    EXPECT_TRUE(c % d == -3);
}

TEST(correctness, operator_div_return_value)
{
    BigInteger a = 100;
    BigInteger b = 2;

    (a /= b) /= b;
    EXPECT_EQ(25, a);
}

TEST(correctness, operator_unary_plus)
{
    BigInteger a = 123;
    BigInteger b = +a;

    EXPECT_TRUE(a == b);
}

TEST(correctness, negation)
{
    BigInteger a = 666;
    BigInteger b = -a;

    EXPECT_TRUE(b == -666);
}

TEST(correctness, operator_increment)
{
    BigInteger a = 42;
    BigInteger pre = ++a;
    BigInteger post = a++;

    EXPECT_EQ(43, pre);
    EXPECT_EQ(43, post);
}

TEST(correctness, operator_decrement)
{
    BigInteger a = 42;
    BigInteger pre = --a;
    BigInteger post = a--;

    EXPECT_EQ(41, pre);
    EXPECT_EQ(41, post);
}

TEST(correctness, operator_and)
{
    BigInteger a = 0x55;
    BigInteger b = 0xaa;

    EXPECT_TRUE((a & b) == 0);
    EXPECT_TRUE((a & 0xcc) == 0x44);
    a &= b;
    EXPECT_TRUE(a == 0);
}

TEST(correctness, operator_and_signed)
{
    BigInteger a = 0x55;
    BigInteger b = 0xaa;
    BigInteger x = a & (0xaa - 256);

    EXPECT_TRUE((b & -1) == 0xaa);
    EXPECT_TRUE((a & (0xaa - 256)) == 0);
    EXPECT_TRUE((a & (0xcc - 256)) == 0x44);

    BigInteger c = 0x55;
    BigInteger d = 0xcc;
    EXPECT_EQ(c & d, BigInteger(0x44));
}

TEST(correctness, operator_and_return_value)
{
    BigInteger a = 7;

    (a &= 3) &= 6;
    EXPECT_EQ(2, a);
}

TEST(correctness, operator_or)
{
    BigInteger a = 0x55;
    BigInteger b = 0xaa;

    EXPECT_TRUE((a | b) == 0xff);
    a |= b;
    EXPECT_TRUE(a == 0xff);

    BigInteger c = 0x55;
    BigInteger d = 0xcc;
    EXPECT_EQ(c | d, BigInteger(0xdd));
}

TEST(correctness, operator_or_signed)
{
    BigInteger a = 0x55;
    BigInteger b = 0xaa;
    EXPECT_TRUE((a | (b - 256)) == -1);
}

TEST(correctness, operator_or_return_value)
{
    BigInteger a = 1;

    (a |= 2) |= 4;
    EXPECT_EQ(7, a);
}

TEST(correctness, operator_xor)
{
    BigInteger a = 0xaa;
    BigInteger b = 0xcc;

    EXPECT_TRUE((a ^ b) == 0x66);

    BigInteger c = 0x55;
    BigInteger d = 0xcc;
    EXPECT_EQ(c ^ d, BigInteger(0x99));
}

TEST(correctness, operator_xor_signed)
{
    BigInteger a = 0xaa;
    BigInteger b = 0xcc;

    EXPECT_TRUE((a ^ (b - 256)) == (0x66 - 256));
}

TEST(correctness, operator_xor_return_value)
{
    BigInteger a = 1;

    (a ^= 2) ^= 1;
    EXPECT_EQ(2, a);
}

TEST(correctness, operator_not)
{
    BigInteger a = 0xaa;
    BigInteger b = ~a;
    BigInteger c = (-a - 1);
    EXPECT_TRUE(~a == c);
}

TEST(correctness, operator_shift_left)
{
    BigInteger a = 23;

    EXPECT_TRUE((a << 5) == 23 * 32);

    a <<= 5;
    EXPECT_TRUE(a == 23 * 32);
}

TEST(correctness, operator_shift_left_return_value)
{
    BigInteger a = 1;

    (a <<= 2) <<= 1;
    EXPECT_EQ(8, a);
}

TEST(correctness, operator_shift_right)
{
    BigInteger a = 23;

    EXPECT_EQ(5, a >> 2);

    a >>= 2;
    EXPECT_EQ(5, a);
}

TEST(correctness, operator_shift_right_signed)
{
    BigInteger a = -1234;

    EXPECT_EQ(-155, a >> 3);

    a >>= 3;
    EXPECT_EQ(-155, a);
}

TEST(correctness, operator_shift_right_return_value)
{
    BigInteger a = 64;

    (a >>= 2) >>= 1;
    EXPECT_EQ(8, a);
}

TEST(correctness, add_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("10000000000000000000000000000000000000000000000000000100000000000000000000000000000000000000");

    EXPECT_EQ(c, a + b);
}

TEST(correctness, add_long_signed)
{
    BigInteger a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(0, a + b);
}

TEST(correctness, add_long_signed2)
{
    BigInteger a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("-999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");

    EXPECT_EQ(c, a + b);
}

TEST(correctness, add_long_pow2)
{
    BigInteger a("18446744073709551616");
    BigInteger b("-18446744073709551616");
    BigInteger c("36893488147419103232");

    EXPECT_EQ(c, a + a);
    EXPECT_EQ(a, b + c);
    EXPECT_EQ(a, c + b);
}

TEST(correctness, sub_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("9999999999999999999999999999999999999999999999999999900000000000000000000000000000000000000");

    EXPECT_EQ(c, a - b);
}

TEST(correctness, sub_long_pow2)
{
    BigInteger a("36893488147419103232");
    BigInteger b("36893488147419103231");

    EXPECT_EQ(1, a - b);
}

TEST(correctness, mul_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000");

    EXPECT_EQ(c, a * b);
}

TEST(correctness, mul_long_signed)
{
    BigInteger a("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("-1000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000"
        "00000000000000000000000000000000000000");

    EXPECT_EQ(c, a * b);
}

TEST(correctness, mul_long_signed2)
{
    BigInteger a("-100000000000000000000000000");
    BigInteger c("100000000000000000000000000"
        "00000000000000000000000000");

    EXPECT_EQ(c, a * a);
}

TEST(correctness, mul_long_pow2)
{
    BigInteger a("18446744073709551616");
    BigInteger b("340282366920938463463374607431768211456");
    BigInteger c("115792089237316195423570985008687907853269984665640564039457584007913129639936");

    EXPECT_EQ(b, a * a);
    EXPECT_EQ(c, b * b);
}


TEST(correctness, div_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(correctness, div_long_signed)
{
    BigInteger a("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("100000000000000000000000000000000000000");
    BigInteger c("-100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(correctness, div_long_signed2)
{
    BigInteger a("-10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000");
    BigInteger b("-100000000000000000000000000000000000000");
    BigInteger c("100000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(c, a / b);
}

TEST(correctness, negation_long)
{
    BigInteger a("10000000000000000000000000000000000000000000000000000");
    BigInteger c("-10000000000000000000000000000000000000000000000000000");

    EXPECT_EQ(-a, c);
    EXPECT_EQ(a, -c);
}

TEST(correctness, shl_long)
{
    EXPECT_EQ(BigInteger("1091951238831590836520041079875950759639875963123939936"),
        BigInteger("34123476213487213641251283746123461238746123847623123") << 5);

    EXPECT_EQ(BigInteger("-104637598388784443044449444577438556334703518260785595038524928"),
        BigInteger("-817481237412378461284761285761238721364871236412387461238476") << 7);

    EXPECT_EQ(BigInteger("26502603392713913241969902328696116541550413468869982914247384891392"),
        BigInteger("12341236412857618761234871264871264128736412836643859238479") << 31);
}

TEST(correctness, shr_long)
{
    EXPECT_EQ(BigInteger("4730073393008085198307104580698364137020387111323398632330851"),
        BigInteger("151362348576258726345827346582347652384652387562348756234587245") >> 5);

    EXPECT_EQ(BigInteger("1118311528397465815295799577134738919815767762822175104787"),
        BigInteger("143143875634875624357862345873246581736418273641238413412741") >> 7);

    EXPECT_EQ(BigInteger("-1591563309890326054125627839548891585559049824963"),
        BigInteger("-3417856182746231874623148723164812376512852437523846123876") >> 31);
}

TEST(correctness, string_conv)
{
    EXPECT_EQ("100", to_string(BigInteger("100")));
    EXPECT_EQ("100", to_string(BigInteger("0100")));
    EXPECT_EQ("0", to_string(BigInteger("0")));
    EXPECT_EQ("0", to_string(BigInteger("-0")));
    EXPECT_EQ("-1000000000000000", to_string(BigInteger("-1000000000000000")));

    EXPECT_EQ("2147483647", to_string(BigInteger("2147483647")));
    EXPECT_EQ("2147483648", to_string(BigInteger("2147483648")));
    EXPECT_EQ("-2147483649", to_string(BigInteger("-2147483649")));
}

namespace
{
    template <typename T>
    void test_converting_ctor(T value)
    {
        using std::to_string;

        BigInteger bi = value;
        EXPECT_EQ(to_string(value), to_string(bi));
    }
}

TEST(correctness, converting_ctor2)
{
    BigInteger a(1);
    BigInteger b(1U);
    BigInteger c(1L);
    BigInteger d(1UL);
    BigInteger e(1LL);
    BigInteger f(1ULL);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == c);
    EXPECT_TRUE(a == d);
    EXPECT_TRUE(a == e);
    EXPECT_TRUE(a == f);
}

TEST(correctness, converting_ctor3)
{
    BigInteger a(-1);
    BigInteger b(-1L);
    BigInteger c(-1LL);

    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a == c);
}
