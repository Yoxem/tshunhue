#ifndef HUGE_NUM
#define HUGE_NUM

typedef struct bigNum BigNum;

BigNum add(BigNum lhs, BigNum rhs);
BigNum absadd(BigNum a, BigNum b);
BigNum abssubtraction(BigNum a, BigNum b);
bool bigNumAbsEqual(BigNum lhs, BigNum rhs);
bool bigNumAbsGreaterThan(BigNum lhs, BigNum rhs);
bool bigNumAbsLessThan(BigNum lhs, BigNum rhs);
bool bigNumEqual(BigNum lhs, BigNum rhs);
bool bigNumGreaterThan(BigNum lhs, BigNum rhs);
bool bigNumLessThan(BigNum lhs, BigNum rhs);
BigNum intToBigNum(int i);
int main(void);
BigNum multiply(BigNum lhs, BigNum rhs);
BigNum quotient(BigNum lhs, BigNum rhs);
BigNum shiftLeft(BigNum num, int n);
BigNum shiftLeftOnce(BigNum num);
BigNum shiftRight(BigNum num, int n);
BigNum shiftRightOnce(BigNum num);
BigNum subtract(BigNum lhs, BigNum rhs);
BigNum BigNumRemainder(BigNum lhs, BigNum rhs);
BigNum findRemainder(BigNum lhs, BigNum rhs, BigNum quotient);
void print_num(BigNum num);
unsigned int lo(unsigned int n);
unsigned int hi(unsigned int n);

#endif