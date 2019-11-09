; 2019/11/8
; program: increase by 1, simplest demo

; state shift
init * * * r_end

; goto rightmost
r_end _ * l incr
r_end * * r r_end

; do incr on the last bit
incr _ * * halt         ; do nothing on empty number
incr 0 1 * halt
incr 1 0 l incr_carry   ; recursively carry on

; carry on
incr_carry 0 1 * halt
incr_carry _ 1 * halt   ; new highest bit
incr_carry 1 0 l incr_carry ; recursively