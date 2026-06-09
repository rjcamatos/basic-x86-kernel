/*
 * Bios Data Area
 *
 * References:
 *		IBM Personal System/2 and Personal Computer BIOS Interface Technical Reference
 * */

#ifndef BIOS_BDA_H_
#define BIOS_BDA_H_

#include "../lib/types.h"

/* Bios Data Area Base Address */
#define BDA_ADDR 0x400

/* Size of Bios Data Adrea */
#define BDA_SIZE 0xFF

/* RS-232-C Port Base Address Data Area */
typedef struct {
	word_t BaseAddrL1;	//40:00 RS-232-C Communications Line 1 Port Base Address Word
	word_t BaseAddrL2;	//40:02 RS-232-C Communications Line 2 Port Base Address Word
	word_t BaseAddrL3;	//40:04 RS-232-C Communications Line 3 Port Base Address Word
	word_t BaseAddrL4;	//40:06 RS-232-C Communications Line 4 Port Base Address Word
} __attribute__((packed)) bda_RS232CPort_t;

/* Printer Port Base Address Data Area */
typedef struct {
	word_t BaseAddrP1;	//40:08 Printer 1 Port Base Address	Word
	word_t BaseAddrP2;	//40:0A Printer 2 Port Base Address Word
	word_t BaseAddrP3;	//40:0C Printer 3 Port Base Address Word
	word_t _r0;			//40:0E Reserved Word
} __attribute__((packed)) bda_PrinterPort_t;

/* System Equipment Data Area */
typedef struct {
	//40:10 Installed Hardware Word
	uint_t NumOfPrinters:2;			//Bits 15,14 Number of Printer Adapters
	uint_t _r0:1;					//Bit 13 Reserved
	uint_t _r1:1;					//Bit 12 Reserved
	uint_t NumOfRS232:3;			//Bits 11,10,9 Number of RS-232-C Adapters
	uint_t _r2:1;					//Bit 8 Reserved
	uint_t NumOfDisketteDrivers:2;	//Bits 7,6 Number of Diskette Drives (O-based)
	uint_t VideoModeType:2;			//Bits 5,4 Video Mode Type (Values are Binary)
										//00 = Reserved
										//01 = 40x25 Color
										//10 = 80x25 Color
										//11 = 80x25 Monochrome
	uint_t _r3:1;					//Bit 3 Reserved
	uint_t PointingDevice:1;		//Bit 2 Pointing Device
	uint_t MathCoprocessor:1;		//Bit 1 Math Coprocessor
	uint_t IPLDiskette:1;			//Bit 0 IPL Diskette
} __attribute__((packed)) bda_SystemEquipment_t;

/* Miscellaneous Data Area 1 */
typedef struct {
	byte_t	_r0;		//40:12 Reserved Byte
} __attribute__((packed)) bda_Miscellaneous1_t;

/* Memory Size Data Area */
typedef struct {
	word_t MemSize;		//40:13 Memory Size in Kb (Range 0 to 640) Word
	byte_t _r0;			//40:15 to 40:16 Reserved Byte
} __attribute__((packed)) bda_MemorySize_t;

