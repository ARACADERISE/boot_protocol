; Function: mouse_callback (FAR)
;           called by the interrupt handler to process a mouse data packet
;           All registers that are modified must be saved and restored
;           Since we are polling manually this handler does nothing
;
; Inputs:   SP+4  = Unused (0)
;           SP+6  = MovementY
;           SP+8  = MovementX
;           SP+10 = Mouse Status
;
; Returns:  None
; Clobbers: None

ARG_OFFSETS      equ 6          ; Offset of args from BP
;
;   __mouse_callback: back-end stub
;
;       Obtain new X/Y values of the mouse
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
global __mouse_callback
__mouse_callback:
    push bp                     ; Function prologue
    mov bp, sp
    push ds                     ; Save registers we modify
    push ax
    push bx
    push cx
    push dx

    push cs
    pop ds                      ; DS = CS, CS = where our variables are stored

    mov al,[bp+ARG_OFFSETS+6]
    mov bl, al                  ; BX = copy of status byte
    mov cl, 3                   ; Shift signY (bit 5) left 3 bits
    shl al, cl                  ; CF = signY
                                ; Sign bit of AL = SignX
    sbb dh, dh                  ; CH = SignY value set in all bits
    cbw                         ; AH = SignX value set in all bits
    mov dl, [bp+ARG_OFFSETS+2]  ; CX = movementY
    mov al, [bp+ARG_OFFSETS+4]  ; AX = movementX

    ; new mouse X_coord = X_Coord + movementX
    ; new mouse Y_coord = Y_Coord + (-movementY)
    neg dx
    mov cx, [mouseY]
    add dx, cx                  ; DX = new mouse Y_coord
    mov cx, [mouseX]
    add ax, cx                  ; AX = new mouse X_coord

    ; Status
    mov [curStatus], bl         ; Update the current status with the new bits
    mov [mouseX], ax            ; Update current virtual mouseX coord
    mov [mouseY], dx            ; Update current virtual mouseY coord

    pop dx                      ; Restore all modified registers
    pop cx
    pop bx
    pop ax
    pop ds
    pop bp                      ; Function epilogue

__mouse_callback_dummy:
    retf                        ; This routine was reached via FAR CALL. Need a FAR RET