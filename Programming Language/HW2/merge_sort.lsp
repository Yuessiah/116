(declaim #+sbcl(sb-ext:muffle-conditions warning))

(defconstant maxn 10000)

(setf a (make-array maxn))
(setf b (make-array maxn))

(setq n (read))
(dotimes (i n) (setf (aref a i) (read)))

(defun mergesort(l r)
  (if (<= (- r l) 1)
    (return-from mergesort 'done)
    (progn
      (let
        ((m (+ l (floor (- r l) 2))))
        (progn
          (mergesort l m)
          (mergesort m r)

          (let
            ((p l) (q m) (k l))
            (progn
              (dotimes
                (i (- r l))
                (setf (aref b (+ i l)) (aref a (+ i l))))

              (loop
                (when (>= k r) (return-from mergesort 'done))
                (if (or (equal q r) (and (/= p m) (<= (aref b p) (aref b q))))
                  (progn
                    (setf (aref a k) (aref b p))
                    (setq p (+ p 1)))
                  (progn
                    (setf (aref a k) (aref b q))
                    (setq q (+ q 1))))
                (setq k (+ k 1)))))))))) 

(mergesort 0 n)
(dotimes (i n) (princ (aref a i)) (princ #\Space))
