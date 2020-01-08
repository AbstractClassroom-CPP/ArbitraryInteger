#include "ArbitraryInteger.hpp"

#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <sstream>
#include <stdexcept>

TEST(Construction, DefaultsToZero) {
	ArbitraryInteger value;
	EXPECT_TRUE(value.isZero());
	EXPECT_EQ(value.toString(), "0");
	EXPECT_EQ(-value, 0);
}

TEST(Construction, AcceptsSignedNativeLimits) {
	EXPECT_EQ(ArbitraryInteger(std::numeric_limits<int>::min()).toString(),
	std::to_string(std::numeric_limits<int>::min()));
	EXPECT_EQ(ArbitraryInteger(std::numeric_limits<int>::max()).toString(),
	std::to_string(std::numeric_limits<int>::max()));
	EXPECT_EQ(ArbitraryInteger(std::numeric_limits<long>::min()).toString(),
	std::to_string(std::numeric_limits<long>::min()));
	EXPECT_EQ(ArbitraryInteger(std::numeric_limits<long>::max()).toString(),
	std::to_string(std::numeric_limits<long>::max()));
	EXPECT_EQ(ArbitraryInteger(std::numeric_limits<long long>::min()).toString(),
	std::to_string(std::numeric_limits<long long>::min()));
	EXPECT_EQ(ArbitraryInteger(std::numeric_limits<long long>::max()).toString(),
	std::to_string(std::numeric_limits<long long>::max()));
}

TEST(Construction, ParsesDecimalSignsAndLeadingZeros) {
	EXPECT_EQ(ArbitraryInteger("+00012345678901234567890").toString(), "12345678901234567890");
	EXPECT_EQ(ArbitraryInteger("-00012345678901234567890").toString(), "-12345678901234567890");
	EXPECT_EQ(ArbitraryInteger("000").toString(), "0");
	EXPECT_EQ(ArbitraryInteger("-000").toString(), "0");
	EXPECT_EQ(ArbitraryInteger("+0"), 0);
}

TEST(Construction, RejectsInvalidDecimalInput) {
	for (const std::string text : {"", "+", "-", " 1", "1 ", "1.0", "1e9", "0xff", "--1", "+-1", "1_000", "12x"}) {
		EXPECT_THROW((void)ArbitraryInteger(text), std::invalid_argument) << text;
	}
	EXPECT_THROW((void)ArbitraryInteger(std::string("12\0", 3)), std::invalid_argument);
}

TEST(Construction, CopiesAndAssignsIndependentValues) {
	ArbitraryInteger original("18446744073709551616");
	ArbitraryInteger copy = original;
	++copy;
	EXPECT_EQ(original.toString(), "18446744073709551616");
	EXPECT_EQ(copy.toString(), "18446744073709551617");
	copy = -42;
	EXPECT_EQ(copy, -42);
	copy = 123L;
	EXPECT_EQ(copy, 123);
	copy = original;
	const ArbitraryInteger& same = copy;
	copy = same;
	EXPECT_EQ(copy, original);
}

TEST(Output, WritesDecimalToStream) {
	std::ostringstream out;
	out << ArbitraryInteger("-123456789012345678901234567890");
	EXPECT_EQ(out.str(), "-123456789012345678901234567890");
}

TEST(Addition, CarriesAcrossWordBoundaries) {
	EXPECT_EQ((ArbitraryInteger("4294967295") + 1).toString(), "4294967296");
	EXPECT_EQ((ArbitraryInteger("18446744073709551615") + 1).toString(), "18446744073709551616");
	EXPECT_EQ((ArbitraryInteger("79228162514264337593543950335") + 1).toString(), "79228162514264337593543950336");
	EXPECT_EQ((ArbitraryInteger("4294967295") + ArbitraryInteger("4294967295")).toString(), "8589934590");
}

TEST(Addition, HandlesDifferentSignsAndCancellation) {
	EXPECT_EQ(ArbitraryInteger(-8) + 3, -5);
	EXPECT_EQ(ArbitraryInteger(3) + -8, -5);
	EXPECT_EQ(ArbitraryInteger(-3) + 8, 5);
	EXPECT_EQ(ArbitraryInteger(-3) + -8, -11);
	ArbitraryInteger large("-79228162514264337593543950336");
	EXPECT_EQ((large + -large).toString(), "0");
	EXPECT_EQ(+large, large);
}

