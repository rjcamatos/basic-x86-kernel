/** COMMENTS By GEMINI */

/**
 * Intel 8042 PS/2 Controller Driver Header
 * 
 * Defines registers, status/command structures, and operational 
 * commands for handling legacy keyboard and mouse interfaces.
 */

#ifndef KERNEL_DRIVERS_I8042_H_
#define KERNEL_DRIVERS_I8042_H_

#include "types.h"
#include "io.h"
#include "idt.h"
#include "printk.h"

// Controller Status Register flags
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

// Controller Configuration/Command Byte flags
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


typedef enum {
    I8042_COMMAND_READ_COMMAND_BYTE             = 0x20, // Reads controller command byte
    I8042_COMMAND_WRITE_COMMAND_BYTE            = 0x60, // Writes controller command byte
    I8042_COMMAND_DISABLE_MOUSE_INTERFACE       = 0xA7, // Disables PS/2 mouse port
    I8042_COMMAND_ENABLE_MOUSE_INTERFACE        = 0xA8, // Enables PS/2 mouse port
    I8042_COMMAND_MOUSE_INTERFACE_TEST          = 0xA9, // Tests mouse port interface
    I8042_COMMAND_CONTROLLER_SELF_TEST          = 0xAA, // Controller self-test (returns 0x55 if OK)
    I8042_COMMAND_KEYBOARD_INTERFACE_TEST       = 0xAB, // Tests keyboard port interface
    I8042_COMMAND_DIAGNOSTIC_DUMP               = 0xAC, // Dumps controller diagnostics
    I8042_COMMAND_DISABLE_KEYBOARD_INTERFACE    = 0xAD, // Disables PS/2 keyboard port
    I8042_COMMAND_ENABLE_KEYBOARD_INTERFACE     = 0xAE, // Enables PS/2 keyboard port
    I8042_COMMAND_GET_VERSION                   = 0xAF, // Gets firmware version
    I8042_COMMAND_READ_INPUT_PORT               = 0xC0, // Reads controller input port
    I8042_COMMAND_COPY_INPUT_PORT_LSN           = 0xC1, // Copies input low nibble to status register
    I8042_COMMAND_COPY_INPUT_PORT_MSN           = 0xC2, // Copies input high nibble to status register
    I8042_COMMAND_READ_OUTPUT_PORT              = 0xD0, // Reads controller output port
    I8042_COMMAND_WRITE_OUTPUT_PORT             = 0xD1, // Writes controller output port
    I8042_COMMAND_WRITE_KEYBOARD_BUFFER         = 0xD2, // Writes to keyboard output buffer
    I8042_COMMAND_WRITE_MOUSE_BUFFER            = 0xD3, // Writes to mouse output buffer
    I8042_COMMAND_WRITE_MOUSE_DEVICE            = 0xD4, // Sends next byte directly to mouse
    I8042_COMMAND_READ_TEST_PORT                = 0xE0, // Reads test lines
    I8042_COMMAND_PULSE_OUTPUT_PORT_BASE        = 0xF0, // Pulse output port base (0xF0-0xFF)
    I8042_COMMAND_PULSE_SYSTEM_RESET            = 0xFE  // Pulses CPU reset line (reboots system)
} i8042_command_t;


