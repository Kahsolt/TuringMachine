; 2019/11/26
; program: add two binary numbers

; basic idea:
;   0. revcopy y to tape[2], revcopy x to tape[1]
;   1. add x + y to tape[0], write form right to left

; and state shift
init *** *** r** r_end

; goto rightmost0
r_end _** *** l** rcp_y
r_end *** *** r** r_end

; revcopy y: tape[0] -> tape[2]
rcp_y +** _** l*l rcp_x
rcp_y 0** _*0 l*r rcp_y
rcp_y 1** _*1 l*r rcp_y

; revcopy x: tape[0] -> tape[1]
rcp_x _** *** rl* 12l_end
rcp_x 0** _0* lr* rcp_x
rcp_x 1** _1* lr* rcp_x

; tape1/2 goto leftmost
12l_end *__ *** *rr add
12l_end *_* *** **l 12l_end
12l_end **_ *** *l* 12l_end
12l_end *** *** *ll 12l_end

; add x + y ->
add *__ _** *** halt
add *00 0** lrr add
add *10 1** lrr add
add *01 1** lrr add
add *11 0** lrr add_cry
add *_1 1** l*r add
add *1_ 1** lr* add
add *_0 0** l*r add
add *0_ 0** lr* add

add_cry *__ 1** *** add
add_cry *00 1** lrr add
add_cry *10 0** lrr add_cry
add_cry *01 0** lrr add_cry
add_cry *11 1** lrr add_cry
add_cry *_1 0** l*r add_cry
add_cry *1_ 0** lr* add_cry
add_cry *_0 1** l*r add
add_cry *0_ 1** lr* add
