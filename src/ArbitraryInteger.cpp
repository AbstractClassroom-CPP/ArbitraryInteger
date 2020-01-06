#include "ArbitraryInteger.hpp"

#include <ostream>
#include <stdexcept>

ArbitraryInteger::ArbitraryInteger() {
	digits_.push_back(0);
	negative_ = false;
}

ArbitraryInteger::ArbitraryInteger(int value) {
	setValue(value);
}

ArbitraryInteger::ArbitraryInteger(long value) {
	setValue(value);
}

ArbitraryInteger::ArbitraryInteger(long long value) {
	setValue(value);
}

ArbitraryInteger::ArbitraryInteger(const ArbitraryInteger& other) {
	digits_ = other.digits_;
	negative_ = other.negative_;
}

ArbitraryInteger& ArbitraryInteger::operator=(const ArbitraryInteger& other) {
	digits_ = other.digits_;
	negative_ = other.negative_;
	return *this;
}

void ArbitraryInteger::setValue(long long value) {
	digits_.clear();
	negative_ = false;
	std::uint64_t magnitude;

	if (value < 0) {
		negative_ = true;

		magnitude = static_cast<std::uint64_t>(-(value + 1));
		magnitude = magnitude + 1;
	}
	else {
		magnitude = static_cast<std::uint64_t>(value);
	}

	if (magnitude == 0) {
		digits_.push_back(0);
	}
	while (magnitude != 0) {
		digits_.push_back(static_cast<std::uint32_t>(magnitude));
		magnitude = magnitude >> 32;
	}
}

ArbitraryInteger::ArbitraryInteger(const std::string& decimal) {
	digits_.push_back(0);
	negative_ = false;
	std::size_t start = 0;
	if (!decimal.empty() && (decimal[0] == '-' || decimal[0] == '+')) {
		start = 1;
	}
	if (start == decimal.size()) {
		throw std::invalid_argument("An integer needs at least one decimal digit");
	}
	for (std::size_t i = start; i < decimal.size(); ++i) {
		if (decimal[i] < '0' || decimal[i] > '9') {
			throw std::invalid_argument("Expected an optional sign followed by decimal digits");
		}
	}

	for (std::size_t i = start; i < decimal.size(); ++i) {
		*this *= 10;
		*this += decimal[i] - '0';
	}
	negative_ = decimal[0] == '-';
	normalize();
}

std::string ArbitraryInteger::toString() const {
	if (isZero()) {
		return "0";
	}
	ArbitraryInteger remaining = *this;
	remaining.negative_ = false;
	std::string result;
	while (!remaining.isZero()) {

		std::uint64_t remainder = 0;
		for (std::size_t i = remaining.digits_.size(); i > 0; --i) {
			const std::uint64_t current = remainder * base + remaining.digits_[i - 1];
			remaining.digits_[i - 1] = static_cast<std::uint32_t>(current / 10);
			remainder = current % 10;
		}
		result += static_cast<char>('0' + remainder);
		remaining.normalize();
	}
	if (negative_) {
		result += '-';
	}

	for (std::size_t i = 0; i < result.size() / 2; ++i) {
		char saved = result[i];
		result[i] = result[result.size() - 1 - i];
		result[result.size() - 1 - i] = saved;
	}
	return result;
}

bool ArbitraryInteger::isZero() const {
	return digits_.size() == 1 && digits_[0] == 0;
}

void ArbitraryInteger::normalize() {
	while (digits_.size() > 1 && digits_.back() == 0) {
		digits_.pop_back();
	}
	if (isZero()) {
		negative_ = false;
	}
}

int ArbitraryInteger::compareMagnitude(const ArbitraryInteger& left, const ArbitraryInteger& right) {
	if (left.digits_.size() != right.digits_.size()) {
		if (left.digits_.size() < right.digits_.size()) {
			return -1;
		}
		return 1;
	}
	for (std::size_t i = left.digits_.size(); i > 0; --i) {
		if (left.digits_[i - 1] != right.digits_[i - 1]) {
			if (left.digits_[i - 1] < right.digits_[i - 1]) {
				return -1;
			}
			return 1;
		}
	}
	return 0;
}

ArbitraryInteger ArbitraryInteger::addMagnitude(const ArbitraryInteger& left, const ArbitraryInteger& right) {
	ArbitraryInteger result;
	result.digits_.clear();
	std::uint64_t carry = 0;
	std::size_t count = left.digits_.size();
	if (right.digits_.size() > count) {
		count = right.digits_.size();
	}
	for (std::size_t i = 0; i < count || carry != 0; ++i) {
		std::uint64_t sum = carry;
		if (i < left.digits_.size()) {
			sum += left.digits_[i];
		}
		if (i < right.digits_.size()) {
			sum += right.digits_[i];
		}
		result.digits_.push_back(static_cast<std::uint32_t>(sum));
		carry = sum >> 32;
	}
	return result;
}

