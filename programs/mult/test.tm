; 2019/11/08
; program: decide language L = { 1^n x 1^m = 1^(n+m) | n, m ∈ N+ }, ouputs True/False
;          e.g. `11x111=111111`, `1x1=11`

; runtime resource limit
#SPACE = 100
#TIME = 3000

; symbol set
; dollar sign $ used to mark which one '1' is functioning in eliminating
#T = { _, 1, x, =, $, T, r, u, e, F, a, l, s }

; input symbol set
#S = { 1, x, = }

; the specific blank symbol
#B = _

; state set
#Q = { accept, accept2, accept3, accept4, chk_1, elim, elim_or_stop, halt, l_clr_rej, l_end, mark_bit, r, r_clr_chk, r_clr_rej, r_end, r_mult_sig, reject, reject2, reject3, reject4, reject5, to_mark, try_elim }

; init state
#q0 = try_elim

; final state set
#F = { halt }

; the transition functions

; basic idea: 
;   for equation n x m = n*m, do n epochs of elimination, on each epochs,
;   eliminate m 1's at rightmost side of the string, eventually:
;     1. check if there's remaining 1's on the right of '='
;     2. or there lacks 1's right during elimination

; if we still have epochs to eliminate
try_elim 1 _ r r_mult_sig     ; start one epoch, count - 1
try_elim x _ r r_clr_chk      ; all epochs done, clear right till '=', then check if remaining 1's

; goto rightside of sign 'x', begin elimination
r_mult_sig x * r mark_bit
r_mult_sig 1 * r r_mult_sig

; mark the bit with $, then goto the rightmost
mark_bit 1 $ r r_end
mark_bit = * l l_end    ; done one epoch, recursively do next

; goto right end, try elim one '1'
r_end _ * l elim
r_end * * r r_end

; elim one 1 and back to makr
elim 1 _ l to_mark
elim = _ l l_clr_rej    ; lack of 1's, clear and reject

; restore mark, then recursively mark next bit
to_mark $ 1 r mark_bit
to_mark * * l to_mark

; clear till leftmost then reject
l_clr_rej _ * * reject
l_clr_rej * _ l l_clr_rej

; goto left end, recursively try next try_elim
l_end _ * r try_elim
l_end * * l l_end

; clear till '=' then check 1
r_clr_chk = _ r chk_1
r_clr_chk * _ r r_clr_chk

; check remaining 1
chk_1 1 _ r r_clr_rej   ; remaining 1 found, clear till right most and reject
chk_1 _ _ * accept      ; ACCEPT!

; clear till rightmost then reject
r_clr_rej _ * * reject
r_clr_rej * _ l r_clr_rej

; write True then halt
accept  * T r accept2
accept2 * r r accept3
accept3 * u r accept4
accept4 * e * halt

; write False then halt
reject  * F r reject2
reject2 * a r reject3
reject3 * l r reject4
reject4 * s r reject5
reject5 * e * halt
