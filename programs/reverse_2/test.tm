; 2019/11/26
; program: reverse a binary string

; state shift
init ** ** ** r1_end

; head 1 goto rightmost
r1_end _* ** l* revcopy
r1_end ** ** r* r1_end

; head 1 left, head 2 right, copy reversely
revcopy __ ** rl l2_end
revcopy 0* *0 lr revcopy
revcopy 1* *1 lr revcopy

; head 2 goto leftmost
l2_end *_ ** *r copy
l2_end ** ** *l l2_end

; head 1 right, head 2 right, copy
copy __ ** ** halt
copy *0 0* rr copy
copy *1 1* rr copy
