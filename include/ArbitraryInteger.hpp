#ifndef ARBITRARY_INTEGER_HPP
#define ARBITRARY_INTEGER_HPP

#include <cstdint>
#include <iosfwd>
#include <string>
#include <utility>
#include <vector>

class ArbitraryInteger {
public:
	ArbitraryInteger();
	ArbitraryInteger(int value);
	ArbitraryInteger(long value);
	ArbitraryInteger(long long value);
	explicit ArbitraryInteger(const std::string& decimal);

	ArbitraryInteger(const ArbitraryInteger& other);
	ArbitraryInteger& operator=(const ArbitraryInteger& other);

	std::string toString() const;
	bool isZero() const;

	ArbitraryInteger operator+() const;
	ArbitraryInteger operator-() const;

	ArbitraryInteger& operator+=(const ArbitraryInteger& other);
	ArbitraryInteger& operator-=(const ArbitraryInteger& other);
	ArbitraryInteger& operator*=(const ArbitraryInteger& other);
	ArbitraryInteger& operator/=(const ArbitraryInteger& other);
	ArbitraryInteger& operator%=(const ArbitraryInteger& other);

	ArbitraryInteger& operator++();
	ArbitraryInteger operator++(int);
	ArbitraryInteger& operator--();
	ArbitraryInteger operator--(int);

	friend ArbitraryInteger operator+(ArbitraryInteger left, const ArbitraryInteger& right) {
		left += right;
		return left;
	}
	friend ArbitraryInteger operator-(ArbitraryInteger left, const ArbitraryInteger& right) {
		left -= right;
		return left;
	}
	friend ArbitraryInteger operator*(ArbitraryInteger left, const ArbitraryInteger& right) {
		left *= right;
		return left;
	}
	friend ArbitraryInteger operator/(ArbitraryInteger left, const ArbitraryInteger& right) {
		left /= right;
		return left;
	}
	friend ArbitraryInteger operator%(ArbitraryInteger left, const ArbitraryInteger& right) {
		left %= right;
		return left;
	}

	friend bool operator==(const ArbitraryInteger& left, const ArbitraryInteger& right);
	friend bool operator<(const ArbitraryInteger& left, const ArbitraryInteger& right);
	friend bool operator!=(const ArbitraryInteger& left, const ArbitraryInteger& right) {
		return !(left == right);
	}
	friend bool operator>(const ArbitraryInteger& left, const ArbitraryInteger& right) {
		return right < left;
	}
	friend bool operator<=(const ArbitraryInteger& left, const ArbitraryInteger& right) {
		return !(right < left);
	}
	friend bool operator>=(const ArbitraryInteger& left, const ArbitraryInteger& right) {
		return !(left < right);
	}

	friend std::ostream& operator<<(std::ostream& out, const ArbitraryInteger& value);

private:

	static const std::uint64_t base = 4294967296ULL;
	std::vector<std::uint32_t> digits_;
	bool negative_;

	void setValue(long long value);
	void normalize();
	static int compareMagnitude(const ArbitraryInteger& left, const ArbitraryInteger& right);
	static ArbitraryInteger addMagnitude(const ArbitraryInteger& left, const ArbitraryInteger& right);

	static ArbitraryInteger subtractMagnitude(const ArbitraryInteger& left, const ArbitraryInteger& right);
	static std::pair<ArbitraryInteger, ArbitraryInteger> divideWithRemainder(
	const ArbitraryInteger& dividend, const ArbitraryInteger& divisor);
};

#endif
