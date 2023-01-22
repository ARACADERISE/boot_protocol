;
;   __mouse_initialize: back-end stub
;
;       Check if mouse is supported, then configure the mouse if it is
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
global __mouse_initialize
__mouse_initialize:
    push es
    push bx

    int 0x11                    ; Get equipment list
    test ax, HW_EQUIP_PS2       ; Is a PS2 mouse installed?
    jz .__no_mouse                ;     if not print error and end

    mov ax, 0xC205              ; Initialize mouse
    mov bh, MOUSE_PKT_BYTES     ; 3 byte packets
    int 0x15                    ; Call BIOS to initialize
    jc .__no_mouse                ;    If not successful assume no mouse

    mov ax, 0xC203              ; Set resolution
    mov bh, MOUSE_RESOLUTION    ; 8 counts / mm
    int 0x15                    ; Call BIOS to set resolution
    jc .__no_mouse                ;    If not successful assume no mouse

    push cs
    pop es                      ; ES = segment where code and mouse handler reside

    mov bx, __mouse_callback_dummy
    mov ax, 0xC207              ; Install a default null handler (ES:BX)
    int 0x15                    ; Call BIOS to set callback
    jc .__no_mouse                ;    If not successful assume no mouse

    clc                         ; CF=0 is success
    jmp .__finished
.__no_mouse:
    ret
.__finished:
    pop bx
    pop es
    ret

;
;   __mouse_enable: back-end stub
;
;       Enable mouse. This allows for mouse-packets to be recieved
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
global __mouse_enable
__mouse_enable:
    push es
    push bx

    call __mouse_disable          ; Disable mouse before enabling

    push cs
    pop es
    mov bx, __mouse_callback
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
;
;   __mouse_disable: back-end stub
;
;       Disable the mouse. This makes it to where mouse-packets are no longer sent
;
;       Input: 
;           None
;       Output:
;           None
;       On Error: This stub does not error
;
global __mouse_disable
__mouse_disable:
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