TEST(Subtraction, BorrowsAcrossWordBoundaries) {
	EXPECT_EQ((ArbitraryInteger("4294967296") - 1).toString(), "4294967295");
	EXPECT_EQ((ArbitraryInteger("18446744073709551616") - 1).toString(), "18446744073709551615");
	EXPECT_EQ((ArbitraryInteger("79228162514264337593543950336") - 1).toString(), "79228162514264337593543950335");
	EXPECT_EQ((ArbitraryInteger(1) - ArbitraryInteger("18446744073709551616")).toString(), "-18446744073709551615");
	EXPECT_EQ(ArbitraryInteger(-7) - -9, 2);
}

TEST(Multiplication, AccumulatesFullWordsAndCarries) {
	ArbitraryInteger word("4294967295");
	EXPECT_EQ((word * word).toString(), "18446744065119617025");
	ArbitraryInteger twoWords("18446744073709551615");
	EXPECT_EQ((twoWords * twoWords).toString(), "340282366920938463426481119284349108225");
	EXPECT_EQ((ArbitraryInteger("18446744073709551616") * ArbitraryInteger("18446744073709551616")).toString(),
	"340282366920938463463374607431768211456");
	EXPECT_EQ(word * -1, -word);
	EXPECT_EQ(-word * -word, word * word);
	EXPECT_EQ((-word * 0).toString(), "0");
}

TEST(Division, TruncatesTowardZeroAndKeepsDividendRemainderSign) {
	EXPECT_EQ(ArbitraryInteger(7) / 3, 2);
	EXPECT_EQ(ArbitraryInteger(-7) / 3, -2);
	EXPECT_EQ(ArbitraryInteger(7) / -3, -2);
	EXPECT_EQ(ArbitraryInteger(-7) / -3, 2);
	EXPECT_EQ(ArbitraryInteger(7) % 3, 1);
	EXPECT_EQ(ArbitraryInteger(-7) % 3, -1);
	EXPECT_EQ(ArbitraryInteger(7) % -3, 1);
	EXPECT_EQ(ArbitraryInteger(-7) % -3, -1);
	EXPECT_EQ((ArbitraryInteger(-6) % 3).toString(), "0");
}

TEST(Division, HandlesZeroAndDivisorsLargerThanDividend) {
	ArbitraryInteger large("18446744073709551616");
	EXPECT_EQ(ArbitraryInteger(0) / large, 0);
	EXPECT_EQ(ArbitraryInteger(0) % -large, 0);
	EXPECT_EQ((ArbitraryInteger(-3) / large).toString(), "0");
	EXPECT_EQ(ArbitraryInteger(-3) % large, -3);
	EXPECT_EQ(large / 1, large);
	EXPECT_EQ(large / -1, -large);
}

TEST(Division, WorksAcrossSeveralWords) {
	ArbitraryInteger dividend("340282366920938463463374607431768211455");
	ArbitraryInteger divisor("18446744073709551616");
	EXPECT_EQ((dividend / divisor).toString(), "18446744073709551615");
	EXPECT_EQ((dividend % divisor).toString(), "18446744073709551615");
	EXPECT_EQ((dividend / ArbitraryInteger("4294967295")).toString(), "79228162532711081671548469249");
	EXPECT_EQ(dividend % ArbitraryInteger("4294967295"), 0);
	ArbitraryInteger minimum("-9223372036854775808");
	EXPECT_EQ((minimum / -1).toString(), "9223372036854775808");
}

TEST(Division, RejectsZeroWithoutChangingLeftOperand) {
	ArbitraryInteger value("18446744073709551616");
	EXPECT_THROW((void)(value / 0), std::domain_error);
	EXPECT_THROW((void)(value % 0L), std::domain_error);
	EXPECT_THROW(value /= ArbitraryInteger("-0"), std::domain_error);
	EXPECT_EQ(value.toString(), "18446744073709551616");
	EXPECT_THROW(value %= 0, std::domain_error);
	EXPECT_EQ(value.toString(), "18446744073709551616");
	EXPECT_THROW((void)(ArbitraryInteger(0) / 0), std::domain_error);
}

template <typename Native>
void checkMixedOperands() {
	const Native seven = 7;
	const Native three = 3;
	const ArbitraryInteger a = 7;
	const ArbitraryInteger b = 3;
	EXPECT_EQ(a + three, 10);
	EXPECT_EQ(seven + b, 10);
	EXPECT_EQ(a - three, 4);
	EXPECT_EQ(seven - b, 4);
	EXPECT_EQ(a * three, 21);
	EXPECT_EQ(seven * b, 21);
	EXPECT_EQ(a / three, 2);
	EXPECT_EQ(seven / b, 2);
	EXPECT_EQ(a % three, 1);
	EXPECT_EQ(seven % b, 1);
	ArbitraryInteger value;
	value = seven;
	EXPECT_EQ(&(value += three), &value);
	EXPECT_EQ(value, 10);
	EXPECT_EQ(&(value -= three), &value);
	EXPECT_EQ(value, 7);
	EXPECT_EQ(&(value *= three), &value);
	EXPECT_EQ(value, 21);
	EXPECT_EQ(&(value /= three), &value);
	EXPECT_EQ(value, 7);
	EXPECT_EQ(&(value %= three), &value);
	EXPECT_EQ(value, 1);
}

