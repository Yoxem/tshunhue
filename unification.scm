#lang racket
; From: https://cs.stackexchange.com/questions/35280/can-someone-clarify-this-unification-algorithm?answertab=active#tab-top

(define atom?
  (lambda (x)
    (and (not (pair? x)) (not (null? x)))))

(define (check-symbol-head-pattern char symbol)
  (eq?
   (string-ref (symbol->string symbol) 0) char))

(define (variable? x)
  (and
   (symbol? x)
   (check-symbol-head-pattern #\% x)
))

(define (unify A B) (unify-iter A B '()))


(define (unify-iter A B result)
  (cond
    ((eq? result #f) #f)
    ((eq? A B) result)
    ((variable? A) (unify-item A B result))
    ((variable? B) (unify-item B A result))
    ((and (pair? A) (pair? B)) (unify-iter (cdr A) (cdr B) (unify-iter (car A) (car B) result)))
    (else #f)
  ))

(define (unify-item var x result)
  (let ((val (lookup var result)))
    (if (not (eq? val #f))
       (unify-iter val x result)
       (let ((val (lookup x result)))
          (if (not (eq? val #f))
             (unify-iter var val result)
             (if (occur-check? var x)
                #f
                (append result `((,var . ,x))))))
  )))

(define (lookup x result)
  (cond
    ((eq? result '()) #f)
    ((eq? x (caar result)) (cdar result))
    (else (lookup x (cdr result)))))

(define (occur-check? var exp)
  (cond ((pair? exp) (or (occur-check? var (car exp))
                          (occur-check? var (cdr exp))))
        ((eq? var exp) #t)
        (else #f)))


(unify '(%A %B) '(2 3))
(unify '(%A %B %C) '(3 %C %A))
(unify '(%A %B %C) '(%B %C 3))
(unify '(%A %B %C) '(3 (86 (C %B)) %A))
(unify '(%A %B %C %B) '(3 %C %A 6))
(unify '(%A (%B "example") %C %B) '(3 (%C %D) %A %B))
(unify '(%A (%B ("example" %D)) %C %B) '(3 (%C %D) %A %B))
(unify '(%A (%B ("example" %D 12)) %C %B) '(3 (%C %D) %A %B))