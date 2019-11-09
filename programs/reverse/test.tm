; 2019/11/08
; program: reverse a binary string

; runtime resource limit
#SPACE = 100
#TIME = 3000

; symbol set 
#T = { _, 0, 1, >, | }

; input symbol set
#S = { 0, 1 }

; the specific blank symbol
#B = _

; state set
#Q = { left_end, right_end, left_sep, find_bit, carry_0, carry_1, put_0, put_1, clear_sep, halt }

; init state
#q0 = left_end

; final state set
#F = { halt }

; transition function rules

; goto left end and write left-end >
left_end _ > r right_end
left_end * * l left_end

; goto right end and write seperator |
right_end _ | l find_bit
right_end * * r right_end

; across sep from right to left
left_sep | * l find_bit
left_sep * * l left_sep

; search from right to left to find a bit to carry
find_bit 0 _ r carry_0
find_bit 1 _ r carry_1
find_bit > _ r clear_sep
find_bit * * l find_bit

; across sep from left to right
carry_0 | * r put_0
carry_0 * * r carry_0

; put the 0 carried
put_0 _ 0 l left_sep
put_0 * * r put_0

; across sep from left to right
carry_1 | * r put_1
carry_1 * * r carry_1

; put the 1 carried
put_1 _ 1 l left_sep
put_1 * * r put_1

; clear right sep then halt
clear_sep | _ r halt
clear_sep * * r clear_sep
