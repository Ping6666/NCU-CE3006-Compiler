(define fact
  (fun (n) (if (< n 3) n
               (* n (fact (- n 1))))))

(print-num (fact 11))
(print-num (fact 12))
(print-num (fact 13))
(print-num (fact 14))

(define fib (fun (x)
  (if (< x 2) x (+
                 (fib (- x 1))
                 (fib (- x 2))))))

(print-num (fib 2))
(print-num (fib 4))
(print-num (fib 6))
(print-num (fib 11))
(print-num (fib 21))

