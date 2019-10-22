#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "huge_num.h"

#define UNSIGNED_INT_MAX 4294967295
#define _2_32 4294967296 // 2^32
typedef struct bigNum{
    bool isNotNeg;
    int length;
    unsigned int * array;
}  BigNum;

BigNum intToBigNum(int i){
  BigNum  bigNum;
  bigNum.length = 1;
  bigNum.array =  (unsigned int*) malloc(sizeof(int) * 1);

  if (i >= 0){
    bigNum.isNotNeg = true;
    bigNum.array[0] = i;
  }
  else{
    bigNum.isNotNeg = false;
    bigNum.array[0] = -i;
  }

  return  bigNum;
}
bool bigNumAbsGreaterThan(BigNum lhs, BigNum rhs){
  if (lhs.length > rhs.length){
      return true;
  }
  else if (lhs.length < rhs.length){
      return false;
  }
  else{
    // compare with reversing order
    for (int i = lhs.length-1; i >= 0; i--){
      if (lhs.array[i] > rhs.array[i]){
        return true;
      }
    }
    return false;
  }
}

bool bigNumAbsEqual(BigNum lhs, BigNum rhs){
  // compare with reversing order
    for (int i = lhs.length-1; i >= 0; i--){
      if (lhs.array[i] != rhs.array[i]){
        return false;
      }
    }
    return true;
  }

bool bigNumEqual(BigNum lhs, BigNum rhs){
  if (lhs.isNotNeg != rhs.isNotNeg){
    return false;
  }
  else{
    return bigNumAbsEqual(lhs, rhs);
  }
}

bool bigNumGreaterThan(BigNum lhs, BigNum rhs){
  if (lhs.isNotNeg == true & rhs.isNotNeg == false){
    return true;
  }
  else if(lhs.isNotNeg == false & rhs.isNotNeg == true){
    return false;
  }
  if (lhs.isNotNeg == true & rhs.isNotNeg == true){
    return bigNumAbsGreaterThan(lhs, rhs);
  }
  else{
    return ! bigNumAbsGreaterThan(lhs, rhs);
  }
}

bool bigNumAbsLessThan(BigNum lhs, BigNum rhs){
  bool result = (! bigNumAbsGreaterThan(lhs, rhs)) & (! bigNumAbsEqual(lhs, rhs));
  return result;
}

bool bigNumLessThan(BigNum lhs, BigNum rhs){
  bool result = (! bigNumGreaterThan(lhs, rhs)) & (! bigNumEqual(lhs, rhs));
  return result;
}

// >> 1
BigNum shiftRightOnce(BigNum num){
  if (num.length == 1){
    num.array[0] = num.array[0] >> 1;
    return num;
  }
  else{
    for(int i = 0; i < num.length; i++){
    unsigned int borrow;

    if (i == num.length - 1){
      borrow = 0;
    }
    else{
      borrow = num.array[i+1] % 2;
    }
      
    unsigned int borrow_added = borrow * 2147483648; // 2147483648 = 2 ** 31
    num.array[i] = (num.array[i] >> 1) | borrow_added;
    }

    if (num.array[num.length-1] == 0){
      num.array = realloc(num.array, sizeof(int)*num.length - 1);
      num.length = num.length - 1;
    }

    return num;
  }
}

BigNum subtract(BigNum lhs, BigNum rhs){
  // lhs - 0 = lhs
  if (bigNumEqual(rhs, intToBigNum(0))){
    return lhs;
  }
  // lhs - rhs = lhs + (- rhs) = lhs - new_rhs
  else{
    BigNum new_rhs = rhs;
    new_rhs.isNotNeg = !(new_rhs.isNotNeg);
    return add(lhs, new_rhs);
    }
  }

