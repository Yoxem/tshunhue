#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "huge_num.h"

#define _2_32_ 4294967296

typedef struct BigNum {
  bool isNotNeg;
  size_t used;
  uint32_t* digit;
} BigNum;

typedef enum BigNumErrorCode {
  OverBorder,
  UnableToInitialized,
  DivZero,
  IllegalBase} BigNumErrorCode;


// init bigNum with digit_num (length of the array of num->digit). input "BigNum**"
int bigNumInit (BigNum** num, size_t digit_num){
  bool unable_to_initalized = false;
  *num = calloc(sizeof(BigNum), 1);
  if (!*num){
    unable_to_initalized = true;
  }

  

  (*num)->digit = calloc(sizeof(uint32_t), digit_num);
  (*num)->used = digit_num - 1;

  if (!((*num)->digit)){
    unable_to_initalized = true;
  }

  if (unable_to_initalized == true){
    return UnableToInitialized;
  }
  else{
    return 0;
  }
}

void bigNumFree (BigNum* num){
  free(num->digit);
  free(num);
}

void bigNumCopy (BigNum* orig, BigNum * dest){
  dest->used = orig->used;
  dest->isNotNeg = orig->isNotNeg;

  for (size_t i=0;i<=orig->used;i++){
    dest->digit[i] = orig->digit[i];
  }
}

int int32ToBigNum(BigNum* bigNum, int32_t i32Num){
  if (i32Num>=0){
    bigNum->isNotNeg = true;
    bigNum->digit[0] = i32Num;
    bigNum->used = 0;
  }
  else{
    bigNum->isNotNeg = false;
    bigNum->digit[0] = -i32Num;
    bigNum->used = 0;
  }

    return 0;
}

bool bigNumAbsLarger(BigNum* lhs, BigNum* rhs){
  if(lhs->used > rhs->used){
    return true;
  }
  else if(lhs->used < rhs->used){
    return false;
  }
  else{
    for(size_t i=lhs->used;i != -1 ; i--){
      if (lhs->digit[i]>rhs->digit[i]){
        return true;
      }
    }
    return false;
  }
}

bool bigNumAbsEqual(BigNum* lhs, BigNum* rhs){
  if(lhs->used > rhs->used){
    return false;
  }
  else if(lhs->used < rhs->used){
    return false;
  }
  else{
    for(size_t i=lhs->used;i != -1;i--){
      if (lhs->digit[i]!=rhs->digit[i]){
        return false;
      }
    }
    return true;
  }
}

bool bigNumEqual(BigNum* lhs, BigNum* rhs){
  if(lhs->isNotNeg == rhs->isNotNeg){
    return bigNumAbsEqual(lhs, rhs);
  }
  else{
    return false;
}}

bool bigNumLarger(BigNum* lhs, BigNum* rhs){
  if(lhs->isNotNeg == true & rhs->isNotNeg == false){
    return true;
  }
  else if(lhs->isNotNeg == false & rhs->isNotNeg == true){
    return false;
  }else if (lhs->isNotNeg == true & rhs->isNotNeg == true){
    return bigNumAbsLarger(lhs,rhs);
  }else{
    return bigNumAbsLarger(rhs,lhs);
  }
}

bool bigNumLess(BigNum* lhs, BigNum* rhs){
  return (!(bigNumEqual(lhs, rhs) || (bigNumAbsLarger(lhs, rhs))));
}


bool bigNumAbsLess(BigNum* lhs, BigNum* rhs){
  return !(bigNumAbsEqual(lhs, rhs) || bigNumAbsLarger(lhs, rhs));
}

int bigNumAbsAdd(BigNum* bigger, BigNum* smaller, BigNum* result){
  size_t n = bigger->used;
  uint64_t carry = 0;
  
  for (size_t i=0; i<=n;i++){
    uint64_t b_i = bigger->digit[i];
    uint64_t s_i;

    if (i>smaller->used){
      s_i = 0;
    }else{
      s_i = smaller->digit[i];
    }

    uint64_t res = (b_i + s_i + carry);

    if (res > _2_32_){
      result->digit[i] = (uint32_t)(res - _2_32_);
      carry = 1;
    }
    else{
      result->digit[i] = (uint32_t)res;
      carry = 0;
    }

  }

  if (carry == 1){
    result->digit[n+1] = carry;
    result->used = n+1;
  }
  else{
    result->used = n;
  }

  return 0;
}

