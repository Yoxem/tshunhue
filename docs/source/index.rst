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


Tshunhue 是什麼呢？
===================
Tshunhue（河洛語：春花）是一個 Lisp 系的玩具程式語言。主要的特徵有：

 - 不可變物件。
 - 閉包作為第一類物件。
 - 靜態型別。
 - 和型別 (Sum Type) 和積型別 (Product Type)。


其他
============

垃圾回收
------------

先不實現垃圾回收。若是要實現垃圾回收，可以用引用記數。做一個這樣的表格：

+------------+------------+
| 記憶體位址 |   引用次數 |
+------------+------------+
| 0x12345    | 1          |
|            |            |
| ……         | ……         |
+------------+------------+

以下有一個範例：

::

  (Type IntPair (IntPair Int Int))
  (def IntPair a (IntPair x y))
  
  ; 這個時候我們可以呼叫引用計數增加函數 inc_ref_count()
  ; inc_ref_count(&a);
  ; inc_ref_count(&x);
  ; inc_ref_count(&y);
  ...程式碼
  ; x 活著的程式碼區段結束時，呼叫引用記數減少函數 dec_ref_count()
  ; dec_ref_count(&a);
  ; dec_ref_count(&a[0]); // &x
  ; dec_ref_count(&a[1]); // &y


實做方式
------------
用 Python3 將原始碼轉成 AST，以 s-expression 表示。再進行型別推測後，進行閉包轉換，不進行 CPS 變換，利用 llvmlite 生成 LLVM IR。

需要的東西：

* llvmlite
* python3