// the real "add function"
BigNum add(BigNum lhs, BigNum rhs){
  if (lhs.isNotNeg == true && rhs.isNotNeg == true){
    BigNum result;

    if (!bigNumAbsLessThan(lhs,rhs)){
    result = absadd(lhs, rhs);
    }
    else{
      result = absadd(rhs, lhs);
    }
    result.isNotNeg = true;
    return result;
  }
  else if(lhs.isNotNeg == false && rhs.isNotNeg == false){
    BigNum result;
    if (!bigNumAbsLessThan(lhs,rhs)){
    result = absadd(lhs, rhs);
    }
    else{
      result = absadd(rhs, lhs);
    }
    result.isNotNeg = false;
    return result;
  }
  else if(lhs.isNotNeg == true && rhs.isNotNeg == false){
    BigNum result;

    // eg. 5 + (-4) > 0 or (6) + (-6) == 0
    if (bigNumAbsGreaterThan(lhs, rhs) || bigNumAbsEqual(lhs, rhs)){
      result = abssubtraction(lhs, rhs);
      result.isNotNeg = true;
      return result;
    }

    // eg. 7 + (-8) < 0
    else{
      result = abssubtraction(rhs, lhs);
      result.isNotNeg = false;
      return result;
    }
  }
  // lhs < 0 and rhs >= 0
  else{
    BigNum result;
    // eg. (-5) + (2) < 0
    if (bigNumAbsGreaterThan(lhs, rhs)){
      result = abssubtraction(lhs, rhs);
      result.isNotNeg = false;
      return result;
    }

    // eg. (-6) + (8) > 0 or (-3) + (3) == 0
    else{
      result = abssubtraction(rhs, lhs);
      result.isNotNeg = true;
      return result;
    }
  }
}

// |a| + |b|. a must be greater than or equal to b. 
BigNum absadd(BigNum a, BigNum b){
  BigNum result;
  if (a.length > b.length){
    result.length = a.length;
  }
  else {
    result.length = b.length;
  }

  result.array = calloc(result.length, sizeof(unsigned int));

  unsigned long int carry = 0;

  for (int i=0; i < result.length; i++){
    unsigned long int b_array_i;
    if (i >= b.length){
      b_array_i = 0;
    }
    else{
      b_array_i = b.array[i];
    }
    unsigned long int a_array_i = a.array[i];

    unsigned long int sum = a_array_i + b_array_i + carry;
    
    unsigned int result_i = sum % _2_32;

    result.array[i] = result_i;
    
    carry = sum / _2_32;
  }

  if (carry > 0){
    result.length += 1;
    result.array[result.length-1] = carry;
  }

  return result;
}

BigNum abssubtraction(BigNum a, BigNum b){
  BigNum result;
  result.length = a.length;
  result.array = malloc(sizeof(unsigned int)* result.length);

  for (int i=0; i < result.length; i++){

    unsigned int b_array_i;
    if (i >= b.length){
      b_array_i = 0;
    }
    else{
      b_array_i = b.array[i];
    }
    
    if (a.array[i] < b_array_i){
      a.array[i+1] -= 1;
      
      result.array[i] =  a.array[i] + (UNSIGNED_INT_MAX - b_array_i);
    }
    else{
      result.array[i] = a.array[i] - b_array_i;
    }
  }

  for(int i=result.length-1; i >= 0; i--){
    if (result.array[i] > 0){
      break;
    }
    else{
      if (i > 0){
        result.array = realloc(result.array, result.length - 1);
        result.length -= 1;
      }
    }
  }

    
  
  
  return result;
}


// << 1
BigNum shiftLeftOnce(BigNum num){
  unsigned int lastItem = num.array[num.length-1];
    
  // append one empty item to catch the carry
  if (lastItem >= 2147483648) // 8FFFFFFF
  {
    num.array = realloc(num.array, sizeof(unsigned int)*(num.length+1));
    num.array[num.length] = 0;
    num.length += 1;
  }

  for (int i = num.length-1; i >= 0; i--){
    unsigned int shiftedNum = num.array[i] << 1;
    unsigned int carryFromTheNext;
    if (i != 0){
      carryFromTheNext = num.array[i-1] >> 31;
    }
    else{
      carryFromTheNext = 0;
    }
    num.array[i] = shiftedNum | carryFromTheNext;
  }
  return num;
}

//  << n
BigNum shiftLeft(BigNum num, int n){
  for(int i=0;i<n;i++){
    num = shiftLeftOnce(num);
  }
  return num;
}

BigNum shiftRight(BigNum num, int n){
  for(int i=0; i<n; i++){
    num = shiftRightOnce(num);
  }

  return num;
}

