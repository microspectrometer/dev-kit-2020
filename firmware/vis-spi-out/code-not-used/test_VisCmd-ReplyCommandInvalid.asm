	.file	"test_VisCmd-ReplyCommandInvalid.c"
	.text
.Ltext0:
	.cfi_sections	.debug_frame
	.globl	OK
	.section .rdata,"dr"
OK:
	.space 1
	.globl	ERROR
ERROR:
	.byte	1
	.globl	INVALID_CMD
INVALID_CMD:
	.byte	2
	.globl	OFF
OFF:
	.space 1
	.globl	GREEN
GREEN:
	.byte	1
	.globl	RED
RED:
	.byte	2
	.globl	PADDING
	.data
PADDING:
	.byte	-1
.lcomm value_in_fake_BiColorLed_ddr,1,1
.lcomm value_in_fake_BiColorLed_port,1,1
	.globl	status_led
	.section .rdata,"dr"
status_led:
	.byte	3
	.globl	led_0
led_0:
	.space 1
	.globl	led_1
led_1:
	.byte	1
	.globl	BiColorLed_ddr
	.align 8
BiColorLed_ddr:
	.quad	value_in_fake_BiColorLed_ddr
	.globl	BiColorLed_port
	.align 8
BiColorLed_port:
	.quad	value_in_fake_BiColorLed_port
.lcomm fake_SpiFlags,1,1
	.globl	Flag_SpiFlags
	.align 8
Flag_SpiFlags:
	.quad	fake_SpiFlags
	.globl	Flag_SlaveRx
Flag_SlaveRx:
	.space 1
	.globl	Flag_TransferDone
Flag_TransferDone:
	.byte	1
.lcomm fake_Spi_PortDirection,1,1
.lcomm fake_Spi_PortInput,1,1
.lcomm fake_Spi_PortOutput,1,1
.lcomm fake_Spi_PortPullup,1,1
.lcomm fake_Spi_SPCR,1,1
.lcomm fake_Spi_SPSR,1,1
.lcomm fake_Spi_SPDR,1,1
	.globl	Spi_PortDirection
	.align 8
Spi_PortDirection:
	.quad	fake_Spi_PortDirection
	.globl	Spi_PortInput
	.align 8
Spi_PortInput:
	.quad	fake_Spi_PortInput
	.globl	Spi_PortOutput
	.align 8
Spi_PortOutput:
	.quad	fake_Spi_PortOutput
	.globl	Spi_SPCR
	.align 8
Spi_SPCR:
	.quad	fake_Spi_SPCR
	.globl	Spi_SPSR
	.align 8
Spi_SPSR:
	.quad	fake_Spi_SPSR
	.globl	Spi_SPDR
	.align 8
Spi_SPDR:
	.quad	fake_Spi_SPDR
	.globl	Spi_DataReady
Spi_DataReady:
	.byte	1
	.globl	Spi_Ss
Spi_Ss:
	.byte	2
	.globl	Spi_Mosi
Spi_Mosi:
	.byte	3
	.globl	Spi_Miso
Spi_Miso:
	.byte	4
	.globl	Spi_Sck
Spi_Sck:
	.byte	5
	.globl	Spi_MasterSlaveSelect
Spi_MasterSlaveSelect:
	.byte	4
	.globl	Spi_ClockBit0
Spi_ClockBit0:
	.space 1
	.globl	Spi_ClockBit1
Spi_ClockBit1:
	.byte	1
	.globl	Spi_DoubleClock
Spi_DoubleClock:
	.space 1
	.globl	Spi_Enable
Spi_Enable:
	.byte	6
	.globl	Spi_InterruptEnable
Spi_InterruptEnable:
	.byte	7
	.globl	Spi_InterruptFlag
Spi_InterruptFlag:
	.byte	7
.lcomm fake_ddr,1,1
.lcomm fake_port,1,1
.lcomm fake_UCSR0A,1,1
.lcomm fake_UCSR0B,1,1
.lcomm fake_UCSR0C,1,1
.lcomm fake_UDR0,1,1
.lcomm fake_UBRR0,2,2
	.globl	UartSpi_ddr
	.align 8
UartSpi_ddr:
	.quad	fake_ddr
	.globl	UartSpi_port
	.align 8
UartSpi_port:
	.quad	fake_port
	.globl	UartSpi_UCSR0A
	.align 8
UartSpi_UCSR0A:
	.quad	fake_UCSR0A
	.globl	UartSpi_UCSR0B
	.align 8
UartSpi_UCSR0B:
	.quad	fake_UCSR0B
	.globl	UartSpi_UCSR0C
	.align 8
UartSpi_UCSR0C:
	.quad	fake_UCSR0C
	.globl	UartSpi_UDR0
	.align 8
UartSpi_UDR0:
	.quad	fake_UDR0
	.globl	UartSpi_UBRR0
	.align 8
UartSpi_UBRR0:
	.quad	fake_UBRR0
	.globl	UartSpi_Miso
UartSpi_Miso:
	.space 1
	.globl	UartSpi_AdcConv
UartSpi_AdcConv:
	.byte	2
	.globl	UartSpi_Sck
UartSpi_Sck:
	.byte	4
	.globl	UartSpi_UMSEL00
UartSpi_UMSEL00:
	.byte	6
	.globl	UartSpi_UMSEL01
UartSpi_UMSEL01:
	.byte	7
	.globl	UartSpi_RXEN0
UartSpi_RXEN0:
	.byte	4
	.globl	UartSpi_TXEN0
UartSpi_TXEN0:
	.byte	3
	.globl	UartSpi_UCPOL0
UartSpi_UCPOL0:
	.space 1
	.globl	UartSpi_UCPHA0
UartSpi_UCPHA0:
	.byte	1
	.globl	UartSpi_UDORD0
UartSpi_UDORD0:
	.byte	2
	.globl	UartSpi_UDRE0
UartSpi_UDRE0:
	.byte	5
	.globl	UartSpi_RXC0
UartSpi_RXC0:
	.byte	7
.lcomm fake_FtCtrl_port,1,1
.lcomm fake_FtCtrl_pin,1,1
.lcomm fake_FtData_pin,1,1
.lcomm fake_FtData_ddr,1,1
.lcomm fake_FtData_port,1,1
	.globl	FtCtrl_port
	.align 8
FtCtrl_port:
	.quad	fake_FtCtrl_port
	.globl	FtCtrl_pin
	.align 8
FtCtrl_pin:
	.quad	fake_FtCtrl_pin
	.globl	FtData_pin
	.align 8
FtData_pin:
	.quad	fake_FtData_pin
	.globl	FtData_ddr
	.align 8
FtData_ddr:
	.quad	fake_FtData_ddr
	.globl	FtData_port
	.align 8
FtData_port:
	.quad	fake_FtData_port
	.globl	FtMiso
FtMiso:
	.byte	2
	.globl	FtChipSelect
FtChipSelect:
	.space 1
	.globl	FtClock
FtClock:
	.byte	1
	.globl	FtMiosio0
FtMiosio0:
	.space 1
.LC0:
	.ascii "`%s` is not call %d\0"
	.text
	.def	_SilentAssertCall;	.scl	3;	.type	32;	.endef
	.seh_proc	_SilentAssertCall
