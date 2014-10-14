	.cpu arm7tdmi
	.fpu softvfp
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 1
	.eabi_attribute 30, 1
	.eabi_attribute 34, 0
	.eabi_attribute 18, 4
	.file	"bufcode.c"
	.global	global_value
	.data
	.align	2
	.type	global_value, %object
	.size	global_value, 4
global_value:
	.word	22
	.ident	"GCC: (4.8.2-14ubuntu1+6) 4.8.2"