unsigned int lo(unsigned int n){
  return ((1U << 16)-1) & n;
}

unsigned int hi(unsigned int n){
  return n >> 16;
}

BigNum multiply(BigNum lhs, BigNum rhs){

  int temp_result_len = lhs.length + rhs.length;

  BigNum result = intToBigNum(0);

  for (int i=0;i<lhs.length;i++){
    for (int j=0;j<rhs.length;j++){
      unsigned long int l = lhs.array[i];
      unsigned long int r = rhs.array[j];
      unsigned long int lr = l * r;

      unsigned long int base = (unsigned int)(lr % _2_32);
      unsigned long int carry = (unsigned int) (lr / _2_32);

      BigNum temp_big_num;
      temp_big_num.isNotNeg = true;
        
      if (carry != 0){
        temp_big_num.length = 2;
        temp_big_num.array = realloc(temp_big_num.array, sizeof(unsigned int)*2);
        temp_big_num.array[0] = (unsigned int) base;
        temp_big_num.array[1] = (unsigned int) carry;
      }else{
        temp_big_num.length = 1;
        temp_big_num.array = malloc(sizeof(unsigned int));
        temp_big_num.array[0] = base;
      }
        
      temp_big_num = shiftLeft(temp_big_num, (i+j)*32);

      result = add(result, temp_big_num);
    }
  }

  if (lhs.isNotNeg == rhs.isNotNeg || bigNumEqual(lhs, intToBigNum(0)) || bigNumEqual(rhs, intToBigNum(0))){
    result.isNotNeg = true;
  }
  else{
    result.isNotNeg = false;
  }

  return result;
}

/*
find remainder (%).
12 % 5 = 2 
12 % -5 = 2
-12 % 5 = -2
-12 % -5 = -2
*/
BigNum BigNumRemainder(BigNum lhs, BigNum rhs){
  BigNum bigNumZero = intToBigNum(0);

  if (bigNumEqual(rhs,bigNumZero)){
    printf("div 0 error, return 0");
    return bigNumZero;
  }
  else{
    BigNum q = quotient(lhs, rhs);
    BigNum res = findRemainder(lhs, rhs, q);

    return res;
  }
};

BigNum findRemainder(BigNum lhs, BigNum rhs, BigNum quotient){
  BigNum res;
  res = subtract(lhs, multiply(rhs, quotient));
  
  return res;
  }

/* find quotient (//).
12 // 5 = 2 
12 // -5 = -2
-12 // 5 = -2
-12 // -5 = 2
*/
BigNum quotient(BigNum lhs, BigNum rhs){
  BigNum bigNumZero = intToBigNum(0);

  if (bigNumEqual(rhs,bigNumZero)){
    printf("div 0 error, return 0");
    return bigNumZero;
  }
  else if (bigNumAbsLessThan(lhs, rhs)){
    return bigNumZero;
  }
  /* with Newton's Method, let's find the quotient.
  see: http://yoxem.github.io/2019/10/12/tua7soo3ji7tu5huat4ian2sng3huat4/
  */
  else{
    BigNum abs_lhs = lhs;
    abs_lhs.isNotNeg = true;

    BigNum abs_rhs = rhs;
    abs_rhs.isNotNeg = true; // force abs_rhs be non-zero

    BigNum y_k_minus_1 = intToBigNum(0); // y_(k-1)
    BigNum y_k = intToBigNum(1); // y_k ; y_0 = 1

    int p = lhs.length + 1;

    while(! bigNumEqual(y_k, y_k_minus_1)){
      y_k_minus_1 = y_k;
      printf("y_k_minus_1: ");
      print_num(y_k_minus_1);

      BigNum _2y_k = multiply(intToBigNum(2), y_k);
      printf("_2y_k: ");
      print_num(_2y_k);
      BigNum y_k_2_d = multiply(multiply(y_k, y_k), abs_rhs); // y_k^(2)*abs_rhs
      printf("y_k_2_d: ");
      print_num(y_k_2_d);

      BigNum y_k_2_d_rightshift_p = shiftRight(y_k_2_d, p * 32); // (y_k^2 * abs_rhs) >> p
      printf("y_k_2_d_rightshift_p");
      print_num(y_k_2_d_rightshift_p);

      y_k = subtract(_2y_k, y_k_2_d_rightshift_p);
      printf("y_k");
      print_num(y_k);


    }

    BigNum lhs_y_k = multiply(lhs, y_k); // lhs * y_k
    BigNum quotient = shiftRight(lhs_y_k,p * 32); // lhs * y_k // >> p


    BigNum remainder = subtract(abs_lhs,multiply(abs_rhs,quotient));
    // if |lhs| - q * |rhs| < 0 ; adjust value of q by decrease q
    while (bigNumLessThan(remainder, bigNumZero)){
      quotient = subtract(quotient, intToBigNum(1));
      remainder = subtract(abs_lhs,multiply(abs_rhs,quotient));
    }
    
    // if |lhs| - q * |rhs| > |rhs|; adjust value of q by increase q
    while (bigNumGreaterThan(remainder, abs_rhs)){
      quotient = add(quotient, intToBigNum(1));
      remainder = subtract(abs_lhs, multiply(abs_rhs, quotient));
    }

    if(lhs.isNotNeg != rhs.isNotNeg){
      quotient.isNotNeg = false;
    }

    return quotient;
  }
}


