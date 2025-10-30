	;; File: uefi.inc

	;; Macro: EFI_CALL
macro EFI_CALL fn, [arg] {
	common
		_ARGS = 0
	reverse
		match anything, arg \{
			_ARGS = _ARGS + 4
			push dword arg
		\}
	common
		call fn
		if _ARGS
			add esp, _ARGS
		end if
}

	;; Macro: EFI_GET_INTERFACE
macro EFI_GET_INTERFACE reg, interface {
	if interface in <ConsoleInHandle,ConIn,ConsOutHandle,ConOut,StandardErrorHandle,StdErr>
		mov reg, [pEfiSystemTable]
		mov reg, [reg + EFI_SYSTEM_TABLE.#interface]
	end if
}

	;; Macro: EFI_INIT
macro EFI_INIT handle, table {
	local efi_init_bad
	local efi_init_end
	clc
	; image handle
	mov eax, handle
	or eax, eax
	jz efi_init_bad
	mov [hEfiImage], eax
	mov eax, table
	mov [pEfiSystemTable], eax

	mov edx, [eax + EFI_SYSTEM_TABLE.BootServices]
	mov [pEfiBootServices], edx

	mov edx, [eax + EFI_SYSTEM_TABLE.RuntimeServices]
	mov [pEfiRuntimeServices], edx

	jmp efi_init_end
efi_init_bad:
	stc
efi_init_end:
}

EFI_SUCCESS           = 0x0
EFI_ERR               = 0x80000000

EFI_LOAD_ERROR           = (EFI_ERR or 1)
EFI_INVALID_PARAMETER    = (EFI_ERR or 2)
EFI_UNSUPPORTED          = (EFI_ERR or 3)
EFI_BAD_BUFFER_SIZE      = (EFI_ERR or 4)
EFI_BUFFER_TOO_SMALL     = (EFI_ERR or 5)
EFI_NOT_READY            = (EFI_ERR or 6)
EFI_DEVICE_ERROR         = (EFI_ERR or 7)
EFI_WRITE_PROTECTED      = (EFI_ERR or 8)
EFI_OUT_OF_RESOURCEs     = (EFI_ERR or 9)
EFI_VOLUME_CORRUPTED     = (EFI_ERR or 10)
EFI_VOLUME_FULL          = (EFI_ERR or 11)
EFI_NO_MEDIA             = (EFI_ERR or 12)
EFI_MEDIA_CHANGED        = (EFI_ERR or 13)
EFI_NOT_FOUND            = (EFI_ERR or 14)
EFI_ACCESS_DEBIED        = (EFI_ERR or 15)
EFI_NO_RESPONSE          = (EFI_ERR or 16)
EFI_NO_MAPPING           = (EFI_ERR or 17)
EFI_TIMEOUT              = (EFI_ERR or 18)
EFI_NOT_STARTED          = (EFI_ERR or 19)
EFI_ALREADY_STARTED      = (EFI_ERR or 20)
EFI_ABORTED              = (EFI_ERR or 21)
EFI_ICMP_ERROR           = (EFI_ERR or 22)
EFI_TFTP_ERROR           = (EFI_ERR or 23)
EFI_PROTOCOL_ERROR       = (EFI_ERR or 24)
EFI_INCOMPATIBLE_VERSION = (EFI_ERR or 25)
EFI_SECURITY_VIOLATION   = (EFI_ERR or 26)
EFI_CRC_ERROR            = (EFI_ERR or 27)
EFI_END_OF_MEDIA         = (EFI_ERR or 28)
EFI_END_OF_FILE          = (EFI_ERR or 31)
EFI_INVALID_LANGUAGE     = (EFI_ERR or 32)
EFI_COMPROMISED_DATA     = (EFI_ERR or 33)
EFI_IP_ADDRESS_CONFLICT  = (EFI_ERR or 34)
EFI_HTTP_ERROR           = (EFI_ERR or 35)

struc BOOLEAN
{
	. db ?
}

struc UINT64
{
	align 8
	. dq ?
}

struc UINT32
{
	align 4
	. dd ?
}

struc INT32
{
	align 4
	. dd ?
}

struc UINT16
{
	align 2
	. dw ?
}

struc INT16
{
	align 2
	. dw ?
}

struc UINT8
{
	. db ?
}

struc UINTN
{
	align 4
	. dd ?
}

struc EFI_HANDLE
{
	align 4
	. dd ?
}

struc UINTPTR
{
	align 4
	. dd ?
}

struc EFI_TIME
{
	.Year       UINT16
	.Month      UINT8
	.Day        UINT8
	.Hour       UINT8
	.Minute     UINT8
	.Second     UINT8
	.Pad1       UINT8
	.Nanosecond UINT32
	.TimeZone   INT16
	.Daylight   UINT8
	.Pad2       UINT8
}
DEFN EFI_TIME

struc EFI_TABLE_HEADER
{
	.Signature   UINT64
	.Revision    UINT32
	.HeaderSize  UINT32
	.CRC32       UINT32
	.Reserved    UINT32
}
DEFN EFI_TABLE_HEADER

	;; ========================================================================
	;; EFI_SYSTEM_TABLE
	;; ========================================================================
EFI_SYSTEM_TABLE_SIGNATURE = 0x5453595320494249

EFI_2_90_SYSTEM_TABLE_REVISION = ((2 shl 16) or (90))
EFI_2_80_SYSTEM_TABLE_REVISION = ((2 shl 16) or (80))
EFI_2_70_SYSTEM_TABLE_REVISION = ((2 shl 16) or (70))
EFI_2_60_SYSTEM_TABLE_REVISION = ((2 shl 16) or (60))
EFI_2_50_SYSTEM_TABLE_REVISION = ((2 shl 16) or (50))
EFI_2_40_SYSTEM_TABLE_REVISION = ((2 shl 16) or (40))
EFI_2_31_SYSTEM_TABLE_REVISION = ((2 shl 16) or (31))
EFI_2_30_SYSTEM_TABLE_REVISION = ((2 shl 16) or (30))
EFI_2_20_SYSTEM_TABLE_REVISION = ((2 shl 16) or (20))
EFI_2_10_SYSTEM_TABLE_REVISION = ((2 shl 16) or (10))
EFI_2_00_SYSTEM_TABLE_REVISION = ((2 shl 16) or (00))
EFI_1_10_SYSTEM_TABLE_REVISION = ((1 shl 16) or (10))
EFI_1_02_SYSTEM_TABLE_REVISION = ((1 shl 16) or (02))

EFI_SYSTEM_TABLE_REVISION = EFI_2_90_SYSTEM_TABLE_REVISION
EFI_SPECIFICATION_VERSION = EFI_SYSTEM_TABLE_REVISION

	;; Struct: EFI_SYSTEM_TABLE
struc EFI_SYSTEM_TABLE
{
	.Hdr                  EFI_TABLE_HEADER
	.FirmwareVendor       UINTPTR
	.FirmwareRevision     UINT32
	.ConsoleInHandle      EFI_HANDLE
	.ConIn                UINTPTR
	.ConsoleOutHandle     EFI_HANDLE
	.ConOut               UINTPTR
	.StandardErrorHandle  EFI_HANDLE
	.StdErr               UINTPTR
	.RuntimeServices      UINTPTR
	.BootServices         UINTPTR
	.NumberOfTableEntries UINTN
	.ConfigurationTable   UINTPTR
}
DEFN EFI_SYSTEM_TABLE

	;; ========================================================================
	;; EFI_BOOT_SERVICES
	;; ========================================================================
EFI_BOOT_SERVICES_SIGNATURE = 0x56524553544f4f42
EFI_BOOT_SERVICES_REVISION  = EFI_SPECIFICATION_VERSION

	;; Struct: EFI_BOOT_SERVICES
struc EFI_BOOT_SERVICES
{
	.Hdr           EFI_TABLE_HEADER

	; Task Priority Services
	.RaiseTPL      UINTPTR
	.RestoreTPL    UINTPTR

	; Memory Services
	.AllocatePages UINTPTR
	.FreePages     UINTPTR
	.GetMemoryMap  UINTPTR
	.AllocatePool  UINTPTR
	.FreePool      UINTPTR

	; Event & Timer Services
	.CreateEvent   UINTPTR
	.SetTimer      UINTPTR
	.WaitForEvent  UINTPTR
	.SignalEvent   UINTPTR
	.CloseEvent    UINTPTR
	.CheckEvent    UINTPTR

	; Protocol Handler Services
	.InstallProtocolInterface   UINTPTR
	.ReinstallProtocolInterface UINTPTR
	.UninstallProtocolInterface UINTPTR
	.HandleProtocol             UINTPTR
	.Reserved                   UINTPTR
	.RegisterProtocolNotify     UINTPTR
	.LocateHandle               UINTPTR
	.LocateDevicePath           UINTPTR
	.InstallConfigurationTable  UINTPTR

	; Image Services
	.LoadImage        UINTPTR
	.StartImage       UINTPTR
	.Exit             UINTPTR
	.UnloadImage      UINTPTR
	.ExitBootServices UINTPTR

	; Misc Services
	.GetNextMonotonicCount UINTPTR
	.Stall                 UINTPTR
	.SetWatchdogTimer      UINTPTR

	; DriverSupport Service
	.ConnectController    UINTPTR
	.DisconnectController UINTPTR

	; Open and Close Protocol Services
	.OpenProtocol            UINTPTR
	.CloseProtocol           UINTPTR
	.OpenProtocolInformation UINTPTR

	; Library Services
	.ProtocolsPerHandle                  UINTPTR
	.LocateHandleBuffer                  UINTPTR
	.LocateProtocol                      UINTPTR
	.InstallMultipleProtocolInterfaces   UINTPTR
	.UninstallMultipleProtocolInterfaces UINTPTR

	; 32bit CRC Services
	.CalculateCrc32 UINTPTR

	; Misc Services
	.CopyMem       UINTPTR
	.SetMem        UINTPTR
	.CreateEventEx UINTPTR
}
DEFN EFI_BOOT_SERVICES

EFI_RESERVED_MEMORY_TYPE        = 0
EFI_LOADER_CODE                 = 1
EFI_LOADER_DATA                 = 2
EFI_BOOT_SERVICES_CODE          = 3
EFI_BOOT_SERVICES_DATA          = 4
EFI_RUNTIME_SERVICES_CODE       = 5
EFI_RUNTIME_SERVICES_DATA       = 6
EFI_CONVENTIONAL_MEMORY         = 7
EFI_UNUSABLE_MEMORY             = 8
EFI_ACPI_RECLAIM_MEMORY         = 9
EFI_ACPI_MEMORY_NVS             = 10
EFI_MEMORY_MAPPED_IO            = 11
EFI_MEMORY_MAPPED_IO_PORT_SPACE = 12
EFI_PAL_CODE                    = 13
EFI_PERSISTENT_MEMORY           = 14
EFI_UNACCEPTED_MEMORY_TYPE      = 15
EFI_MAX_MEMORY_TYPE             = 16

EFI_LOCATE_SEARCH_ALL_HANDLES        = 0x0
EFI_LOCATE_SEARCH_BY_REGISTER_NOTIFY = 0x1
EFI_LOCATE_SEARCH_BY_PROTOCOL        = 0x2

EFI_OPEN_PROTOCOL_BY_HANDLE_PROTOCOL  = 0x00000001
EFI_OPEN_PROTOCOL_GET_PROTOCOL        = 0x00000002
EFI_OPEN_PROTOCOL_TEST_PROTOCOL       = 0x00000004
EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER = 0x00000008
EFI_OPEN_PROTOCOL_BY_DRIVER           = 0x00000010
EFI_OPEN_PROTOCOL_EXCLUSIVE           = 0x00000020

struc EFI_MEMORY_DESCRIPTOR {
	.type UINT32
	.physical_start UINT64
	.virtual_start  UINT64
	.number_of_pages UINT64
	.attribute UINT64
}

EFI_MEMORY_DESCRIPTOR_VERSION = 1

EFI_MEMORY_UC            = 0x0000000000000001
EFI_MEMORY_WC            = 0x0000000000000002
EFI_MEMORY_WT            = 0x0000000000000004
EFI_MEMORY_WB            = 0x0000000000000008
EFI_MEMORY_UCE           = 0x0000000000000010
EFI_MEMORY_WP            = 0x0000000000001000
EFI_MEMORY_RP            = 0x0000000000002000
EFI_MEMORY_XP            = 0x0000000000004000
EFI_MEMORY_NV            = 0x0000000000008000
EFI_MEMORY_MORE_RELIABLE = 0x0000000000010000
EFI_MEMORY_RO            = 0x0000000000020000
EFI_MEMORY_SP            = 0x0000000000040000
EFI_MEMORY_CPU_CRYPTO    = 0x0000000000080000
EFI_MEMORY_RUNTIME       = 0x8000000000000000
	;; ========================================================================
	;; EFI_RUNTIMES_SERVICES
	;; ========================================================================
EFI_RUNTIMES_SERVICES_SIGNATURE = 0x56524553544e5552
EFI_RUNTIMES_SERVICES_REVISION  = EFI_SPECIFICATION_VERSION

	;; Struct: EFI_RUNTIMES_SERVICES
struc EFI_RUNTIMES_SERVICES
{
	.Hdr EFI_TABLE_HEADER

	; Time Services
	.GetTime       UINTPTR
	.SetTime       UINTPTR
	.GetWakeupTime UINTPTR
	.SetWakeupTime UINTPTR

	; Virtual Memory Services
	.SetVirtualAddressMap UINTPTR
	.ConvertPointer       UINTPTR

	; Variable Services
	.GetVariable         UINTPTR
	.GetNextVariableName UINTPTR
	.SetVariable         UINTPTR

	; Misc Services


}
DEFN EFI_RUNTIMES_SERVICES

	;; ========================================================================
	;; EFI_LOADED_IMAGE_PROTOCOL
	;; ========================================================================
EFI_LOADED_IMAGE_PROTOCOL_GUID equ 0x5B1B31A1, 0x9562, 0x11d2, 0x8E, 0x3F, 0x00, 0xA0, 0xC9, 0x69, 0x72, 0x3D
EFI_LOADED_IMAGE_PROTOCOL_REVISION = 0x1000

	;; Struct: EFI_LOADED_IMAGE_PROTOCOL
struc EFI_LOADED_IMAGE_PROTOCOL
{
	.Revision       UINT32
	.ParentHandle   EFI_HANDLE
	.SystemTable    UINTPTR

	.DeviceHandle   EFI_HANDLE
	.FilePath       UINTPTR
	.Reserved       UINTPTR

	.LoadOptionsSize UINT32
	.LoadOptions     UINTPTR

	.ImageBase       UINTPTR
	.ImageSize       UINT64
	.ImageCodeType   UINT32
	.ImageDataType   UINT32
	.Unload          UINTPTR
}
DEFN EFI_LOADED_IMAGE_PROTOCOL

	;; ========================================================================
	;; EFI_DEVICE_PATH_PROTOCOL
	;; ========================================================================
EFI_DEVICE_PATH_PROTOCOL_GUID equ 0x09576e91, 0x6d3f, 0x11d2, 0x8e, 0x39, 0x00, 0xa0, 0x69, 0x72, 0x3b

struc EFI_DEVICE_PATH_PROTOCOL
{
	.Type    UINT8
	.SubType UINT8
	.Length  db 2 dup(?)
}
DEFN EFI_DEVICE_PATH_PROTOCOL

	;; ========================================================================
	;; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
	;; ========================================================================
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL_GUID equ 0x387477c2, 0x69c7, 0x11d2, 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b

struc EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
{
	.Reset             UINTPTR
	.OutputString      UINTPTR
	.TestString        UINTPTR
	.QueryMode         UINTPTR
	.SetMode           UINTPTR
	.SetAttribute      UINTPTR
	.ClearScreen       UINTPTR
	.SetCursorPosition UINTPTR
	.EnableCursor      UINTPTR
	.Mode              UINTPTR
}
DEFN EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL

struc SIMPLE_TEXT_OUTPUT_MODE
{
	.MaxMode       INT32
	.Mode          INT32
	.Attribute     INT32
	.CursorColumn  INT32
	.CursorRow     INT32
	.CursorVisible BOOLEAN
}

EFI_BLACK        = 0x00
EFI_BLUE         = 0x01
EFI_GREEN        = 0x02
EFI_CYAN         = 0x03
EFI_RED          = 0x04
EFI_MAGENTA      = 0x05
EFI_BROWN        = 0x06
EFI_LIGHTGRAY    = 0x07
EFI_BRIGHT       = 0x08
EFI_DARKGRAY     = (EFI_BLACK or EFI_BRIGHT)
EFI_LIGHTBLUE    = 0x09
EFI_LIGHTGREEN   = 0x0A
EFI_LIGHTCYAN    = 0x0B
EFI_LIGHTRED     = 0x0C
EFI_LIGHTMAGENTA = 0x0D
EFI_YELLOW       = 0x0E
EFI_WHITE        = 0x0F

EFI_BACKGROUND_BLACK     = 0x00
EFI_BACKGROUND_BLUE      = 0x10
EFI_BACKGROUND_GREEN     = 0x20
EFI_BACKGROUND_CYAN      = 0x30
EFI_BACKGROUND_RED       = 0x40
EFI_BACKGROUND_MAGENTA   = 0x50
EFI_BACKGROUND_BROWN     = 0x60
EFI_BACKGROUND_LIGHTGRAY = 0x70

	;; ========================================================================
	;; EFI_LOAD_FILE_PROTOCOL
	;; ========================================================================
EFI_LOAD_FILE_PROTOCOL_GUID equ 0x56ec3091, 0x954c, 0x11d2, 0x8e, 0x3f, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b

struc EFI_LOAD_FILE_PROTOCOL
{
	.LoadFile UINTPTR
}
DEFN EFI_LOAD_FILE_PROTOCOL


	;; ========================================================================
	;; EFI_LOAD_FILE2_PROTOCOL
	;; ========================================================================
EFI_LOAD_FILE2_PROTOCOL_GUID equ 0x4006c0c1, 0xfcb3, 0x403e, 0x99, 0x6d, 0x4a, 0x6c, 0x87, 0x24, 0xe0, 0x6d

EFI_LOAD_FILE2_PROTOCOL equ EFI_LOAD_FILE_PROTOCOL

	;; ========================================================================
	;; EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
	;; ========================================================================
EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID equ 0x22, 0x5b, 0x4e, 0x96, 0x59, 0x64, 0xd2, 0x11, 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b
;EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_GUID equ 0x0964e5b22, 0x6459, 0x11d2, 0x8e, 0x39, 0x00, 0xa0, 0xc9, 0x69, 0x72, 0x3b

EFI_SIMPLE_FILE_SYSTEM_PROTOCOL_REVISION = 0x00010000

struc EFI_SIMPLE_FILE_SYSTEM_PROTOCOL
{
	.Revision   UINT64
	.OpenVolume UINTPTR
}
DEFN EFI_SIMPLE_FILE_SYSTEM_PROTOCOL

	;; ========================================================================
	;; EFI_FILE_PROTOCOL
	;; ========================================================================

EFI_FILE_PROTOCOL_REVISION        = 0x00010000
EFI_FILE_PROTOCOL_REVISION2       = 0x00020000
EFI_FILE_PROTOCOL_LATEST_REVISION = EFI_FILE_PROTOCOL_REVISION2

struc EFI_FILE_PROTOCOL
{
	.Revision    UINT64
	.Open        UINTPTR
	.Close       UINTPTR
	.Delete      UINTPTR
	.Read        UINTPTR
	.Write       UINTPTR
	.GetPosition UINTPTR
	.SetPosition UINTPTR
	.GetInfo     UINTPTR
	.SetInfo     UINTPTR
	.Flush       UINTPTR
	.OpenEx      UINTPTR
	.ReadEx      UINTPTR
	.WriteEx     UINTPTR
	.FlushEx     UINTPTR
}
DEFN EFI_FILE_PROTOCOL

	; Open Mode
EFI_FILE_MODE_READ   = 0x0000000000000001
EFI_FILE_MODE_WRITE  = 0x0000000000000002
EFI_FILE_MODE_CREATE = 0x8000000000000000

	; File Attributes
EFI_FILE_READ_ONLY  = 0x0000000000000001
EFI_FILE_HIDDEN     = 0x0000000000000002
EFI_FILE_SYSTEM     = 0x0000000000000004
EFI_FILE_RESERVED   = 0x0000000000000008
EFI_FILE_DIRECTORY  = 0x0000000000000010
EFI_FILE_ARCHIVE    = 0x0000000000000020
EFI_FILE_VALID_ATTR = 0x0000000000000037

section '.data' data readable writeable

hEfiImage           dd 0
pEfiSystemTable     dd 0
pEfiBootServices    dd 0
pEfiRuntimeServices dd 0
