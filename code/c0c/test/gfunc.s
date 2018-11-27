	.file	"gfunc.c"
 # GNU C11 (tdm-1) version 5.1.0 (mingw32)
 #	compiled by GNU C version 5.1.0, GMP version 4.3.2, MPFR version 2.4.2, MPC version 0.8.2
 # GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
 # options passed: 
 # -iprefix C:/Program Files (x86)/CodeBlocks/MinGW/bin/../lib/gcc/mingw32/5.1.0/
 # -D_REENTRANT gfunc.c -mtune=generic -march=pentiumpro
 # -auxbase-strip gfunc.s -fverbose-asm
 # options enabled:  -faggressive-loop-optimizations
 # -fasynchronous-unwind-tables -fauto-inc-dec -fchkp-check-incomplete-type
 # -fchkp-check-read -fchkp-check-write -fchkp-instrument-calls
 # -fchkp-narrow-bounds -fchkp-optimize -fchkp-store-bounds
 # -fchkp-use-static-bounds -fchkp-use-static-const-bounds
 # -fchkp-use-wrappers -fcommon -fdelete-null-pointer-checks
 # -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-types
 # -ffunction-cse -fgcse-lm -fgnu-runtime -fgnu-unique -fident
 # -finline-atomics -fira-hoist-pressure -fira-share-save-slots
 # -fira-share-spill-slots -fivopts -fkeep-inline-dllexport
 # -fkeep-static-consts -fleading-underscore -flifetime-dse
 # -flto-odr-type-merging -fmath-errno -fmerge-debug-strings -fpeephole
 # -fprefetch-loop-arrays -freg-struct-return
 # -fsched-critical-path-heuristic -fsched-dep-count-heuristic
 # -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
 # -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
 # -fsched-stalled-insns-dep -fschedule-fusion -fsemantic-interposition
 # -fset-stack-executable -fshow-column -fsigned-zeros
 # -fsplit-ivs-in-unroller -fstdarg-opt -fstrict-volatile-bitfields
 # -fsync-libcalls -ftrapping-math -ftree-coalesce-vars -ftree-cselim
 # -ftree-forwprop -ftree-loop-if-convert -ftree-loop-im
 # -ftree-loop-ivcanon -ftree-loop-optimize -ftree-parallelize-loops=
 # -ftree-phiprop -ftree-reassoc -ftree-scev-cprop -funit-at-a-time
 # -funwind-tables -fverbose-asm -fzero-initialized-in-bss -m32 -m80387
 # -m96bit-long-double -maccumulate-outgoing-args -malign-double
 # -malign-stringops -mavx256-split-unaligned-load
 # -mavx256-split-unaligned-store -mfancy-math-387 -mfp-ret-in-387
 # -mieee-fp -mlong-double-80 -mms-bitfields -mno-red-zone -mno-sse4
 # -mpush-args -msahf -mstack-arg-probe -mvzeroupper

	.comm	_s, 4, 2
	.comm	_i, 4, 2
	.comm	_n, 4, 2
	.text
	.globl	_sum
	.def	_sum;	.scl	2;	.type	32;	.endef
_sum:
	pushl	%ebp	 #
	movl	%esp, %ebp	 #,
	movl	$0, _s	 #, s
	movl	$1, _i	 #, i
	jmp	L2	 #
L3:
	movl	_s, %edx	 # s, D.1947
	movl	_i, %eax	 # i, D.1947
	addl	%edx, %eax	 # D.1947, D.1947
	movl	%eax, _s	 # D.1947, s
L2:
	movl	_i, %edx	 # i, D.1947
	movl	_n, %eax	 # n, D.1947
	cmpl	%eax, %edx	 # D.1947, D.1947
	jle	L3	 #,
	movl	_s, %eax	 # s, D.1947
	popl	%ebp	 #
	ret
	.def	___main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
LC0:
	.ascii "sum(%d)=%d\12\0"
	.text
	.globl	_main
	.def	_main;	.scl	2;	.type	32;	.endef
_main:
	pushl	%ebp	 #
	movl	%esp, %ebp	 #,
	andl	$-16, %esp	 #,
	subl	$16, %esp	 #,
	call	___main	 #
	movl	$10, _n	 #, n
	call	_sum	 #
	movl	%eax, %edx	 #, D.1948
	movl	_n, %eax	 # n, D.1948
	movl	%edx, 8(%esp)	 # D.1948,
	movl	%eax, 4(%esp)	 # D.1948,
	movl	$LC0, (%esp)	 #,
	call	_printf	 #
	movl	$0, %eax	 #, D.1948
	leave
	ret
	.ident	"GCC: (tdm-1) 5.1.0"
	.def	_printf;	.scl	2;	.type	32;	.endef
