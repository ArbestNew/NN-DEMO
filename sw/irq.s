
.globl irq_handler

irq_handler:
	.func irq_handler

	/* Correct LR_irq; this is a quirk of how the ARM processor calls the
	 * IRQ handler.  */
	sub lr, lr, #4
	/* Store registers, frame pointer and Link register in stack */
	stmfd sp!,{r0-r3,fp,r12,lr}
	/* Execute interrupt handler function*/
	BL  handle_interrupt
	/* Load registers, frame pointer and Link register (to Program counter) from stack */
	ldmfd sp!,{r0-r3,fp,r12,pc}

	.endfunc


.globl irq_ena

irq_ena:
	.func irq_ena

	mrs   r1, cpsr
	bic r1,r1, #0x80
	msr  cpsr,r1
	.endfunc


.globl irq_dis

irq_dis:
	.func irq_dis
	mrs   r1, cpsr
	orr r1,r1, #0x80
	msr  cpsr,r1
	.endfunc

