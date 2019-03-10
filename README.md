春花 Tshunhue
=============
這是一個叫做春花的程式語言（玩具語言）的考案。

語法
-----------
      int a = 10; // 變數宣告需要指定型別
      int b = 23;
      bool s =  a > b; // 布爾運算
      print("%d",a); // 印出字串

      /*函數定義*/
      add : forall A in num, (A, A -> A);
      add = lambda(a, b){return a + b;};
      /*函數重載*/
      add2 : (int, int -> int);
      add2 = lambda(a, b){return a + b;};
      add2 : (float, float -> float);
      add2 = lambda(a, b)
                    {return a + b; // 之後的指令不執行
                     a + 1; 
                     };

## BNF 表示法
（待補完）

## Parser：
* [70 行 Python 代码编写一个递归下降解析器](https://www.oschina.net/translate/how-to-write-a-calculator-in-70-python-lines-by-writing-a-recursive-descent-parser?print)
* [遞迴下降 OR 遞歸下降 Python - Google 搜尋](https://www.google.com/search?client=firefox-b-ab&ei=V9jMW4L7HceB8gXj6JGoDQ&q=%E9%81%9E%E8%BF%B4%E4%B8%8B%E9%99%8D+OR+%E9%81%9E%E6%AD%B8%E4%B8%8B%E9%99%8D+Python&oq=%E9%81%9E%E8%BF%B4%E4%B8%8B%E9%99%8D+OR+%E9%81%9E%E6%AD%B8%E4%B8%8B%E9%99%8D+Python&gs_l=psy-ab.3...3677.3677.0.3855.1.1.0.0.0.0.47.47.1.1.0....0...1c.1.64.psy-ab..0.0.0....0.SD09rz1x3RA)

實做方式
------------
用 Python3 將原始碼轉成 AST，以 s-expression 表示。再進行型別推測後，轉成 CPS 風格（問題：需不需要第二次型別轉換呢？），利用 llvmlite 生成 LLVM IR，最後使用 。

需要的東西：

* llvmlite
* python3


型別
---------
待實做的型別：

* any: 任何型
    * num: 數字
        * int：整數型，signed i64。
        * float：浮點數，double presision。
   * bool：布爾，i1。
   * void：不傳回或輸入值，或是代表函數沒有自變數（如 `(void -> void)`）。
   * fun：函數。可以用 `(any -> Any)`, `(num, num -> bool)`, `(int -> void)`, `forall A, B, C in num, (A, B -> C)`來表示。[1]
   * (union A B) A B 型別的聯集（以後再實做）。
（沒問題以後再實做 char、list、struct、typedef）

[1]： 註：(num, num -> bool)可轉為，forall A, B in num, A, B -> bool


### 型別推測
可以用邏輯推論引擎的方式取得。但也可以簡單一點。
把 `(TypeA, Type B -> TypeC)` 轉成：
`forall A in TypeA, forall B in TypeB -> TypeC`。
確認 TypeC 不能夠是多於一個型別的母集合後（初期，之後可以改變），
然後再試圖合一 (unification)，如果確認可以綁定如：
`{A : "subtype_of_typeA", B: "subtype_of_typeB"}`
，回傳 `TypeC`。

`forall A in TypeA, forall B in TypeB -> B`則回傳 `subtype_of_typeB`。

（以下採用 scheme 表示法）

`(int a 12)`

轉為

`(define [a . int] [12 . int])`；

`(+ 12 (- a b))`

轉為

`[(+ [12 . int] [(- [a . int] [b . int]) . int]) . int]`

`(add3 3.0 2)`

    // (forall A, B in num, (A, B -> A)) = ([(forall_in A num) (forall_in B num)] (-> A B A))
    [([add3 . ([(forall_in A num) (forall_in B num)] (-> A B A))] [3.0 . float] [2 . int]) . int]

或許可以用邏輯推論機器實做，要自己做一個類似的機器，可以參考以下：

* [Prover9 - Wikipedia](https://en.wikipedia.org/wiki/Prover9)
* [Conjunctive_normal_form - Wikipedia](https://en.wikipedia.org/wiki/Conjunctive_normal_form#Conversion_into_CNF)
* [歸結原理 - Wikipedia](https://zh.wikipedia.org/wiki/%E5%BD%92%E7%BB%93%E5%8E%9F%E7%90%86)
* [NLTK Logic 1 : First Order Logic and Semantics - Mark Chang's Blog](https://ckmarkoh.github.io/blog/2014/03/13/python-nltk-logic-1/)
* [謂詞邏輯、一階邏輯與「哥德爾完備定理」](http://programmermagazine.github.io/201403/htm/focus3.html)
* [Prolog 入門 (作者： KuoE0)](https://programmermagazine.github.io/201308/htm/article3.html#prolog-%E5%85%A5%E9%96%80-%E4%BD%9C%E8%80%85-kuoe0)
* [布林邏輯與公理系統 - 把數學程式化 -- 使用 JavaScript 的 Rlab 套件](https://ccckmit.gitbooks.io/rlab/content/axiom.html)

### 閉包
可以用接收 scope 外變數的物件加上自己的方法來模擬。

    A a = 10
    B b = 12
    d = closure(C c){return a+b+c}
    d(13)
則為

    a = 10
    b = 12
    c = struct_A_B_C_1(A a: 10, B b: 12);
    def fun_struct_A_B_C_1(struct_A_B_C_1 self, C c):
        return self.a + self.b + c
    call(fun_struct_A_B_C_1, c, 13)

垃圾回收
------------------
採用引用計數法，使用不可變物件的概念，使用 table 紀錄位址引用次數。

函數區塊 call variable 則 count + 1；函數區塊執行結束則 count - 1。變數指定的位址改變時，`a = count[old_ptr]; count[old_ptr] = 0; count[new_ptr] = a`。

LLVM IR 生成
--------------------
未來可能用到，雖然初期應該是先建立 C code。 https://github.com/bytedeco/javacpp-presets/tree/master/llvm