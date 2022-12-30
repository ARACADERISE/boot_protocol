use16

global print
print:
	mov ah, 0x0e
.print:
	mov al, [si]
	cmp al, 0x0
	je .end_print
	int 0x10
	
	add si, 1

	jmp .print

.end_print:
	xor ax, ax
	mov si, ax
	ret

global print_char
print_char:
    mov ah, 0x0e                ; TTY function to display character in AL
    int 0x10                    ; Make BIOS call
    ret

; Function: print_word_hex
;           Print a 16-bit unsigned integer in hexadecimal on specified
;           page and in a specified color if running in a graphics mode
;
; Inputs:   AX = Unsigned 16-bit integer to print
;           BH = Page number
;           BL = foreground color (graphics modes only)
; Returns:  None
; Clobbers: Mone
global print_word_hex
print_word_hex:
    xchg al, ah                 ; Print the high byte first
    call print_byte_hex
    xchg al, ah                 ; Print the low byte second
    call print_byte_hex
    ret

; Function: print_byte_hex
;           Print a 8-bit unsigned integer in hexadecimal on specified
;           page and in a specified color if running in a graphics mode
;
; Inputs:   AL = Unsigned 8-bit integer to print
;           BH = Page number
;           BL = foreground color (graphics modes only)
; Returns:  None
; Clobbers: Mone
global print_byte_hex
print_byte_hex:
    push ax
    push cx
    push bx

    lea bx, [.table]            ; Get translation table address

    ; Translate each nibble to its ASCII equivalent
    mov ah, al                  ; Make copy of byte to print
    and al, 0x0f                ;     Isolate lower nibble in AL
    mov cl, 4
    shr ah, cl                  ; Isolate the upper nibble in AH
    xlat                        ; Translate lower nibble to ASCII
    xchg ah, al
    xlat                        ; Translate upper nibble to ASCII

    pop bx                      ; Restore attribute and page
    mov ch, ah                  ; Make copy of lower nibble
    mov ah, 0x0e
    int 0x10                    ; Print the high nibble
    mov al, ch
    int 0x10                    ; Print the low nibble

    pop cx
    pop ax
    ret
.table: db "0123456789ABCDEF", 0