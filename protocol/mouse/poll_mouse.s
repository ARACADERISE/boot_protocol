; Function: poll_mouse
;           Poll the mouse state and display the X and Y coordinates and the status byte
;
; Inputs:   None
; Returns:  None
; Clobbers: None
;
;
;   __poll_mouse: back-end stub
;
;       Obtain new information over mouse
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
global __poll_mouse
__poll_mouse:
    push ax
    push bx
    push dx

    mov bx, 0x0002              ; Set display page to 0 (BH) and color green (BL)

    mov si, xcoord
    call __asm_print

    mov ax, [mouseX]            ; Retrieve current mouse coordinates. Disable interrupts

    ; Store mouse x value
    mov [mouseX_addr], ax

    call __print_word_hex         ; Print the mouseX coordinate
    mov si, delimCommaSpc
    call __asm_print

    mov si, ycoord
    call __asm_print

    mov ax, [mouseY]

    ; Store mouse y value
    mov [mouseY_addr], ax
    
    call __print_word_hex         ; Print the mouseY coordinate
    mov si, delimCommaSpc
    call __asm_print

    mov al, [curStatus]

    ; Store cursor status in memory
    mov [mouseStatus_addr], al

    call __print_byte_hex         ; Print the last read mouse state byte

    mov al, 0x0d
    call __print_char             ; Print carriage return to return to beginning of line

    pop dx
    pop bx
    pop ax
    ret