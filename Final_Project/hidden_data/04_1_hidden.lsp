(print-bool #f)
(print-bool #t)

(print-bool (or #f #f))
(print-bool (or #t #f))

(print-bool (not #f))
(print-bool (not #t))

(print-bool (and #t #t))
(print-bool (and #t #f))

