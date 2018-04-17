.data
    szBuf DB 10 DUP(0) 
.code

GetGDTR proc
    XOR RAX, RAX
    push RBX
    SGDT szBuf
    lea RBX, szBuf
    MOV RAX, qword ptr [RBX + 2]
    pop RBX
    ret
GetGDTR endp


GetGDTL proc
    XOR RAX, RAX
    SGDT szBuf
    MOV AX, word ptr szBuf
    ret
GetGDTL endp



GetIDTR proc
    XOR RAX, RAX
    push RBX
    SIDT szBuf
    lea RBX, szBuf
    MOV RAX, qword ptr [RBX + 2]
    pop RBX
    ret
GetIDTR endp


GetIDTL proc
    XOR RAX, RAX
    SIDT szBuf
    MOV AX, word ptr szBuf
    ret
GetIDTL endp

end 