_SilentAssertCall:
.LFB114:
	.file 1 "code-not-used/test-boiler-plate.h"
	.loc 1 8 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movl	%ecx, %eax
	movq	%rdx, 24(%rbp)
	movw	%ax, 16(%rbp)
	.loc 1 11 24
	movl	$0, %ecx
	call	g_string_new
	movq	%rax, -8(%rbp)
	.loc 1 12 5
	movzwl	16(%rbp), %ecx
	movq	24(%rbp), %rdx
	movq	-8(%rbp), %rax
	movl	%ecx, %r9d
	movq	%rdx, %r8
	leaq	.LC0(%rip), %rdx
	movq	%rax, %rcx
	call	g_string_append_printf
	.loc 1 14 5
	movzwl	16(%rbp), %edx
	movq	mock(%rip), %rax
	movq	24(%rbp), %rcx
	movq	%rcx, %r8
	movq	%rax, %rcx
	call	SilentAssertCall
	testb	%al, %al
	jne	.L2
	.loc 1 14 5 is_stmt 0 discriminator 1
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	$14, %edx
	movq	%rax, %rcx
	call	UnityFail
.L2:
	.loc 1 19 5 is_stmt 1
	movq	-8(%rbp), %rax
	movl	$1, %edx
	movq	%rax, %rcx
	call	g_string_free
	.loc 1 20 1
	nop
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE114:
	.seh_endproc
	.def	_AssertCall;	.scl	3;	.type	32;	.endef
	.seh_proc	_AssertCall
_AssertCall:
.LFB115:
	.loc 1 22 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movl	%ecx, %eax
	movq	%rdx, 24(%rbp)
	movw	%ax, 16(%rbp)
	.loc 1 25 24
	movl	$0, %ecx
	call	g_string_new
	movq	%rax, -8(%rbp)
	.loc 1 26 5
	movzwl	16(%rbp), %ecx
	movq	24(%rbp), %rdx
	movq	-8(%rbp), %rax
	movl	%ecx, %r9d
	movq	%rdx, %r8
	leaq	.LC0(%rip), %rdx
	movq	%rax, %rcx
	call	g_string_append_printf
	.loc 1 28 5
	movzwl	16(%rbp), %edx
	movq	mock(%rip), %rax
	movq	24(%rbp), %rcx
	movq	%rcx, %r8
	movq	%rax, %rcx
	call	AssertCall
	testb	%al, %al
	jne	.L4
	.loc 1 28 5 is_stmt 0 discriminator 1
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	$28, %edx
	movq	%rax, %rcx
	call	UnityFail
.L4:
	.loc 1 33 5 is_stmt 1
	movq	-8(%rbp), %rax
	movl	$1, %edx
	movq	%rax, %rcx
	call	g_string_free
	.loc 1 34 1
	nop
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE115:
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC1:
	.ascii "Expect different value for call %d arg %d.\0"
	.text
	.def	_AssertArg;	.scl	3;	.type	32;	.endef
	.seh_proc	_AssertArg
_AssertArg:
.LFB116:
	.loc 1 36 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movl	%ecx, %eax
	movq	%r8, 32(%rbp)
	movw	%ax, 16(%rbp)
	movl	%edx, %eax
	movb	%al, 24(%rbp)
	.loc 1 75 20
	movl	$0, %ecx
	call	g_string_new
	movq	%rax, -8(%rbp)
	.loc 1 76 5
	movzbl	24(%rbp), %ecx
	movzwl	16(%rbp), %edx
	movq	-8(%rbp), %rax
	movl	%ecx, %r9d
	movl	%edx, %r8d
	leaq	.LC1(%rip), %rdx
	movq	%rax, %rcx
	call	g_string_printf
	.loc 1 80 5
	movzbl	24(%rbp), %ecx
	movzwl	16(%rbp), %edx
	movq	mock(%rip), %rax
	movq	32(%rbp), %r8
	movq	%r8, %r9
	movl	%ecx, %r8d
	movq	%rax, %rcx
	call	AssertArg
	testb	%al, %al
	jne	.L6
	.loc 1 80 5 is_stmt 0 discriminator 1
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	$80, %edx
	movq	%rax, %rcx
	call	UnityFail
.L6:
	.loc 1 84 5 is_stmt 1
	movq	-8(%rbp), %rax
	movl	$1, %edx
	movq	%rax, %rcx
	call	g_string_free
	.loc 1 85 1
	nop
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE116:
	.seh_endproc
	.def	_AssertArgByteVal;	.scl	3;	.type	32;	.endef
	.seh_proc	_AssertArgByteVal
_AssertArgByteVal:
.LFB117:
	.loc 1 86 81
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	movl	%ecx, %eax
	movl	%r8d, %ecx
	movw	%ax, 16(%rbp)
	movl	%edx, %eax
	movb	%al, 24(%rbp)
	movl	%ecx, %eax
	movb	%al, 32(%rbp)
	.loc 1 86 83
	movzbl	24(%rbp), %edx
	movzwl	16(%rbp), %eax
	leaq	32(%rbp), %rcx
	movq	%rcx, %r8
	movl	%eax, %ecx
	call	_AssertArg
	.loc 1 86 121
	nop
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE117:
	.seh_endproc
	.section .rdata,"dr"
	.align 8
.LC2:
	.ascii "Expect call count is %d, but was %d.\0"
	.text
	.def	_test_call_count_is;	.scl	3;	.type	32;	.endef
	.seh_proc	_test_call_count_is
_test_call_count_is:
.LFB118:
	.loc 1 88 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	movl	%ecx, %eax
	movw	%ax, 16(%rbp)
	.loc 1 92 20
	movl	$0, %ecx
	call	g_string_new
	movq	%rax, -8(%rbp)
	.loc 1 95 14
	movq	mock(%rip), %rax
	movq	%rax, %rcx
	call	NumberOfActualCalls
	.loc 1 93 5
	movzwl	%ax, %ecx
	movzwl	16(%rbp), %edx
	movq	-8(%rbp), %rax
	movl	%ecx, %r9d
	movl	%edx, %r8d
	leaq	.LC2(%rip), %rdx
	movq	%rax, %rcx
	call	g_string_printf
	.loc 1 98 16
	movq	mock(%rip), %rax
	movq	%rax, %rcx
	call	NumberOfActualCalls
	.loc 1 98 8
	cmpw	%ax, 16(%rbp)
	jne	.L9
	.loc 1 98 43 discriminator 1
	movl	$1, %edx
	movq	.refptr.Unity(%rip), %rax
	leaq	80(%rax), %rcx
	call	longjmp
.L9:
	.loc 1 99 10
	movq	-8(%rbp), %rax
	movq	(%rax), %rax
	movl	$99, %edx
	movq	%rax, %rcx
	call	UnityFail
	.loc 1 101 5
	movq	-8(%rbp), %rax
	movl	$1, %edx
	movq	%rax, %rcx
	call	g_string_free
	.loc 1 102 1
	nop
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE118:
	.seh_endproc
	.globl	setUp
	.bss
	.align 8
setUp:
	.space 8
	.globl	tearDown
	.align 8
tearDown:
	.space 8
	.globl	mock
	.align 8
mock:
	.space 8
	.text
	.globl	SetUp_Mock
	.def	SetUp_Mock;	.scl	2;	.type	32;	.endef
	.seh_proc	SetUp_Mock
