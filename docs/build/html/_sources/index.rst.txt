.. Tshunhue Documents documentation master file, created by
   sphinx-quickstart on Tue Sep 24 21:40:29 2019.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Tshunhue Documents's documentation!
==============================================

.. toctree::
   :maxdepth: 3
   :caption: Contents
    
   基本函數介紹
   型別


Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


其他
===================

* 垃圾回收採用引用記數法 `./GarbageCollection/`，用哈希表紀錄，待完成。

採用引用計數法，使用不可變物件的概念，使用 table 紀錄位址引用次數。

函數區塊 call variable 則 count + 1；函數區塊執行結束則 count - 1。變數指定的位址改變時，`a = count[old_ptr]; count[old_ptr] = 0; count[new_ptr] = a`。


* 型別判定使用合一演算法
* 遞迴下降分析
* llvmlite 輸出 LLVM IR。

實做方式
------------
用 Python3 將原始碼轉成 AST，以 s-expression 表示。再進行型別推測後，進行閉包轉換，不進行 CPS 變換，利用 llvmlite 生成 LLVM IR。

需要的東西：

* llvmlite
* python3

