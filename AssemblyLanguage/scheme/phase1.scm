;; 要用 nanopass 的表示型態嗎？
;;
;;

(define 64-register-ls '(rax rcx rdx rbx rsp rbp rsi rdi r8 r9 r10 r11 r12 r13 r14 r15))
(define 32-register-ls '(eax ecx ecx edx ebx ebp esi edi))
(define move-cmd-ls '(mov))
(define noary-cmd-ls '(cltd ret))
(define uni-arthimetic-cmd-ls '(mul div idiv))
(define bi-arthimetic-cmd-ls '(add sub mul imul div idiv))

(define (is-in-32-register-ls reg) (if (memq reg 32-register-ls) #t #f))
(define (is-in-64-register-ls reg) (if (memq reg 64-register-ls) #t #f))
(define (is-location loc) (if (or (is-in-32-register-ls loc) (is-in-64-register-ls loc)) #t #f))
(define (is-uniary-operator op) (if (memq op uni-arthimetic-cmd-ls) #t #f))
(define (is-binary-operator op) (if (memq op bi-arthimetic-cmd-ls) #t #f))
(define (is-noary-cmd op) (if (memq op noary-cmd-ls) #t #f))

(load "pmatch.scm")

(define (verify-phase1-line input)
  (pmatch input
      ((mov ,des ,src)
       (and (is-location des) (is-location src)))
      ((,bi-op ,des ,src)
       (and (is-binary-operator bi-op) (is-location des) (is-location src)))
      ((,uni-op ,des)
       (and (is-uniary-operator bi-op) (is-location des)))
      ((,op) (guard (is-noary-cmd op)) #t )
      ))

(verify-phase1-line '(mov eax ecx))
(verify-phase1-line '(cltd))

(define (verify-phase1 input)
  (cond
    (((eq? input '()) #t)
     (#t (and (verify-phase-line (car input))
                (verify-phase (cdr input)))))))

(define (print-assembly-code input)
  (print-assembly-code-line (car input)
  (print-assembly-code (cdr input))))

(define (print-assembly-codeline line)
  (display (car line))
  (cond
    ((= (length line) 2) (display " ")(display (cdar line)))
    ((= (length line) 3) (display " ")(display (cadr line))(display ",")(display (caddr line)))
    (else '())))

(print-assembly-codeline
 '(mov eax edx))