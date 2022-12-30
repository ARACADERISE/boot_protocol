; Function: poll_mouse
;           Poll the mouse state and display the X and Y coordinates and the status byte
;
; Inputs:   None
; Returns:  None
; Clobbers: None
global poll_mouse
poll_mouse:
    push ax
    push bx
    push dx

    mov bx, 0x0002              ; Set display page to 0 (BH) and color green (BL)

    mov si, xcoord
    call print

    mov ax, [mouseX]            ; Retrieve current mouse coordinates. Disable interrupts

    ; Store mouse x value
    mov [mouseX_addr], ax

    call print_word_hex         ; Print the mouseX coordinate
    mov si, delimCommaSpc
    call print

    mov si, ycoord
    call print

    mov ax, [mouseY]

    ; Store mouse y value
    mov [mouseY_addr], ax
    
    call print_word_hex         ; Print the mouseY coordinate
    mov si, delimCommaSpc
    call print

    mov al, [curStatus]

    ; Store cursor status in memory
    mov [mouseStatus_addr], al

    call print_byte_hex         ; Print the last read mouse state byte

    mov al, 0x0d
    call print_char             ; Print carriage return to return to beginning of line

    pop dx
    pop bx
    pop ax
    ret