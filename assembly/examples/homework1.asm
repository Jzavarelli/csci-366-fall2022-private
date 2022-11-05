        LDA POINTER
LOOP    OUT
        SUB END
        BRP EXIT
        LDA POINTER
        ADD ONE
        STA POINTER
        BRA LOOP
EXIT    HLT
POINTER DAT 1
END     DAT 10
ONE     DAT 1

-----

.text

li $t0, 1
li $v1, 10

loop:
 li $v0, 1 #print int
 move $a0, $t0
 syscall
 addi $t0, $t0, 1
 ble $t0, $v1, loop

 li $v0, 10 #exit
 syscall