; 2019/11/26
; program: decide language L = { 0^n | n ∈ Fibonacci }, ouputs True/False

; runtime resource limit
#SPACE = 100
#TIME = 3000

; basic idea:
;   0. use three working tapes to store fib(n), fib(n-1) and fib(n-2)
;   1. compare tape[0] with tape[1], reject if not enough 0 (aka. tape[1] is longer), accept if equal
;   2. calc next fib(n): `tape[3] = tape[2]; tape[2] = tape[1]; tape[1] = tape[2] + tape[3]`

; init and state shift
init **** *00* **** cmp         ; init fib(1) = 1, fib(0) = 1

; compare tape0 with tape1
cmp __** **** ll** l_clr_acc    ; accept
cmp _0** _*** ll** l_clr_rej    ; reject, 'cos not enough 0
cmp 0_** **** ll** 01l_end      ; tape0 has more 0, reset
cmp 00** **** rr** cmp

; tape0/1 goto leftmost
01l_end __** **** rr** cp_3_2   ; calc next fib(n)
01l_end _*** **** *l** 01l_end
01l_end *_** **** l*** 01l_end
01l_end **** **** ll** 01l_end

; copy tape2 to tape3
cp_3_2 **_* **** **ll 23l_end
cp_3_2 **0* ***0 **rr cp_3_2

; tape2/3 goto leftmost
23l_end **__ **** **rr cp_1_2
23l_end **_* **** ***l 23l_end
23l_end ***_ **** **l* 23l_end
23l_end **** **** **ll 23l_end

; copy tape1 to tape2
cp_1_2 *_** **** *ll* 12l_end
cp_1_2 *0** **0* *rr* cp_1_2

; tape1/2 goto leftmost
12l_end *__* **** *rr* cp_2_1
12l_end *_** **** **l* 12l_end
12l_end **_* **** *l** 12l_end
12l_end **** **** *ll* 12l_end

; add tape2 + tape3 -> tape1
cp_2_1 **_* **** **l* cp_3_1
cp_2_1 **0* *0** *rr* cp_2_1
cp_3_1 ***_ **** *l*l 123l_end
cp_3_1 ***0 *0** *r*r cp_3_1

; tape1/2/3 goto leftmost
123l_end *___ **** *rrr cmp      ; test again
123l_end *__* **** ***l 123l_end
123l_end **__ **** *l** 123l_end
123l_end *_*_ **** **l* 123l_end
123l_end **** **** *lll 123l_end

; clear till leftend then accept
l_clr_acc _*** **** r*** accept
l_clr_acc **** _*** l*** l_clr_acc

; clear till leftend then reject
l_clr_rej _*** **** r*** reject
l_clr_rej **** _*** l*** l_clr_rej

; write True then halt
accept  **** T*** r*** accept2
accept2 **** r*** r*** accept3
accept3 **** u*** r*** accept4
accept4 **** e*** **** halt

; write False then halt
reject  **** F*** r*** reject2
reject2 **** a*** r*** reject3
reject3 **** l*** r*** reject4
reject4 **** s*** r*** reject5
reject5 **** e*** **** halt
