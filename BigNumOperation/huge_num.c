#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct  bigNum{
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

int main(void){
    BigNum a = int_to_BigNum(-12345);
    BigNum b = int_to_BigNum(-74892074);
    bool c = bigNumGreaterThan(a, b);
    bool d = bigNumLessThan(a, b);
    bool e = bigNumEqual(a, b);
    // BigNum l = get_length_n_zero_array(10 >)
    return 0;

}