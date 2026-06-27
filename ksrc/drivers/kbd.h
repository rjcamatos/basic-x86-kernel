/** COMMENTS By GEMINI */

/**
 * PS/2 Keyboard and Mouse Driver Header
 * 
 * Defines command constants, configuration structures, and initialization 
 * routines for handling hardware inputs via the PS/2 controller.
 */

/*
 * References:
 * 		Intel PC-AT Technical Reference Mar84., SECTION 4. KEYBOARD
 *		http://osdev.org
 */

#ifndef KERNEL_DRIVERS_KBD_H_
#define KERNEL_DRIVERS_KBD_H_

#include "types.h"
#include "idt.h"

/* Commands to keyboard */
#define KBD_OUT_COMMAND_RESET					0xFF // Resets keyboard
#define KBD_OUT_COMMAND_RESEND					0xFE // Requests resend of last byte
#define KBD_OUT_COMMAND_SET_DEFAULT				0xF6 // Loads default settings
#define KBD_OUT_COMMAND_DISABLE					0xF5 // Disables keyboard scanning
#define KBD_OUT_COMMAND_ENABLE					0xF4 // Enables keyboard scanning
#define KBD_OUT_COMMAND_TYPEMATIC_DELAYRATE		0xF3 // Sets delay and repeat rate
#define KBD_OUT_COMMAND_ECHO					0xEE // Echo diagnostic command
#define KBD_OUT_COMMAND_SET_INDICATOR			0xED // Sets keyboard LEDs status

/* Commands from keyboard */
#define KBD_IN_COMMAND_RESEND					0xFE // Controller error, retry transmission
#define KBD_IN_COMMAND_ACK						0xFA // Command acknowledged
#define KBD_IN_COMMAND_OVERRUN					0x00 // Key buffer overflow error
#define KBD_IN_COMMAND_DIAGNOSTIC_FAILURE		0xFD // Self-test failure code
#define KBD_IN_COMMAND_BREAK_CODE_PREFIX		0xF0 // Scan Code Set 2/3 break prefix
#define KBD_IN_COMMAND_BAT_COMPLETION_CODE		0xAA // Basic Assurance Test passed
#define KBD_IN_COMMAND_ECHO_RESPONSE			0xEE // Response to echo command


// Keyboard LED indicators configuration state
typedef struct __attribute__((packed)) {
	uint8_t scroll_lock:1;
	uint8_t numeric_lock:1;
	uint8_t caps_lock:1;
	uint8_t _r0:1;
	uint8_t _r1:1;
	uint8_t _r2:1;
	uint8_t _r3:1;
	uint8_t _r4:1;
} kbd_command_indicator_t;

// Initializes the PS/2 keyboard interface
extern void kbd_init(void);

// Initializes the PS/2 mouse interface
extern void mouse_init(void);

#endif /* DRIVERS_KBD_H_ */
