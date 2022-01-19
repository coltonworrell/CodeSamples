/******************************************************************************

Math Notes:
K choose
N         C(k-1,n-1)

K choose from n with remainders    C(n,k)_R = C(n+k-1,k)

choose m 0's and n 1's, and assum m >= n  =>  C(m+n,n) - C(m+n,m)
                                             number of 0-dominated strings

choose m 0's and n 1's not consecutive   C(m+1,n)

Find the number of distributions of a given set of identical balls into a given
set of distinct boxes: m = balls, n = boxes   =>         C(n+1-1,m)

https://www.careerbless.com/aptitude/qa/permutations_combinations_imp7.php is a cool table

https://oeis.org/ //integer sequences fuckin awesome

//This is for exponent shit
(b) 8! / (1! X 5! X 2!) * (1)^1 * (-1)^5 * (3)^2

C46 Find the coefficient of X^2Y^4Z in the expansion of each of the following.
(a) (2X + Y - Z)^7      => A = 2x, b=
(b) (X + Y^2 + Z)^5     =>
(c) (Y + Z - X^2 + 2)^9 =>

word of m length, from a set of n letters
|Order Matters | Order Does Not Matter
|------------------------------------
|              |         |         |
|No Repeats:   |n!/(n-m)!| C(n,m)  |
|--------------|---------|---------|
|              |         |         |
|   Repeats:   |   n^m   |Cr(n,m)  |
|--------------|---------|---------|

| Objects m     Boxes n      None        <= 1       >= 1       = 1
|----------------------------------------------------------------------
|            |            |          |          |          |          |
|  distinct  |  distinct  |  n^m     |n!/(n-m)! | T(m,n)   |n!=m?0:n! |
|------------|------------|----------|----------|----------|----------|
|            |            |          |          |          |          |
| identical  |  distinct  | Cr(n,m)  |  C(n,m)  | C(n,m-n) |n!=m?0:1  |
|------------|------------|----------|----------|----------|----------|
|            |            |          |          |          |          |
|  distinct  | identical  |          |          |          |n!=m?0:1  |
|------------|------------|----------|----------|----------|----------|
|            |            |          |          |          |          |
| identical  | identical  |          |          |          |n!=m?0:1  |
|------------|------------|----------|----------|----------|----------|


geometric equation r^2 = c_1 * r + c_2

A and B are constants
use two a_whatever to find A and B

if one value  for r => a_n = r_(1)^n * ( A + n*B)
if two values for r => a_n = r_(1)^n * A + r_(2)^n * B)

either use two equations are:
if(1 r value ) => A + B = a_0,    r_(1)^n * A + r_(2)^n * B) = a_1
if(2 r values) => A     = a_0,    r_(1)^n * (A + B)          = a_1

*******************************************************************************/
#include <stdio.h>
#include <vector>
#include <set>
#include <iostream>
#include <cmath>

#include "Partition.h"

#define Pow boost::multiprecision::pow

using namespace boost::multiprecision;

#define V(...) std::vector<num>({__VA_ARGS__})

std::vector<std::string> all;

const bool alphabeticalOrder = false;
const unsigned length = 16;
const char start = 'A';
const char end = 'C';
const unsigned possibilities = end - start + 1;

std::set<std::string> result;

//uses stuff above
void GenerateFromSet(std::string permutation, unsigned steps)
{
  if (steps == 0)
    all.push_back(permutation);
  else
  {
    unsigned firstValue = (alphabeticalOrder && permutation.length() > 0) ? permutation[permutation.length() - 1] - start: 0;
    for (unsigned i = firstValue; i < possibilities; ++i)
    {
      GenerateFromSet(permutation + char(start + i), steps - 1);
    }
  }
}

//rearangements
void GenerateRearangements(std::string total, std::string running)
{
  if (total == "")
  {
    all.push_back(running);
    return;
  }
  for (unsigned i = 0; i < total.size(); ++i)
  {
    GenerateRearangements(total.substr(0, i) + total.substr(i + 1, total.size() - i), running + total[i]);
  }
}

