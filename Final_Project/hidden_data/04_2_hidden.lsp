(print-bool (or #t (and #t #f) (not #t)))
(print-bool (and #t (not #t) (or #t #f) (and #f (not #f))))
(print-bool (or #f #f #t))

