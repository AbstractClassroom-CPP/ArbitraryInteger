#include "ArbitraryInteger.hpp"

#include <gtest/gtest.h>
#include <random>

TEST(Bitwise, MatchesSmallBinaryExamples) {
	EXPECT_EQ(ArbitraryInteger(12) & 10, 8);
	EXPECT_EQ(12L & ArbitraryInteger(10), 8);
	EXPECT_EQ(ArbitraryInteger(12) ^ 10L, 6);
	EXPECT_EQ(12 ^ ArbitraryInteger(10), 6);
	EXPECT_EQ(ArbitraryInteger(12) | 10, 14);
	EXPECT_EQ(12L | ArbitraryInteger(10), 14);
}

TEST(Bitwise, SignExtendsNegativeOperands) {
	EXPECT_EQ(ArbitraryInteger(-1) & 42, 42);
	EXPECT_EQ(ArbitraryInteger(-1) | 42, -1);
	EXPECT_EQ(ArbitraryInteger(-1) ^ 42, -43);
	EXPECT_EQ(ArbitraryInteger(-12) & -10, -12);
	EXPECT_EQ(ArbitraryInteger(-12) | -10, -10);
	EXPECT_EQ(ArbitraryInteger(-12) ^ -10, 2);
	EXPECT_EQ(ArbitraryInteger(-1) ^ -1, 0);
}

TEST(Bitwise, KeepsHighMagnitudeBitsAndDifferentWordCounts) {
	ArbitraryInteger high("18446744073709551616");
	ArbitraryInteger low("18446744073709551615");
	EXPECT_EQ(high & low, 0);
	EXPECT_EQ((high | low).toString(), "36893488147419103231");
	EXPECT_EQ(high ^ low, high | low);
	EXPECT_EQ(high & -1, high);
	EXPECT_EQ((high ^ -1).toString(), "-18446744073709551617");
	EXPECT_EQ(ArbitraryInteger("4294967295") | 0, ArbitraryInteger("4294967295"));
	EXPECT_EQ(-high & low, 0);
	EXPECT_EQ(-high | low, -1);
}

TEST(Bitwise, CompoundAssignmentAndSelfOperands) {
	ArbitraryInteger value = 12;
	EXPECT_EQ(&(value &= 10L), &value);
	EXPECT_EQ(value, 8);
	EXPECT_EQ(&(value |= 3), &value);
	EXPECT_EQ(value, 11);
	EXPECT_EQ(&(value ^= ArbitraryInteger(5)), &value);
	EXPECT_EQ(value, 14);
	value = ArbitraryInteger("-18446744073709551617");
	const ArbitraryInteger original = value;
	value &= value;
	EXPECT_EQ(value, original);
	value |= value;
	EXPECT_EQ(value, original);
	value ^= value;
	EXPECT_EQ(value.toString(), "0");
}

TEST(Bitwise, MatchesUnsignedNativeArithmetic) {
	std::mt19937_64 random(625);
	for (int i = 0; i < 1000; ++i) {
		const std::uint64_t a = random();
		const std::uint64_t b = random();
		const ArbitraryInteger left(std::to_string(a));
		const ArbitraryInteger right(std::to_string(b));
		EXPECT_EQ((left & right).toString(), std::to_string(a & b));
		EXPECT_EQ((left ^ right).toString(), std::to_string(a ^ b));
		EXPECT_EQ((left | right).toString(), std::to_string(a | b));
	}
}
