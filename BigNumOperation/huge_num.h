#ifndef HUGE_NUM
#define HUGE_NUM

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
int bigNumDivideOther(BigNum* lhs, BigNum* rhs , BigNum* quotient, BigNum* remainder);
bool bigNumEqual(BigNum* lhs, BigNum* rhs);
void bigNumFree(BigNum* num);
int bigNumInit(BigNum** num, size_t digit_num);
bool bigNumLarger(BigNum* lhs, BigNum* rhs);
bool bigNumLess(BigNum* lhs, BigNum* rhs);
int bigNumMultiply(BigNum*lhs, BigNum* rhs, BigNum* result);
int bigNumShiftLeft(BigNum* big_num, size_t n);
int bigNumShiftRight(BigNum* big_num, size_t n);
int bigNumSubtract(BigNum* lhs, BigNum* rhs, BigNum* result);
int bigNumToStr(BigNum* num, char* str, uint32_t base);
int int32ToBigNum(BigNum* bigNum, int32_t i32Num);
int strToBigNum(BigNum* num, char* str, uint32_t base);
void strToBigNumAux(BigNum* num, int32_t digit, uint32_t base);
bool isOverBorder(int32_t digit, uint32_t base);
bool is16base(char c);
int32_t hexCharToStr(char c);
void bigNumClearZero(BigNum* n, size_t initValue);
char intToHexChar(int i);
uint32_t uint32ToBigNum(BigNum* num, uint32_t u);
void main();
void strrev(char* str);
#endif