typedef enum {
    I8042_KEYBOARD_COMMAND_SET_LEDS             = 0xED, // Sets keyboard LEDs status
    I8042_KEYBOARD_COMMAND_ECHO                 = 0xEE, // Keyboard echo diagnostic (returns 0xEE)
    I8042_KEYBOARD_COMMAND_SET_SCAN_CODE_SET    = 0xF0, // Gets/Sets Scan Code set (1, 2 or 3)
    I8042_KEYBOARD_COMMAND_READ_ID              = 0xF2, // Reads keyboard ID
    I8042_KEYBOARD_COMMAND_SET_TYPEMATIC        = 0xF3, // Sets typematic delay and rate
    I8042_KEYBOARD_COMMAND_ENABLE_SCANNING      = 0xF4, // Enables scanning (starts key reporting)
    I8042_KEYBOARD_COMMAND_DISABLE_SCANNING     = 0xF5, // Disables scanning and clears buffer
    I8042_KEYBOARD_COMMAND_SET_DEFAULTS         = 0xF6, // Resets keyboard defaults
    I8042_KEYBOARD_COMMAND_ALL_TYPEMATIC        = 0xF7, // Sets all keys to typematic (Set 3 only)
    I8042_KEYBOARD_COMMAND_ALL_MAKE_BREAK       = 0xF8, // Sets all keys to make/break (Set 3 only)
    I8042_KEYBOARD_COMMAND_ALL_MAKE             = 0xF9, // Sets all keys to make only (Set 3 only)
    I8042_KEYBOARD_COMMAND_ALL_TYPEMATIC_MB     = 0xFA, // Sets all keys to typematic/make/break
    I8042_KEYBOARD_COMMAND_RESEND               = 0xFE, // Requests resend of last byte
    I8042_KEYBOARD_COMMAND_RESET                = 0xFF  // Resets keyboard and runs BAT self-test
} i8042_keyboard_command_t;

typedef enum {
    I8042_MOUSE_COMMAND_SET_SCALING_1_1         = 0xE6, // Sets mouse scaling to 1:1
    I8042_MOUSE_COMMAND_SET_SCALING_2_1         = 0xE7, // Sets mouse scaling to 2:1
    I8042_MOUSE_COMMAND_SET_RESOLUTION          = 0xE8, // Sets mouse resolution
    I8042_MOUSE_COMMAND_STATUS_REQUEST          = 0xE9, // Requests 3-byte status packet
    I8042_MOUSE_COMMAND_SET_STREAM_MODE         = 0xEA, // Enables stream mode (auto data transmission)
    I8042_MOUSE_COMMAND_READ_DATA               = 0xEB, // Requests data packet in remote mode
    I8042_MOUSE_COMMAND_RESET_WRAP_MODE         = 0xEC, // Exits wrap mode test
    I8042_MOUSE_COMMAND_SET_WRAP_MODE           = 0xEE, // Enters wrap mode test
    I8042_MOUSE_COMMAND_SET_REMOTE_MODE         = 0xF0, // Enters polled remote mode
    I8042_MOUSE_COMMAND_READ_ID                 = 0xF2, // Reads mouse ID (0x00=Standard, 0x03=Scroll wheel)
    I8042_MOUSE_COMMAND_SET_SAMPLE_RATE         = 0xF3, // Sets mouse sampling rate (Hz)
    I8042_MOUSE_COMMAND_ENABLE_DATA_REPORTING   = 0xF4, // Enables data reporting (Stream mode start)
    I8042_MOUSE_COMMAND_DISABLE_DATA_REPORTING  = 0xF5, // Disables data reporting
    I8042_MOUSE_COMMAND_SET_DEFAULTS            = 0xF6, // Resets mouse defaults
    I8042_MOUSE_COMMAND_RESEND                  = 0xFE, // Requests resend of last byte
    I8042_MOUSE_COMMAND_RESET                   = 0xFF  // Resets mouse and runs self-test
} i8042_mouse_command_t;


/* Function Prototypes */

// Reads the controller status register byte
extern i8042_status_t i8042_status();

// Sends a command to the controller command port
extern void i8042_command(i8042_command_t command);

// Reads a byte from the controller data port
extern byte_t i8042_read();

// Writes a byte to the controller data port
extern void i8042_write(byte_t data);

// Sends a command and then writes an associated data value
extern void i8042_command_set(i8042_command_t command, byte_t value);

// Sends a command and reads the returned data byte
extern byte_t i8042_command_get(i8042_command_t command);

// Initializes the i8042 controller subsystem
extern void i8042_init();

#endif /* DRIVERS_I8042_H_ */
