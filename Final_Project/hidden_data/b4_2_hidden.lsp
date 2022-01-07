(define foo
  (fun (f x) (f x)))

(print-num
  (foo (fun (x) (- x 1)) 11))

