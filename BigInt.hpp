#pragma once
#include <vector>

template <class T1, class T2>
struct Pair;

namespace bigint
{
  const unsigned int RADIX { 5 }; //RADIX shouldn't be more than half of the max representable type of it's integer size

  class BigInt
  {
    private:
      bool sign { true }; //isPositive
      std::vector<int> nums;

      void pad (std::size_t zeroes);

      bool static compare (const BigInt& a, const BigInt& b);

    public:
      void print () const;

      BigInt ();
      BigInt (long long num);
      BigInt (const BigInt& num);

      static BigInt   toBigInt (long long num);

      void clear ();
      void trim  ();

      bool                    getSign   () const;
      std::size_t             size      () const;
      std::vector<int>&       digits    ();
      const std::vector<int>& digits    () const;

      int& operator[] (int n);
      int  operator[] (int n) const;

      bool isZero  () const;
      bool isEmpty () const;

      bool isOdd () const;

      BigInt operator- () const;

      bool operator== (const BigInt& num) const;
      bool operator!= (const BigInt& num) const;
      bool operator>  (const BigInt& num) const;
      bool operator>= (const BigInt& num) const;
      bool operator<  (const BigInt& num) const;
      bool operator<= (const BigInt& num) const;

      BigInt abs () const;

      BigInt  operator+  (const BigInt& addend) const;
      BigInt& operator+= (const BigInt& addend);
      BigInt  operator+  (long long num) const;
      BigInt& operator+= (long long num);
      
      BigInt  operator-  (const BigInt& subtrahend) const;
      BigInt& operator-= (const BigInt& subtrahend);
      BigInt  operator-  (long long num) const;
      BigInt& operator-= (long long num);

      BigInt& operator-- ();
      BigInt  operator-- (int);
      BigInt& operator++ ();
      BigInt  operator++ (int);

      BigInt  operator<<  (const BigInt& num) const;
      BigInt& operator<<= (const BigInt& num);
      BigInt  operator<<  (long long num) const;
      BigInt& operator<<= (long long num);

      BigInt  operator>>  (const BigInt& num) const;
      BigInt& operator>>= (const BigInt& num);
      BigInt  operator>>  (long long num) const;
      BigInt& operator>>= (long long num);

      BigInt  operator*  (BigInt multiplicand) const;
      BigInt& operator*= (const BigInt& multiplicand);
      BigInt  operator*  (long long num) const;
      BigInt& operator*= (long long num);

      Pair<BigInt, BigInt> divmod (const BigInt& dividend, const BigInt& divisor) const;
      
      BigInt  operator/  (const BigInt& divisor) const;
      BigInt& operator/= (const BigInt& divisor);
      BigInt  operator/  (long long num) const;
      BigInt& operator/= (long long num);
  };
  
  long long toInt (const bigint::BigInt& num);
};