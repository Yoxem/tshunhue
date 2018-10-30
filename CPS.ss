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
       (cps (list-ref expr 2) k1)))

    ((eq? (car expr) 'if)
    (let*
      (
        (r1 (gen-new-symbol))
        (e1 (list-ref expr 1))
        (e2 (list-ref expr 2))
        (e3 (list-ref expr 3))
        (e2-c (cps e2 k))
        (e3-c (cps e3 k))
        (e1-c `(lambda (,r1) (if ,r1 ,e2-c ,e3-c))))
      (cps e1 e1-c)))


    ((eq? (car expr) 'begin)
    (cond
      ;; (begin) = Error
      ((= (length expr) 1)
      (error "begin must have its argument"))
      
      ;; (begin E0) = E0
      ((= (length expr) 2)
        (cps (cadr expr) k)) 
      
      ;; (begin E0 E1)
      ((= (length expr) 3)
      (let*
        ((r1 (gen-new-symbol))
        (e0 (list-ref expr 1))
        (e1 (list-ref expr 2))
        (e0-c `(lambda (,r1) ,(cps e1 k))))
        (cps e0 e0-c)
        ))
      ;; (begin E0 En ...) = (begin E0 (begin En ...))
      (else
        (let*
          ((begin-tail (cons 'begin (cddr expr)))  ;; (begin En ...)
          (e0 (cadr expr))
          ;; (begin E0 En...) = (begin E0 (begin En ...)))
          (new-begin (list 'begin e0 begin-tail)))
          (cps new-begin k)
      ))
))))

(define id (lambda (v) v))
(cps '(set! a (+ 7 8)) 'id)
(cps '(+ 1 2) 'id)
(cps '(if (+ 0 1) (+ 1 2) 2) 'id)
(cps '(begin (+ 79 89) (- (* 96 78) 95) (/ 687 77)) 'id)