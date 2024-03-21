macro defn name
{
	virtual at 0
		name name
	end virtual
}

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

struc EFI_TABLE_HEADER
{
	.Signature   UINT64
	.Revision    UINT32
	.HeaderSize  UINT32
	.CRC32       UINT32
	.Reserved    UINT32
}
defn EFI_TABLE_HEADER

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
defn EFI_SYSTEM_TABLE

	;; ========================================================================
	;; EFI_BOOT_SERVICES
	;; ========================================================================
EFI_BOOT_SERVICES_SIGNATURE = 0x56524553544f4f42
EFI_BOOT_SERVICES_REVISION  = EFI_SPECIFICATION_VERSION

struc EFI_BOOT_SERVICES
{
	.Hdr EFI_TABLE_HEADER
}

	;; ========================================================================
	;; EFI_RUNTIMES_SERVICES
	;; ========================================================================
EFI_RUNTIMES_SERVICES_SIGNATURE = 0x56524553544e5552
EFI_RUNTIMES_SERVICES_REVISION  = EFI_SPECIFICATION_VERSION

struc EFI_RUNTIMES_SERVICES
{
	.Hdr EFI_TABLE_HEADER
}

	;; ========================================================================
	;; EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL
	;; ========================================================================
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
defn EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL

struc SIMPLE_TEXT_OUTPUT_MODE
{
	.MaxMode       INT32
	.Mode          INT32
	.Attribute     INT32
	.CursorColumn  INT32
	.CursorRow     INT32
	.CursorVisible BOOLEAN
}