//derangments
void GenerateDerangements(std::string total, std::string running)
{
  static std::string original;
  if (running == "")
  {
    original = total;
  }
  if (total == "")
  {
    all.push_back(running);
    return;
  }
  for (unsigned i = 0; i < total.size(); ++i)
  {
    if (total[i] == original[running.size()])
      continue;
    GenerateDerangements(total.substr(0, i) + total.substr(i + 1, total.size() - i), running + total[i]);
  }
}

bool isDominated(std::string s, char dom)
{
  unsigned domCount = 0;
  unsigned otherCount = 0;
  for (char c : s)
  {
    if (c == dom) ++domCount;
    else ++otherCount;
    if (domCount < otherCount) return false;
  }
  return true;

}

void GenerateSpecial(std::string permutation, unsigned steps)
{
  unsigned _start;
  unsigned _possibilities;
  switch (steps)
  {
  case 7:  _start = 'a'; _possibilities = 3; break;
  case 6:  _start = 't'; _possibilities = 3; break;
  case 5:  _start = 'w'; _possibilities = 4; break;
  case 4:  _start = 'd'; _possibilities = 3; break;
  case 3:  _start = 'g'; _possibilities = 3; break;
  case 2:  _start = 'g'; _possibilities = 3; break;
  case 1:  _start = 'p'; _possibilities = 4; break;
  default: _start = '0'; _possibilities = 1; break;
  }
  if (steps == 0)
    all.push_back(permutation);
  else
  {
    unsigned firstValue = 0;
    for (unsigned i = firstValue; i < _possibilities; ++i)
    {
      GenerateSpecial(permutation + char(_start + i), steps - 1);
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////

enum class Function {
  factorial,
  C,
  Cr,
  T,
  S,
  P,
  StringsOfNElementsAndKLength,
  MFlagsOnNPoles,
  BinsBalls,
  Partition,
  D,
  PStar,
  SolveRecurenceRelation,
  Default
};

num factorial(num n, bool print = false)
{
  if(print)std::cout << n << "! ";
  num sum = 1;
  for (num i = 1; i <= n; ++i)
  {
    sum *= i;
  }
  return sum;
}

//return number of combinations of n distinct elements of r length
num C(num n, num r, bool print = false)
{
  if (print)std::cout << "C(" << n << "," << r << ") ";
  return factorial(n) / (factorial(r) * factorial(n - r));
}

//distribute m distinct balls to n distinct boxes
//m balls, m_2 => balls in box 2, m_n is number of boxes
// C(m!,m_1!,m_2!,...,m_n!)
num C(num m, std::vector<num> n, bool print = false)
{
  if (print)
  {
    std::cout << "C(" << m << ", {";
    for (unsigned i = 0; i < n.size() - 1; ++i)
      std::cout << n[i] << (i == n.size() - 1 ? "" : ", ");
    std::cout << "}) ";
  }
  num denominator = 1;
  for (auto i : n)
    denominator *= factorial(i);
  return factorial(m) / denominator;
}

num C(num m, std::vector<num>& n, bool print = false)
{
  if (print)
  {
    std::cout << "C(" << m << ", {";
    for (unsigned i = 0; i < n.size() - 1; ++i)
      std::cout << n[i] << (i == n.size() - 1 ? "" : ", ");
    std::cout << "}) ";
  }
  num denominator = 1;
  for (auto i : n)
    denominator *= factorial(i);
  return factorial(m) / denominator;
}

num Cr(num n, num r, bool print = false)
{
  if (print)std::cout << "Cr(" << n << "," << r << ") ";
  return C(n + r - 1, r);
}

//M distinct balls into N distinct boxes, non can be empty
num T(num m, num n, bool print = false)
{
  if (print)std::cout << "Cr(" << m << "," << n << ") ";
  num sum = 0;
  for (num k = 0; k <= n; ++k)
  {
    sum += (num)pow(-1, (long long)k) * C(n, k) * Pow(n - k, (long long)m);
  }
  return sum;
}

num S(num k, num n, bool print = false)
{
  if (print)std::cout << "S(" << k << "," << n << ") ";

  if (k == 0 && n == 0) return 1;
  if (n == 0) return 0;
  if (k < n) return 0;
  num sum = 0;
  for (int i = 0; i <= n - 1; ++i)
  {
    sum += Pow(cpp_int(-1), i) * C(n, i) * Pow(n - i, (unsigned long long)k);
  }
  num factorialN = factorial(n);
  return (sum / factorialN);
}

num P(num n, num r, bool print = false)
{
  if (print)std::cout << "P(" << n << "," << r << ") ";
  return factorial(n) / (factorial(n - r));
}

num StringsOfNElementsAndKLength(num n, num k, bool noElementAppearsMoreThanOnce = false, bool print = false)
{
  if (noElementAppearsMoreThanOnce)
  {
    auto a = factorial(n, print);
    if(print) std::cout << "/ ";
    auto b = factorial(n - k, print);
    return a / b;
  }
  if (print) std::cout << n << "^" << k << " ";
  return Pow(n, (unsigned long long)k);
}

num MFlagsOnNPoles( num m, num n, bool identicalFlags = true, bool identicalPoles = true, bool print = false)
{
  num sum = 1;
  //for (int i = 1; i < m; ++i)
  //{
  //  sum *= n*(n + i);
  //}
  //return sum;
  if (identicalFlags && identicalPoles)
  {
    throw("not implimented");
  }
  if (identicalFlags && !identicalPoles)
  {
    if (print) std::cout << "C(" << m << " + " << n << " - 1, " << m << " ) ";
    return C(m + n - 1, m);
    //C(m-1,n-1) if at least one flag per pole
  }
  if (!identicalFlags && identicalPoles)
  {
    throw("not implimented");
  }
  if (!identicalFlags && !identicalPoles)
  {
    if (print) std::cout << "C(" << m << " + " << n << " - 1, " << m << ") * ";
    return C(m + n - 1, m) * factorial(m, print);
    //at least one means C(m - 1, n - 1) * factorial(m);
  }
  throw("how did you even get here?");
  return -1;
}

enum Restriction { None, AtMostOne, AtLeastOne, ExactlyOne};

num BinsBalls(num n, num k, bool identicalBins = true, bool identicalBalls = true, Restriction restriction = Restriction::None, bool print = false)
{
  //std::cout << "n = " << n << ", k = " << k << "\n";
  num sum = 0;
  bool temp = printPartitions;
  printPartitions = false;
  if ( identicalBalls &&  identicalBins)
  {
    switch (restriction)
    {
    case None :
      for (int i = 1; i <= n; ++i) { sum += Partition((ull)k, i); } 
      if (print) std::cout << "for (int i = 1; i <= "<<n<<"; ++i) { sum += Partition(k, i); } "; 
      break;
    case AtMostOne:
      sum = k > n ? 0 : 1;
      if (print) std::cout << "" << k << " > " << n << " ? 0 : 1 "; break;
    case AtLeastOne:
      sum = Partition((ull)k, (ull)n);
      if (print) std::cout << "Partition(" << k << ", " << n << ") ";
      break;
    case ExactlyOne:
      sum = k == n ? 1 : 0;
      if (print) std::cout << "" << k << " == " << n << " ? 1 : 0 "; break;
    default: break;
    }
  }
  if ( identicalBalls && !identicalBins)
  {
    switch (restriction)
    {
    case None:
      sum = C(k + n - 1,n - 1); 
      if (print) std::cout << "C(" << k << " + " << n << " - 1," << n << " - 1) "; break;
    case AtMostOne:
      sum = C(n, k);            
      if (print) std::cout << "C(" << n << ", " << k << ") "; break;
    case AtLeastOne:
      sum = C(k - 1, n - 1);    
      if (print) std::cout << "C(" << k << " - 1, " << n << " - 1) "; break;
    case ExactlyOne:
      sum = k == n ? 1 : 0;     
      if (print) std::cout << "" << k << " == " << n << " ? 1 : 0 "; break;
    default: break;
    }
  }
  if (!identicalBalls &&  identicalBins)
  {
    switch (restriction)
    {
    case None:
      for (int i = 1; i <= n; ++i) { sum += S(k, i); } 
      if (print) std::cout << "for (int i = 1; i <= " << n << "; ++i) { sum += S(" << k << ", i); } "; break;
    case AtMostOne:
      sum = k > n ? 0 : 1;                             
      if (print) std::cout << "" << k << " > " << n << " ? 0 : 1 "; break;
    case AtLeastOne:
      sum = S(k, n);                                   
      if (print) std::cout << "S(" << k << ", " << n << ") "; break;
    case ExactlyOne:
      sum = k == n ? 1 : 0;                            
      if (print) std::cout << "" << k << " == " << n << " ? 1 : 0 "; break;
    default: break;
    }
  }
  if (!identicalBalls && !identicalBins)
  {
    switch (restriction)
    {
    case None:
      sum = Pow(n, (unsigned long long)k);                 
      if (print) std::cout << "" << n << "^" << k << " "; break;
    case AtMostOne:
      sum = P(n, k);                   
      if (print) std::cout << "P(" << n << ", " << k << ") "; break;
    case AtLeastOne:
      sum = S(k, n) * factorial(n);    
      if (print) std::cout << "S(" << k << ", " << n << ") * " << n << "! "; break;
    case ExactlyOne:
      sum = k == n ? factorial(n) : 0; 
      if (print) std::cout << "" << k << " == " << n << " ? " << n << "! : 0 "; break;
    default: break;
    }
  }


  printPartitions = temp;
  return sum;//WHAT GOES HERE? C(n-1,r-1);
}

num D(num n, bool print = false)
{
  if (print)
  {
    std::cout << "D_" << n;
    print = false;
  }
  if (n == 1) return 0;
  if (n == 2) return 1;

  // countDer(n) = (n-1)[countDer(n-1) + der(n-2)]
  return (n - 1) * (D(n - 1) + D(n - 2));
}

num PStar(num m, num n, bool print = false)
{
  if (print) std::cout << "P*(" << m << "," << n << ")";
  num x = m - C(n, 2);
  return Partition((long long)x, (long long)n);
}

num SolveRecurenceRelation(std::vector<num> s, num nToFind, num n = 0, bool print = false)
{
  //find proper r values
  std::set<double> rSet;
  for (unsigned i = 1; i < s.size(); ++i)
  {
    rSet.insert(double(s[i]) / double(s[i - 1]));
    //r.insert(
    //  pow(double(s[i]) / double(s[i - 1]), 1.0 / double(n_))
    //);
  }
  std::vector<double> r;
  for (auto e : rSet) r.push_back(e);


  //a0+b0=c0
  //a1+b1=c1
  double a0;
  double b0;
  double c0;

  double a1;
  double b1;
  double c1;

  //if (2 r values) => A + B = a_0, r_(1) ^ n * A + r_(2) ^ n * B) = a_1
  //if (1 r value ) => A = a_0,     r_(1) ^ n * (A + B) = a_1
  if (r.size() == 1)
  {
    if (print) std::cout << "r = \n" << r[0];
    if (print) std::cout << "a_n = r^n * (A + n * B)\n";
    //a_n = r_(0)^n * ( A + n*B)
    a0 = pow(r[0],double(n));
    b0 = pow(r[0], double(n)) * double(n);
    c0 = double(s[0]);
    //a_n = r_(0)^n * ( A + n*B)
    a1 = pow(r[0], double(n+1));
    b1 = pow(r[0], double(n+1)) * double(n+1);
    c1 = double(s[1]);
  }
  else if (r.size() == 2)
  {
    if (print) std::cout << "r0 = \n" << r[0];
    if (print) std::cout << "r1 = \n" << r[1];
    if (print) std::cout << "a_n = r0^n * A + r1^ n * B)\n";
    //a_n = r_(0)^n * A + r_(1)^n * B)
    a0 = pow(r[0], double(n));
    b0 = pow(r[1], double(n));
    c0 = double(s[0]);
    //a_n = r_(0)^n * A + r_(1)^n * B)
    a1 = pow(r[0], double(n + 1));
    b1 = pow(r[1], double(n + 1));
    c1 = double(s[1]);
  }
  else
  {
    printf("More than 2 r values. SUM TING WONG!\n");
    return -1;
  }

  double A;
  double B;

  double determinant = a0 * b1 - b0 * a1;
  if (determinant != 0) 
  {
    A = (c0 * b1 - b0 * c1) / determinant;
    B = (a0 * c1 - c0 * a1) / determinant;
  }
  else
  {
    printf("Cramer equations system: determinant is zero\n"
      "there are either no solutions or many solutions exist.\n");
    return -1;
  }
  if (print) std::cout << "A = " << A << "\nB = " << B << "\n";
  return num(
    r.size() == 2
    ? pow(r[0], double(nToFind)) * A + pow(r[1], double(nToFind)) * B 
    : pow(r[0], double(nToFind)) * A + pow(r[1], double(nToFind)) * double(nToFind) * B);
}

/*   
     1  31   90        65     15    1

  1       63     301   350   140      21   1

1   127   966   1701   1050   266   28   1
*/
num reachedGoal = 0;
std::pair<unsigned, unsigned> avoid[4] = { {2,3}, {4,1}, {7,2}, {8,5}};
unsigned top = 6; //6
unsigned right = 10; //10

void gridMovement(unsigned x, unsigned y, std::string moves, unsigned counter = 0)
{
  if (x == right && y == top)
  {
    ++reachedGoal;
    all.push_back(moves);
    return;
  }

  if (x > right)
    return;
  if (y > top)
    return;

  for (auto i : avoid)
  {
    if (i.first == x && i.second == y)
      ++counter;
  }
  if (counter > 1)
    return;

  gridMovement(x + 1, y, moves + "R", counter);
  gridMovement(x, y + 1, moves + "U", counter);
}

void WinsAndLosses(std::string tally)
{
  unsigned a = 0;
  unsigned b = 0;
  for (auto i : tally)
  {
    if (i == 'A') ++a;
    if (i == 'B') ++b;
  }
  if (a == 5 || b == 5)
  {
    all.push_back(tally);
    return;
  }
  WinsAndLosses(tally + "A");
  WinsAndLosses(tally + "B");
}

void Calculator()
{
  ////////////////////////////////////////////////
  Function function = Function::factorial;
  bool printEquation = false;
  partitionLimit = -1;
  ////////////////////////////////////////////////
  num total = 0;
  switch (function)
  {
  case Function::factorial:
      total = factorial(8, printEquation); break;
  case Function::C:
      total = C(15, 3, printEquation); break;
  case Function::Cr:
      total = Cr(5, 3, printEquation); break;
  case Function::T:
      total = T(5, 3, printEquation); break;
  case Function::S:
      total = S(5, 3, printEquation); break;
  case Function::P:
      total = P(5, 3, printEquation); break;
  case Function::StringsOfNElementsAndKLength:
      total = StringsOfNElementsAndKLength(5, 3, false, printEquation); break;
  case Function::MFlagsOnNPoles:
      total = MFlagsOnNPoles(5, 3, false, false, printEquation); break;
  case Function::BinsBalls:
      total = BinsBalls(3, 7, true, false, AtLeastOne, printEquation); break;
  case Function::Partition:
    total = Partition(10, 1, printEquation); break;
  case Function::D:
    total = D(4, printEquation); break;
  case Function::PStar:
    total = PStar(15, 4, printEquation); break;
  case Function::SolveRecurenceRelation:
    total = SolveRecurenceRelation(V(1, 1, 3, 5, 11), 5, 0, printEquation); break;

    default:
    {
      //int test = 0;
      //for (int i = 0; i < 60; ++i)
      //{
      //  if (i % 2 == 0 || i % 3 == 0 || i % 5 == 0)
      //  {
      //    std::cout << i << ", ";
      //    ++test;
      //  }
      //}
      //std::cout << "\n\n" << test;

      //std::cout << C(9, 0) << ",\n";
      //std::cout << D(9) << ",\n";
      //std::cout << C(9, 1) << ",\n";
      //std::cout << D(8) << ",\n";
      //std::cout << C(9, 2) << ",\n";
      //std::cout << D(7) << ",\n";
      ////partitionLimit = 3;
      //num tally = 0;
      //for (unsigned i = 1; i < 6; ++i)
      //{
      //  tally += S(5, i, false);
      //}
      //std::cout << "thar she blows: " << tally;
      //std::cout << Partition(10, 3, true);
      //std::vector<std::pair<int, int>> results;
      //Table printing
      //for (int i = 1; i <= 17; ++i)
      //{
      //  std::cout << "Row " << (i <= 9 ? (std::string(" ") + std::to_string(i)) : std::to_string(i)) << ":  ";
      //  for (int j = 1; j <= i; ++j)
      //  {
      //    std::cout << C(i, j) << ", ";
      //    results.push_back({ i, j });
      //  }
      //  std::cout << "\n";
      //}

      //grid movement
      //gridMovement(0, 0,"");
      //std::cout << all.size() << "\n";
      //for (auto i : all)
      //{
      //  std::cout << i << "\n";
      //}
      //std::cout << all.size() << "\n";

      //guess and check bullshit
      //for (unsigned i = 0; i < results.size(); ++i)
      //{
      //  for (unsigned j = i; j < results.size(); ++j)
      //  {
      //    if (
      //      C(results[i].first, results[i].second) * C(results[j].first, results[j].second) == 588
      //      )
      //      all.push_back(
      //        std::string("\nFound: C(") 
      //        + std::to_string(results[i].first) 
      //        + "," 
      //        + std::to_string(results[i].second) 
      //        + ") * C(" 
      //        + std::to_string(results[j].first) 
      //        + "," 
      //        + std::to_string(results[j].second)
      //        + ")\n");
      //  }
      //}
      //for (auto i : all)
      //{
      //  std::cout << i;
      //}

      //std::cout << (S(4, 2) + S(3, 3)) + (S(3, 2) + S(4, 3)) + (S(2, 2) + S(5, 3));
      //std::cout << "\n\n";
      //std::cout << 
      //    (BinsBalls(2, 7, true, false, None, false))
      //  + (BinsBalls(2, 6, true, false, None, false) + BinsBalls(3, 1, true, false, None, false))
      //  + (BinsBalls(2, 5, true, false, None, false) + BinsBalls(3, 2, true, false, None, false))
      //  + (BinsBalls(2, 4, true, false, None, false) + BinsBalls(3, 3, true, false, None, false))
      //  + (BinsBalls(2, 3, true, false, None, false) + BinsBalls(3, 4, true, false, None, false))
      //  + (BinsBalls(2, 2, true, false, None, false) + BinsBalls(3, 5, true, false, None, false))
      //  + (BinsBalls(2, 1, true, false, None, false) + BinsBalls(3, 6, true, false, None, false))
      //  +                                             (BinsBalls(3, 7, true, false, None, false))
      //  ;
      //std::cout << "\n\n";

      //std::cout <<
      //    (BinsBalls(2, 4, true, false, AtLeastOne, false) + BinsBalls(3, 3, true, false, AtLeastOne, false))
      //  + (BinsBalls(2, 3, true, false, AtLeastOne, false) + BinsBalls(3, 4, true, false, AtLeastOne, false))
      //  + (BinsBalls(2, 2, true, false, AtLeastOne, false) + BinsBalls(3, 5, true, false, AtLeastOne, false));


      //Count all partitions
      //for (int i = 1; i <= 5; ++i)
      //{
      //  total += Partition(10, i);
      //}
      //std::cout << "=> " << total << "\n";
     //total = Partition(7, 3, true);
     //total += Partition(7, 2, false);
     //std::cout << "Swiggity Swooty: " << total << "\n";
     //total = 0;
     //total += 4 * C(7, V(4, 1, 1, 1));
     //total += 12 * C(7, V(3, 2, 2, 1));
     //total += 4 * C(7, V(2, 2, 2, 1));
     //std::cout << "Swiggity Swooty2: " << total << "\n";

      std::cout << BinsBalls(2, 7, true, false, AtLeastOne, true);


      //WinsAndLosses("");
      //for (auto p : all)
      //{
      //  std::cout << p << "\n";
      //}
      //std::cout << all.size() << "       ~~~\n";
      return;
    }
  }
  std::cout << "=> " << total << "\n";
}

void Generator()
{
  //generate all permutations in range
  //GenerateSpecial("", 7);
  //GenerateFromSet("", length);
  GenerateRearangements("4111", "");
  //GenerateDerangements("1234", "");


  //std::cout << all.size();



  //std::string s1 = "234";
  //unsigned s1Count = 0;
  //std::string s2 = "3456";
  //unsigned s2Count = 0;
  //std::string s3 = "45678";
  //unsigned s3Count = 0;

  //std::string s1s2 = "23456";
  //unsigned s1s2Count = 0;
  //std::string s1s3 = "2345678";
  //unsigned s1s3Count = 0;
  //std::string s2s3 = "345678";
  //unsigned s2s3Count = 0;

  //std::string s1s2s3 = "2345678";
  //unsigned s1s2s3Count = 0;

  for (auto p : all)
  {
    //cull ones that don't work
    //if(p[0] == p[1]) continue;
    unsigned check = 0;

    //if (p[0] > p[1] || p[0] > p[2] || p[1] > p[2]) continue;
    //if(p[1] == 'B') continue;
    //if(p[2] == 'C') continue;

    //std::set<char> tester;

    //if(isDominated(p, '0')) continue;

    //std::string comp = "INCLUSION";

    //unsigned wins = 0;
    //unsigned loss = 0;
    //unsigned draw = 0;

    //unsigned counter = 0;

    //bool check = false;

    //for (unsigned i = 0; i < p.length(); ++i)
    //{
    //  if (p[i] == 'A')
    //  {
    //    if (++counter == 10)
    //    {
    //      check = true;
    //    }
    //    if (counter == 11)
    //    {
    //      check = false;
    //    }
    //  }
    //  else
    //  {
    //    counter = 0;
    //  }
    //  switch (p[i])
    //  {
    //  case 'A': ++wins; break;
    //  case 'B': ++loss; break;
    //  case 'C': ++draw; break;
    //  default:          break;
    //  }
    //}

    //if (p.find(s1) != std::string::npos)     ++check;
    //if (p.find(s2) != std::string::npos)     ++check;
    //if (p.find(s3) != std::string::npos)     ++check;

    //if (p.find(s1s2) != std::string::npos)   ++check;
    //if (p.find(s1s3) != std::string::npos)   ++check;
    //if (p.find(s2s3) != std::string::npos)   ++check;

    //if (p.find(s1s2s3) != std::string::npos) ++check;
    //std::cout <<

    //if (check == 0)
      result.insert(p);
    //answer for part c is 111300
    //compliment to answer is 8820

    //exactly 7 is 5880
    //exactly 8 is 2205
    //exactly 9 is 630
    //exactly 10 is 105
  }

  unsigned count = 100;

  for (auto p : result)
  {
    std::cout << p << "\n";
    if (--count == 0) break;
  }
  std::cout << "\n\ntotal: " << result.size() << "\n\n";

  if (true)
  {
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";


    //unsigned total1 = s1Count + s2Count + s3Count;

    //unsigned total2 = s1s2Count + s1s3Count + s2s3Count;

    //std::cout << s1Count << "\n";
    //std::cout << s2Count << "\n";
    //std::cout << s3Count << "\n";
    //std::cout << s1s2Count << "\n";
    //std::cout << s1s3Count << "\n";
    //std::cout << s2s3Count << "\n";
    //std::cout << s1s2s3Count << "\n";
    //std::cout << total1 << "\n";
    //std::cout << total2 << "\n";
    //std::cout << s1s2s3Count << "\n";
  }



}


#define CALCULATOR
int main()
{
#ifdef CALCULATOR
  Calculator();
#else
  Generator();
#endif
  return 0;
}

/*
4,3,2,1
7,2,1,0
6,3,1,0
5,4,1,0
5,3,2,0

*/