/* Keyboard Data Area 1 */
typedef struct {
	//40:17 Keyboard Control Byte
	uint_t InsertLocked:1;				//Bit 7 Insert Locked
	uint_t CapsLockLocked:1;			//Bit 6 Caps Lock Locked
	uint_t NumLockLocked:1;				//Bit 5 Num Lock Locked
	uint_t ScrollLockLocked:1;			//Bit 4 Scroll Lock Locked
	uint_t AltKeyPressed:1;				//Bit 3 Alt Key Pressed
	uint_t CtrlKeyPressed:1;			//Bit 2 Ctrl Key Pressed
	uint_t LefShiftKeyPressed:1;		//Bit 1 Left Shift Key Pressed
	uint_t RightShiftKeyPressed:1;		//Bit 0 Right Shift Key Pressed
	//40:18 Keyboard Control Byte
	uint_t InsertKeyPressed:1;			//Bit 7 Insert Key Pressed
	uint_t CapsLockKeyPressed:1;		//Bit 6 Caps Lock Key Pressed
	uint_t NumLockKeyPressed:1;			//Bit 5 Num Lock Key Pressed
	uint_t ScrollLockKeyPressed:1;		//Bit 4 Scroll Lock Key Pressed
	uint_t PauseLocked:1;				//Bit 3 Pause Locked
	uint_t SystemRequestKeyPressed:1;	//Bit 2 System Request Key Pressed
	uint_t LefAltKeyPressed:1;			//Bit 1 Left Alt Key Pressed
	uint_t LefCtrlKeyPressed:1;			//Bit 0 Left Ctrl Key Pressed
	//
	byte_t AlternateKeypadEntry;		//40:19 Alternate Keypad Entry Byte
	word_t KeyboardBufferHeadPointer;	//40:1A Keyboard Buffer Head Pointer Word
	word_t KeyboardBufferTailPointer;	//40:1C Keyboard Buffer Tail Pointer Word
	byte_t KeyboardBuffer[32];			//40:1E Keyboard Buffer 32 Bytes
} __attribute__((packed)) bda_Keyboard1_t;

/* Diskette Drive Data Area */
typedef struct {
	//40:3E Recalibrate status Byte
	uint_t InterruptFlag:1;				//Bit 7 Interrupt Flag
	uint_t _r00:1;						//Bit 6 Reserved
	uint_t _r01:1;						//Bit 5 Reserved
	uint_t _r02:1;						//Bit 4 Reserved
	uint_t RecalibrateDrive3:1;			//Bit 3 Recalibrate Drive 3
	uint_t RecalibrateDrive2:1;			//Bit 2 Recalibrate Drive 2
	uint_t RecalibrateDrive1:1;			//Bit 1 Recalibrate Drive 1
	uint_t RecalibrateDrive0:1;			//Bit 0 Recalibrate Drive 0
	//40:3F Motor Status Byte
	uint_t WriteReadOperation:1;		//Bit 7 Write/Read Operation
	uint_t _r10:1;						//Bit 6 Reserved
	uint_t DriveSelectStatus:2;			//Bits 5,4 Diskette Drive Select Status (Values in Binary)
											//00 = Diskette Drive 0 Selected
											//01 = Diskette Drive 1 Selected
											//10 = Diskette Drive 2 Selected
											//11 = Diskette Drive 3 Selected
	uint_t MotorDrive3:1;				//Bit 3 Diskette Drive 3 Motor On Status
	uint_t MotorDrive2:1;				//Bit 2 Diskette Drive 2 Motor On Status
	uint_t MotorDrive1:1;				//Bit 1 Diskette Drive 1 Motor On Status
	uint_t MotorDrive0:1;				//Bit 0 Diskette Drive 0 Motor On Status
	//
	byte_t MotorOffCounter;				//40:40 Motor off counter Byte
	byte_t LastDriveOperationStatus;	//40:41 Last Diskette Drive Operation Status Byte
											//OOH = No Error
											//01H = Invalid Diskette Drive Parameter
											//02H = Address Mark not Found
											//03H = Write-protect Error
											//04H = Requested Sector not Found
											//06H = Diskette Change Line Active
											//08H = DMA Overrun on Operation
											//09H = Attempt to DMA Across a 64Kb Boundary
											//OCH = Media Type not Found
											//10H = CRC Error on Diskette Read
											//20H = General Controller Failure
											//40H = Seek Operation Failed
											//80H = Diskette Drive not Ready
	byte_t ControllerStatus[7];			//40:42 Diskette Drive Controller Status Bytes 7 Bytes
} __attribute__((packed)) bda_DisketteDrive_t;

