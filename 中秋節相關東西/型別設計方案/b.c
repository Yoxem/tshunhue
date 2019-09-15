#include <stdio.h>

// 非函數基本型別包含整數、浮點數和字串

typedef union basic_type {
    long int i;
    double f;
    char* str;
} BasicType; 

typedef struct closure Closure;

typedef struct object Object;

// 閉包的函數型態，傳入閉包，回傳物件
typedef Object (*ClosureFunc)(Closure, Object*);

// 閉包是自由變數和閉包函數的集合
typedef struct closure {
    Object* free_var;
    ClosureFunc function_thunk;
} Closure;

// 定義任何物件。
typedef struct object{
    char* type;
    union{
        struct object* list_of_obj;
        BasicType b;
        Closure c;
    } content;
} Object;

Object plus(Closure closure, Object* l){
    Object d;
    d.type = "i";
    d.content.b.i = l[0].content.b.i + l[1].content.b.i;
    return d;
};


int main(void){
    Object a;
    a.type = "i";
    a.content.b.i = 6789;
    Object b;
    b.type = "i";
    b.content.b.i = 6;
	
	Object list[2];
	list[0] = a;
	list[1] = b;
    Object clo;
    clo.type = "i2i";
    clo.content.c.function_thunk = plus;

    Object d = clo.content.c.function_thunk(clo.content.c, list);
    printf("%ld", d.content.b.i);
    return 0;
}