int bigNumAbsSubtract(BigNum* bigger, BigNum* smaller, BigNum* result){
  size_t n = bigger->used;
  int64_t carry = 0;



  for (size_t i=0; i<=n;i++){
    int64_t b_i = bigger->digit[i];
    int64_t s_i;

    if(n>smaller->used){
      s_i = 0;
    }
    else{
      s_i = smaller->digit[i];
    }

    int64_t res_i;
    if (b_i < s_i){
      res_i = (b_i - s_i + _2_32_) + carry;
      result->digit[i] = (uint32_t) res_i;
      carry = -1;
    }
    else{
      res_i =  b_i + carry - s_i;
      if (res_i < 0){
        res_i = res_i + _2_32_;
        result->digit[i] = (uint32_t) res_i;
        carry = -1;
      }
      else{
        result->digit[i] = res_i;
        carry = 0;
      }
    }
  }

  // update the res->used.
  bigNumClearZero(result, n);

  return 0;
  
}

// clear the initial zero
void bigNumClearZero(BigNum* n, size_t initValue){
  for (size_t i=initValue;i != -1;i--){
    if(n->digit[i] != 0){
      n->used = i;
      break;
    }
    if (n->digit[i] == 0 & i == 0){
      n->used = i;
      break;
    }
  }
}

int bigNumSubtract(BigNum* lhs, BigNum* rhs, BigNum* result){
  if (lhs->isNotNeg == true & rhs->isNotNeg == false){
    result->isNotNeg = true;
    if (bigNumAbsLarger(lhs, rhs)){
      bigNumAbsAdd(lhs,rhs,result);
    }
    else{
      bigNumAbsAdd(rhs,lhs,result);
    }
  }
  else if(lhs->isNotNeg == false & rhs->isNotNeg == true){
    result->isNotNeg = false;
    if (bigNumAbsLarger(lhs, rhs)){
      bigNumAbsAdd(lhs,rhs,result);
    }
    else{
      bigNumAbsAdd(rhs,lhs,result);
    }
  }
  else if(lhs->isNotNeg == true & rhs->isNotNeg == true){
    // 5 - 3
    if(bigNumAbsLarger(lhs, rhs)){
      result->isNotNeg = true;
      bigNumAbsSubtract(lhs, rhs, result);
    }
    // 5 - 8
    else{
      result->isNotNeg = false;
      bigNumAbsAdd(rhs, lhs, result);
    }
  }
  else{
    // (-5) - (-5) or (-5) - (-6)
    if(bigNumAbsLarger(rhs, lhs) || bigNumAbsEqual(rhs,lhs)){
      result->isNotNeg = true;
      bigNumAbsSubtract(rhs, lhs, result);
  }
  // (-5) - (-3)
    else{
      result->isNotNeg = false;
      bigNumAbsSubtract(lhs, rhs, result);
    }
  }
  return 0;
}

int bigNumMultiply(BigNum*lhs, BigNum* rhs, BigNum* result){
  if (lhs->isNotNeg == rhs->isNotNeg){
    result->isNotNeg = true;
  }else{
    result->isNotNeg = false;
  }

  size_t n = lhs->used + rhs->used + 1;

  for (size_t i=0;i<=n;i++){
    result->digit[i] = 0;
  }

  for(size_t i=0; i<=rhs->used; i++){
    uint64_t c = 0;

    for (size_t j=0; j<=lhs->used; j++){
      uint64_t res_i_j = (uint64_t)result->digit[i+j];
      uint64_t l_j_r_i =  (uint64_t)(lhs->digit[j]) * (uint64_t)(rhs->digit[i]);
      uint64_t current_res = res_i_j + l_j_r_i + c;
      
      uint64_t res_base = current_res % _2_32_;
      uint64_t res_carry = current_res / _2_32_;
      
      result->digit[i+j] = (uint32_t) res_base;
      c = res_carry;
    }

    result->digit[i+(lhs->used)+1] = c;
  }

  // correct the result->used
  bigNumClearZero(result, n);
  
  return 0;
}

int bigNumDivideOneDigit(BigNum* lhs, BigNum* rhs , BigNum* quotient, BigNum* remainder){
  uint32_t rhs_number = rhs->digit[0];

  for (size_t i=0;i<=lhs->used;i++){
      quotient->digit[i]=0;
  }

  quotient->used = lhs->used;

  uint64_t temp = 0;

  for(size_t i=lhs->used; i != -1; i--){
    temp = temp * _2_32_ + lhs->digit[i];
    quotient->digit[i] = (uint32_t) (temp / (uint64_t)rhs_number);
    temp = temp % (uint64_t) rhs_number;
  }

  remainder->used = 0;
  remainder->digit[0] = (uint32_t)temp;

  bigNumClearZero(quotient, quotient->used);

  return 0;
}

