; 2019/12/02
; program: decide language L = { 0^n1^n2^n | n ∈ N }, ouputs True/False

; runtime resource limit
#SPACE = 100
#TIME = 3000

; basic idea:
;   0. scan from left to right, emit one 0, one 1 and one 2, then return leftmost
;   1. repeat step 0 until no 0 left, check if nothing left then accept, otherwise reject

; state shift
init _ * * accept
init * * * emit0

; emit one 0
emit0 0 _ r emit1   ; emit 0
emit0 . _ r r_chk_pad  ; accept if right is seq of .
emit0 * * * lr_clr_rej

; emit one 1
emit1 1 . r emit2   ; replace with padding .
emit1 0 * r emit1   ; skip seq of 0
emit1 . * r emit1   ; skip seq of .
emit1 * * * lr_clr_rej

; emit one 2
emit2 2 . r r_end   ; replace with padding .
emit2 1 * r emit2   ; skip seq of 1
emit2 . * r emit2   ; skip seq of .
emit2 * * * lr_clr_rej

; goto rightmost, assuring seq of 2
r_end _ * l l_end
r_end 2 * r r_end   ; skip seq of 2
r_end * * * lr_clr_rej

; goto leftmost
l_end _ * r emit0   ; next round
l_end * * l l_end

; rightward check if empty
r_chk_pad _ * * accept
r_chk_pad . _ r r_chk_pad      ; clear .
r_chk_pad * _ * lr_clr_rej

; r_clr then l_clr then rej
lr_clr_rej _ * r r_clr_rej
lr_clr_rej * . l lr_clr_rej    ; dummy . later clear up
r_clr_rej  _ * * reject
r_clr_rej  * _ r r_clr_rej     ; clear .

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
