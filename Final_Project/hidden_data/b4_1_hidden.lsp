(define add-x
  (fun (x) (fun (y) (+ x y))))

(define z (add-x 10))

(print-num (z 5))