/* Video Control Data Area 1 */
typedef struct {
	byte_t DisplayModeSet;					//40:49 Display Mode set Byte
	word_t NumberOfColumns;					//40:4A Number of Columns Word
	word_t LengthOfRegenBuffer;				//40:4C Length of Regen Buffer in Bytes Word
	word_t StartingAddressInRegenBuffer;	//40:4E Starting Address in Regen Buffer Word
	word_t CursorPositionPage1;				//40:50 Cursor Position Page 1 Word
	word_t CursorPositionPage2;				//40:52 Cursor Position Page 2 Word
	word_t CursorPositionPage3;				//40:54 Cursor Position Page 3 Word
	word_t CursorPositionPage4;				//40:56 Cursor Position Page 4 Word
	word_t CursorPositionPage5;				//40:58 Cursor Position Page 5 Word
	word_t CursorPositionPage6;				//40:5A Cursor Position Page 6 Word
	word_t CursorPositionPage7;				//40:5C Cursor Position Page 7 Word
	word_t CursorPositionPage8;				//40:5E Cursor Position Page 8 Word
	word_t CursorType;						//40:60 Cursor Type Word
	byte_t DisplayPage;						//40:62 Display Page Byte
	word_t CRTControllerBaseAddress;		//40:63 CRT Controller Base Address Word
	byte_t CurrentSettingOf3x8Register;		//40:65 Current Setting of 3x8 Register Byte
	byte_t CurrentSettingOf3x9Register;		//40:66 Current Setting of 3x9 Register Byte
} __attribute__((packed)) bda_VideoControl1_t;

/* System Data Area 1 */
typedef struct {
	dword_t _r0;	//40:67 Reserved
	byte_t _r1;		//40:6B Reserved
} __attribute__((packed)) bda_System1_t;

/* System-Timer Data Area */
typedef struct {
	dword_t Counter;	//40:6C Timer Counter DWord
	byte_t Overflow;	//40:70 Timer Overflow Byte
} __attribute__((packed)) bda_SystemTimer_t;

/* System Data Area 2 */
typedef struct {
	byte_t BreakKeyStatus;	//40:71 Break Key State Byte
	word_t ResetFlag;		//40:72 Reset Flag Word
								//1234H = Bypass Memory Test
								//4321H = Preserve Memory (Personal System/2 products except Model 30)
								//5678H = System Suspended (PC Convertible)
								//9ABCH = Manufacturing Test Mode (PC Convertible)
								//ABCDH = System POST Loop Mode (PC Convertible)
} __attribute__((packed)) bda_System2_t;


/* Fixed Disk Drive Data Area */
typedef struct {
	byte_t LastDriveOperation;	//40:74 Last Fixed Disk Drive Operation Status Byte
									//OOH = No Error
									//01H = Invalid Function Request
									//02H = Address Mark not Found
									//03H = Write Protect Error
									//04H = Sector not Found
									//05H = Reset Failed
									//07H = Drive Parameter Activity Failed
									//08H = DMA Overrun on Operation
									//09H = Data Boundary Error
									//OAH = Bad Sector Flag Detected
									//OBH = Bad Track Detected
									//ODH = Invalid Number of Sectors on Format
									//OEH = Control Data Address Mark Detected
									//OFH = DMA Arbitration Level Out of Range
									//10H = Uncorrectable ECC or CRC Error
									//11H = ECC Corrected Data Error
									//20H = General Controller Failure
									//40H = Seek Operation Failed
									//80H = Time Out
									//AAH = Drive not Ready
									//BBH = Undefined Error Occurred
									//CCH = Write Fault on Selected Drive
									//EOH = Status Error/Error Register is 0
									//FFH = Sense Operation Failed
	byte_t NumOfDrives;			//40:75 Number of Fixed Disk Drives Attached Byte
	byte_t _r0;					//40:76 Reserved Byte
	byte_t _r1;					//40:77 Reserved Byte
} __attribute__((packed)) bda_FixedDiskDrive_t;

/* Printer Time-Out Value Data Area */
typedef struct {
	byte_t Printer1;	//40:78 Printer 1 Time-out Value Byte
	byte_t Printer2;	//40:79 Printer 2 Time-out Value Byte
	byte_t Printer3;	//40:7A Printer 3 Time-out Value Byte
	byte_t _r0;			//40:7B Reserved Byte
} __attribute__((packed)) bda_PrinterTimeoutValue_t;