/* 
convert to decminal number string.
*/
char* BigNumToDecStr(BigNum n){
  char* result;


  bool n_sign = n.isNotNeg;
  n.isNotNeg = true; // set it to be non-zero
  
  BigNum bigNumZero = intToBigNum(0);
  BigNum bigNum10 = intToBigNum(10);

  while(!bigNumEqual(n, bigNumZero)){
    char* digitStr;
    BigNum quotient_of_div_10 = quotient(n,bigNum10);
    BigNum digitBigNum =  findRemainder(n, bigNum10, quotient_of_div_10);

    unsigned int digit = digitBigNum.array[0];
    sprintf(digitStr, "%u", digit);

    char* new_result = malloc(sizeof(char)*(strlen(result)+1)); // strlen(digitStr) == 1
    strcat(new_result, result);
    strcat(new_result, digitStr);
    
    result = new_result;

    n = quotient_of_div_10;
    }

  // if it is neg. num, add a minus "-" sign.
  if (!n_sign){
    char* result_sign = "-";
    char* new_result = malloc(sizeof(char)*(strlen(result)+1));
    strcat(new_result, result_sign);
    strcat(new_result, result);

    result = new_result;
  };

  return result;
}

void print_num(BigNum num){
  printf("[");
  for (int i=0; i<num.length; i++){
    printf("%u, ", num.array[i]);
  }
  printf("]\n");
}

int main(void){
    /*BigNum a = intToBigNum(-12345);Program terminated with signal SIGABRT, Aborted.

    BigNum b = intToBigNum(-74892074);
    BigNum g = intToBigNum(4294967296/2-1);
    g = shiftLeft(g, 3);
    g = shiftRight(g, 3);*/

    /*

    BigNum x1;
    x1.isNotNeg = true;
    x1.length = 2;

    x1.array = malloc(sizeof(unsigned int) * 2);
    x1.array[0] = 2576980387;
    x1.array[1] = 429496729;
    BigNum x2;
    x2.isNotNeg = true;
    x2.length = 1;
    x2.array = malloc(sizeof(unsigned int) * 1);

    x2.array[0] = 120;

    BigNum z = multiply(x1, x2); */

    BigNum y1 = intToBigNum(120);
    BigNum y2 = intToBigNum(76);

    BigNum x =  quotient(y1, y2);// quotient(intToBigNum(76),intToBigNum(4));
    for(int i=0;i<x.length;i++){
      printf("%d\t", x.array[i]);
    }
    // printf("%s", BigNumToDecStr(x)); */

    /*
    printf("%s", BigNumToDecStr(a));
    printf("%s", BigNumToDecStr(b));
    printf("%s", BigNumToDecStr(g));
    */


    // bool c = bigNumGreaterThan(a, b);
    // bool d = bigNumLessThan(a, b);
    // bool e = bigNumEqual(a, b);
    
    // BigNum l = get_length_n_zero_array(10 >)
    return 0;

}