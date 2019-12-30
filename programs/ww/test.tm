; 2019/11/08
; program: decide language L = { ww | w ∈ {a, b}* }, ouputs True/False

; runtime resource limit
#SPACE = 100
#TIME = 3000

; symbol set 
#T = { _, a, b, T, r, u, e, F, a, l, s, <, >, . }

; input symbol set
#S = { a, b }

; the specific blank symbol
#B = _

; state set
#Q = { accept, accept2, accept3, accept4, elim, elim_a, elim_b, halt, l_clr_rej, l_lsig_l, l_sig_loc, l_sig_put, l_sig_swp, l_sig_swp_a, l_sig_swp_b, l_str_end, len_even, len_odd, r, r_clr_acc, r_end_a, r_end_b, r_sig_loc, r_sig_swp, r_sig_swp_a, r_sig_swp_b, reject, reject2, reject3, reject4, reject5 }

; init state
#q0 = len_even

; final state set
#F = { halt }

; the transition functions

; basic idea:
;   0. scan to assure length is even, otherwise reject
;   1. put right/left boundary signs '<' and '>'
;   2. alternatively move the right '<' leftward and move the left '>' rightward, until they meet
;   3. got the mid-point, and the left string has equal chars with right string, or one char longer
;   4. eliminate the string at right side based on string at left side
;   5. if not match, clearup and reject, otherwise clearup and accept

; test length even state
len_even _ < l l_sig_put  ; put down right boundary sign
len_even * * r len_odd

; test length odd state
len_odd _ * l l_clr_rej   ; reject if odd length 
len_odd * * r len_even

; leftward clear then reject
l_clr_rej _ * * reject
l_clr_rej * _ l l_clr_rej

; put down left boundary sign
l_sig_put _ > r r_sig_loc   ; find right sig for swap
l_sig_put * * l l_sig_put

; locate right sign
r_sig_loc < * l r_sig_swp
r_sig_loc * * r r_sig_loc

; determine sym for swap
r_sig_swp a < r r_sig_swp_a
r_sig_swp b < r r_sig_swp_b
r_sig_swp > * r l_lsig_l     ; mid-point found, goto left of left symbol

; swap r_sig: '_<' -> '<_'
r_sig_swp_a * a l l_sig_loc ; find left sig for swap
r_sig_swp_b * b l l_sig_loc

; locate left sign
l_sig_loc > * r l_sig_swp
l_sig_loc * * l l_sig_loc

; determine sym for swap
l_sig_swp a > l l_sig_swp_a
l_sig_swp b > l l_sig_swp_b

; swap l_sig: '>_' -> '_>'
l_sig_swp_a * a l r_sig_loc ; find right sig for swap
l_sig_swp_b * b l r_sig_loc

; locate end of left string
l_lsig_l > * l l_str_end
l_lsig_l * * l l_lsig_l

; goto end of leaft string
l_str_end . * l l_str_end
l_str_end * * * elim

; decide what sym to elim
elim a . r r_end_a    ; padding with .
elim b . r r_end_b
elim _ * r r_clr_acc  ; rightward clear and accept

; goto end of rightmost aming for a
r_end_a _ * l elim_a
r_end_a * * r r_end_a

; goto end of rightmost aming for b
r_end_b _ * l elim_b
r_end_b * * r r_end_b

; elim end of right string
elim_a a _ l l_lsig_l   ; recursively elim, so find left string end again
elim_a * _ l l_clr_rej  ; reject if mismacth
elim_b b _ l l_lsig_l
elim_b * _ l l_clr_rej

; rightward clear then accept
r_clr_acc _ * * accept  ; ACCEPT
r_clr_acc * _ r r_clr_acc

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