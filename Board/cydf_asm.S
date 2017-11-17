
#include "cydf_zynq_peripherals.h"
#include "uart_regs.h"

#define TIMER_INITIAL	0x10000

.extern test_main
.extern pmu_setting
.align 8

cydf_entry:
	b cydf_reset
	b cydf_undefined
	b cydf_software_interrupt
	b cydf_prefetch
	b cydf_data
	b cydf_not_used
	b cydf_irq
	b cydf_fiq

.global main
main:
	// Set VBAR (Vector Base Address Register) to my own interrupt vectors
    LDR     r0, =cydf_entry
    MCR     p15, 0, r0, c12, c0, 0
    dsb
    isb

	mrs	r0, cpsr			/* get the current PSR */
	mvn	r1, #0x1f			/* set up the irq stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x12			/* IRQ mode */
	msr	cpsr, r2
	ldr	r13,=irq_stack_top	/* IRQ stack pointer */

	mrs	r0, cpsr			/* get the current PSR */
	mvn	r1, #0x1f			/* set up the supervisor stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x13			/* supervisor mode */
	msr	cpsr, r2
	ldr	r13,=svc_stack_top	/* Supervisor stack pointer */

	mrs	r0, cpsr			/* get the current PSR */
	mvn	r1, #0x1f			/* set up the FIQ stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x11			/* FIQ mode */
	msr	cpsr, r2
	ldr	r13,=fiq_stack_top	/* FIQ stack pointer */

	mrs	r0, cpsr			/* get the current PSR */
	mvn	r1, #0x1f			/* set up the system stack pointer */
	and	r2, r1, r0
	orr	r2, r2, #0x1F			/* SYS mode */
	msr	cpsr, r2


	// Turn off I bit in CPSR to enable interrupt
	mrs r1, cpsr
	mov r2, #1<<7
	bic r1, r1, r2
	msr cpsr_c, r1

	// CPU Interface ID Register
	ldr r0, =GICC_IIDR
	ldr r3, [r0]

	// CPU Controller Type Register
	ldr r0, =GICD_TYPER
	ldr r3, [r0]

	// CPU Binary Pointer Register
	ldr r0, =GICC_BPR
	ldr r3, [r0]

	// Distributor Control Register
	ldr r0, =GICD_CTLR
	ldr r1, [r0]
	mov r2, #1       // Enable
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	 // Interrupt Set-Enable Register 0
	ldr r0, =GICD_ISENABLER0
	ldr r1, [r0]
	mov r2, #1 << 29   // Enable #29 (Private Timer)
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	// Interrupt Priority Register #7
	ldr r0, =GICD_PRIOR7
	ldr r1, [r0]
	mov r2, #1 << 8    // Highest - 1 for ID# 29 (Private Timer)
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

	// CPU Interface Control Register
	ldr r0, =GICC_CTLR
	ldr r1, [r0]
	mov r2, #1        // Enable
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

 	// CPU Interface Interrupt Priority Mask Register
	ldr r0, =GICC_PMR
	ldr r1, [r0]
	mov r2, #0xFF     // Lowest
	orr r1, r1, r2
	str r1, [r0]
	ldr r3, [r0]

 	// Private Timer Load Register
	ldr r0, =PRIVATE_LOAD
 	ldr r1, =TIMER_INITIAL
 	str r1, [r0]

 	// Private Timer Control Register
  	ldr r0, =PRIVATE_CONTROL
 	mov r1, #10 << 8   // Prescalar
 	orr r1, r1, #7     // IRQ Enable, Auto-Reload, Timer Enable
 	str r1, [r0]


	//-------------------------------------------- UART CONFIGURATION START-----------------------------------

	ldr r0,	=slcr_UART_RST_CTRL
	ldr r1, =0x0	//	reset UART
	str r1, [r0]

	ldr r0, =slcr_UART_CLK_CTRL
	ldr r1, =0x1402	// divisor = 0x14 (ref clk = 50MHz), srcsel = 0, CLKACT1 = true, CLKACT0 = false
	str r1, [r0]

	ldr r0, =uart_mode_reg0
	ldr r1, =0x20
	str r1, [r0]

	ldr r0, =uart_Control_reg0
	ldr r1, =0x28	//uart off
	str r1, [r0]

	ldr r0, =uart_Baud_rate_gen_reg0
	ldr r1, =0x3e
	str r1, [r0]

	ldr r0, =uart_Baud_rate_divider_reg0
	ldr r1, =0x6
	str r1, [r0]

	ldr r0, =uart_Control_reg0
	ldr r1, =0x00000117	//uart start
	str r1, [r0]

	ldr r0, =uart_Modem_ctrl_reg0
	ldr r1, [r0]

	ldr r0, =uart_Modem_sts_reg0
	ldr r1, [r0]

	//-------------------------------------------- UART CONFIGURATION END-----------------------------------

	//-------------------------------------------- PMU CONFIGURATION START---------------------------------

	bl pmu_setting

	//--------------------------------------------- PMU CONFIGURATION END------------------------------
	b test_main

   	ldr r0, =PRIVATE_COUNTER
  	ldr r1, =PRIVATE_STATUS
  	ldr r2, =GICD_ISPENDR0

