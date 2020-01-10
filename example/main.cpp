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
}