SetUp_Mock:
.LFB127:
	.file 2 "code-not-used/test_VisCmd-ReplyCommandInvalid.c"
	.loc 2 9 22
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	.loc 2 10 12
	call	Mock_new
	.loc 2 10 10
	movq	%rax, mock(%rip)
	.loc 2 11 1
	nop
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE127:
	.seh_endproc
	.globl	TearDown_Mock
	.def	TearDown_Mock;	.scl	2;	.type	32;	.endef
	.seh_proc	TearDown_Mock
TearDown_Mock:
.LFB128:
	.loc 2 12 25
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	.loc 2 13 5
	movq	mock(%rip), %rax
	movq	%rax, %rcx
	call	Mock_destroy
	.loc 2 14 10
	movq	$0, mock(%rip)
	.loc 2 15 1
	nop
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE128:
	.seh_endproc
	.section .rdata,"dr"
.LC3:
	.ascii "SpiSlaveTxByte\0"
	.text
	.globl	ReplyCommandInvalid_transmits_one_byte_over_SPI
	.def	ReplyCommandInvalid_transmits_one_byte_over_SPI;	.scl	2;	.type	32;	.endef
	.seh_proc	ReplyCommandInvalid_transmits_one_byte_over_SPI
ReplyCommandInvalid_transmits_one_byte_over_SPI:
.LFB129:
	.loc 2 18 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	.loc 2 20 5
	call	ReplyCommandInvalid
	.loc 2 22 5
	leaq	.LC3(%rip), %rdx
	movl	$1, %ecx
	call	_AssertCall
	.loc 2 23 5
	movl	$1, %ecx
	call	_test_call_count_is
	.loc 2 24 1
	nop
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE129:
	.seh_endproc
	.globl	ReplyCommandInvalid_sends_byte_INVALID_CMD
	.def	ReplyCommandInvalid_sends_byte_INVALID_CMD;	.scl	2;	.type	32;	.endef
	.seh_proc	ReplyCommandInvalid_sends_byte_INVALID_CMD
ReplyCommandInvalid_sends_byte_INVALID_CMD:
.LFB130:
	.loc 2 26 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.seh_stackalloc	48
	.seh_endprologue
	.loc 2 28 5
	call	ReplyCommandInvalid
	.loc 2 31 14
	movw	$1, -2(%rbp)
	.loc 2 32 5
	movzwl	-2(%rbp), %eax
	leaq	.LC3(%rip), %rdx
	movl	%eax, %ecx
	call	_SilentAssertCall
	.loc 2 33 5
	movl	$2, %eax
	movzbl	%al, %edx
	movzwl	-2(%rbp), %eax
	movl	%edx, %r8d
	movl	$1, %edx
	movl	%eax, %ecx
	call	_AssertArgByteVal
	.loc 2 34 1
	nop
	addq	$48, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE130:
	.seh_endproc
	.def	__main;	.scl	2;	.type	32;	.endef
	.section .rdata,"dr"
	.align 8
.LC4:
	.ascii "code-not-used/test_VisCmd-ReplyCommandInvalid.c\0"
.LC5:
	.ascii "# test VisCmd\0"
.LC6:
	.ascii "## VisCmd.ReplyCommandInvalid\0"
	.align 8
.LC7:
	.ascii "ReplyCommandInvalid_transmits_one_byte_over_SPI\0"
	.align 8
.LC8:
	.ascii "ReplyCommandInvalid_sends_byte_INVALID_CMD\0"
	.text
	.globl	main
	.def	main;	.scl	2;	.type	32;	.endef
	.seh_proc	main
main:
.LFB131:
	.loc 2 37 1
	.cfi_startproc
	pushq	%rbp
	.seh_pushreg	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.seh_setframe	%rbp, 0
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	.seh_stackalloc	32
	.seh_endprologue
	.loc 2 37 1
	call	__main
	.loc 2 38 5
	leaq	.LC4(%rip), %rcx
	call	UnityBegin
	.loc 2 39 5
	leaq	.LC5(%rip), %rcx
	call	puts
	.loc 2 40 5
	leaq	.LC6(%rip), %rcx
	call	puts
	.loc 2 41 11
	leaq	SetUp_Mock(%rip), %rax
	movq	%rax, setUp(%rip)
	.loc 2 42 14
	leaq	TearDown_Mock(%rip), %rax
	movq	%rax, tearDown(%rip)
	.loc 2 43 5
	movl	$43, %r8d
	leaq	.LC7(%rip), %rdx
	leaq	ReplyCommandInvalid_transmits_one_byte_over_SPI(%rip), %rcx
	call	UnityDefaultTestRun
	.loc 2 44 5
	movl	$44, %r8d
	leaq	.LC8(%rip), %rdx
	leaq	ReplyCommandInvalid_sends_byte_INVALID_CMD(%rip), %rcx
	call	UnityDefaultTestRun
	.loc 2 45 12
	call	UnityEnd
	.loc 2 46 1
	addq	$32, %rsp
	popq	%rbp
	.cfi_restore 6
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE131:
	.seh_endproc
.Letext0:
	.file 3 "/usr/include/machine/setjmp.h"
	.file 4 "/usr/include/machine/_default_types.h"
	.file 5 "/usr/include/sys/_stdint.h"
	.file 6 "../mock-c/code/unity_internals.h"
	.file 7 "../mock-c/code/unity.h"
	.file 8 "/usr/lib/glib-2.0/include/glibconfig.h"
	.file 9 "/usr/include/glib-2.0/glib/gtypes.h"
	.file 10 "/usr/include/glib-2.0/glib/gstring.h"
	.file 11 "../mock-c/code/Mock.h"
	.file 12 "../lib/src/StatusCode.h"
	.file 13 "../lib/src/StatusCodes.h"
	.file 14 "../lib/src/BiColorLed.h"
	.file 15 "../lib/test/BiColorLed-HardwareFake.h"
	.file 16 "../lib/src/Flag.h"
	.file 17 "../lib/test/Flag-HardwareFake.h"
	.file 18 "../lib/src/Spi.h"
	.file 19 "../lib/test/Spi-HardwareFake.h"
	.file 20 "../lib/src/UartSpi.h"
	.file 21 "../lib/test/UartSpi-HardwareFake.h"
	.file 22 "../lib/src/Usb.h"
	.file 23 "../lib/test/Usb-HardwareFake.h"
	.section	.debug_info,"dr"