int bigNumShiftLeft(BigNum* big_num, size_t n){
  if(n == 0){
    return 0;
  }

  big_num->used += n;
  for(size_t i=big_num->used; i>=n;i--){
    big_num->digit[i] = big_num->digit[i-n];
  }

  for(size_t i=0;i<n;i++){
    big_num->digit[i] = 0;
  }

  return 0;
}

// if n > big_num->used, set n = big_num->used
int bigNumShiftRight(BigNum* big_num, size_t n){
  if (n>big_num->used){
    n = big_num->used;
  }
  
  big_num->used -= n;
  
  for(size_t i=0; i<big_num->used;i++){
    big_num->digit[i] = big_num->digit[i+n];
  }

  

  return 0;
}

int bigNumDivide(BigNum* lhs, BigNum* rhs, BigNum* quotient, BigNum* remainder){

  if (bigNumAbsLarger(rhs, lhs)){
    quotient->used = 0;
    quotient->digit[0] = 0;

    remainder->used = lhs->used;
    for (size_t i=0; i<= lhs->used;i++){
      remainder->digit[i] = lhs->digit[i];
    }

    return 0;
  }
  else{

    BigNum* bigNumZero;
    bigNumInit(&bigNumZero,1);
    int32ToBigNum(bigNumZero, 0);

    if(bigNumAbsEqual(rhs, bigNumZero)){
      bigNumFree(bigNumZero);

      return DivZero;
    }else{
      bigNumFree(bigNumZero);
      remainder->isNotNeg = lhs->isNotNeg;

      if(lhs->isNotNeg == rhs->isNotNeg){
        quotient->isNotNeg = true;
      }
      else{
        quotient->isNotNeg = false;
      }

      if (rhs->used == 0){
        return bigNumDivideOneDigit(lhs,rhs,quotient,remainder);
      }
      else{

        size_t t = rhs->used; // rhs->used
        
        size_t diff_l_and_r = lhs->used-t;

        for(size_t j=0;j<=lhs->used-t;j++){
          quotient->digit[j] = 0;
        }
        /* todo : 2. While (x ≥ yb n−t ) do the following: q n−t ←q n−t + 1, x←x − yb n−t . */
        bigNumCopy(lhs, remainder);

        bigNumShiftLeft(rhs,diff_l_and_r);
        while(!bigNumLess(remainder,rhs)){
          quotient->digit[diff_l_and_r] = quotient->digit[diff_l_and_r] + 1;
          bigNumAbsSubtract(remainder,rhs,remainder);
        }

        bigNumShiftRight(rhs,diff_l_and_r); // recover rhs to the right value;

        for(size_t i=lhs->used;i>=t;i--){
          if (lhs->digit[i]==rhs->digit[t]){
            quotient->digit[i-t-1] = _2_32_ - 1;
          }else{
            uint64_t x_i = (uint64_t)(lhs->digit[i]);
            uint64_t x_i_1 = (uint64_t)(lhs->digit[i-1]);
            quotient->digit[i-t-1] = (uint32_t)((x_i * _2_32_ + x_i_1)/((uint64_t)(rhs->digit[t])));
          }
          
          /* While (q i−t−1 (y t b + y t−1 ) > x i b 2 + x i−1 b + x i−2 )
           do: q i−t−1 ←q i−t−1 − 1. */
          BigNum* temp_lhs;
          BigNum* temp_rhs;
          bigNumInit(&temp_lhs,3);
          bigNumInit(&temp_rhs,3);

          uint64_t q_i_t_1_rhs_t = ((uint64_t)quotient->digit[i-t-1]*(uint64_t)rhs->digit[t]);
          uint32_t temp_lhs_2 = (uint32_t)(q_i_t_1_rhs_t/_2_32_);
          uint32_t temp_lhs_1_l = (uint32_t)(q_i_t_1_rhs_t %_2_32_);
          uint64_t q_i_t_1_rhs_t_1 = (uint64_t)quotient->digit[i-t-1]*(uint64_t)rhs->digit[t-1];
          uint32_t temp_lhs_1_r = (uint32_t)(q_i_t_1_rhs_t_1 / _2_32_);
          uint32_t temp_lhs_1 = temp_lhs_1_l + temp_lhs_1_r;

          uint32_t temp_lhs_cons = (uint32_t) (q_i_t_1_rhs_t_1 % _2_32_);
          
          temp_lhs->isNotNeg = true;
          temp_lhs->digit[2] = temp_lhs_2;
          temp_lhs->digit[1] = temp_lhs_1;
          temp_lhs->digit[0] = temp_lhs_cons;

          temp_rhs->isNotNeg = true;
          temp_rhs->digit[2] = lhs->digit[i];
          temp_rhs->digit[1] = lhs->digit[i-1];
          temp_rhs->digit[0] = lhs->digit[i-2];

          while(bigNumLarger(temp_lhs,temp_rhs)){
            quotient->digit[i-t-1] = quotient->digit[i-t-1] - 1;
          }
          bigNumFree(temp_lhs);
          bigNumFree(temp_rhs);

          /* x←x − q i−t−1 yb i−t−1 . */
          BigNum* rhs_clone;
          bigNumInit(&rhs_clone, rhs->used);
          bigNumCopy(rhs,rhs_clone);
          bigNumShiftRight(rhs_clone,i-t-1);
          
          for (size_t k=0;k<=rhs_clone->used;k++){
            rhs_clone->digit[k] *= quotient->digit[i-t-1];
          }

          bigNumSubtract(remainder,rhs_clone,remainder);

          bigNumFree(rhs_clone);


          
          if(remainder->isNotNeg == false){
            BigNum* rhs_clone2;
            bigNumInit(&rhs_clone2, rhs->used);
            bigNumCopy(rhs,rhs_clone2);
            bigNumShiftRight(rhs_clone2,i-t-1);

            bigNumAdd(remainder,rhs_clone2,remainder);
            quotient->digit[i-t-1] = quotient->digit[i-t-1] - 1;

            bigNumFree(rhs_clone2);

          }
        }

        return 0;

      }
    }
  }
}
  
