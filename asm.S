#include "flags.h"
	.section .text
	.global top_head

	.global here
here:
	.ascii "<###"
	.global RETX_0
	.global RETX_1
RETX_0:
RETX_1:
	ret

.macro FLASH_PTR addr
	.word \addr
	.byte 0x00
.endm
.macro DEFWORD lbl, attr, name, codeword
	.global \lbl
	.global \lbl\()_head
\lbl\():
\lbl\()_head:
	FLASH_PTR 1b-3	; prev header
1:
	.byte  \attr	; attributes
	.byte len\@ 	; name len 
	name_start_\@:
	.ascii "\name"				; name without \0
	.equ len\@,(. - name_start_\@)
	
	.global \lbl\()_cw
\lbl\()_cw:
	.extern \codeword
	FLASH_PTR \codeword				; 3B address of function in FLASH
	.global \lbl\()_data
\lbl\()_data:
	; more payload may be outside macro
.endm	;

;;;; trick for STOP value for search: 0.header with none data
	.byte 0,0,0 ; prev header none
1:
	.byte 0+FLG_HIDDEN ;len 0 + FLG_HIDDEN = impossible to find
	.word 0		; emty codeword

;;;; word definitions

DEFWORD w_dup, 0, "DUP", f_dup
DEFWORD w_plus,0, "+", f_plus
DEFWORD w_double, 0, "DOUBLE", f_docol
	FLASH_PTR w_dup_cw
	FLASH_PTR w_plus_cw
	FLASH_PTR w_exit_cw
DEFWORD w_trouble, FLG_HIDDEN + FLG_IMMEDIATE, "TROUBLE", f_plus
DEFWORD w_debug, 0, "DEBUG", f_debug	; primitivum, kde se opet zmocnim vlady
DEFWORD w_exit, 0, "EXIT", f_exit
DEFWORD w_test, 0, "TEST", f_docol
	FLASH_PTR w_double_cw
	FLASH_PTR w_debug_cw
	FLASH_PTR w_exit_cw

	.EQU	top_head,1b-3

.balign 2
	.ascii "###>"
	.global end
end:
