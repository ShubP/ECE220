; partners: , mamir6
;
;
.ORIG x3000
	

READ_NEXT_INPUT
	GETC
	OUT

CHECK_EQUALS
	LD R1, EQUALS
	NOT R1, R1
	ADD R1, R1, #1
	ADD R1, R1, R0
	BRz STACK_CHECK

CHECK_SPACE
	LD R1, SPACE
	NOT R1, R1
	ADD R1, R1, #1
	ADD R1, R1, R0	
	BRz READ_NEXT_INPUT
	JSR EVALUATE
	BR READ_NEXT_INPUT

STACK_CHECK
	LD R5, STACK_DIFF
	LD R6, STACK_TOP
	NOT R5, R5
	ADD R5, R5, #1
	ADD R5, R6, R6
	BRz INVALID ; Branch to invalid message if more than 1 result on stack
	JSR POP
	JSR PRINT_HEX
	BRnzp DONE 	

EQUALS		.FILL x3D	
SPACE		.FILL x20
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
	
PRINT_HEX
	ST R3, Save_R3_print
	AND R3, R3, #0
	ADD R3, R3, R0
	AND R4, R4, #0
	ADD R4, R3, R4
	AND R5, R5, #0		; clear R5
	ADD R5, R5, #4		; sets R5, the digit counter	
DIGIT
	AND R6, R6, #0		; clear R6
	ADD R6, R6, #4		; sets R6, the individual bit counter
	AND R0, R0, #0		; clear R0

EXTRACT	
	ADD R0, R0, R0		; left shift R0	
	ADD R4, R4, #0		; clear 
	BRzp SHIFT		; no bit is added since MSB is 0-bit
	ADD R0, R0, #1		; adds 1 to R0 since MSB is 1-bit

SHIFT	
	ADD R4, R4, R4		; left shifts the histogram value
	ADD R6, R6, #-1		; decrements bit counter
	BRp EXTRACT		; loops back until first 4 digits are extracted

HEX_PRINT
	AND R6,R6, #0		; clear R6 for re-use
	ADD R6, R6, R0		; R6 <- R0, since R0 will be used for OUT	
	ADD R6, R6, #-9		; checks if the value <=9
	BRnz PRINT_NUM		; branches if value <=9
	LD R0, LETTER		; loads the hex value for 'A'
	ADD R0, R0, R6		; adds the digit to R0
	ADD R0, R0, #-10	; adds -10 for letter offset (from flowchart)	
	BR PRINT		; prints hex digit

PRINT_NUM
	LD R0, NUMBER		; Loads the hex value for '0'
	ADD R0, R0, R6		; adds the digit(R6) as the number offset

PRINT	ADD R0, R0, #9		; balances back the <=9 check performed
	OUT			; prints the extracted digit in hex
	ADD R5, R5, #-1		; decrements R5 after each digit is printed
	BRp DIGIT		; Goes back to printing until 4 digits are printed
	AND R5, R5, #0
	ADD R5, R5, R3
	LD R3, Save_R3_print
	BR DONE
NUMBER		.FILL x30 	; ASCII '0'
LETTER		.FILL x41 	; ASCII 'A'
OFFSET		.FILL x40 	; ASCII '@'
STACK_DIFF 	.FILL x3FFF
Save_R3_print	.BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output
;
;
EVALUATE
ST R7, Save_R7_Eval

CHECK_OPERAND
	LD R1, NINE
	NOT R2, R0
	ADD R2, R2, #1
	ADD R1, R1, R2
	BRn INVALID 
	LD R1, ZERO
	NOT R1, R1
	ADD R1, R1, #1
	ADD R1, R1, R0
	BRn CHECK_PLUS

OPERAND
	LD R1, ZERO
	NOT R1, R1
	ADD R1, R1, #1
	ADD R0, R1, R0	
	BR EVAL_FIN

CHECK_PLUS
	LD R1, PLUSCHAR
	ADD R1, R1, R0
	BRnp CHECK_MINUS
	JSR OP_POP
	JSR PLUS
	BR EVAL_FIN

CHECK_MINUS
	LD R1, MINUSCHAR
	ADD R1, R1, R0
	BRnp CHECK_MULT
	JSR OP_POP
	JSR MIN
	BR EVAL_FIN

CHECK_MULT
	LD R1, MULTCHAR
	ADD R1, R1, R0
	BRnp CHECK_DIV
	JSR OP_POP
	JSR MUL
	BR EVAL_FIN
	
CHECK_DIV
	LD R1, DIVISCHAR
	ADD R1, R1, R0
	BRnp CHECK_EXP
	JSR OP_POP
	JSR DIV
	BR EVAL_FIN

CHECK_EXP
	LD R1, EXPCHAR
	ADD R1, R1, R0
	BRnp INVALID
	JSR OP_POP
	JSR EXP
	LD R7, Save_R7_OPPOP
	BR EVAL_FIN
	
INVALID
	LEA R0, MESSAGE
	PUTS
	BR DONE	

