bits 16         ; Tell NASM this is 16 bit code
org 0x7c00      ; Tell NASM to start outputting stuff at offset 0x7c00

start:

boot:
    mov si, message     ; Point SI register to our message buffer
    mov ah, 0Eh         ; Specify 'int 10h' 'teletype output' function
	                ; [AL = Character, BH = Page Number, BL = Colour (in graphics mode)]
    lodsb               ; Load byte at address SI into AL, and increment SI
    int 10h             ; Invoke the interupt to print out the character
    cli                 ; Clear interrupt flag
    hlt                 ; Halt execution

data:
	message db 'Hello OS Class', 10, 13, 0

; Pad to 510 bytes (boot sector size minus 2) with 0s, and finish with the two-byte standard boot signature
times 510-($-$$) db 0 
dw 0xAA55       ; Magic bytes - marks this 512 byte sector bootable!