/* RS-232-C Time-Out Value Data Area */
typedef struct {
	byte_t Line1;	//40:7C RS-232-C Communications Line 1 Time-out Value Byte
	byte_t Line2;	//40:70 RS-232-C Communications Line 2 Time-out Value Byte
	byte_t Line3;	//40:7E RS-232-C Communications Line 3 Time-out Value Byte
	byte_t Line4;	//40:7F RS-232-C Communications Line 4 Time-out Value Byte
} __attribute__((packed)) bda_RS232CTimeoutValue_t;

/* Keyboard Data Area 2 */
typedef struct {
	word_t BuffStartOffsetPtr;	//40:80 Keyboard Buffer Start Offset Pointer Word
	word_t BuffEndOffsetPtr;	//40:82 Keyboard Buffer End Offset Pointer Word
} __attribute__((packed)) bda_Keyboard2_t;

/* Video Control Data Area 2 */
typedef struct {
	byte_t NumberOfRows;			//40:84 Number of Rows on the Screen (Minus 1) Byte
	word_t CharacterHeight;			//40:85 Character Height (Bytes/Character) Word
	byte_t VideoControlStates1;		//40:87 Video Control States Byte
	byte_t VideoControlStates2;		//40:88 Video Control States Byte
	byte_t _r0;						//40:89 Reserved Byte
	byte_t _r1;						//40:8A Reserved Byte
} __attribute__((packed)) bda_VideoControl2_t;

/* Diskette Drive/Fixed Disk Drive Control Data Area */
typedef struct {
	//40:8B Media Control Byte
	uint_t DisketDataRateSel:2;		//Bits 7,6 Last Diskette Drive Data Rate Selected (Values inBinary)
										//00 = 500Kb Per Second
										//01 = 300Kb Per Second
										//10 = 250Kb Per Second
										//11 = Reserved
	uint_t LastDiskettDriveStep:2;	//Bits 5,4 Last Diskette Drive Step Rate Selected
	uint_t _r1:1;					//Bit 3 Reserved
	uint_t _r2:1;					//Bit 2 Reserved
	uint_t _r3:1;					//Bit 1 Reserved
	uint_t _r4:1;					//Bit 0 Reserved

	byte_t FixedDiskStatus;			//40:8C Fixed Disk Drive Controller Status Byte
	byte_t FixedDiskError;			//40:8D Fixed Disk Drive Controller Error Status Byte
	byte_t FixedDiskIntCtrl;		//40:8E Fixed Disk Drive Interrupt Control Byte
	byte_t _r5;						//40:8F Reserved Byte
	byte_t StateDrive0;				//40:90 Drive 0 Media State Byte
	byte_t StateDrive1;				//40:91 Drive 1 Media State Byte
										//Bits 7,6 Diskette Drive Data Rate (Values in Binary)
										//00 = 500Kb Per Second
										//01 = 300Kb Per Second
										//10 = 250Kb Per Second
										//11 = Reserved
										//Bit 5 Double Stepping Required
										//Bit 4 Media Established
										//Bit 3 Reserved
										//Bits 2,1,0 Drive/Media State (Values in Binary)
										//000 = 360Kb Diskette/360Kb Drive not Established
										//001 = 360Kb Diskette/1.2Mb Drive not Established
										//010 = 1.2Mb Diskette/1.2Mb Drive not Established
										//011 = 360Kb Diskette/360Kb Drive Established
										//100 = 360Kb Diskette/1.2Mb Drive Established
										//101 = 1.2Mb Diskette/1.2Mb Drive Established
										//110 = Reserved
										//111 = None of the Above
	byte_t _r6;						//40:92 Reserved Byte
	byte_t _r7;						//40:93 Reserved Byte
	byte_t CurrCylDrive0;			//40:94 Drive 0 Current Cylinder Byte
	byte_t CurrCylDrive1;			//40:95 Drive 1 Current Cylinder Byte
} __attribute__((packed)) bda_DisketOrDriveControl_t;

