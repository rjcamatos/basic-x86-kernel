.extern idt_gate_callback
.global idt_gate_handler

.altmacro

.section .text

    .macro create_handler num
        idt_gate_handler_\num\():
			# Check if the interrupt pushes a native hardware error code.
            # x86 exceptions with error codes: 8, 10, 11, 12, 13, 14, and 17.
            .if (\num == 8) || (\num >= 10 && \num <= 14) || (\num == 17)
                # Error code already pushed by hardware, do nothing.
                #pop %ecx # REMOVE ERROR FROM STACK TO TEST
            .else
                # No error code pushed, push a dummy 0 to align the stack frame.
                push $0
            .endif

            # 2. Segue com o fluxo normal
            pusha
            push $\num

            mov (idt_gate_callback + (\num * 4)), %eax
            cmp $0, %eax

            je 1f # Jump equal forward to label 1:

            call *%eax
        1:
            add $4, %esp
            popa
            
            # 3. Limpa o Error Code (verdadeiro ou falso) antes do iretl
            add $4, %esp  
            iretl
    .endm

    # Evaluate the Macro
    current = 0
    .rept 256
        create_handler %current
        current = current + 1 # Increment
    .endr

    # Remove this Macro
    .purgem create_handler


.section .data
	.align 4
	idt_gate_handler:
	    .macro create_pointer num
			.long idt_gate_handler_\num\()
	    .endm
	
        # Evaluate the Macro
		current = 0
		.rept 256
			create_pointer %current
			current = current + 1 # Increment
		.endr

        # Remove the Macro
		.purgem create_pointer


