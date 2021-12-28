#include <bits/stdc++.h>

#include "BigInt.hpp"


int main()
{
  long long num1, num2;
  std::cin >> num1 >> num2;
  bigint::BigInt a { num1 }, b { num2 };

  a.print();
  b.print();
  
  bigint::BigInt c { a * b };
  c.print();
  std::cout << bigint::toInt(c) << '\n' << (bigint::toInt(a) * bigint::toInt(b)) << '\n';
}

//9482
//95 18964