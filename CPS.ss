#lang racket
(define n 0)

(define (gen-new-symbol)
  (let ((new-symbol-string (string-append "n" (number->string n))))
  (set! n (+ n 1))
  (string->symbol new-symbol-string)
))

(define (cps expr k)
  (cond
    ;; indentifier -> (k i)
    ((not (list? expr)) `(,k ,expr))
    ;; + - * /
    ((memq (car expr) '(+ - * /))
     (let* ((r1 (gen-new-symbol))
           (r2 (gen-new-symbol))
           (k1 `(lambda (,r2) (,k (,(car expr) ,r1 ,r2))))
           (k2 `(lambda (,r1) ,(cps (list-ref expr 2) k1))))
           (cps (list-ref expr 1) k2)
       ))
    ((eq? (car expr) 'set!)
     (let* ((r1 (gen-new-symbol))
            (k1 `(lambda (,r1) (,k (set! ,(list-ref expr 1) ,r1)))))
       (cps (list-ref expr 2) k1))
)))

(define id (lambda (v) v))
(cps '(set! a (+ 7 8)) 'id)