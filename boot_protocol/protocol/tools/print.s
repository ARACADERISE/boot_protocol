use16

;
;   __move_cursor: back-end stub
;
;       Moves text-mode cursor in real-mode
;
;       Input: 
;           uint8 x_pos(dl, ebp+8)
;           uint8 y_pos(dh, ebp+12)
;       Output:
;           None
;       On Error: This stub does not error
;
global __move_cursor
__move_cursor:
    pusha
    mov ebp, esp

    ; column
    mov dl, [ebp + 8]
    ; row
    mov dh, [ebp + 12]

    mov  bh, 0
    mov  ah, 02h
    int  10h

    popa

    ret

;
;   __print_char: back-end stub
;
;       Display single character to screen using real-mode BIOS teletyping interrupt AH=0x0E, AL=character, INT=0x10
;
;       Input: 
;           None
;       Required Register:
;           AL=character
;       Output:
;           None
;       On Error: This stub does not error
;
__print_char:
    pusha
    mov ah, 0x0E
    int 0x10
    popa

    ret

;
;   __print_char_color: back-end stub
;
;       Display single character to screen in color using real-mode BIOS teletyping interrupt AH=0x0E, AL=character, INT=0x10
;
;       Input: 
;           uint8 character(al, ebp + 8)
;           uint8 page(bh, ebp + 12)
;           uint8 color(bl, ebp + 16)
;       Required Register:
;           AL=character
;       Output:
;           None
;       On Error: This stub does not error
;
global __print_char_color
__print_char_color:
    push ebp
    mov ebp, esp

    mov al, [ebp + 8]
    mov bh, [ebp + 12]
    mov bl, [ebp + 16]

    pop ebp

    MOV AH,09H
    MOV CX, 1      ; Character count
    INT 10h

    ret

;
;   __asm_print: back-end stub
;
;       Print to screen using real-mode BIOS teletyping interrupt AH=0x0E, AL=character, INT=0x10
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
global __asm_print
__asm_print:
    pusha
	mov ah, 0x0e
.print:
	mov al, [si]
	cmp al, 0x0
    cmp al, 0x0A
    je .reformat
	je .end_print
	int 0x10
    jmp .cont
.reformat:
    mov ah, 0x0E
    mov al, 'A'
    int 0x10
    hlt
.cont:
	add si, 1

	jmp .print
.end_print:
    popa
	xor ax, ax
	mov si, ax
	ret

;
;   __print_word_hex: back-end stub
;
;       Print 16-bit(word) hex value
;
;       Input: 
;           AX = 16-bit number to print
;           BH = Page number
;           BL = forground color
;       Output:
;           None
;       On Error: This stub does not error
;
global __print_word_hex
__print_word_hex:
    pusha
    xchg al, ah                 ; Print the high byte first
    call __print_byte_hex
    xchg al, ah                 ; Print the low byte second
    call __print_byte_hex

    mov ah, 0x0E
    mov al, 0x0D
    int 0x10
    mov ah, 0x0E
    mov al, 0x0A
    int 0x10

    popa
    ret

;
;   __print_byte_hex: back-end stub
;
;       Print 8-bit(byte) hex value
;
;       Input: 
;           AL = 8-bit number to print
;           BH = Page number
;           BL = forground color
;       Output:
;           None
;       On Error: This stub does not error
;
global __print_byte_hex
__print_byte_hex:
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