; partners: shubbp2, ykko2
;
; This code is responsible for inputting a set of characters from the user and checking for a valid postfix 
; expression using a stack in LC-3. If an invalid post-fix expression is inputted, by the user, the user is notified 
; about the the same and the program ends. Each character is checked individually and accordingly placed in the stack 
; until an operator forces an arithmetic operation. Once an equal sign is inputted by the user, the program checks 
; the stack for validity and errors and accordingly prints a result in hex using the program created in MP1. The 
; program currently only calculates a single postfix expression and then ends. Result is shown on display of user.
;
;


.ORIG x3000
	

READ_NEXT_INPUT
	GETC
	OUT

CHECK_EQUALS
	LD R1, EQUALS 	; load the ASCII value of equal sign 
	NOT R1, R1 	; 2's complement 
	ADD R1, R1, #1 	; 2's complement
	ADD R1, R1, R0	; Check RO is equal 
	BRz STACK_CHECK	; If it is '='sign 

CHECK_SPACE
	LD R1, SPACE	; load the ASCII value of space
	NOT R1, R1	
	ADD R1, R1, #1
	ADD R1, R1, R0	
	BRz READ_NEXT_INPUT ; Check for next char 
	JSR EVALUATE 	; Check for operator/operand 
	BR READ_NEXT_INPUT	; Check for next char 

STACK_CHECK
	LD R5, STACK_DIFF ; Point to the start of the stack 
	LD R6, STACK_TOP ; current stack location pointer 
	NOT R5, R5
	ADD R5, R5, #1
	ADD R5, R6, R6 	;Check for exactly one result 
	BRz INVALID 	; Branch to invalid message if more than 1 result on stack
	JSR POP 	; pop the result 
	JSR PRINT_HEX 	; print the hex
	BRnzp DONE 	; end program 

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
	AND R5, R5, #0
	ADD R5, R5, R3		; store final result in R5
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
ST R7, Save_R7_Eval	; Store R7 for JSR location 

CHECK_EXP
	LD R1, EXPCHAR 	; 2's Complement ASCII value for '^'
	ADD R1, R1, R0	; Check RO is '^'
	BRnp CHECK_OPERAND ; Check if between 0-9
	JSR OP_POP	; Check if at least 2 items in stack 
	JSR EXP		; perform exponent operation 
	BR EVAL_FIN	; finish evaluating 

CHECK_OPERAND
	LD R1, NINE	; load the ASCII value of '9'
	NOT R2, R0	
	ADD R2, R2, #1
	ADD R1, R1, R2 	; check if below 9
	BRn INVALID 	; invalid if above 9 
	LD R1, ZERO 	; load the ASCII value of '0'
	NOT R1, R1
	ADD R1, R1, #1
	ADD R1, R1, R0 	; check if above 0
	BRn CHECK_PLUS 	; check for operators

OPERAND
	LD R1, ZERO 	; load zero
	NOT R1, R1
	ADD R1, R1, #1
	ADD R0, R1, R0 	; calculate exact operand	
	BR EVAL_FIN 	; finish evaluate

CHECK_PLUS
	LD R1, PLUSCHAR	; load two's complement value of "+"
	ADD R1, R1, R0 	; check if operator is '+'
	BRnp CHECK_MINUS	; check for next operator
	JSR OP_POP 	; Check if at least 2 items in stack 
	JSR PLUS 	; perform plus operation
	BR EVAL_FIN 	; finish evaluate

CHECK_MINUS
	LD R1, MINUSCHAR 	; load two's complement value of "-"
	ADD R1, R1, R0 	; check if operator is '-'
	BRnp CHECK_MULT
	JSR OP_POP	; Check if at least 2 items in stack 
	JSR MIN 	; perform minus operation
	BR EVAL_FIN	; finish evaluate

CHECK_MULT
	LD R1, MULTCHAR 	; load two's complement value of "*"
	ADD R1, R1, R0 	; check if operator is '*'
	BRnp CHECK_DIV
	JSR OP_POP	; Check if at least 2 items in stack 
	JSR MUL 	; perform multiplication operation
	BR EVAL_FIN 	; finish evaluate
	
CHECK_DIV
	LD R1, DIVISCHAR 	; load two's complement value of "/"
	ADD R1, R1, R0 	; check if operator is '/'
	BRnp INVALID
	JSR OP_POP	; Check if at least 2 items in stack 
	JSR DIV 	; perform division operation
	BR EVAL_FIN	; finish evaluate
	
INVALID
	LEA R0, MESSAGE 	; load the invalid string
	PUTS 	; print invalid string
	BR DONE 	; end program

OP_POP
	ST R7, Save_R7_OPPOP 	; store R7 for JSR location
	ST R7, Save_R7_POPONE 	; store R7 for JSR location
	JSR POP 	; first pop into R4
	LD R7, Save_R7_POPONE 	; load R7 for JSR
	AND R4, R4, #0
	ADD R4, R4, R0
	ST R7, Save_R7_POPTWO 	; store R7 for JSR location
	JSR POP 	; second pop into R3
	LD R7, Save_R7_POPTWO 	; load R7 for JSR
	AND R3, R3, #0
	ADD R3, R3, R0
	ADD R5, R5, #0	; check status of stack
	BRp INVALID 	; branch if failed
	LD R7, Save_R7_OPPOP 	; load R7 for JSR
	RET

Save_R7_OPPOP 	.BLKW #1
Save_R7_POPONE	.BLKW #1
Save_R7_POPTWO	.BLKW #1

EVAL_FIN
	JSR PUSH 	; push onto stack
	LD R7, Save_R7_Eval 	; load R7 for JSR
	RET

