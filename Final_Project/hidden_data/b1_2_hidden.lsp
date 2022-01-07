(define min
  (fun (a b)
    (if (< a b) a b)))

(define max
  (fun (a b)
    (if (> a b) a b)))

(define gcd
  (fun (a b)
    (if (= 0 (mod (max a b) (min a b)))
        (min a b)
        (gcd (min a b) (mod (max a b) (min a b))))))

(print-num (gcd 15 35))

(print-num (gcd 448 189))

(print-num (gcd 330 715))

