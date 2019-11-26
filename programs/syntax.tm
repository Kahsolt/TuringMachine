; This is a sample TM definition file
; as you can see, comma marks a line comment
;   by Armit - 2019/11/8
;

; definition item names starts with '#', namely they are:
;   #N =        number of tapes; default: 1
;   #B =        blank cell symbol; default: '_'
;   #T = { }    symbol set, alias is '#G', implict containing '#B'
;   #S = { }    symbol set of input data, input will be validated firstly if this is defined
;   #Q = { }    state set
;   #q = { }    init state, alias is '#q0'; default: 'init' 
;   #F = { }    halt state set; default: '{ halt }'
;   #TIME =     time limit (aka. transition step counts); default: see [config.h](/src/config.h)
;   #SPACE =    space limit (aka. cell count of the tape which is accessed); default: see [config.h](/src/config.h)
; these items are **NOT** neccessary since they have default values 
; or they could be infered from transition function rules
; (naming rule: symbols are single literal chars, states are literal strings)
;

#S = { 0, 1 }

; transition functions rules are vital, each of them is defined as a five-tuple:
;   <cur_state> <cur_symbol> <new_symbol> <move_direction> <new_state>
; wildcard '*' could be used for:
;   - cur_symbol: meaning 'for any content of the cell'
;   - new_symbol: meaning 'no overwrite on this cell'
;   - move_direction: meaning just stick here, no movement
;

; here just write some 0 and 1
init 0 1 l wrt_0
init 1 0 r wrt_1

wrt_0 * 0 r init
wrt_1 * 1 * halt

; if you wanna a multi-tape machine, then the transition functions rules will become:
;   <cur_state> <cur_symbols> <new_symbols> <move_directions> <new_state>
; where symbols and directions are given as a continous string without seperators
;

; here we just do a copy
init ** ** ** cpy   ; state shift

cpy _* ** ll halt
cpy 0* *0 rr cpy
cpy 1* *1 rr cpy