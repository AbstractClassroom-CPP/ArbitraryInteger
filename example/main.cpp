#include "ArbitraryInteger.hpp"

#include <iostream>

int main() {
	ArbitraryInteger a("123456789012345678901234567890");
	ArbitraryInteger b = 25L;

	std::cout << "a = " << a << '\n';
	std::cout << "a + b = " << a + b << '\n';
	std::cout << "a - b = " << a - b << '\n';
	std::cout << "a * b = " << a * b << '\n';
	std::cout << "a / b = " << a / b << '\n';
	std::cout << "a % b = " << a % b << '\n';

	ArbitraryInteger factorial = 1;
	for (int i = 2; i <= 50; ++i) {
		factorial *= i;
	}
	std::cout << "50! = " << factorial << '\n';

	ArbitraryInteger count;
	++count;
	count += 10;
	count--;
	std::cout << "count = " << count << '\n';

	ArbitraryInteger bits = 12;
	std::cout << "12 & 10 = " << (bits & 10) << '\n';
	std::cout << "12 ^ 10 = " << (bits ^ 10) << '\n';
	std::cout << "12 | 10 = " << (bits | 10) << '\n';
	bits &= 10;
	bits |= 3L;
	bits ^= 5;
	std::cout << "bits after compound assignments = " << bits << '\n';

	ArbitraryInteger shifted = 1;
	shifted <<= 65;
	std::cout << "1 << 65 = " << shifted << '\n';
	shifted >>= 64;
	std::cout << "then >> 64 = " << shifted << '\n';
	std::cout << "-3 >> 1 = " << (ArbitraryInteger(-3) >> 1) << '\n';

	ArbitraryInteger two = 2;
	std::cout << "2^100 = " << two.power(100) << '\n';
	ArbitraryInteger four = 4;
	std::cout << "4^13 mod 497 = " << four.powermod(13, 497) << '\n';
	ArbitraryInteger modulus("18446744073709551629");
	std::cout << "a^12345 mod " << modulus << " = " << a.powermod(12345, modulus) << '\n';
}