int bigNumAdd(BigNum* lhs, BigNum* rhs, BigNum* result){
  if (lhs->isNotNeg == true & rhs->isNotNeg == true){
    result->isNotNeg = true;
    if (bigNumAbsLarger(lhs, rhs)){
      bigNumAbsAdd(lhs,rhs,result);
    }
    else{
      bigNumAbsAdd(rhs,lhs,result);
    }
  }
  else if (lhs->isNotNeg == false & rhs->isNotNeg == false){
    result->isNotNeg = false;
    if(bigNumAbsLarger(lhs, rhs)){
      bigNumAbsAdd(lhs, rhs, result);
    }
    else{
      bigNumAbsAdd(rhs, lhs, result);
    }
  }
  else if (lhs->isNotNeg == true & rhs->isNotNeg == false){
    // 5 + (-3)
    if(bigNumAbsLarger(lhs, rhs)){
      result->isNotNeg = true;
      bigNumAbsSubtract(lhs, rhs, result);
    }
    // 5 + (-8)
    else{
      result->isNotNeg = false;
      bigNumAbsAdd(rhs, lhs, result);
    }
  }
  else{
    // (-5) + (5) or (-5) + (6)
    if(bigNumAbsLarger(rhs, lhs) || bigNumAbsEqual(rhs,lhs)){
      result->isNotNeg = true;
      bigNumAbsSubtract(rhs, lhs, result);
    }
    // (-5) + (3)
    else{
      result->isNotNeg = false;
      bigNumAbsSubtract(lhs, rhs, result);
    }
  }

  return 0;
}

void strrev(char* str){
  if( str == 0 || *str == 0){
    return ;
  }

  char x;
  int i = 0;
  int j = strlen(str) - 1;

  while(i < j){
    x = str[i];
    str[i] = str[j];
    str[j] = x;
    i++;
    j--;
  }

}

// str to num (result num, should be inited first) in base (2 ~ 64).
int strToBigNum(BigNum* num, char* str, uint32_t base){
  if (base < 2 || base > 64){
    return IllegalBase;
  }

  size_t digitBorder = 0; // the border of the digit (0~9)

  if(str[0] == '-'){
    digitBorder = 1; // the border of the digit (0~9);
  }

  // set result num to be 0
  num->used = 0;
  num->digit[0] = 0;


  for (size_t i= digitBorder; i<strlen(str); i++){
    int32_t digit = str[i] - '0';

    if(digit < 0 || digit > 9){
      return OverBorder;
    }
    else{

      strToBigNumAux(num,digit,base);
    }
  }

  bool isNeg = (str[0] == '-') & (num->digit>0 || num->used>0);

  if (isNeg){
    num->isNotNeg = false;
  }

  return 0;
}

