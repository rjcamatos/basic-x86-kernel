#include "bda.h"


volatile bda_RS232CPort_t			*bda_RS232CPort				= (voidptr_t)(BDA_ADDR|0x00);
volatile bda_PrinterPort_t			*bda_PrinterPort			= (voidptr_t)(BDA_ADDR|0x08);
volatile bda_SystemEquipment_t		*bda_SystemEquipment		= (voidptr_t)(BDA_ADDR|0x10);
volatile bda_Miscellaneous1_t		*bda_Miscellaneous1			= (voidptr_t)(BDA_ADDR|0x12);
volatile bda_MemorySize_t			*bda_MemorySize				= (voidptr_t)(BDA_ADDR|0x13);
volatile bda_Keyboard1_t			*bda_Keyboard1				= (voidptr_t)(BDA_ADDR|0x17);
volatile bda_DisketteDrive_t		*bda_DisketteDrive			= (voidptr_t)(BDA_ADDR|0x3E);
volatile bda_VideoControl1_t 		*bda_VideoControl1			= (voidptr_t)(BDA_ADDR|0x49);
volatile bda_System1_t				*bda_System1				= (voidptr_t)(BDA_ADDR|0x67);
volatile bda_SystemTimer_t			*bda_SystemTimer			= (voidptr_t)(BDA_ADDR|0x6C);
volatile bda_System2_t				*bda_System2				= (voidptr_t)(BDA_ADDR|0x71);
volatile bda_FixedDiskDrive_t		*bda_FixedDiskDrive			= (voidptr_t)(BDA_ADDR|0x74);
volatile bda_PrinterTimeoutValue_t	*bda_PrinterTimeoutValue	= (voidptr_t)(BDA_ADDR|0x78);
volatile bda_RS232CTimeoutValue_t	*bda_RS232CTimeoutValue		= (voidptr_t)(BDA_ADDR|0x7C);
volatile bda_Keyboard2_t			*bda_Keyboard2				= (voidptr_t)(BDA_ADDR|0x80);
volatile bda_VideoControl2_t 		*bda_VideoControl2			= (voidptr_t)(BDA_ADDR|0x84);
volatile bda_DisketOrDriveControl_t	*bda_DisketOrDriveControl	= (voidptr_t)(BDA_ADDR|0x8B);
volatile bda_Keyboard3_t			*bda_Keyboard3				= (voidptr_t)(BDA_ADDR|0x96);
volatile bda_RealTimeClock_t		*bda_RealTimeClock			= (voidptr_t)(BDA_ADDR|0x98);
volatile bda_SavePointer_t			*bda_SavePointer			= (voidptr_t)(BDA_ADDR|0xA8);
volatile bda_Miscellaneous2_t		*bda_Miscellaneous2			= (voidptr_t)(BDA_ADDR|0xAC);

