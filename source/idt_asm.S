.extern idt_gate_callback
.global idt_gate_handler

.section .text
	.altmacro
    .macro loop from, to
    	idt_gate_handler_&\from&:
    		pushal
    		push $&\from
    		mov (idt_gate_callback+(\from*4)),%eax
			cmp $0,%eax
			je .+4
			call *%eax
    		add $4,%esp
    		popal
    		iretl
    	.if \from < \to
    		loop %from+1, %to
    	.endif
    .endm
	loop 0, 99
	loop 100, 199
	loop 200, 254

.section .data
	.align 4
	idt_gate_handler:
		.purgem loop
		.altmacro
	    .macro loop from, to
			.long idt_gate_handler_&\from&
	    	.if \from < \to
	    		loop %from+1, %to
	    	.endif
	    .endm
		loop 0, 99
		loop 100, 199
		loop 200, 254
