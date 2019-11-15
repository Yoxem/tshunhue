#ifndef HUGE_NUM
#define HUGE_NUM

typedef struct BigNum BigNum;

int bigNumAbsAdd(BigNum* bigger, BigNum* smaller, BigNum* result);
bool bigNumAbsEqual(BigNum* lhs, BigNum* rhs);
bool bigNumAbsLarger(BigNum* lhs, BigNum* rhs);
bool bigNumAbsLess(BigNum* lhs, BigNum* rhs);
int bigNumAbsSubtract(BigNum* bigger, BigNum* smaller, BigNum* result);
int bigNumAdd(BigNum* lhs, BigNum* rhs, BigNum* result);
void bigNumCopy(BigNum* orig, BigNum * dest);
int bigNumDivide(BigNum* lhs, BigNum* rhs, BigNum* quotient, BigNum* remainder);
int bigNumDivideOneDigit(BigNum* lhs, BigNum* rhs , BigNum* quotient, BigNum* remainder);
bool bigNumEqual(BigNum* lhs, BigNum* rhs);
void bigNumFree(BigNum* num);
int bigNumInit(BigNum** num, size_t digit_num);
bool bigNumLarger(BigNum* lhs, BigNum* rhs);
bool bigNumLess(BigNum* lhs, BigNum* rhs);
int bigNumMultiply(BigNum*lhs, BigNum* rhs, BigNum* result);
int bigNumShiftLeft(BigNum* big_num, size_t n);
int bigNumShiftRight(BigNum* big_num, size_t n);
int bigNumSubtract(BigNum* lhs, BigNum* rhs, BigNum* result);
int BigNumToStr(BigNum* num, char* str, uint32_t base);
int int32ToBigNum(BigNum* bigNum, int32_t i32Num);
void StrToBigNumAux(BigNum* num, int32_t digit, uint32_t base);
bool is16base(char c);
int32_t hexCharToStr(char c);
void bigNumClearZero(BigNum* n, size_t initValue);
void main();
void strrev(char* str);
#endif