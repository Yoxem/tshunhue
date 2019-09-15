(define x p_int 12)
;(define y int 12)

(define add1 (-> int int) (lambda (a) (%+ a 1)) ; OK

(define add2 (-> int int) (lambda (a) (%+ x 1)) ; OK

(define add3 (-> int int) (lambda (x) (%+ x 1)) ; OK

(define add4 (-> int int) (lambda (x) (lambda (y) (%+ x y))) ; OK

(define add5 (-> int int) (lambda ((x int)) (define x int 12) (lambda ((y int)) (%+ x y))) ; Error

(define add6 (-> int int) (lambda ((y int)) (lambda ((z int)) (%+ x y))) ; OK

(define add7 (-> int int) (lambda ((y int)) (lambda ((z int)) (define y int 8) (%+ x y))) ; OK

(define add8 (-> int int) (lambda ((y int))
                            (lambda ((z int))
                                (%p y)
                                (define y int 8)
                                (%+ x y))) ; Error




;(%+ 3 4)
;(define add1 (-> int int) (lambda ((x int)) (%+ x 1)))

;(define alt_add (-> int (-> int int)) (lambda (x) (lambda (y) (%+ x y))))