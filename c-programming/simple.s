	.arch armv8-a
	.file	"simple.c"
	.text
	.align	2
	.global	gcfr
	.type	gcfr, %function
gcfr:
.LFB0:
	.cfi_startproc
	mov	w0, 65
	ret
	.cfi_endproc
.LFE0:
	.size	gcfr, .-gcfr
	.align	2
	.global	gffr
	.type	gffr, %function
gffr:
.LFB1:
	.cfi_startproc
	mov	w0, 62915
	movk	w0, 0x4048, lsl 16
	fmov	s0, w0
	ret
	.cfi_endproc
.LFE1:
	.size	gffr, .-gffr
	.section	.rodata
	.align	3
.LC0:
	.string	"Fixed character: %c\n"
	.align	3
.LC1:
	.string	"Fixed float: %.2f\n"
	.align	3
.LC2:
	.string	"Hello, World!"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB2:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	bl	gcfr
	strb	w0, [sp, 31]
	ldrb	w0, [sp, 31]
	mov	w1, w0
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	printf
	bl	gffr
	str	s0, [sp, 24]
	ldr	s0, [sp, 24]
	fcvt	d0, s0
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	printf
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	puts
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE2:
	.size	main, .-main
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
