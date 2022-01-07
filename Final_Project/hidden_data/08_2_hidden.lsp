(define foo (fun (y) (+ y 1)))

(define foo-z (fun () 5))

(print-num (foo (foo-z)))

