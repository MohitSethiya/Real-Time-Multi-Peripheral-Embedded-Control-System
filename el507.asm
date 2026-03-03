;-----------------------------------------------------------------------------
; COMPANY      : Confederation College
; FILE         : EL507.ASM
; FILE VERSION : 1.0
; PROGRAMMER   : Mohit Sethiya
;-----------------------------------------------------------------------------
; REVISION HISTORY
;-----------------------------------------------------------------------------
;
; 1.0, 2025-01-28, Mohit Sethiya
;   - Initial release
;
;-----------------------------------------------------------------------------
; MODULE DESCRIPTION
;-----------------------------------------------------------------------------
;
; Main program entry for Lab 5: Serial Communication
;
;-----------------------------------------------------------------------------
; GLOBALS
;-----------------------------------------------------------------------------

            .global     EL507_Example
			.global 	EL507_ToUpper
;-----------------------------------------------------------------------------
; MACROS
;-----------------------------------------------------------------------------

mov32       .macro      reg, imm
            movw        reg, #( imm & 0x0000ffff )
            movt        reg, #( imm >> 16 )
            .endm

;-----------------------------------------------------------------------------
; FUNCTION : EL507_Example
; PURPOSE  : Mixing C and assembly language example
;-----------------------------------------------------------------------------
;
; Description
;-------------------------------------
; This function adds one to the data addressed by the argument.
;
;
; Arguments:
;-------------------------------------
; R0: uint8_t* puiData
;
;
; Return:
;-------------------------------------
; None
;
;
			.text

EL507_Example:

    push    {lr, r1}

    ldrb    r1,[r0]
    add     r1,r1,#1
    strb    r1,[r0]

    pop     {pc, r1}

EL507_ToUpper:
	push 	{lr, r1}

	ldrb	r1, [r0]			; Load byte from *r0 into r1
	cmp		r1, #'a'			; Compare with 'a'
	BLT 	EL507_NoConversion	; If less, not lowercase
	cmp		r1, #'z'			; Compare with 'z'
	BGT 	EL507_NoConversion	; If greater, not lowercase
	sub		r1, r1, #0x20		; Convert to uppercase
	strb	r1, [r0]			; Store result back to *r0
	mov		r0, #1				; Conversion Complete
	b		EL507_Done

EL507_NoConversion:
	mov		r0, #0				; No conversion

EL507_Done:

	pop		{pc, r1}		; Restore r1 and return


            .end

;-----------------------------------------------------------------------------
; END EL507.ASM
;-----------------------------------------------------------------------------
