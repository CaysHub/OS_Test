        org     0x7c00
        [bits 16]
        mov     ax, 0xB800
        mov     es, ax
        mov     si,0
    l1:
        mov     ah, 0x07
        mov     al, ' '
        mov     [es:si], ax
        add     si,2
        cmp     si,4000
        jl      l1
        mov     [es:si],ax
stop:   jmp     stop

        times 510-($-$$) db 0
        dw      0xAA55