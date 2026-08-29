	.file	"a.c"
	.option nopic
	.attribute arch, "rv32e2p0"
	.attribute unaligned_access, 0
	.attribute stack_align, 4
	.text
	.align	2
	.globl	square
	.type	square, @function
square:
	addi	sp,sp,-12
	sw	ra,8(sp)
	sw	s0,4(sp)
	addi	s0,sp,12
	sw	a0,-12(s0)
	lw	a5,-12(s0)
	slli	a5,a5,1
	mv	a0,a5
	lw	ra,8(sp)
	lw	s0,4(sp)
	addi	sp,sp,12
	jr	ra
	.size	square, .-square
	.align	2
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-12
	sw	ra,8(sp)
	sw	s0,4(sp)
	addi	s0,sp,12
	li	a5,10
	sw	a5,-12(s0)
	lw	a0,-12(s0)
	call	square
	sw	a0,-12(s0)
	lw	a5,-12(s0)
	mv	a0,a5
	lw	ra,8(sp)
	lw	s0,4(sp)
	addi	sp,sp,12
	jr	ra
	.size	main, .-main
	.ident	"GCC: (g6afcc4f6da) 16.1.0"
	.section	.note.GNU-stack,"",@progbits
