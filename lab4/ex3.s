.data
n: .word 3 # number of test data
data: # input, answer
.word 0x12345678, 0x1E6A2C48
.word 0x71C924BF, 0xFD24938E
.word 0x19260817, 0xE8106498

.text
# === main and helper functions ===
# You don't need to understand these, but reading them may be useful
main:
    la t0, n
    lw s0, 0(t0)
    la s1, data
    main_loop: # run each of the n tests
        beq s0, zero, main_loop_end
        lw a0, 0(s1)
        jal print_hex_and_space # print input
        lw a0, 4(s1)
        jal print_hex_and_space # print answer
        lw a0, 0(s1)
        jal bitrev1
        jal print_hex_and_space # print result of bitrev1
        lw a0, 0(s1)
        jal bitrev2
        jal print_hex_and_space # print result of bitrev2
        jal print_newline
        addi s0, s0, -1
        addi s1, s1, 8
        j main_loop
    main_loop_end:
    li a0, 10
    ecall # exit

print_hex_and_space:
    mv a1, a0
    li a0, 34
    ecall
    li a1, ' '
    li a0, 11
    ecall
    ret

print_newline:
    li a1, '\n'
    li a0, 11
    ecall
    ret

# === The first version ===
# Reverse the bits in a0 with a loop
bitrev1:
    li a2, 0
    li a3, 31
    li a4, 1    # const
    loop:
        sll t0, a0, a3
        sll t1, a4, a3
        and t2, t0, t1  # obtain the single bit
        xor a2, a2, t2
        addi a3, a3, -1
        srli a0, a0, 1
        bnez a3, loop
    mv a0, a2
    ret


# === The second version ===
# Reverse the bits in a0. Only use li, and, or, slli, srli wiht no loop!
bitrev2:
    li a2, 0xAAAAAAAA   # 1010
    li a3, 0x55555555   # 0101
    and t0, a2, a0
    srli t0, t0, 1
    and t1, a3, a0
    slli t1, t1, 1
    or a0, t0, t1

    li a2, 0xCCCCCCCC   # 1100
    li a3, 0x33333333   # 0011
    and t0, a2, a0
    srli t0, t0, 2
    and t1, a3, a0
    slli t1, t1, 2
    or a0, t0, t1


    li a2, 0xF0F0F0F0   # 1111 0000
    li a3, 0x0F0F0F0F   # 0000 1111
    and t0, a2, a0
    srli t0, t0, 4
    and t1, a3, a0
    slli t1, t1, 4
    or a0, t0, t1

    li a2, 0xFF00FF00   # 1111 1111
    li a3, 0x00FF00FF   # 0000 0000
    and t0, a2, a0
    srli t0, t0, 8
    and t1, a3, a0
    slli t1, t1, 8
    or a0, t0, t1

    li a2, 0xFFFF0000   
    li a3, 0x0000FFFF   
    and t0, a2, a0
    srli t0, t0, 16
    and t1, a3, a0
    slli t1, t1, 16
    or a0, t0, t1
    
    ret