Save_R7_Eval .BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
	ADD R0, R3, R4 	; addition
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN	
	NOT R4, R4 	; minus 
	ADD R4, R4, #1
	ADD R0, R3, R4
	RET	
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
	ST R7, Save_R7_MUL 	; store R7 for JSR
	AND R0, R0, #0 	; clear R0
	AND R1, R1, #0	; clear R1
	ADD R3, R3, #0 	; check value of R3
	BRz MUL_ZERO 	; branch to case zero
	BRn R3_SIGN 	; branch for negative R3
R4_CHK	ADD R4, R4, #0 	; check value of R4
	BRn R4_SIGN	; branch for negative R4
	BRz MUL_ZERO	; branch to case zero
MUL_LOOP
	ADD R0, R3, R0 	; loop for multiplication
	ADD R4, R4, #-1 	; decrement counter
	BRnp MUL_LOOP 	; loop back
	ADD R1, R1, #-1 	; check number of negative signs
	BRz NEG_SIGN 	; branch to negative result
	BRnp FINISH_MUL 	; finish operation

R3_SIGN
	ADD R1, R1, #1 	; increment the negative sign counter
	BR R4_CHK

R4_SIGN
	ADD R1, R1, #1 	; increment the negative sign counter
	NOT R4, R4
	ADD R4, R4, #1 	; adjust R4 as positive counter
	BR MUL_LOOP 	
	
NEG_SIGN
	NOT R0, R0
	ADD R0, R0, #1 	; adjust R0 as negative result
	BR FINISH_MUL 	; finish operation
	
MUL_ZERO
	AND R0, R0, #0 	; adjust R0 as zero

FINISH_MUL
	LD R7, Save_R7_MUL 	; load R7 for JSR
	RET

Save_R7_MUL	.BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
DIV		
	AND R0, R0, #0	; clear R0
	AND R1, R1, #0	; clear R1
	ADD R3, R3, #0 	; check value of R3 
	BRz DIV_ZERO	; branch to case zero
	BRn R3_DSIGN	; branch to negative R3 case
R4_DCHK	ADD R4, R4, #0 	; check value of R4
	BRp R4_DSIGN	; branch to positive R4 case
	BRz DIV_INVALID	; branch to case invalid
	ADD R1, R1, #1	; increment negative sign counter
DIV_LOOP
	ADD R3, R3, R4 	; loop for division
	ADD R0, R0, #1	; increment quotient
	ADD R3, R3, #0	; check if R3 is negative
	BRzp DIV_LOOP	
	ADD R0, R0, #-1	; adjust quotient
	ADD R1, R1, #-1	; check number of negative
	BRz NEG_DSIGN 	; branch to negative result
	BRnp FINISH_DIV ; finish operation

R3_DSIGN
	ADD R1, R1, #1 ; increment negative sign counter
	NOT R3, R3
	ADD R3, R3, #1 ; adjust R3 to be positive
	BR R4_DCHK	

R4_DSIGN
	NOT R4, R4
	ADD R4, R4, #1 ; adjust R4 to be negative
	BR DIV_LOOP 	; continue operation
	
NEG_DSIGN
	NOT R0, R0
	ADD R0, R0, #1 	; adjust R0 to be negative result
	BR FINISH_DIV	; finish operation
	
DIV_ZERO
	AND R0, R0, #0 	; adjust R0 to be zero
	BR FINISH_DIV 	; finish operation

DIV_INVALID
	LEA R0, DIV_ERROR 	; load invalid message
	PUTS 	; print invalid message
	BR INVALID 	; print general invalid message 

FINISH_DIV
	RET
	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
EXP
	ST R7, Save_R7_EXP 	; store R7 for JSR
	AND R1, R1, #0
	ADD R1, R1, R4
	ADD R1, R1, #-1 	; check if R4 is equal to 1
	BRz EXP_SAME 		; branch to ones case
	ADD R4, R4, #0 		; check R4 '^' 	
	BRz EXP_ONE 		; branch to zero case
	BRp EXP_POSITIVE 	; branch to positive case
	BRn EXP_NEGATIVE 	; branch to negative case

EXP_SAME
	AND R0, R0, #0 	; clear R0
	ADD R0, R0, R3 	; set result as base value
	BRnzp FINISH_EXP 	; finish operation

EXP_POSITIVE
	AND R1, R1, #0 	; clear R1
	ADD R1, R1, R4 	; set R1 as exponent
	ADD R1, R1, #-1 ; set JSR MUL counter
	AND R4, R4, #0 	; clear R4
	ADD R4, R3, R4 	; set R4 as base value

EXP_LOOP
	ST R4, Save_R4_EXPLOOP 	; store R4 for JSR EXP use
	ST R1, Save_R1_EXPLOOP 	; store R1 for JSR EXP use
	JSR MUL
	LD R4, Save_R4_EXPLOOP 	; load R4 for JSR EXP use
	LD R1, Save_R1_EXPLOOP 	; load R1 for JSR EXP use
	AND R3, R3, #0	; clear R3
	ADD R3, R3, R0 	; set R3 as JSR MUL result
	ADD R1, R1, #-1 	; decrement JSR MUL counter
	BRp EXP_LOOP 	; branch back if positive
	BR FINISH_EXP 	; finish operation

EXP_NEGATIVE
	AND R0, R0, #0 	; set R0 as result 0
	BR FINISH_EXP 	; finish operation

EXP_ONE
	AND R0, R0, #0 	; clear R0
	ADD R0, R0, #1 	; set R0 as result 1

FINISH_EXP
	LD R7, Save_R7_EXP 	; load R7 for JSRs
	RET
	
Save_R7_EXP	.BLKW #1
Save_R1_EXPLOOP .BLKW #1
Save_R4_EXPLOOP .BLKW #1

DONE HALT

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

