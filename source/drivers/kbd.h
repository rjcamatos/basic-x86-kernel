/*
 * References:
 * 		Intel PC-AT Technical Reference Mar84., SECTION 4. KEYBOARD
 *		http://wiki.osdev.org/PS/2_Keyboard
 */

#ifndef DRIVERS_KBD_H_
#define DRIVERS_KBD_H_

#include "../lib/types.h"
#include "../idt.h"

/* Commands to keyboard */
#define KBD_OUT_COMMAND_RESET					0xFF
#define KBD_OUT_COMMAND_RESEND					0xFE
#define KBD_OUT_COMMAND_SET_DEFAULT				0xF6
#define KBD_OUT_COMMAND_DISABLE					0xF5
#define KBD_OUT_COMMAND_ENABLE					0xF4
#define KBD_OUT_COMMAND_TYPEMATIC_DELAYRATE		0xF3
#define KBD_OUT_COMMAND_ECHO					0xEE
#define KBD_OUT_COMMAND_SET_INDICATOR			0xED

/* Commands from keyboard */
#define KBD_IN_COMMAND_RESEND					0xFE
#define KBD_IN_COMMAND_ACK						0xFA
#define KBD_IN_COMMAND_OVERRUN					0x00
#define KBD_IN_COMMAND_DIAGNOSTIC_FAILURE		0xFD
#define KBD_IN_COMMAND_BREAK_CODE_PREFIX		0xF0
#define KBD_IN_COMMAND_BAT_COMPLETION_CODE		0xAA
#define KBD_IN_COMMAND_ECHO_RESPONSE			0xEE


typedef struct __attribute__((packed)) {
	uint_t scroll_lock:1;
	uint_t numeric_lock:1;
	uint_t caps_lock:1;
	uint_t _r0:1;
	uint_t _r1:1;
	uint_t _r2:1;
	uint_t _r3:1;
	uint_t _r4:1;
} kbd_command_indicator_t;

extern void kbd_init(void);

extern void mouse_init(void);

#endif /* DRIVERS_KBD_H_ */
