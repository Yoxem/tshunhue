#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct bigNum{
    bool isNotNeg;
    int length;
    unsigned int * array;
}  BigNum;

BigNum int_to_BigNum(int i){
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
      return false;
    }
  }
}

bool bigNumAbsEqual(BigNum lhs, BigNum rhs){
  // compare with reversing order
  for (int i = lhs.length-1; i >= 0; i--){
    if (lhs.array[i] != rhs.array[i]){
      return false;
    }
  return true;
  }
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

// << 1
BigNum shiftLeftOnce(BigNum num){
  unsigned int lastItem = num.array[num.length-1];
    
  // append one empty item to catch the carry
  if (lastItem >= 2147483648) // 8FFFFFFF
  {
    num.array = realloc(num.array, sizeof(unsigned int)* (num.length+1));
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

int main(void){
    BigNum a = int_to_BigNum(-12345);
    BigNum b = int_to_BigNum(-74892074);
    BigNum g = int_to_BigNum(4294967296/2-1);
    g = shiftLeft(g, 3);
    g = shiftRight(g, 3);

    bool c = bigNumGreaterThan(a, b);
    bool d = bigNumLessThan(a, b);
    bool e = bigNumEqual(a, b);
    
    // BigNum l = get_length_n_zero_array(10 >)
    return 0;

}