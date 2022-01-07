(define dist-square
  (fun (x y)
    (define square (fun (x) (* x x)))
    (+ (square x) (square y))))

(print-num (dist-square 6 10))