.Ldebug_info0:
	.long	0x1443
	.word	0x4
	.secrel32	.Ldebug_abbrev0
	.byte	0x8
	.uleb128 0x1
	.ascii "GNU C17 10.2.0 -mtune=generic -march=x86-64 -g\0"
	.byte	0xc
	.ascii "code-not-used/test_VisCmd-ReplyCommandInvalid.c\0"
	.ascii "/home/mike/chromation/dev-kit-2020/firmware/vis-spi-out\0"
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.secrel32	.Ldebug_line0
	.uleb128 0x2
	.ascii "jmp_buf\0"
	.byte	0x3
	.word	0x17e
	.byte	0x11
	.long	0xc9
	.uleb128 0x3
	.long	0xee
	.long	0xd9
	.uleb128 0x4
	.long	0xd9
	.byte	0x1f
	.byte	0
	.uleb128 0x5
	.byte	0x8
	.byte	0x7
	.ascii "long unsigned int\0"
	.uleb128 0x5
	.byte	0x8
	.byte	0x5
	.ascii "long int\0"
	.uleb128 0x5
	.byte	0x1
	.byte	0x6
	.ascii "signed char\0"
	.uleb128 0x6
	.ascii "__uint8_t\0"
	.byte	0x4
	.byte	0x2b
	.byte	0x18
	.long	0x11b
	.uleb128 0x5
	.byte	0x1
	.byte	0x8
	.ascii "unsigned char\0"
	.uleb128 0x5
	.byte	0x2
	.byte	0x5
	.ascii "short int\0"
	.uleb128 0x6
	.ascii "__uint16_t\0"
	.byte	0x4
	.byte	0x39
	.byte	0x19
	.long	0x14c
	.uleb128 0x5
	.byte	0x2
	.byte	0x7
	.ascii "short unsigned int\0"
	.uleb128 0x5
	.byte	0x4
	.byte	0x5
	.ascii "int\0"
	.uleb128 0x5
	.byte	0x4
	.byte	0x7
	.ascii "unsigned int\0"
	.uleb128 0x5
	.byte	0x8
	.byte	0x5
	.ascii "long long int\0"
	.uleb128 0x5
	.byte	0x10
	.byte	0x4
	.ascii "long double\0"
	.uleb128 0x7
	.byte	0x8
	.uleb128 0x5
	.byte	0x1
	.byte	0x6
	.ascii "char\0"
	.uleb128 0x8
	.long	0x19b
	.uleb128 0x6
	.ascii "uint8_t\0"
	.byte	0x5
	.byte	0x18
	.byte	0x13
	.long	0x109
	.uleb128 0x8
	.long	0x1a8
	.uleb128 0x9
	.long	0x1a8
	.uleb128 0x6
	.ascii "uint16_t\0"
	.byte	0x5
	.byte	0x24
	.byte	0x14
	.long	0x139
	.uleb128 0x9
	.long	0x1c2
	.uleb128 0xa
	.byte	0x8
	.long	0x1a3
	.uleb128 0xa
	.byte	0x8
	.long	0x1e4
	.uleb128 0xb
	.uleb128 0x5
	.byte	0x8
	.byte	0x7
	.ascii "long long unsigned int\0"
	.uleb128 0x5
	.byte	0x4
	.byte	0x4
	.ascii "float\0"
	.uleb128 0x5
	.byte	0x8
	.byte	0x4
	.ascii "double\0"
	.uleb128 0x6
	.ascii "UNITY_UINT64\0"
	.byte	0x6
	.byte	0x82
	.byte	0x20
	.long	0xd9
	.uleb128 0x6
	.ascii "UNITY_UINT\0"
	.byte	0x6
	.byte	0x87
	.byte	0x1a
	.long	0x212
	.uleb128 0xc
	.ascii "UNITY_STORAGE_T\0"
	.word	0x150
	.byte	0x6
	.word	0x178
	.byte	0x8
	.long	0x368
	.uleb128 0xd
	.ascii "TestFile\0"
	.byte	0x6
	.word	0x17a
	.byte	0x11
	.long	0x1d8
	.byte	0
	.uleb128 0xd
	.ascii "CurrentTestName\0"
	.byte	0x6
	.word	0x17b
	.byte	0x11
	.long	0x1d8
	.byte	0x8
	.uleb128 0xd
	.ascii "CurrentDetail1\0"
	.byte	0x6
	.word	0x17d
	.byte	0x11
	.long	0x1d8
	.byte	0x10
	.uleb128 0xd
	.ascii "CurrentDetail2\0"
	.byte	0x6
	.word	0x17e
	.byte	0x11
	.long	0x1d8
	.byte	0x18
	.uleb128 0xd
	.ascii "CurrentTestLineNumber\0"
	.byte	0x6
	.word	0x180
	.byte	0x15
	.long	0x227
	.byte	0x20
	.uleb128 0xd
	.ascii "NumberOfTests\0"
	.byte	0x6
	.word	0x181
	.byte	0x18
	.long	0x227
	.byte	0x28
	.uleb128 0xd
	.ascii "TestFailures\0"
	.byte	0x6
	.word	0x182
	.byte	0x18
	.long	0x227
	.byte	0x30
	.uleb128 0xd
	.ascii "TestIgnores\0"
	.byte	0x6
	.word	0x183
	.byte	0x18
	.long	0x227
	.byte	0x38
	.uleb128 0xd
	.ascii "CurrentTestFailed\0"
	.byte	0x6
	.word	0x184
	.byte	0x18
	.long	0x227
	.byte	0x40
	.uleb128 0xd
	.ascii "CurrentTestIgnored\0"
	.byte	0x6
	.word	0x185
	.byte	0x18
	.long	0x227
	.byte	0x48
	.uleb128 0xd
	.ascii "AbortFrame\0"
	.byte	0x6
	.word	0x187
	.byte	0xd
	.long	0xb8
	.byte	0x50
	.byte	0
	.uleb128 0xe
	.ascii "Unity\0"
	.byte	0x6
	.word	0x18b
	.byte	0x1f
	.long	0x23a
	.uleb128 0xf
	.ascii "setUp\0"
	.byte	0x7
	.byte	0x12
	.byte	0xf
	.long	0x1de
	.uleb128 0xf
	.ascii "tearDown\0"
	.byte	0x7
	.byte	0x13
	.byte	0xf
	.long	0x1de
	.uleb128 0x6
	.ascii "gsize\0"
	.byte	0x8
	.byte	0x42
	.byte	0x17
	.long	0xd9
	.uleb128 0x6
	.ascii "gchar\0"
	.byte	0x9
	.byte	0x2e
	.byte	0x10
	.long	0x19b
	.uleb128 0xa
	.byte	0x8
	.long	0x3a4
	.uleb128 0x6
	.ascii "GString\0"
	.byte	0xa
	.byte	0x27
	.byte	0x21
	.long	0x3c8
	.uleb128 0x10
	.ascii "_GString\0"
	.byte	0x18
	.byte	0xa
	.byte	0x29
	.byte	0x8
	.long	0x40c
	.uleb128 0x11
	.ascii "str\0"
	.byte	0xa
	.byte	0x2b
	.byte	0xb
	.long	0x3b2
	.byte	0
	.uleb128 0x11
	.ascii "len\0"
	.byte	0xa
	.byte	0x2c
	.byte	0x9
	.long	0x396
	.byte	0x8
	.uleb128 0x11
	.ascii "allocated_len\0"
	.byte	0xa
	.byte	0x2d
	.byte	0x9
	.long	0x396
	.byte	0x10
	.byte	0
	.uleb128 0xa
	.byte	0x8
	.long	0x3b8
	.uleb128 0x5
	.byte	0x1
	.byte	0x2
	.ascii "_Bool\0"
	.uleb128 0x6
	.ascii "Mock_s\0"
	.byte	0xb
	.byte	0x9
	.byte	0x17
	.long	0x42a
	.uleb128 0x12
	.ascii "Mock_s\0"
	.uleb128 0xf
	.ascii "mock\0"
	.byte	0xb
	.byte	0xa
	.byte	0x10
	.long	0x43f
	.uleb128 0xa
	.byte	0x8
	.long	0x41b
	.uleb128 0x6
	.ascii "status_code\0"
	.byte	0xc
	.byte	0x29
	.byte	0x17
	.long	0x1b8
	.uleb128 0xf
	.ascii "OK\0"
	.byte	0xc
	.byte	0x2b
	.byte	0x14
	.long	0x445
	.uleb128 0xf
	.ascii "ERROR\0"
	.byte	0xc
	.byte	0x2c
	.byte	0x14
	.long	0x445
	.uleb128 0xf
	.ascii "INVALID_CMD\0"
	.byte	0xc
	.byte	0x2d
	.byte	0x14
	.long	0x445
	.uleb128 0x6
	.ascii "led_state\0"
	.byte	0xc
	.byte	0x30
	.byte	0x17
	.long	0x1b8
	.uleb128 0xf
	.ascii "OFF\0"
	.byte	0xc
	.byte	0x31
	.byte	0x12
	.long	0x486
	.uleb128 0xf
	.ascii "GREEN\0"
	.byte	0xc
	.byte	0x32
	.byte	0x12
	.long	0x486
	.uleb128 0xf
	.ascii "RED\0"
	.byte	0xc
	.byte	0x33
	.byte	0x12
	.long	0x486
	.uleb128 0xf
	.ascii "PADDING\0"
	.byte	0xc
	.byte	0x41
	.byte	0x10
	.long	0x1a8
	.uleb128 0x13
	.long	0x459
	.byte	0xd
	.byte	0xd
	.byte	0xd
	.uleb128 0x9
	.byte	0x3
	.quad	OK
	.uleb128 0x13
	.long	0x464
	.byte	0xd
	.byte	0xe
	.byte	0xd
	.uleb128 0x9
	.byte	0x3
	.quad	ERROR
	.uleb128 0x13
	.long	0x472
	.byte	0xd
	.byte	0xf
	.byte	0xd
	.uleb128 0x9
	.byte	0x3
	.quad	INVALID_CMD
	.uleb128 0x13
	.long	0x498
	.byte	0xd
	.byte	0x11
	.byte	0xb
	.uleb128 0x9
	.byte	0x3
	.quad	OFF
	.uleb128 0x13
	.long	0x4a4
	.byte	0xd
	.byte	0x12
	.byte	0xb
	.uleb128 0x9
	.byte	0x3
	.quad	GREEN
	.uleb128 0x13
	.long	0x4b2
	.byte	0xd
	.byte	0x13
	.byte	0xb
	.uleb128 0x9
	.byte	0x3
	.quad	RED
	.uleb128 0x13
	.long	0x4be
	.byte	0xd
	.byte	0x15
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	PADDING
	.uleb128 0xa
	.byte	0x8
	.long	0x1bd
	.uleb128 0x8
	.long	0x54c
	.uleb128 0x6
	.ascii "bicolorled_ptr\0"
	.byte	0xe
	.byte	0x7
	.byte	0x22
	.long	0x552
	.uleb128 0x6
	.ascii "bicolorled_num\0"
	.byte	0xe
	.byte	0xa
	.byte	0x17
	.long	0x1b8
	.uleb128 0xf
	.ascii "BiColorLed_ddr\0"
	.byte	0xe
	.byte	0xd
	.byte	0x17
	.long	0x557
	.uleb128 0xf
	.ascii "BiColorLed_port\0"
	.byte	0xe
	.byte	0xe
	.byte	0x17
	.long	0x557
	.uleb128 0xf
	.ascii "status_led\0"
	.byte	0xe
	.byte	0x10
	.byte	0x17
	.long	0x56e
	.uleb128 0xf
	.ascii "led_0\0"
	.byte	0xe
	.byte	0x11
	.byte	0x17
	.long	0x56e
	.uleb128 0xf
	.ascii "led_1\0"
	.byte	0xe
	.byte	0x12
	.byte	0x17
	.long	0x56e
	.uleb128 0x14
	.ascii "value_in_fake_BiColorLed_ddr\0"
	.byte	0xf
	.byte	0x7
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	value_in_fake_BiColorLed_ddr
	.uleb128 0x14
	.ascii "value_in_fake_BiColorLed_port\0"
	.byte	0xf
	.byte	0x8
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	value_in_fake_BiColorLed_port
	.uleb128 0x13
	.long	0x5b4
	.byte	0xf
	.byte	0xa
	.byte	0x10
	.uleb128 0x9
	.byte	0x3
	.quad	status_led
	.uleb128 0x13
	.long	0x5c7
	.byte	0xf
	.byte	0xb
	.byte	0x10
	.uleb128 0x9
	.byte	0x3
	.quad	led_0
	.uleb128 0x13
	.long	0x5d5
	.byte	0xf
	.byte	0xc
	.byte	0x10
	.uleb128 0x9
	.byte	0x3
	.quad	led_1
	.uleb128 0x15
	.long	0x585
	.byte	0xf
	.byte	0x10
	.uleb128 0x9
	.byte	0x3
	.quad	BiColorLed_ddr
	.uleb128 0x15
	.long	0x59c
	.byte	0xf
	.byte	0x10
	.uleb128 0x9
	.byte	0x3
	.quad	BiColorLed_port
	.uleb128 0x6
	.ascii "flag_reg\0"
	.byte	0x10
	.byte	0x5
	.byte	0x22
	.long	0x552
	.uleb128 0x6
	.ascii "flag_bit\0"
	.byte	0x10
	.byte	0x6
	.byte	0x17
	.long	0x1b8
	.uleb128 0xf
	.ascii "Flag_SpiFlags\0"
	.byte	0x10
	.byte	0xb
	.byte	0x11
	.long	0x69a
	.uleb128 0xf
	.ascii "Flag_SlaveRx\0"
	.byte	0x10
	.byte	0xd
	.byte	0x11
	.long	0x6ab
	.uleb128 0xf
	.ascii "Flag_TransferDone\0"
	.byte	0x10
	.byte	0xe
	.byte	0x11
	.long	0x6ab
	.uleb128 0x14
	.ascii "fake_SpiFlags\0"
	.byte	0x11
	.byte	0x6
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_SpiFlags
	.uleb128 0x13
	.long	0x6bc
	.byte	0x11
	.byte	0x8
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	Flag_SpiFlags
	.uleb128 0x13
	.long	0x6d2
	.byte	0x11
	.byte	0xa
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	Flag_SlaveRx
	.uleb128 0x13
	.long	0x6e7
	.byte	0x11
	.byte	0xb
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	Flag_TransferDone
	.uleb128 0x6
	.ascii "spi_reg\0"
	.byte	0x12
	.byte	0x23
	.byte	0x22
	.long	0x552
	.uleb128 0x6
	.ascii "spi_bit\0"
	.byte	0x12
	.byte	0x24
	.byte	0x17
	.long	0x1b8
	.uleb128 0xf
	.ascii "Spi_PortDirection\0"
	.byte	0x12
	.byte	0x28
	.byte	0x10
	.long	0x757
	.uleb128 0xf
	.ascii "Spi_PortInput\0"
	.byte	0x12
	.byte	0x2a
	.byte	0x10
	.long	0x757
	.uleb128 0xf
	.ascii "Spi_PortOutput\0"
	.byte	0x12
	.byte	0x2c
	.byte	0x10
	.long	0x757
	.uleb128 0xf
	.ascii "Spi_SPCR\0"
	.byte	0x12
	.byte	0x30
	.byte	0x10
	.long	0x757
	.uleb128 0xf
	.ascii "Spi_SPSR\0"
	.byte	0x12
	.byte	0x32
	.byte	0x10
	.long	0x757
	.uleb128 0xf
	.ascii "Spi_SPDR\0"
	.byte	0x12
	.byte	0x34
	.byte	0x10
	.long	0x757
	.uleb128 0xf
	.ascii "Spi_DataReady\0"
	.byte	0x12
	.byte	0x37
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_Ss\0"
	.byte	0x12
	.byte	0x38
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_Mosi\0"
	.byte	0x12
	.byte	0x39
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_Miso\0"
	.byte	0x12
	.byte	0x3a
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_Sck\0"
	.byte	0x12
	.byte	0x3b
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_MasterSlaveSelect\0"
	.byte	0x12
	.byte	0x3e
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_ClockBit0\0"
	.byte	0x12
	.byte	0x3f
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_ClockBit1\0"
	.byte	0x12
	.byte	0x40
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_DoubleClock\0"
	.byte	0x12
	.byte	0x41
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_Enable\0"
	.byte	0x12
	.byte	0x42
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_InterruptEnable\0"
	.byte	0x12
	.byte	0x43
	.byte	0x10
	.long	0x767
	.uleb128 0xf
	.ascii "Spi_InterruptFlag\0"
	.byte	0x12
	.byte	0x44
	.byte	0x10
	.long	0x767
	.uleb128 0x14
	.ascii "fake_Spi_PortDirection\0"
	.byte	0x13
	.byte	0x8
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_Spi_PortDirection
	.uleb128 0x14
	.ascii "fake_Spi_PortInput\0"
	.byte	0x13
	.byte	0x9
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_Spi_PortInput
	.uleb128 0x14
	.ascii "fake_Spi_PortOutput\0"
	.byte	0x13
	.byte	0xa
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_Spi_PortOutput
	.uleb128 0x14
	.ascii "fake_Spi_PortPullup\0"
	.byte	0x13
	.byte	0xb
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_Spi_PortPullup
	.uleb128 0x14
	.ascii "fake_Spi_SPCR\0"
	.byte	0x13
	.byte	0xc
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_Spi_SPCR
	.uleb128 0x14
	.ascii "fake_Spi_SPSR\0"
	.byte	0x13
	.byte	0xd
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_Spi_SPSR
	.uleb128 0x14
	.ascii "fake_Spi_SPDR\0"
	.byte	0x13
	.byte	0xe
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_Spi_SPDR
	.uleb128 0x13
	.long	0x777
	.byte	0x13
	.byte	0x11
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_PortDirection
	.uleb128 0x13
	.long	0x791
	.byte	0x13
	.byte	0x12
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_PortInput
	.uleb128 0x13
	.long	0x7a7
	.byte	0x13
	.byte	0x13
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_PortOutput
	.uleb128 0x13
	.long	0x7be
	.byte	0x13
	.byte	0x14
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_SPCR
	.uleb128 0x13
	.long	0x7cf
	.byte	0x13
	.byte	0x15
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_SPSR
	.uleb128 0x13
	.long	0x7e0
	.byte	0x13
	.byte	0x16
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_SPDR
	.uleb128 0x13
	.long	0x7f1
	.byte	0x13
	.byte	0x19
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_DataReady
	.uleb128 0x13
	.long	0x807
	.byte	0x13
	.byte	0x1a
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_Ss
	.uleb128 0x13
	.long	0x816
	.byte	0x13
	.byte	0x1b
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_Mosi
	.uleb128 0x13
	.long	0x827
	.byte	0x13
	.byte	0x1c
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_Miso
	.uleb128 0x13
	.long	0x838
	.byte	0x13
	.byte	0x1d
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_Sck
	.uleb128 0x13
	.long	0x848
	.byte	0x13
	.byte	0x1f
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_MasterSlaveSelect
	.uleb128 0x13
	.long	0x866
	.byte	0x13
	.byte	0x20
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_ClockBit0
	.uleb128 0x13
	.long	0x87c
	.byte	0x13
	.byte	0x21
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_ClockBit1
	.uleb128 0x13
	.long	0x892
	.byte	0x13
	.byte	0x22
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_DoubleClock
	.uleb128 0x13
	.long	0x8aa
	.byte	0x13
	.byte	0x23
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_Enable
	.uleb128 0x13
	.long	0x8bd
	.byte	0x13
	.byte	0x24
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_InterruptEnable
	.uleb128 0x13
	.long	0x8d9
	.byte	0x13
	.byte	0x25
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	Spi_InterruptFlag
	.uleb128 0x6
	.ascii "uspi_ptr\0"
	.byte	0x14
	.byte	0x6
	.byte	0x22
	.long	0x552
	.uleb128 0x6
	.ascii "uspi_ptr16\0"
	.byte	0x14
	.byte	0x7
	.byte	0x23
	.long	0xb5b
	.uleb128 0xa
	.byte	0x8
	.long	0x1d3
	.uleb128 0x8
	.long	0xb55
	.uleb128 0x6
	.ascii "uspi_pin\0"
	.byte	0x14
	.byte	0x8
	.byte	0x17
	.long	0x1b8
	.uleb128 0x6
	.ascii "uspi_bit\0"
	.byte	0x14
	.byte	0x9
	.byte	0x17
	.long	0x1b8
	.uleb128 0xf
	.ascii "UartSpi_ddr\0"
	.byte	0x14
	.byte	0xe
	.byte	0x11
	.long	0xb31
	.uleb128 0xf
	.ascii "UartSpi_port\0"
	.byte	0x14
	.byte	0xf
	.byte	0x11
	.long	0xb31
	.uleb128 0xf
	.ascii "UartSpi_UCSR0A\0"
	.byte	0x14
	.byte	0x10
	.byte	0x11
	.long	0xb31
	.uleb128 0xf
	.ascii "UartSpi_UCSR0B\0"
	.byte	0x14
	.byte	0x11
	.byte	0x11
	.long	0xb31
	.uleb128 0xf
	.ascii "UartSpi_UCSR0C\0"
	.byte	0x14
	.byte	0x12
	.byte	0x11
	.long	0xb31
	.uleb128 0xf
	.ascii "UartSpi_UDR0\0"
	.byte	0x14
	.byte	0x13
	.byte	0x11
	.long	0xb31
	.uleb128 0xf
	.ascii "UartSpi_UBRR0\0"
	.byte	0x14
	.byte	0x14
	.byte	0x13
	.long	0xb42
	.uleb128 0xf
	.ascii "UartSpi_Miso\0"
	.byte	0x14
	.byte	0x16
	.byte	0x11
	.long	0xb60
	.uleb128 0xf
	.ascii "UartSpi_AdcConv\0"
	.byte	0x14
	.byte	0x17
	.byte	0x11
	.long	0xb60
	.uleb128 0xf
	.ascii "UartSpi_Sck\0"
	.byte	0x14
	.byte	0x18
	.byte	0x11
	.long	0xb60
	.uleb128 0xf
	.ascii "UartSpi_UMSEL00\0"
	.byte	0x14
	.byte	0x1a
	.byte	0x11
	.long	0xb71
	.uleb128 0xf
	.ascii "UartSpi_UMSEL01\0"
	.byte	0x14
	.byte	0x1b
	.byte	0x11
	.long	0xb71
	.uleb128 0xf
	.ascii "UartSpi_RXEN0\0"
	.byte	0x14
	.byte	0x1c
	.byte	0x11
	.long	0xb71
	.uleb128 0xf
	.ascii "UartSpi_TXEN0\0"
	.byte	0x14
	.byte	0x1d
	.byte	0x11
	.long	0xb71
	.uleb128 0xf
	.ascii "UartSpi_UCPOL0\0"
	.byte	0x14
	.byte	0x1e
	.byte	0x11
	.long	0xb71
	.uleb128 0xf
	.ascii "UartSpi_UCPHA0\0"
	.byte	0x14
	.byte	0x1f
	.byte	0x11
	.long	0xb71
	.uleb128 0xf
	.ascii "UartSpi_UDORD0\0"
	.byte	0x14
	.byte	0x20
	.byte	0x11
	.long	0xb71
	.uleb128 0xf
	.ascii "UartSpi_UDRE0\0"
	.byte	0x14
	.byte	0x21
	.byte	0x11
	.long	0xb71
	.uleb128 0xf
	.ascii "UartSpi_RXC0\0"
	.byte	0x14
	.byte	0x22
	.byte	0x11
	.long	0xb71
	.uleb128 0x14
	.ascii "fake_ddr\0"
	.byte	0x15
	.byte	0x6
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_ddr
	.uleb128 0x14
	.ascii "fake_port\0"
	.byte	0x15
	.byte	0x7
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_port
	.uleb128 0x14
	.ascii "fake_UCSR0A\0"
	.byte	0x15
	.byte	0x8
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_UCSR0A
	.uleb128 0x14
	.ascii "fake_UCSR0B\0"
	.byte	0x15
	.byte	0x9
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_UCSR0B
	.uleb128 0x14
	.ascii "fake_UCSR0C\0"
	.byte	0x15
	.byte	0xa
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_UCSR0C
	.uleb128 0x14
	.ascii "fake_UDR0\0"
	.byte	0x15
	.byte	0xb
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_UDR0
	.uleb128 0x14
	.ascii "fake_UBRR0\0"
	.byte	0x15
	.byte	0xc
	.byte	0x1a
	.long	0x1d3
	.uleb128 0x9
	.byte	0x3
	.quad	fake_UBRR0
	.uleb128 0x15
	.long	0xb82
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_ddr
	.uleb128 0x15
	.long	0xb96
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_port
	.uleb128 0x15
	.long	0xbab
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UCSR0A
	.uleb128 0x15
	.long	0xbc2
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UCSR0B
	.uleb128 0x15
	.long	0xbd9
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UCSR0C
	.uleb128 0x15
	.long	0xbf0
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UDR0
	.uleb128 0x15
	.long	0xc05
	.byte	0x15
	.byte	0xc
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UBRR0
	.uleb128 0x15
	.long	0xc1b
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_Miso
	.uleb128 0x15
	.long	0xc30
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_AdcConv
	.uleb128 0x15
	.long	0xc48
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_Sck
	.uleb128 0x15
	.long	0xc5c
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UMSEL00
	.uleb128 0x15
	.long	0xc74
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UMSEL01
	.uleb128 0x15
	.long	0xc8c
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_RXEN0
	.uleb128 0x15
	.long	0xca2
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_TXEN0
	.uleb128 0x15
	.long	0xcb8
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UCPOL0
	.uleb128 0x15
	.long	0xccf
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UCPHA0
	.uleb128 0x15
	.long	0xce6
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UDORD0
	.uleb128 0x15
	.long	0xcfd
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_UDRE0
	.uleb128 0x15
	.long	0xd13
	.byte	0x15
	.byte	0xa
	.uleb128 0x9
	.byte	0x3
	.quad	UartSpi_RXC0
	.uleb128 0x6
	.ascii "usb_reg\0"
	.byte	0x16
	.byte	0x33
	.byte	0x22
	.long	0x552
	.uleb128 0x6
	.ascii "usb_pin\0"
	.byte	0x16
	.byte	0x34
	.byte	0x17
	.long	0x1b8
	.uleb128 0xf
	.ascii "FtCtrl_port\0"
	.byte	0x16
	.byte	0x3b
	.byte	0x10
	.long	0xf35
	.uleb128 0xf
	.ascii "FtCtrl_pin\0"
	.byte	0x16
	.byte	0x3c
	.byte	0x10
	.long	0xf35
	.uleb128 0xf
	.ascii "FtData_pin\0"
	.byte	0x16
	.byte	0x3e
	.byte	0x10
	.long	0xf35
	.uleb128 0xf
	.ascii "FtData_ddr\0"
	.byte	0x16
	.byte	0x3f
	.byte	0x10
	.long	0xf35
	.uleb128 0xf
	.ascii "FtData_port\0"
	.byte	0x16
	.byte	0x40
	.byte	0x10
	.long	0xf35
	.uleb128 0xf
	.ascii "FtMiso\0"
	.byte	0x16
	.byte	0x43
	.byte	0x10
	.long	0xf45
	.uleb128 0xf
	.ascii "FtChipSelect\0"
	.byte	0x16
	.byte	0x44
	.byte	0x10
	.long	0xf45
	.uleb128 0xf
	.ascii "FtClock\0"
	.byte	0x16
	.byte	0x45
	.byte	0x10
	.long	0xf45
	.uleb128 0xf
	.ascii "FtMiosio0\0"
	.byte	0x16
	.byte	0x47
	.byte	0x10
	.long	0xf45
	.uleb128 0x14
	.ascii "fake_FtCtrl_port\0"
	.byte	0x17
	.byte	0x6
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_FtCtrl_port
	.uleb128 0x14
	.ascii "fake_FtCtrl_pin\0"
	.byte	0x17
	.byte	0x7
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_FtCtrl_pin
	.uleb128 0x14
	.ascii "fake_FtData_pin\0"
	.byte	0x17
	.byte	0x8
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_FtData_pin
	.uleb128 0x14
	.ascii "fake_FtData_ddr\0"
	.byte	0x17
	.byte	0x9
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_FtData_ddr
	.uleb128 0x14
	.ascii "fake_FtData_port\0"
	.byte	0x17
	.byte	0xa
	.byte	0x19
	.long	0x1bd
	.uleb128 0x9
	.byte	0x3
	.quad	fake_FtData_port
	.uleb128 0x13
	.long	0xf55
	.byte	0x17
	.byte	0xd
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtCtrl_port
	.uleb128 0x13
	.long	0xf69
	.byte	0x17
	.byte	0xe
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtCtrl_pin
	.uleb128 0x13
	.long	0xf7c
	.byte	0x17
	.byte	0x10
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtData_pin
	.uleb128 0x13
	.long	0xf8f
	.byte	0x17
	.byte	0x11
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtData_ddr
	.uleb128 0x13
	.long	0xfa2
	.byte	0x17
	.byte	0x12
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtData_port
	.uleb128 0x13
	.long	0xfb6
	.byte	0x17
	.byte	0x15
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtMiso
	.uleb128 0x13
	.long	0xfc5
	.byte	0x17
	.byte	0x16
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtChipSelect
	.uleb128 0x13
	.long	0xfda
	.byte	0x17
	.byte	0x17
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtClock
	.uleb128 0x13
	.long	0xfea
	.byte	0x17
	.byte	0x19
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	FtMiosio0
	.uleb128 0x13
	.long	0x377
	.byte	0x2
	.byte	0x6
	.byte	0x8
	.uleb128 0x9
	.byte	0x3
	.quad	setUp
	.uleb128 0x13
	.long	0x385
	.byte	0x2
	.byte	0x7
	.byte	0x8
	.uleb128 0x9
	.byte	0x3
	.quad	tearDown
	.uleb128 0x13
	.long	0x432
	.byte	0x2
	.byte	0x8
	.byte	0x9
	.uleb128 0x9
	.byte	0x3
	.quad	mock
	.uleb128 0x16
	.ascii "main\0"
	.byte	0x2
	.byte	0x24
	.byte	0x5
	.long	0x162
	.quad	.LFB131
	.quad	.LFE131-.LFB131
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x17
	.ascii "ReplyCommandInvalid_sends_byte_INVALID_CMD\0"
	.byte	0x2
	.byte	0x19
	.byte	0x6
	.quad	.LFB130
	.quad	.LFE130-.LFB130
	.uleb128 0x1
	.byte	0x9c
	.long	0x11f4
	.uleb128 0x18
	.secrel32	.LASF0
	.byte	0x2
	.byte	0x1f
	.byte	0xe
	.long	0x1c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 -18
	.byte	0
	.uleb128 0x19
	.ascii "ReplyCommandInvalid_transmits_one_byte_over_SPI\0"
	.byte	0x2
	.byte	0x11
	.byte	0x6
	.quad	.LFB129
	.quad	.LFE129-.LFB129
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x19
	.ascii "TearDown_Mock\0"
	.byte	0x2
	.byte	0xc
	.byte	0x6
	.quad	.LFB128
	.quad	.LFE128-.LFB128
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x19
	.ascii "SetUp_Mock\0"
	.byte	0x2
	.byte	0x9
	.byte	0x6
	.quad	.LFB127
	.quad	.LFE127-.LFB127
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x1a
	.ascii "_test_call_count_is\0"
	.byte	0x1
	.byte	0x57
	.byte	0xd
	.quad	.LFB118
	.quad	.LFE118-.LFB118
	.uleb128 0x1
	.byte	0x9c
	.long	0x12cc
	.uleb128 0x1b
	.ascii "num\0"
	.byte	0x1
	.byte	0x57
	.byte	0x2a
	.long	0x1c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x14
	.ascii "msg\0"
	.byte	0x1
	.byte	0x5c
	.byte	0xe
	.long	0x40c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1a
	.ascii "_AssertArgByteVal\0"
	.byte	0x1
	.byte	0x56
	.byte	0xd
	.quad	.LFB117
	.quad	.LFE117-.LFB117
	.uleb128 0x1
	.byte	0x9c
	.long	0x132d
	.uleb128 0x1c
	.secrel32	.LASF0
	.byte	0x1
	.byte	0x56
	.byte	0x28
	.long	0x1c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x1b
	.ascii "arg_n\0"
	.byte	0x1
	.byte	0x56
	.byte	0x38
	.long	0x1a8
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x1b
	.ascii "byte_val\0"
	.byte	0x1
	.byte	0x56
	.byte	0x47
	.long	0x1a8
	.uleb128 0x2
	.byte	0x91
	.sleb128 16
	.byte	0
	.uleb128 0x1a
	.ascii "_AssertArg\0"
	.byte	0x1
	.byte	0x23
	.byte	0xd
	.quad	.LFB116
	.quad	.LFE116-.LFB116
	.uleb128 0x1
	.byte	0x9c
	.long	0x1392
	.uleb128 0x1c
	.secrel32	.LASF0
	.byte	0x1
	.byte	0x23
	.byte	0x21
	.long	0x1c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x1b
	.ascii "arg_n\0"
	.byte	0x1
	.byte	0x23
	.byte	0x31
	.long	0x1a8
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x1b
	.ascii "pval\0"
	.byte	0x1
	.byte	0x23
	.byte	0x3e
	.long	0x199
	.uleb128 0x2
	.byte	0x91
	.sleb128 16
	.uleb128 0x14
	.ascii "msg\0"
	.byte	0x1
	.byte	0x4b
	.byte	0xe
	.long	0x40c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1a
	.ascii "_AssertCall\0"
	.byte	0x1
	.byte	0x15
	.byte	0xd
	.quad	.LFB115
	.quad	.LFE115-.LFB115
	.uleb128 0x1
	.byte	0x9c
	.long	0x13eb
	.uleb128 0x1b
	.ascii "num\0"
	.byte	0x1
	.byte	0x15
	.byte	0x22
	.long	0x1c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x1b
	.ascii "name\0"
	.byte	0x1
	.byte	0x15
	.byte	0x34
	.long	0x1d8
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x14
	.ascii "message\0"
	.byte	0x1
	.byte	0x19
	.byte	0xe
	.long	0x40c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.uleb128 0x1d
	.ascii "_SilentAssertCall\0"
	.byte	0x1
	.byte	0x7
	.byte	0xd
	.quad	.LFB114
	.quad	.LFE114-.LFB114
	.uleb128 0x1
	.byte	0x9c
	.uleb128 0x1b
	.ascii "num\0"
	.byte	0x1
	.byte	0x7
	.byte	0x28
	.long	0x1c2
	.uleb128 0x2
	.byte	0x91
	.sleb128 0
	.uleb128 0x1b
	.ascii "name\0"
	.byte	0x1
	.byte	0x7
	.byte	0x3a
	.long	0x1d8
	.uleb128 0x2
	.byte	0x91
	.sleb128 8
	.uleb128 0x14
	.ascii "message\0"
	.byte	0x1
	.byte	0xb
	.byte	0xe
	.long	0x40c
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.byte	0
	.byte	0
	.section	.debug_abbrev,"dr"
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0x8
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x1b
	.uleb128 0x8
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x35
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x15
	.byte	0
	.uleb128 0x27
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0x5
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x34
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0
	.uleb128 0x47
	.uleb128 0x13
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1b
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1c
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x1d
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x2116
	.uleb128 0x19
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"dr"
	.long	0x2c
	.word	0x2
	.secrel32	.Ldebug_info0
	.byte	0x8
	.byte	0
	.word	0
	.word	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"dr"
