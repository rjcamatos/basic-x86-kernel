/*
 * Intel 8042 - PS2 keyboard and mouse controller
 */

#ifndef DRIVERS_I8042_H_
#define DRIVERS_I8042_H_

#include "../lib/types.h"
#include "../lib/ioport.h"
#include "../idt.h"
#include "../lib/printk.h"

typedef union __attribute__((packed)) {
	byte_t raw_flags;
	struct {
		uint_t output_buffer_full:1;
		uint_t input_buffer_full:1;
		uint_t system_flag:1;
		uint_t command_or_data:1;
		uint_t inhibit_switch:1;
		uint_t transmit_time_out:1;
		uint_t receive_time_out:1;
		uint_t parity_error:1;
	} at_mode;
	struct {
		uint_t output_buffer_full:1;
		uint_t input_buffer_full:1;
		uint_t system_flag:1;
		uint_t command_or_data:1;
		uint_t inhibit_switch:1;
		uint_t mouse_output_buffer_full:1;
		uint_t general_timeout:1;
		uint_t parity_error:1;
	} ps2_mode;
} i8042_status_t;

typedef union __attribute__((packed)) {
	byte_t raw_flags;
	struct {
		uint_t keyboard_interrupt:1;
		uint_t mouse_interrupt:1;
		uint_t system_flag:1;
		uint_t _r0:1;
		uint_t keyboard_port_clock:1;
		uint_t mouse_port_clock:1;
		uint_t keyboard_translation:1;
		uint_t _r1:1;
	} ps2_mode;
} i8042_command_byte_t;

/*
typedef enum {
	NO_ERROR = 0x55
} i8042_self_test_t;

typedef enum {
	NO_ERROR_DETECTED,
	CLOCK_LINE_STUCK_LOW,
	CLOCK_LINE_STUCK_HIGH,
	DATA_LINE_STUCK_LOW,
	DATA_LINE_STUCK_HIGH
} I8042_INTERFACE_TEST;
*/

typedef enum {
	I8042_COMMAND_READ_COMMAND_BYTE				=0x20,	//0x20 (Read Command Byte) - Returns command byte.  (See "Write Command Byte").
	I8042_COMMAND_WRITE_COMMAND_BYTE			=0x60,	//0x60 (Write Command Byte) - Stores parameter as command byte.
														//?0x90-0x9F (Write to output port) - Writes command's lower nibble to lower nibble of output port (see Output Port definition.)
														//?0xA1 (Get version number) - Returns firmware version number.
														//?0xA4 (Get password) - Returns 0xFA if password exists; otherwise, 0xF1.
														//?0xA5 (Set password) - Set the new password by sending a null-terminated string of scan codes as this command's parameter.
														//?0xA6 (Check password) - Compares keyboard input with current password.
	I8042_COMMAND_DISABLE_MOUSE_INTERFACE		=0xA7,	//0xA7 (Disable mouse interface) - PS/2 mode only.  Similar to "Disable keyboard interface" (0xAD) command.
	I8042_COMMAND_ENABLE_MOUSE_INTERFACE		=0xA8,	//0xA8 (Enable mouse interface) - PS/2 mode only.  Similar to "Enable keyboard interface" (0xAE) command.
	I8042_COMMAND_MOUSE_INTERFACE_TEST			=0xA9,	//0xA9 (Mouse interface test) - Returns 0x00 if okay, 0x01 if Clock line stuck low, 0x02 if clock line stuck high, 0x03 if data line stuck low, and 0x04 if data line stuck high.
	I8042_COMMAND_CONTROLLER_SELF_TEST			=0xAA,	//0xAA (Controller self-test) - Returns 0x55 if okay.
	I8042_COMMAND_KEYBOARD_INTERFACE_TEST		=0xAB,	//0xAB (Keyboard interface test) - Returns 0x00 if okay, 0x01 if Clock line stuck low, 0x02 if clock line stuck high, 0x03 if data line stuck low, and 0x04 if data line stuck high.
	I8042_COMMAND_DISABLE_KEYBOARD_INTERFACE	=0xAD,	//0xAD (Disable keyboard interface) - Sets bit 4 of command byte and disables all communication with keyboard.
	I8042_COMMAND_ENABLE_KEYBOARD_INTERFACE		=0xAE,	//0xAE (Enable keyboard interface) - Clears bit 4 of command byte and re-enables communication with keyboard.
	I8042_COMMAND_GET_VERSION					=0xAF,	//0xAF (Get version)
	I8042_COMMAND_READ_INPUT_PORT				=0xC0,	//0xC0 (Read input port) - Returns values on input port (see Input Port definition.)
														//0xC1 (Copy input port LSn) - PS/2 mode only. Copy input port's low nibble to Status register (see Input Port definition)
														//0xC2 (Copy input port MSn) - PS/2 mode only. Copy input port's high nibble to Status register (see Input Port definition.)
	I8042_COMMAND_READ_OUTPUT_PORT				=0xD0,	//0xD0 (Read output port) - Returns values on output port (see Output Port definition.)
	I8042_COMMAND_WRITE_OUTPUT_PORT				=0xD1,	//0xD1 (Write output port) - Write parameter to output port (see Output Port definition.)
	I8042_COMMAND_WRITE_KEYBOARD_BUFFER			=0xD2,	//0xD2 (Write keyboard buffer) - Parameter written to input buffer as if received from keyboard.
	I8042_COMMAND_WRITE_MOUSE_BUFFER			=0xD3,	//0xD3 (Write mouse buffer) - Parameter written to input buffer as if received from mouse.
	I8042_COMMAND_WRITE_MOUSE_DEVICE			=0xD4,	//0xD4 (Write mouse Device) - Sends parameter to the auxillary PS/2 device.
	I8042_COMMAND_READ_TEST_PORT				=0xE0,	//0xE0 (Read test port) - Returns values on test port (see Test Port definition.)
														//0xF0-0xFF (Pulse output port) - Pulses command's lower nibble onto lower nibble of output port (see Output Port definition.)
} i8042_command_t;


/* read controller status byte */
extern i8042_status_t i8042_status();

/* notify controller to initialize a read or write command */
extern void i8042_command(i8042_command_t command);

/* read an initialized command */
extern byte_t i8042_read();

/* write an initialized command */
extern void i8042_write(byte_t data);

/* initialize a command and write a given value */
extern void i8042_command_set(i8042_command_t command, byte_t value);

/* initialize a command and read the returned value */
extern byte_t i8042_command_get(i8042_command_t command);

/* initialize the controller */
extern void i8042_init();

#endif /* DRIVERS_I8042_H_ */