forever:
   	ldr r5, [r0]
   	ldr r6, [r1]
   	ldr r7, [r2]
   	b forever



recvloop:

	b recvloop

cydf_software_interrupt:
//	ldr r0, =cydf_LED_ADDR
//	mov r1, #0xC
//	str r1, [r0]
//	bl test_main
	b cydf_software_interrupt

cydf_reset:
cydf_undefined:
cydf_prefetch:
cydf_data:
cydf_not_used:
cydf_irq:

	stmfd sp!, {r0-r12, lr}

 	// Interrupt Ack
  	ldr r0, =GICC_IAR
	ldr r3, [r0]

	 // Clear Interrupt Status bit
  	ldr r0, =PRIVATE_STATUS
  	mov r1, #1
	str r1, [r0]

 	// End-of-Interrupt
  	ldr r0, =GICC_EOIR
	str r3, [r0]

	ldr r6,=0
	ldr r0,=0x1
	MCR p15, 0, r0, c9, c12, 5
	MRC p15, 0, r1, c9, c13, 2
	MCR p15, 0, r6, c9, c13, 2

	ldr r0,=0x2
	MCR p15, 0, r0, c9, c12, 5
	MRC p15, 0, r2, c9, c13, 2
	MCR p15, 0, r6, c9, c13, 2

	ldr r0,=0x3
	MCR p15, 0, r0, c9, c12, 5
	MRC p15, 0, r3, c9, c13, 2
	MCR p15, 0, r6, c9, c13, 2

	ldr r0,=0x4
	MCR p15, 0, r0, c9, c12, 5
	MRC p15, 0, r4, c9, c13, 2
	MCR p15, 0, r6, c9, c13, 2


	// send data to UARTz
	ldr r5, =uart_TX_RX_FIFO0
	ldr r6, =uart_Channel_sts_reg0

	lsr r7, r1, #24
	str r7, [r5]
	lsr r7, r1, #16
	and r7, r7, #0xff
	str r7, [r5]
	lsr r7, r1, #8
	and r7, r7, #0xff
	str r7, [r5]
	lsr r7, r1, #0
	and r7, r7, #0xff
	str r7, [r5]

	lsr r7, r2, #24
	str r7, [r5]
	lsr r7, r2, #16
	and r7, r7, #0xff
	str r7, [r5]
	lsr r7, r2, #8
	and r7, r7, #0xff
	str r7, [r5]
	lsr r7, r2, #0
	and r7, r7, #0xff
	str r7, [r5]

	lsr r7, r3, #24
	str r7, [r5]
	lsr r7, r3, #16
	and r7, r7, #0xff
	str r7, [r5]
	lsr r7, r3, #8
	and r7, r7, #0xff
	str r7, [r5]
	lsr r7, r3, #0
	and r7, r7, #0xff
	str r7, [r5]

	lsr r7, r4, #24
	str r7, [r5]
	lsr r7, r4, #16
	and r7, r7, #0xff
	str r7, [r5]
	lsr r7, r4, #8
	and r7, r7, #0xff
	str r7, [r5]
	lsr r7, r4, #0
	and r7, r7, #0xff
	str r7, [r5]

later:

	ldmfd sp!, {r0-r12, lr}
    subs pc, lr, #4

cydf_fiq:
	b .

.data
.align 4

irq_stack:     .space 1024
irq_stack_top:
fiq_stack:     .space 1024
fiq_stack_top:
svc_stack:     .space 1024
svc_stack_top:

led_initial: .word 0x5