.Ldebug_line0:
	.section	.debug_str,"dr"
.LASF0:
	.ascii "call_n\0"
	.ident	"GCC: (GNU) 10.2.0"
	.def	g_string_new;	.scl	2;	.type	32;	.endef
	.def	g_string_append_printf;	.scl	2;	.type	32;	.endef
	.def	SilentAssertCall;	.scl	2;	.type	32;	.endef
	.def	UnityFail;	.scl	2;	.type	32;	.endef
	.def	g_string_free;	.scl	2;	.type	32;	.endef
	.def	AssertCall;	.scl	2;	.type	32;	.endef
	.def	g_string_printf;	.scl	2;	.type	32;	.endef
	.def	AssertArg;	.scl	2;	.type	32;	.endef
	.def	NumberOfActualCalls;	.scl	2;	.type	32;	.endef
	.def	longjmp;	.scl	2;	.type	32;	.endef
	.def	Mock_new;	.scl	2;	.type	32;	.endef
	.def	Mock_destroy;	.scl	2;	.type	32;	.endef
	.def	ReplyCommandInvalid;	.scl	2;	.type	32;	.endef
	.def	UnityBegin;	.scl	2;	.type	32;	.endef
	.def	puts;	.scl	2;	.type	32;	.endef
	.def	UnityDefaultTestRun;	.scl	2;	.type	32;	.endef
	.def	UnityEnd;	.scl	2;	.type	32;	.endef
	.section	.rdata$.refptr.Unity, "dr"
	.globl	.refptr.Unity
	.linkonce	discard
.refptr.Unity:
	.quad	Unity
