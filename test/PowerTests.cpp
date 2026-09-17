#include "ArbitraryInteger.hpp"

#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>

TEST(Power, HandlesZeroExponentAndIdentity) {
	const ArbitraryInteger value("123456789012345678901234567890");
	EXPECT_EQ(value.power(0), 1);
	EXPECT_EQ(value.power(1), value);
	EXPECT_EQ(ArbitraryInteger(0).power(0), 1);
	EXPECT_EQ(ArbitraryInteger(0).power(7), 0);
	EXPECT_EQ(value.toString(), "123456789012345678901234567890");
}

TEST(Power, SquaresLargeValuesAndPreservesSigns) {
	const ArbitraryInteger value = -3;
	EXPECT_EQ(ArbitraryInteger(3).power(5), 243);
	EXPECT_EQ(value.power(5), -243);
	EXPECT_EQ(value.power(6), 729);
	EXPECT_EQ(ArbitraryInteger(2).power(100).toString(), "1267650600228229401496703205376");
	EXPECT_EQ(ArbitraryInteger(1).power(std::numeric_limits<int>::max()), 1);
	EXPECT_EQ(ArbitraryInteger(-1).power(std::numeric_limits<int>::max()), -1);
	EXPECT_EQ(value, -3);
}

TEST(Power, RejectsNegativeExponents) {
	const ArbitraryInteger value = 2;
	EXPECT_THROW(value.power(-1), std::invalid_argument);
	EXPECT_THROW(value.power(std::numeric_limits<int>::min()), std::invalid_argument);
	EXPECT_EQ(value, 2);
}

TEST(PowerMod, AcceptsNativeAndArbitraryIntegerModuli) {
	const ArbitraryInteger value = 4;
	EXPECT_EQ(value.powermod(13, 497), 445);
	EXPECT_EQ(value.powermod(13, ArbitraryInteger(497)), 445);
	EXPECT_EQ(value.powermod(13, 497L), 445);
	EXPECT_EQ(value.powermod(5, value), 0);
	EXPECT_EQ(value, 4);

	const ArbitraryInteger large("123456789012345678901234567890");
	const ArbitraryInteger modulus("18446744073709551629");
	EXPECT_EQ(large.powermod(12345, modulus).toString(), "4817569928602088519");
	EXPECT_EQ(large.toString(), "123456789012345678901234567890");
	EXPECT_EQ(modulus.toString(), "18446744073709551629");
}

TEST(PowerMod, ReturnsNonnegativeResiduesForNegativeBases) {
	const ArbitraryInteger value = -2;
	EXPECT_EQ(value.powermod(3, 5), 2);
	EXPECT_EQ(value.powermod(4, 5), 1);
	EXPECT_EQ(ArbitraryInteger(-10).powermod(3, 5), 0);
	EXPECT_EQ(value, -2);
}

TEST(PowerMod, HandlesZeroExponentAndModulusOne) {
	EXPECT_EQ(ArbitraryInteger(7).powermod(0, 5), 1);
	EXPECT_EQ(ArbitraryInteger(0).powermod(0, 5), 1);
	EXPECT_EQ(ArbitraryInteger(0).powermod(3, 5), 0);
	EXPECT_EQ(ArbitraryInteger(0).powermod(0, 1), 0);
	EXPECT_EQ(ArbitraryInteger(-7).powermod(0, 1), 0);
	EXPECT_EQ(ArbitraryInteger(7).powermod(123, 1), 0);
}

TEST(PowerMod, HandlesLargeExponentsWithoutBuildingTheFullPower) {
	EXPECT_EQ(ArbitraryInteger(2).powermod(2147483647, 97), 66);
}

TEST(PowerMod, RejectsNegativeExponentsAndNonpositiveModuli) {
	const ArbitraryInteger value = 2;
	EXPECT_THROW(value.powermod(-1, 5), std::invalid_argument);
	EXPECT_THROW(value.powermod(std::numeric_limits<int>::min(), 5), std::invalid_argument);
	EXPECT_THROW(value.powermod(3, 0), std::invalid_argument);
	EXPECT_THROW(value.powermod(0, 0), std::invalid_argument);
	EXPECT_THROW(value.powermod(3, -5), std::invalid_argument);
	EXPECT_THROW(value.powermod(3, ArbitraryInteger("-18446744073709551616")), std::invalid_argument);
	EXPECT_EQ(value, 2);
}