bool is16base(char c){
  if (c >= '0' & c <= '9'){
    return true;
  }
  else if (c >= 'a' & c <= 'f'){
    return true;
  }
  else if (c >= 'A' & c <= 'F'){
    return true;
  }
  else{
    return false;
  }
}

void strToBigNumAux(BigNum* num, int32_t digit, uint32_t base){
  BigNum* numTimesBase;
  BigNum* bigNumBase;
  BigNum* bigNumDigit;
  BigNum* tempResult;

  bigNumInit(&numTimesBase, (num->used)+1);
  bigNumInit(&bigNumBase, 1);
  bigNumInit(&bigNumDigit, 1);
  bigNumInit(&tempResult, (num->used)+1);

  int32ToBigNum(bigNumBase,(int32_t)base);
  int32ToBigNum(bigNumDigit, digit);
  bigNumMultiply(num, bigNumBase, numTimesBase);
  bigNumAdd(numTimesBase, bigNumDigit, tempResult);

  bigNumCopy(tempResult, num);

  bigNumFree(numTimesBase);
  bigNumFree(bigNumBase);
  bigNumFree(bigNumDigit);
  bigNumFree(tempResult);

  }

// convert 0~9, a~f, and A~F to number
int32_t hexCharToStr(char c){
  if (c >= '0' & c <= '9'){
    return c - '0';
  }
  else if (c >= 'a' & c <= 'f'){
    return c - 'a' + 10;
  }
  // A~F
  else{
    return c - 'A' + 10;
  }
}

// BigNum to str (should be calloc-ed first) in 16-base
int HexbigNumToStr(BigNum* num, char* str){

  uint32_t base = 16;
  size_t digitBorder = 0; // the border of the digit (0~9)

  if(str[0] == '-'){
    digitBorder = 1; // the border of the digit (0~9);
  }

  // set result num to be 0
  num->used = 0;
  num->digit[0] = 0;


  for (size_t i=digitBorder; i<strlen(str); i++){

    if (!is16base(str[i])){
      return OverBorder;
    }else{

      int32_t digit = hexCharToStr(str[i]);

      strToBigNumAux(num,digit,base);
    }
  }

  bool isNeg = (str[0] == '-') & (num->digit>0 || num->used>0);

  if (isNeg){
    num->isNotNeg = false;
  }

  return 0;
}

// BigNum to str (should be calloc-ed first) in base (2 / 8 / 10 / 16 / 64)
int bigNumToStr(BigNum* num, char* str, uint32_t base){
  if (base != 2 & base != 8 & base != 10 & base != 16 & base != 64){
    return IllegalBase;
  }

  if (base == 16){
    return HexbigNumToStr(num, str);
  }
  
  else if (num->used == 0 & num->digit[0] == 0){ // num == 0
    *str++ = '0';
    *str++ = '\0';
    return 0;
  }else{
    char* negChar = NULL;
    if (num->isNotNeg == false){
      negChar = "-\0";
    }

    size_t i = 0;
    BigNum* x; 
    BigNum* q; //quotient
    BigNum* b; // base
    BigNum* r; // remainder
    bigNumInit(&x, num->used);
    bigNumInit(&q, num->used);
    bigNumInit(&b, 0);
    bigNumInit(&r, 0);

    bigNumCopy(num,x);
    int32ToBigNum(b, (int32_t)base);
    
    char* str_orig = str;
    
    while(x->used>0||x->digit[0]>0){
      i++;
      int divide_error_code = bigNumDivide(x,b,q,r);
      if (divide_error_code){
        return divide_error_code;
      }
      bigNumCopy(q,x);

      char str_i = (char)(((int)r->digit[0]) + '0');
      *str++ = str_i;
    }
    *str++ = '\0';

    str = str_orig;

    if(negChar != NULL){
      strcat(str, negChar);
    }

    strrev(str);

    bigNumFree(x); 
    bigNumFree(q); //quotient
    bigNumFree(b); // base
    bigNumFree(r); 

  }

  return 0;
}

void main(){
  BigNum* a;
  bigNumInit(&a,88);
  strToBigNum(a, "-123456778122345566", 10);
  char* s = calloc(sizeof(char),20);
  bigNumToStr(a,s,10);
  printf("%s", s);

  bigNumFree(a);
  free(s);
}