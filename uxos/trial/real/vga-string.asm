        org     0x7c00
        [bits 16]

msg:
        db "HELLO", 0 
        times 510-($-$$) db 0
        dw      0xAA55
