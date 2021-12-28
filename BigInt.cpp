#include "BigInt.hpp"
using bigint::BigInt;

#include <iostream>
#include <vector>


template <class T1, class T2>
struct Pair
{
  T1 a;
  T2 b;
};


void BigInt::print() const
{
  std::cout << (sign ? '+' : '-') << ' ';
  for (std::size_t i { 0 }; i < this->nums.size(); ++i)
    std::cout << nums[i] << ' ';
  std::cout << '\n';
}


BigInt BigInt::toBigInt(long long num)
{
  BigInt a;
  if (num < 0) { a.sign = false; num = -num; } //For negatives
  if (num < bigint::RADIX) //If num is less than radix, conversion is unnecessary
    a.nums.push_back(num);
  else
    for (; num; num /= bigint::RADIX)
      a.nums.push_back( num % bigint::RADIX );

  return a;
}

long long toInt(const BigInt& num) //Will lose accuracy if greater than max number representable by integer type
{
  long long res { 0 };

  unsigned int pow { 1 };
  for (std::size_t i { 0 }; i < num.size(); ++i, pow *= bigint::RADIX)
    res += num[i] * pow;

  if (!num.getSign())
    res = -res;
  return res;
}


BigInt::BigInt() { this->sign = true; }

BigInt::BigInt(long long num) { *this = toBigInt(num); }

BigInt::BigInt(const BigInt& num)
{
  this->sign = num.sign;
  this->nums = num.nums;
}


void BigInt::clear()
{
  this->nums.clear();
  this->sign = true;
}

void BigInt::pad(std::size_t zeroes)
{
  for (std::size_t i { 0 }; i < zeroes; ++i)
    this->nums.push_back(0);
}

void BigInt::trim()
{
  for (std::size_t i { this->size() - 1 }; i > 0; --i)
  {
    if ((*this)[i] != 0) //If a non-zero num is found, break out of loop
      break;
    this->nums.pop_back();
  }
}


bool BigInt::getSign() const { return this->sign; }

std::size_t BigInt::size() const { return this->nums.size(); }

std::vector<int>& BigInt::digits() { return this->nums; }

const std::vector<int>& BigInt::digits() const { return this->nums; }


int& BigInt::operator[] (int n) { return this->nums[n]; }
int  BigInt::operator[] (int n) const { return this->nums[n]; }


bool BigInt::isZero() const { return (this->size() == 1 && (*this)[0] == 0) || this->size() == 0; }

bool BigInt::isEmpty() const { return this->size() == 0; }


bool BigInt::isOdd() const
{
  if (this->isZero())
    return false;
  
  if (bigint::RADIX & 0x01)
  {
    bool odd { 0 };
    for (std::size_t i { 0 }; i < this->size(); ++i)
      odd ^= (*this)[i] & 0x01;
    
    return odd;
  }

  return (*this)[0] & 0x01;
}


BigInt BigInt::operator- () const
{
  BigInt a { *this };
  a.sign = a.sign ? false : true;
  return a;
}


bool BigInt::compare(const BigInt& a, const BigInt& b) //0 if a greater than (or equal to), 1 if less
{
  if (a.sign ^ b.sign) //If signs are different
    return (a.sign ? 0 : 1); //return positive
  
  if (a.size() != b.size()) //If sizes are different
    return (a.size() > b.size() ? 0 : 1) ^ !(a.sign); // ^ !(a.sign) in case both are negative
  
  for (std::size_t i { a.size() }; i --> 0 ;)
    if (a[i] != b[i])
      return (a[i] > b[i] ? 0 : 1) ^ !(a.sign);

  return 0; //returns 0 if a and b are equal, should never be the case as compare will never be called for equality
}

bool BigInt::operator==(const BigInt& num) const { return (this->sign == num.sign) && (this->nums == num.nums); }

bool BigInt::operator!=(const BigInt& num) const { return !(*this == num); }

bool BigInt::operator> (const BigInt& num) const { return !(*this == num) & !compare(*this, num); }

bool BigInt::operator>=(const BigInt& num) const { return !compare(*this, num); }

bool BigInt::operator< (const BigInt& num) const { return compare(*this, num); }

bool BigInt::operator<=(const BigInt& num) const { return (*this == num) | compare(*this, num); }


BigInt BigInt::abs() const
{
  BigInt a { *this };
  a.sign = true;
  return a;
}


BigInt BigInt::operator+(const BigInt& addend) const
{
  BigInt res;

  if (this->sign ^ addend.sign) //If signs are different
    return *this - -addend; //subtract by negative of addend
  res.sign = this->sign;

  if (this->isZero() | addend.isZero()) //case for zeroes
    return (this->isZero() ? addend : *this);
  
  const BigInt& big { this->size() >= addend.size() ? *this : addend }, smol { big == *this ? addend : *this };
  int carry { 0 };
  for (std::size_t i { 0 }; i < big.size(); ++i)
  {
    int a { i < smol.size() ? smol[i] : 0 };
    res.nums.push_back(big[i] + a + carry);

    if (res[i] >= bigint::RADIX)
    {
      carry = 1;
      res[i] -= bigint::RADIX;
    }
    else
      carry = 0;
  }
  if (carry)
    res.nums.push_back(1);

  return res;
}