/* Keyboard Data Area 3 */
typedef struct {
	//40:96 Keyboard Mode State and Type Flags Byte
	uint_t Read10Progress:1;		//Bit 7 Read 10 in Progress
	uint_t LastCharFirst10Char:1;	//Bit 6 Last Character was First 10 Character
	uint_t ForceNumLockIfRead10:1;	//Bit 5 Force Num Lock if Read 10 and KBX
	uint_t Keyboard101102:1;		//Bit 4 101/102-Key Keyboard Installed
	uint_t RightAltPressed:1;		//Bit 3 Right Alt Key Pressed
	uint_t RightCtrlPressed:1;		//Bit 2 Right Ctrl Key Pressed
	uint_t LastCodeEO:1;			//Bit 1 Last Code was EO Hidden Code
	uint_t LastCodeE1:1;			//Bit 0 Last Code was E1 Hidden Code
	//40:97 Keyboard LED Flags Byte
	uint_t TransmitError:1;			//Bit 7 Keyboard Transmit Error Flag
	uint_t ModeIndicatorUpdate:1;	//Bit 6 Mode Indicator Update
	uint_t ResendReceive:1;			//Bit 5 Resend Receive Flag
	uint_t AckReceived:1;			//Bit 4 Acknowledgment Received
	uint_t _r0:1;					//Bit 3 Reserved (Must be 0)
	uint_t LedStatus:3;				//Bits 2,1,0 Keyboard LED State Bits
} __attribute__((packed)) bda_Keyboard3_t;

/* Real-Time Clock Data Area */
typedef struct {
	word_t OffsetAddr;		//40:98 Offset Address to User Wait Complete Flag Word
	word_t SegAddr;			//40:9A Segment Address to User Wait Complete Flag Word
	word_t WaitCountL;		//40:9C User Wait Count - Low Word (Microseconds) Word
	word_t WaitCountH;		//40:9E User Wait Count - High Word (Microseconds) Word
	byte_t WaitActiveFlag;	//40:AO Wait Active Flag Byte
								//Bit 7 Wait Time Elapsed and Post
								//Bits 6 to 1 Reserved
								//Bit 0 INT 15H, AH = 86H (Wait) has Occurred
	byte_t _r0[7];			//40:A1 to 40:A7 Reserved Byte
} __attribute__((packed)) bda_RealTimeClock_t;

/* Save Pointer Data Area */
typedef struct {
	dword_t PtrVideoParam;	//40:A8 Pointer to Video Parameters and Overrides DWord
} __attribute__((packed)) bda_SavePointer_t;

/* Miscellaneous Data Area 2 */
typedef struct {
	byte_t	_r0[83];			//40:AC to 40:FF Reserved Byte
	word_t	PrintScreenStatus;	//Print Screen Status Byte (INT 05H Status) Word
} __attribute__((packed)) bda_Miscellaneous2_t;

extern volatile bda_RS232CPort_t			*bda_RS232CPort;
extern volatile bda_PrinterPort_t			*bda_PrinterPort;
extern volatile bda_SystemEquipment_t		*bda_SystemEquipment;
extern volatile bda_Miscellaneous1_t		*bda_Miscellaneous1;
extern volatile bda_MemorySize_t			*bda_MemorySize;
extern volatile bda_Keyboard1_t				*bda_Keyboard1;
extern volatile bda_DisketteDrive_t			*bda_DisketteDrive;
extern volatile bda_VideoControl1_t 		*bda_VideoControl1;
extern volatile bda_System1_t				*bda_System1;
extern volatile bda_SystemTimer_t			*bda_SystemTimer;
extern volatile bda_System2_t				*bdaSystem2;
extern volatile bda_FixedDiskDrive_t		*bda_FixedDiskDrive;
extern volatile bda_PrinterTimeoutValue_t	*bda_PrinterTimeoutValue;
extern volatile bda_RS232CTimeoutValue_t	*bda_RS232CTimeoutValue;
extern volatile bda_Keyboard2_t				*bda_Keyboard2;
extern volatile bda_VideoControl2_t 		*bda_VideoControl2;
extern volatile bda_DisketOrDriveControl_t	*bda_DisketOrDriveControl;
extern volatile bda_Keyboard3_t				*bda_Keyboard3;
extern volatile bda_RealTimeClock_t			*bda_RealTimeClock;
extern volatile bda_SavePointer_t			*bda_SavePointer;
extern volatile bda_Miscellaneous2_t		*bda_Miscellaneous2;

#endif /* BIOS_BDA_H_ */