OP_POP
	ST R7, Save_R7_OPPOP	
	ST R7, Save_R7_POPONE
	JSR POP
	LD R7, Save_R7_POPONE
	AND R4, R4, #0
	ADD R4, R4, R0
	ST R7, Save_R7_POPTWO
	JSR POP
	LD R7, Save_R7_POPTWO
	AND R3, R3, #0
	ADD R3, R3, R0
	ADD R5, R5, #0
	BRp INVALID
	LD R7, Save_R7_OPPOP
	RET

Save_R7_OPPOP 	.BLKW #1
Save_R7_POPONE	.BLKW #1
Save_R7_POPTWO	.BLKW #1

EVAL_FIN
	JSR PUSH
	LD R7, Save_R7_Eval
	RET

Save_R7_Eval .BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
	ADD R0, R3, R4
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN	
	NOT R4, R4
	ADD R4, R4, #1
	ADD R0, R3, R4
	RET	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
	ST R7, Save_R7_MUL
	AND R0, R0, #0
	AND R1, R1, #0	
	ADD R3, R3, #0
	BRz MUL_ZERO
	BRn R3_SIGN
R4_CHK	ADD R4, R4, #0
	BRn R4_SIGN	
	BRz MUL_ZERO		
MUL_LOOP
	ADD R0, R3, R0
	ADD R4, R4, #-1
	BRnp MUL_LOOP
	ADD R1, R1, #-1
	BRz NEG_SIGN
	BRnp FINISH_MUL

R3_SIGN
	ADD R1, R1, #1
	BR R4_CHK

R4_SIGN
	ADD R1, R1, #1
	NOT R4, R4
	ADD R4, R4, #1
	BR MUL_LOOP
	
NEG_SIGN
	NOT R0, R0
	ADD R0, R0, #1
	BR FINISH_MUL
	
MUL_ZERO
	AND R0, R0, #0

FINISH_MUL
	LD R7, Save_R7_MUL
	RET

Save_R7_MUL	.BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
DIV		
	AND R1, R1, #0	
	ADD R3, R3, #0
	BRz DIV_ZERO
	BRn R3_DSIGN
R4_DCHK	ADD R4, R4, #0
	BRp R4_DSIGN	
	BRz DIV_INVALID	
	ADD R1, R1, #1	
DIV_LOOP
	ADD R3, R3, R4
	ADD R0, R0, #1
	ADD R3, R3, #0
	BRp DIV_LOOP
	ADD R1, R1, #-1
	BRz NEG_DSIGN
	BRnp FINISH_DIV

R3_DSIGN
	ADD R1, R1, #1
	NOT R3, R3
	ADD R3, R3, #1
	BR R4_DCHK

R4_DSIGN
	NOT R4, R4
	ADD R4, R4, #1
	BR DIV_LOOP
	
NEG_DSIGN
	NOT R0, R0
	ADD R0, R0, #1
	BR FINISH_DIV
	
DIV_ZERO
	AND R0, R0, #0
	BR FINISH_DIV

DIV_INVALID
	LEA R0, DIV_ERROR
	PUTS
	BR INVALID

FINISH_DIV
	ADD R0, R0, #1
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
EXP
	ST R7, Save_R7_EXP
	ADD R4, R4, #0;
	BRz EXP_ONE
	BRp EXP_POSITIVE
	BRn EXP_NEGATIVE
	AND R1, R1, #0;
	ADD R1, R1, R4;
	ADD R1, R1, #-1;

EXP_SAME
	AND R0, R0, #0;
	ADD R0, R0, R3;
	BRnzp FINISH_EXP

EXP_POSITIVE
	AND R0, R0, #0;
	AND R1, R1, #0;
	ADD R1, R1, R4
	ADD R1, R1, #-1;
EXP_LOOP
	JSR MUL 
	AND R3, R3, #0
	ADD R3, R3, R0
	ADD R1, R1, #-1
	BRp EXP_LOOP
	BR FINISH_EXP

EXP_NEGATIVE
	AND R0, R0, #0
	BR FINISH_EXP

EXP_ONE
	AND R0, R0, #0;
	ADD R0, R0, #1;

FINISH_EXP
	LD R7, Save_R7_EXP
	RET
	
Save_R7_EXP	.BLKW #1

;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;
STACK_TOP	.FILL x4000	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET

DONE 	HALT

POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
ZERO		.FILL x30
NINE		.FILL x39
PLUSCHAR	.FILL xFFD5	; negative equivalent of '+'
MINUSCHAR	.FILL xFFD3	; negative equivalent of '-'
DIVISCHAR	.FILL xFFD1	; negative equivalent of '/'
MULTCHAR	.FILL xFFD6	; negative equivalent of '*'
EXPCHAR		.FILL xFFA2	; negative equivalent of '^'
MESSAGE		.STRINGZ " Invalid post-fix expression entered. "
DIV_ERROR	.STRINGZ " MATH ERROR (DIV BY 0). "



.END