ArbitraryInteger ArbitraryInteger::subtractMagnitude(const ArbitraryInteger& left, const ArbitraryInteger& right) {
	ArbitraryInteger result;
	result.digits_.resize(left.digits_.size());
	std::uint64_t borrow = 0;
	for (std::size_t i = 0; i < left.digits_.size(); ++i) {
		std::uint64_t subtrahend = borrow;
		if (i < right.digits_.size()) {
			subtrahend = subtrahend + right.digits_[i];
		}
		borrow = 0;
		if (left.digits_[i] < subtrahend) {
			borrow = 1;
		}
		result.digits_[i] = static_cast<std::uint32_t>(left.digits_[i] + borrow * base - subtrahend);
	}
	result.normalize();
	return result;
}

ArbitraryInteger ArbitraryInteger::operator+() const {
	return *this;
}

ArbitraryInteger ArbitraryInteger::operator-() const {
	ArbitraryInteger result = *this;
	if (!result.isZero()) {
		result.negative_ = !result.negative_;
	}
	return result;
}

ArbitraryInteger& ArbitraryInteger::operator+=(const ArbitraryInteger& other) {
	ArbitraryInteger result;
	if (negative_ == other.negative_) {
		result = addMagnitude(*this, other);
		result.negative_ = negative_;
	}
	else if (compareMagnitude(*this, other) >= 0) {
		result = subtractMagnitude(*this, other);
		result.negative_ = negative_;
	}
	else {
		result = subtractMagnitude(other, *this);
		result.negative_ = other.negative_;
	}
	result.normalize();
	*this = result;
	return *this;
}

ArbitraryInteger& ArbitraryInteger::operator-=(const ArbitraryInteger& other) {
	*this += -other;
	return *this;
}

ArbitraryInteger& ArbitraryInteger::operator*=(const ArbitraryInteger& other) {
	ArbitraryInteger result;
	result.digits_.assign(digits_.size() + other.digits_.size(), 0);
	for (std::size_t i = 0; i < digits_.size(); ++i) {
		std::uint64_t carry = 0;
		for (std::size_t j = 0; j < other.digits_.size(); ++j) {

			const std::uint64_t product = static_cast<std::uint64_t>(digits_[i]) * other.digits_[j]
			+ result.digits_[i + j] + carry;
			result.digits_[i + j] = static_cast<std::uint32_t>(product);
			carry = product >> 32;
		}
		result.digits_[i + other.digits_.size()] = static_cast<std::uint32_t>(carry);
	}
	result.negative_ = negative_ != other.negative_;
	result.normalize();
	*this = result;
	return *this;
}

std::pair<ArbitraryInteger, ArbitraryInteger> ArbitraryInteger::divideWithRemainder(const ArbitraryInteger& dividend, const ArbitraryInteger& divisor) {
	if (divisor.isZero()) {
		throw std::domain_error("Division by zero");
	}
	ArbitraryInteger positiveDivisor = divisor;
	positiveDivisor.negative_ = false;
	ArbitraryInteger quotient;
	ArbitraryInteger remainder;
	quotient.digits_.assign(dividend.digits_.size(), 0);

	for (std::size_t i = dividend.digits_.size(); i > 0; --i) {
		remainder.digits_.insert(remainder.digits_.begin(), dividend.digits_[i - 1]);
		remainder.normalize();

		std::uint64_t low = 0;
		std::uint64_t high = base;
		while (low < high) {
			const std::uint64_t middle = low + (high - low) / 2;
			if (positiveDivisor * ArbitraryInteger(static_cast<long long>(middle)) <= remainder) {
				low = middle + 1;
			}
			else {
				high = middle;
			}
		}
		quotient.digits_[i - 1] = static_cast<std::uint32_t>(low - 1);
		remainder -= positiveDivisor * ArbitraryInteger(static_cast<long long>(low - 1));
	}

	quotient.negative_ = dividend.negative_ != divisor.negative_;
	remainder.negative_ = dividend.negative_;
	quotient.normalize();
	remainder.normalize();
	std::pair<ArbitraryInteger, ArbitraryInteger> answer;
	answer.first = quotient;
	answer.second = remainder;
	return answer;
}

ArbitraryInteger& ArbitraryInteger::operator/=(const ArbitraryInteger& other) {
	*this = divideWithRemainder(*this, other).first;
	return *this;
}

ArbitraryInteger& ArbitraryInteger::operator%=(const ArbitraryInteger& other) {
	*this = divideWithRemainder(*this, other).second;
	return *this;
}

ArbitraryInteger& ArbitraryInteger::operator++() {
	*this += 1;
	return *this;
}

ArbitraryInteger ArbitraryInteger::operator++(int) {
	ArbitraryInteger before = *this;
	++*this;
	return before;
}

ArbitraryInteger& ArbitraryInteger::operator--() {
	*this -= 1;
	return *this;
}

ArbitraryInteger ArbitraryInteger::operator--(int) {
	ArbitraryInteger before = *this;
	--*this;
	return before;
}

bool operator==(const ArbitraryInteger& left, const ArbitraryInteger& right) {
	return left.negative_ == right.negative_ && left.digits_ == right.digits_;
}

bool operator<(const ArbitraryInteger& left, const ArbitraryInteger& right) {
	if (left.negative_ != right.negative_) {
		return left.negative_;
	}
	const int comparison = ArbitraryInteger::compareMagnitude(left, right);
	if (left.negative_) {
		return comparison > 0;
	}
	return comparison < 0;
}

std::ostream& operator<<(std::ostream& out, const ArbitraryInteger& value) {
	out << value.toString();
	return out;
}
