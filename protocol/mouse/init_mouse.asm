; Function: mouse_initialize
;           Initialize the mouse if present
;
; Inputs:   None
; Returns:  CF = 1 if error, CF=0 success
; Clobbers: AX
global mouse_initialize
mouse_initialize:
    push es
    push bx

    int 0x11                    ; Get equipment list
    test ax, HW_EQUIP_PS2       ; Is a PS2 mouse installed?
    jz .no_mouse                ;     if not print error and end

    mov ax, 0xC205              ; Initialize mouse
    mov bh, MOUSE_PKT_BYTES     ; 3 byte packets
    int 0x15                    ; Call BIOS to initialize
    jc .no_mouse                ;    If not successful assume no mouse

    mov ax, 0xC203              ; Set resolution
    mov bh, MOUSE_RESOLUTION    ; 8 counts / mm
    int 0x15                    ; Call BIOS to set resolution
    jc .no_mouse                ;    If not successful assume no mouse

    push cs
    pop es                      ; ES = segment where code and mouse handler reside

    mov bx, mouse_callback_dummy
    mov ax, 0xC207              ; Install a default null handler (ES:BX)
    int 0x15                    ; Call BIOS to set callback
    jc .no_mouse                ;    If not successful assume no mouse

    clc                         ; CF=0 is success
    jmp .finished
.no_mouse:
    stc                         ; CF=1 is error
.finished:
    pop bx
    pop es
    ret

; Function: mouse_enable
;           Enable the mouse
;
; Inputs:   None
; Returns:  None
; Clobbers: AX
global mouse_enable
mouse_enable:
    push es
    push bx

    call mouse_disable          ; Disable mouse before enabling

    push cs
    pop es
    mov bx, mouse_callback
    mov ax, 0xC207              ; Set mouse callback function (ES:BX)
    int 0x15                    ; Call BIOS to set callback

    mov ax, 0xC200              ; Enable/Disable mouse
    mov bh, 1                   ; BH = Enable = 1
    int 0x15                    ; Call BIOS to disable mouse

    pop bx
    pop es
    ret

; Function: mouse_disable
;           Disable the mouse
;
; Inputs:   None
; Returns:  None
; Clobbers: AX
global mouse_disable
mouse_disable:
    push es
    push bx

    mov ax, 0xC200              ; Enable/Disable mouse
    xor bx, bx                  ; BH = Disable = 0
    int 0x15                    ; Call BIOS to disable mouse

    mov es, bx
    mov ax, 0xC207              ; Clear callback function (ES:BX=0:0)
    int 0x15                    ; Call BIOS to set callback

    pop bx
    pop es
    ret