BigInt& BigInt::operator+=(const BigInt& addend)
{
  *this = *this + addend;
  return *this;
}

BigInt BigInt::operator+(long long num) const { return *this + BigInt::toBigInt(num); }

BigInt& BigInt::operator+=(long long num) { return *this += BigInt::toBigInt(num); }

BigInt BigInt::operator-(const BigInt& subtrahend) const
{
  BigInt res;

  if (this->sign ^ subtrahend.sign) //if signs are different
    return *this + -subtrahend; //add negative of subtrahend
  res.sign = this->sign;

  if (this->isZero() | subtrahend.isZero()) //case for zeroes
    return (this->isZero() ? -subtrahend : *this);
  
  if (this->abs() < subtrahend.abs())
    return -(subtrahend - *this);
  
  std::size_t bigSize { this->size() >= subtrahend.size() ? this->size() : subtrahend.size() };
  int borrow { 0 };
  for (std::size_t i { 0 }; i < bigSize; ++i)
  {
    res.nums.push_back((i < this->size() ? (*this)[i] : 0) - (i < subtrahend.size() ? subtrahend[i] : 0) - borrow);

    if (res[i] < 0)
    {
      borrow = 1;
      res[i] += bigint::RADIX;
    }
    else
      borrow = 0;
  }
  
  res.trim();
  return res;
}

BigInt& BigInt::operator-=(const BigInt& subtrahend)
{
  *this = *this - subtrahend;
  return *this;
}

BigInt BigInt::operator-(long long num) const { return *this - BigInt::toBigInt(num); }

BigInt& BigInt::operator-=(long long num) { return *this -= BigInt::toBigInt(num); }


BigInt& BigInt::operator++()
{
  *this += 1;
  return *this;
}

BigInt BigInt::operator++(int)
{
  BigInt temp { *this };
  *this += 1;
  return temp;
}

BigInt& BigInt::operator--()
{
  *this -= 1;
  return *this;
}

BigInt BigInt::operator--(int)
{
  BigInt temp { *this };
  *this -= 1;
  return temp;
}


BigInt BigInt::operator<<(const BigInt& num) const
{
  BigInt res { *this };

  for (BigInt i { num }; i > 0; --i)
  {
    int carry { 0 };
    for (std::size_t j { 0 }; j < res.size(); ++j)
    {
      res[j] <<= 1;
      res[j] += carry;
      if (res[j] >= bigint::RADIX)
      {
        carry = 1;
        res[j] -= bigint::RADIX;
      }
      else
        carry = 0;
    }
    if (carry)
      res.nums.push_back(1);
  }

  return res;
}

BigInt BigInt::operator<<(long long num) const { return *this << BigInt::toBigInt(num); }

BigInt& BigInt::operator<<=(const BigInt& num)
{
  *this = *this << num;
  return *this;
}

BigInt& BigInt::operator<<=(long long num) { return *this <<= BigInt::toBigInt(num); }


BigInt BigInt::operator>>(const BigInt& num) const
{
  BigInt res { *this };

  for (BigInt i { num }; i > 0; --i)
  {
    if (res.isZero())
      break;
      
    int borrow { 0 };
    for (std::size_t j { res.size() }; j --> 0;)
    {
      res[j] += borrow;
      borrow = (res[j] & 0x01) * bigint::RADIX; //sets borrow to radix if res[j] is odd, otherwise 0

      res[j] >>= 1;
    }
  }

  res.trim();
  return res;
}

BigInt BigInt::operator>>(long long num) const { return *this >> BigInt::toBigInt(num); }

BigInt& BigInt::operator>>=(const BigInt& num)
{
  *this = *this >> num;
  return *this;
}

BigInt& BigInt::operator>>=(long long num) { return *this >>= BigInt::toBigInt(num); }


BigInt BigInt::operator*(BigInt multiplicand) const
{
  BigInt res { 0 };

  BigInt a { *this };
  for (; !multiplicand.isZero(); multiplicand >>= 1, a <<= 1)
    if (multiplicand.isOdd()) //if multiplicand is odd
      res += a;

  return res;
}

BigInt& BigInt::operator*=(const BigInt& multiplicand)
{
  *this = *this * multiplicand;
  return *this;
}

BigInt BigInt::operator*(long long num) const { return *this * bigint::toInt(num); }

BigInt& BigInt::operator*=(long long num) { return *this *= bigint::toInt(num); }


// Pair<BigInt, BigInt> BigInt::divmod(const BigInt& dividend, const BigInt& divisor) const
// {
  
// }