TEST(Operators, MixesIntAndLongOnEitherSide) {
	checkMixedOperands<int>();
	checkMixedOperands<long>();
}

TEST(Operators, SupportsChainedAssignmentAndCompoundOperations) {
	ArbitraryInteger a;
	ArbitraryInteger b;
	a = b = 20L;
	EXPECT_EQ(a, 20);
	EXPECT_EQ(b, 20);
	(((a += 5) -= 3L) *= ArbitraryInteger(-7)) /= 3L;
	EXPECT_EQ(a, -51);
	a %= ArbitraryInteger(7);
	EXPECT_EQ(a, -2);
}

TEST(Operators, SupportsSelfOperands) {
	ArbitraryInteger value("18446744073709551615");
	value += value;
	EXPECT_EQ(value.toString(), "36893488147419103230");
	value -= value;
	EXPECT_EQ(value.toString(), "0");
	value = ArbitraryInteger("4294967295");
	value *= value;
	EXPECT_EQ(value.toString(), "18446744065119617025");
	value /= value;
	EXPECT_EQ(value, 1);
	value %= value;
	EXPECT_EQ(value, 0);
}

TEST(Operators, PrefixAndPostfixCrossZeroAndWordBoundaries) {
	ArbitraryInteger value = -1;
	EXPECT_EQ(value++, -1);
	EXPECT_EQ(value, 0);
	EXPECT_EQ(&(++value), &value);
	EXPECT_EQ(value, 1);
	EXPECT_EQ(value--, 1);
	EXPECT_EQ(value, 0);
	EXPECT_EQ(&(--value), &value);
	EXPECT_EQ(value, -1);
	value = ArbitraryInteger("4294967295");
	EXPECT_EQ((++value).toString(), "4294967296");
	EXPECT_EQ((--value).toString(), "4294967295");
}

TEST(Comparison, OrdersSignsMagnitudesAndNativeValues) {
	ArbitraryInteger large("18446744073709551616");
	EXPECT_LT(-large, -1);
	EXPECT_LT(-1L, large);
	EXPECT_GT(large, ArbitraryInteger("18446744073709551615"));
	EXPECT_LT(ArbitraryInteger("4294967296"), ArbitraryInteger("4294967297"));
	EXPECT_EQ(ArbitraryInteger("-0"), 0L);
	EXPECT_NE(large, -large);
	EXPECT_LE(large, large);
	EXPECT_GE(large, large);
	EXPECT_FALSE(large < large);
	EXPECT_FALSE(large > large);
}

TEST(Arithmetic, MatchesNativeArithmeticForDeterministicRandomValues) {
	std::mt19937_64 random(625);
	for (int i = 0; i < 1000; ++i) {

		const long long a = static_cast<long long>(random() % 2000000001ULL) - 1000000000LL;
		const long long b = static_cast<long long>(random() % 2000000001ULL) - 1000000000LL;
		const ArbitraryInteger left = a;
		const ArbitraryInteger right = b;
		SCOPED_TRACE(std::to_string(a) + ", " + std::to_string(b));
		EXPECT_EQ((left + right).toString(), std::to_string(a + b));
		EXPECT_EQ((left - right).toString(), std::to_string(a - b));
		EXPECT_EQ((left * right).toString(), std::to_string(a * b));
		EXPECT_EQ(left < right, a < b);
		if (b != 0) {
			EXPECT_EQ((left / right).toString(), std::to_string(a / b));
			EXPECT_EQ((left % right).toString(), std::to_string(a % b));
		}
	}
}

TEST(Arithmetic, GrowsFarBeyondNativeIntegerSizes) {
	const std::string nines(500, '9');
	ArbitraryInteger value(nines);
	EXPECT_EQ(value.toString(), nines);
	++value;
	EXPECT_EQ(value.toString(), "1" + std::string(500, '0'));
	--value;
	EXPECT_EQ(value.toString(), nines);
	value *= 0;
	EXPECT_TRUE(value.isZero());
	EXPECT_EQ(value.toString(), "0");
}
