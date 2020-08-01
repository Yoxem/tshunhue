#include "../huge_num.h"
#include "../huge_num.c"

void main(){
  BigNum* a;
  BigNum* b;
  bigNumInit(&a,88);
  bigNumInit(&b,88);
  strToBigNum(a, "-123456778122345566123345", 10);
  strToBigNum(b, "-12345677812234556632254534", 10);
  char* s = calloc(sizeof(char),100);
  bigNumToStr(a,s,2);// -11010001001001001101011111000011000000011101010110000010000110011010101010001
  printf("%s\n", s); 
  bigNumToStr(a,s,8); // -32111153703003526020632521
  printf("%s\n", s);
  bigNumToStr(a,s,10); // -123456778122345566123345
  printf("%s\n", s);
  bigNumToStr(a,s,16); // -1a249af8603ab0433551
  printf("%s\n", s);
  printf("%s\n", s);
  strToBigNum(b, "-11010001001001001101011111000011000000011101010110000010000110011010101010001", 2);
  bigNumToStr(b,s,10);
  printf("%s\n", s); // -123456778122345566123345
  strToBigNum(b, "-32111153703003526020632521", 8);
  bigNumToStr(b,s,10); // -123456778122345566123345
  printf("%s\n", s);
  strToBigNum(b, "-123456778122345566123345", 10);
  bigNumToStr(b,s,10); // -123456778122345566123345
  printf("%s\n", s);
  strToBigNum(b, "-1a249af8603ab0433551", 16);
  
  BigNum* x1;
  BigNum* x2;
  BigNum* x3;
  BigNum* x4;
  bigNumInit(&x1,5);
  bigNumInit(&x2,5);
  bigNumInit(&x3,10);
  bigNumInit(&x4,10);
  strToBigNum(x1, "-340282366920938463463374607431768211455", 10);
  strToBigNum(x2, "-340282366920938463463374607431768211457", 10);
  strToBigNum(x4, "7", 10);
  bigNumMultiply(x1,x2,x3);
  bigNumToStr(x3,s,10); // 115792089237316195423570985008687907853269984665640564039457584007913129639935
  printf("%s\n", s);
  bigNumToStr(x3,s,16); // ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff
  printf("%s\n", s);
  bigNumAdd(x1,x2,x3);
  bigNumToStr(x3,s,10); // -680564733841876926926749214863536422912
  printf("%s\n", s);
  bigNumSubtract(x1,x2,x3);
  bigNumToStr(x3,s,10);
  printf("%s\n", s); // 2

  strToBigNum(x1, "340282366920938463463374607431768211461", 10);
  bigNumAbsSubtract(x1, x4, x3);
  bigNumToStr(x3,s,10);
  printf("%s\n", s); // 340282366920938463463374607431768211454

  BigNum *fac100;
  BigNum *bigNumI;

  // factorial 100 (i.e. 100!) = 93326215443944152681699238856266700490715968264381621468592963895217599993229915608941463976156518286253697920827223758251185210916864000000000000000000000000
  bigNumInit(&fac100, 1000);
  int32ToBigNum(fac100, 1);
  bigNumInit(&bigNumI, 1);
  char* stringfac100 = malloc(sizeof(char)* 1000);

  for (int i=1;i<=100;i++){
    int32ToBigNum(bigNumI,i);
    BigNum *bigNumTemp;
    bigNumInit(&bigNumTemp, 1000);
    bigNumMultiply(fac100,bigNumI,bigNumTemp);
    bigNumCopy(bigNumTemp, fac100);
    bigNumFree(bigNumTemp);
  }

  bigNumToStr(fac100, stringfac100, 10);

  printf(stringfac100, "%s");

  strToBigNum(x1, "432342134213421948921303840923289032829489328", 10);
  strToBigNum(x2, "-12341234134913489343241234143231", 10);

  printf("=====");
  bigNumDivide(x1,x2,x3,x4);
  bigNumToStr(x3,s,10); // 35032325737206
  printf(s, "%s"); 
    printf("------餘數");
  bigNumToStr(x4,s,10);
  printf(s, "%s"); // 6891701806548243026832959736742
  printf("-----乘法");
  bigNumMultiply(x1,x2,x3);
  bigNumToStr(x3,s,10); // 5335635504716032129210531763619338947800253085209396735089431993751337938768
  printf(s, "%s");
  printf("------減法");
  bigNumSubtract(x1,x2,x3); // 432342134213409607687168927433945791595346097
  bigNumToStr(x3,s,10);
  printf(s, "%s");
  printf("------加法");
  bigNumAdd(x1,x2,x3); // 432342134213434290155438754412632274063632559
  bigNumToStr(x3,s,10);
  printf(s, "%s");

  printf("=====");
  bigNumDivide(x2,x1,x3,x4);
  bigNumToStr(x3,s,10); // 0
  printf(s, "%s");
  printf("------餘數");
  bigNumToStr(x4,s,10);
  printf(s, "%s"); // 12341234134913489343241234143231
  printf("-----乘法");
  bigNumMultiply(x2,x1,x3);
  bigNumToStr(x3,s,10); // 5335635504716032129210531763619338947800253085209396735089431993751337938768
  printf(s, "%s");
  printf("------減法");
  bigNumSubtract(x2,x1,x3); // -432342134213409607687168927433945791595346097
  bigNumToStr(x3,s,10);
  printf(s, "%s");
  printf("------加法");
  bigNumAdd(x2,x1,x3); // 432342134213434290155438754412632274063632559
  bigNumToStr(x3,s,10);
  printf(s, "%s");
}
