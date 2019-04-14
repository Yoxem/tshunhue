#include <stdbool.h>

typedef enum {INT, DOUBLE, CHAR} simple_type_id;

char* simple_type_list[] = {"INT", "DOUBLE", "CHAR"};

union simple_type{
  int integer;
  double double_num;
  char ch;
};

typedef struct variable_type{
  simple_type_id id;
  union simple_type simple_type;
} VarType;

int main() {
  VarType a;
  a.id = 1;
  a.simple_type.double_num = 0.789;
  
  return 0;
}
