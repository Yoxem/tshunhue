/*
huge_num.c - a simpified version of libtommath,
which is a public domain library or arbitary-precision number
arthimetic library.

*/

#include "huge_num.h"

#define _2_32_ 4294967296
#define OVER_BOUND 999

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
      if(lhs->digit[i]<rhs->digit[i]){
        return false;
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

    if (res >= _2_32_){
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

    if(i>smaller->used){
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
  // -3 - 5 or -5 - 3
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
      bigNumAbsSubtract(rhs, lhs, result);
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

  size_t n = (lhs->used + 1) + (rhs->used + 1);
  uint32_t* temp_result = calloc(sizeof(uint32_t), n); // actually length; n_used = n - 1
  
  for (size_t i=0;i<n;i++){
    temp_result[i] = ((uint32_t)0);
  }


  for(size_t i=0; i<=rhs->used; i++){
    uint64_t c = 0;

    for (size_t j=0; j<=lhs->used; j++){
      uint64_t res_i_j = (uint64_t)temp_result[i+j];
      uint64_t l_j_r_i =  (uint64_t)(lhs->digit[j]) * (uint64_t)(rhs->digit[i]);
      uint64_t current_res = res_i_j + l_j_r_i + c;
      
      uint64_t res_base = current_res % _2_32_;
      uint64_t res_carry = current_res / _2_32_;
      
      temp_result[i+j] = (uint32_t) res_base;
      c = res_carry;
    }

    temp_result[i+(lhs->used)+1] = (uint32_t)c;
  }
  
  for(size_t i=0;i<n;i++){
	  result->digit[i] = temp_result[i];
  }

  // correct the result->used
  size_t n_used = n - 1;
  bigNumClearZero(result, n_used);

  free(temp_result);
  
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
  for(size_t i=big_num->used; i != n-1;i--){
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
  
  for(size_t i=0; i<=big_num->used;i++){
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
    bool lhsIsNotNeg = lhs->isNotNeg;
    bool rhsIsNotNeg = rhs->isNotNeg;
    bool quotientIsNotNeg;
    bool remainderIsNotNeg;

    BigNum* bigNumZero;
    bigNumInit(&bigNumZero,1);
    int32ToBigNum(bigNumZero, 0);

    if(bigNumAbsEqual(rhs, bigNumZero)){
      bigNumFree(bigNumZero);

      return DivZero;
    }else{
      bigNumFree(bigNumZero);
      remainderIsNotNeg = lhs->isNotNeg;

      if(lhs->isNotNeg == rhs->isNotNeg){
        quotientIsNotNeg = true;
      }
      else{
        quotientIsNotNeg = false;
      }

      // set the isNotNeg to true temporily
      lhs->isNotNeg = true;
      rhs->isNotNeg = true;
      quotient->isNotNeg = true;
      remainder->isNotNeg = true;

      if (rhs->used == 0){
        bigNumDivideOneDigit(lhs,rhs,quotient,remainder);
      }else{
        bigNumDivideOther(lhs,rhs,quotient,remainder);
      }


        // recover to their isNotNeg
        lhs->isNotNeg = lhsIsNotNeg;
        rhs->isNotNeg = rhsIsNotNeg;
        quotient->isNotNeg = quotientIsNotNeg;
        remainder->isNotNeg = remainderIsNotNeg;

        return 0;
    
    }
  }
}

/* big num division with larger digits(num->used >= 1). The Algorithm is from HoAC.
https://github.com/libtom/libtommath/blob/develop/s_mp_div_school.c
*/
int bigNumDivideOther(BigNum* lhs, BigNum* rhs, BigNum* quotient, BigNum* remainder){
  // normalization such that y_t * normalcoeff >= 2 ** 32 / 2
  uint32_t normalcoeff = (uint32_t)((_2_32_ / 2 + 1) / (uint64_t)rhs->digit[rhs->used]);

  BigNum* bigNumNormalCoeff;
  bigNumInit(&bigNumNormalCoeff, 1);

  uint32ToBigNum(bigNumNormalCoeff, normalcoeff);

  BigNum* temp;
  bigNumInit(&temp, lhs->used+2);
  // magnify temporily
  bigNumMultiply(lhs, bigNumNormalCoeff, temp);
  bigNumCopy(temp, lhs);
  bigNumMultiply(rhs, bigNumNormalCoeff, temp);
  bigNumCopy(temp, rhs);


  size_t t = rhs->used; // rhs->used

  size_t diff_l_and_r = lhs->used-t;
  quotient->used = diff_l_and_r;

  for(size_t j=0;j<=diff_l_and_r;j++){
    quotient->digit[j] = 0;
  }        

  // the algorithm is from HoAC Ch14.
  /* 2. While (x ≥ yb n−t ) do the following: q n−t ←q n−t + 1, x←x − yb n−t . */
  bigNumCopy(lhs, remainder);


  bigNumShiftLeft(rhs,diff_l_and_r);
  while(!bigNumLess(remainder,rhs)){
    quotient->digit[diff_l_and_r]++;
    bigNumAbsSubtract(remainder,rhs,remainder);
  }

  bigNumShiftRight(rhs,diff_l_and_r); // recover rhs to the right value;

  for(size_t i=remainder->used;i >= t + 1;i--){
    if (remainder->digit[i]==rhs->digit[t]){
      quotient->digit[i-t-1] = _2_32_ - 1;
    }else{
      uint64_t x_i = (uint64_t)(remainder->digit[i]);
      uint64_t x_i_1 = (uint64_t)(remainder->digit[i-1]);
      quotient->digit[i-t-1] = (uint32_t)((x_i * _2_32_ + x_i_1)/((uint64_t)(rhs->digit[t])));
    }
    
    /* While (q i−t−1 (y t b + y t−1 ) > x i b 2 + x i−1 b + x i−2 )
      do: q i−t−1 ←q i−t−1 − 1. */
    BigNum* temp_lhs;
    BigNum* temp_rhs;
    BigNum* temp2;
    
    bigNumInit(&temp_lhs,3);
    bigNumInit(&temp_rhs,3);
    bigNumInit(&temp2,3);
    
    quotient->digit[i-t-1] += 1;

    do{
      quotient->digit[i-t-1] -= 1;
      




      temp_lhs->isNotNeg = true;
      temp_lhs->digit[1] = ((t - 1) < 0) ? 0u : rhs->digit[t];
      temp_lhs->digit[0] = rhs->digit[t-1];
      temp_lhs->used = 1;

      uint32ToBigNum(temp, quotient->digit[i-t-1]);

      
      bigNumMultiply(temp_lhs, temp, temp2);
      bigNumCopy(temp2, temp_lhs);

      temp_rhs->isNotNeg = true;
      temp_rhs->used = 2;
      temp_rhs->digit[2] = remainder->digit[i];
      temp_rhs->digit[1] = ((i - 1) < 0) ? 0u : remainder->digit[i-1];
      temp_rhs->digit[0] = ((i - 2) < 0) ? 0u : remainder->digit[i-2];

    }while(bigNumLarger(temp_lhs,temp_rhs));
    
    bigNumFree(temp_lhs);
    bigNumFree(temp_rhs);

    /* x←x − q i−t−1 yb i−t−1 . */
    BigNum* rhs_clone;
    bigNumInit(&rhs_clone, (rhs->used)+1);
    bigNumCopy(rhs,rhs_clone);
    bigNumShiftLeft(rhs_clone,i-t-1);
    
    uint32ToBigNum(temp, quotient->digit[i-t-1]);
    bigNumMultiply(rhs_clone,temp,temp2);
    bigNumCopy(temp2, rhs_clone);

    bigNumSubtract(remainder,rhs_clone,remainder);

    bigNumFree(rhs_clone);
    bigNumFree(temp2);


    
    if(remainder->isNotNeg == false){
      BigNum* rhs_clone2;
      bigNumInit(&rhs_clone2, (rhs->used)+1);
      bigNumCopy(rhs,rhs_clone2);
      bigNumShiftLeft(rhs_clone2,i-t-1);

      bigNumAdd(remainder,rhs_clone2,remainder);
      quotient->digit[i-t-1] = quotient->digit[i-t-1] - 1;

      bigNumFree(rhs_clone2);

    }
  }


  BigNum* temp3;
  bigNumInit(&temp3, lhs->used+1);

  bigNumDivide(remainder, bigNumNormalCoeff, temp3, temp);
  bigNumCopy(temp3, remainder);
  bigNumDivide(lhs, bigNumNormalCoeff, temp3, temp);
  bigNumCopy(temp3, lhs);
  bigNumDivide(rhs, bigNumNormalCoeff, temp3, temp);
  bigNumCopy(temp3, rhs);
  bigNumFree(temp3);


  bigNumClearZero(remainder, remainder->used);
  bigNumClearZero(quotient, quotient->used);

  bigNumFree(temp);

  bigNumFree(bigNumNormalCoeff);
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
      bigNumAbsSubtract(rhs, lhs, result);
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

// str to num (result num, should be inited first) in base (2 ~ 16).
int strToBigNum(BigNum* num, char* str, uint32_t base){
  if (base != 2 & base != 8 & base != 10 & base != 16 ){
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
    int32_t digit = hexCharToStr(str[i]);

    if(isOverBorder(digit, base)){
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

bool isOverBorder(int32_t digit, uint32_t base){
  if (digit < 0 || digit >= base){
    return true;
  }
  else{
    return false;
  }
};



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

  bigNumInit(&numTimesBase, (num->used)+2);
  bigNumInit(&bigNumBase, 1);
  bigNumInit(&bigNumDigit, 1);
  bigNumInit(&tempResult, (num->used)+2);

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
  else if (c >='A' & c <= 'F'){
    return c - 'A' + 10;
  }
  else{
    return  OVER_BOUND; // OverBound
  }
}



// BigNum to str (should be calloc-ed first) in base (2 / 8 / 10 / 16)
int bigNumToStr(BigNum* num, char* str, uint32_t base){
  // clear the string
  if (strlen(str) !=0){
    memset(str,0,sizeof(char)*strlen(str));
  }

  if (base != 2 & base != 8 & base != 10 & base != 16){
    return IllegalBase;
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

    BigNum* x; 
    BigNum* q; //quotient
    BigNum* b; // base
    BigNum* r; // remainder
    bigNumInit(&x, num->used+1);
    bigNumInit(&q, num->used+1);
    bigNumInit(&b, 1);
    bigNumInit(&r, 1);

    bigNumCopy(num,x);
    int32ToBigNum(b, (int32_t)base);
    
    char* str_orig = str;
    
    while(x->used>0||x->digit[0]>0){
      int divide_error_code = bigNumDivide(x,b,q,r);
      if (divide_error_code){
        return divide_error_code;
      }
      bigNumCopy(q,x);

      char str_i; // storing current digit char
      if (base == 16){
        str_i = intToHexChar(r->digit[0]);
      }else{
        str_i = (char)(((int)r->digit[0]) + '0');
      }
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

uint32_t uint32ToBigNum(BigNum* num, uint32_t ui32Num){
    num->isNotNeg = true;
    num->digit[0] = ui32Num;
    num->used = 0;
  

    return 0;
}

// 0 ~ 9 => '0' ~ '9' ; 10 ~ 15 => 'a' ~ 'f'
char intToHexChar(int i){
  if (i < 10){
    return (char) (i + '0');
  }
  // 'a' ~ 'f'
  else{
    return (char) (i - 10 + 'a');
  }
}

