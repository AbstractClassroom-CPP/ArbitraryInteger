#include "ArbitraryInteger.hpp"

#include <gtest/gtest.h>
#include <limits>
#include <random>
#include <stdexcept>

TEST(Shifts, MovesBitsAcrossWordBoundaries) {
	EXPECT_EQ((ArbitraryInteger(1) << 31).toString(), "2147483648");
	EXPECT_EQ((ArbitraryInteger(1) << 32).toString(), "4294967296");
	EXPECT_EQ((ArbitraryInteger(1) << 33).toString(), "8589934592");
	EXPECT_EQ((ArbitraryInteger(1) << 64).toString(), "18446744073709551616");
	EXPECT_EQ((ArbitraryInteger(1) << 65).toString(), "36893488147419103232");
	ArbitraryInteger ones("18446744073709551615");
	EXPECT_EQ((ones << 1).toString(), "36893488147419103230");
	EXPECT_EQ((ones >> 1).toString(), "9223372036854775807");
	EXPECT_EQ((ones >> 31).toString(), "8589934591");
	EXPECT_EQ((ones >> 32).toString(), "4294967295");
	EXPECT_EQ(ones >> 63, 1);
	EXPECT_EQ(ones >> 64, 0);
}

TEST(Shifts, UsesArithmeticRightShiftForNegativeValues) {
	EXPECT_EQ(ArbitraryInteger(-3) >> 1, -2);
	EXPECT_EQ(ArbitraryInteger(-4) >> 1, -2);
	EXPECT_EQ(ArbitraryInteger(-1) >> 1000, -1);
	EXPECT_EQ(ArbitraryInteger("-4294967296") >> 32, -1);
	EXPECT_EQ(ArbitraryInteger("-4294967297") >> 32, -2);
	EXPECT_EQ(ArbitraryInteger("-18446744073709551617") >> 64, -2);
	EXPECT_EQ((ArbitraryInteger(-3) << 32).toString(), "-12884901888");
}

TEST(Shifts, HandlesZeroIdentityAndLargeRightCounts) {
	EXPECT_EQ((ArbitraryInteger(0) << 1000).toString(), "0");
	EXPECT_EQ(ArbitraryInteger(0) >> 1000, 0);
	EXPECT_EQ(ArbitraryInteger(-17) << 0, -17);
	EXPECT_EQ(ArbitraryInteger(-17) >> 0, -17);
	EXPECT_EQ(ArbitraryInteger(17) >> std::numeric_limits<int>::max(), 0);
	EXPECT_EQ(ArbitraryInteger(-17) >> std::numeric_limits<int>::max(), -1);
}

TEST(Shifts, RejectsNegativeCountsWithoutChangingValue) {
	ArbitraryInteger value = 17;
	EXPECT_THROW((void)(value << -1), std::invalid_argument);
	EXPECT_THROW((void)(value >> -1), std::invalid_argument);
	EXPECT_THROW(value <<= -1, std::invalid_argument);
	EXPECT_THROW(value >>= -1, std::invalid_argument);
	EXPECT_EQ(value, 17);
	EXPECT_THROW((void)(ArbitraryInteger(0) << -1), std::invalid_argument);
}

TEST(Shifts, SupportsCompoundAssignmentAndLargeRoundTrips) {
	ArbitraryInteger value("-123456789012345678901234567890");
	const ArbitraryInteger original = value;
	for (int count : {0, 1, 31, 32, 33, 63, 64, 65, 96, 1000}) {
		EXPECT_EQ(&(value <<= count), &value);
		EXPECT_EQ(&(value >>= count), &value);
		EXPECT_EQ(value, original);
	}
}

TEST(Shifts, MatchesUnsignedNativeArithmetic) {
	std::mt19937_64 random(625);
	for (int i = 0; i < 1000; ++i) {
		const std::uint64_t value = random();
		const int count = static_cast<int>(random() % 64);
		const ArbitraryInteger a(std::to_string(value));
		EXPECT_EQ((a >> count).toString(), std::to_string(value >> count));

		const std::uint64_t small = value >> count;
		EXPECT_EQ((ArbitraryInteger(std::to_string(small)) << count).toString(),
		std::to_string(small << count));
	}
}
