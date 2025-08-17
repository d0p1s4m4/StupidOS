	;; File: video.inc

struc VesaInfo
{
	.Signature             dd 'VBE2'
	.Version               dw ?
	.OEMNamePtr            dd ?
	.Capabilities          dd ?
	.VideoModesPtr         dd ?
	.CountOf64KBlocks      dw ?
	.OEMSoftwareRevision   dw ?
	.OEMVendorNamePtr      dd ?
	.OEMProductNamePtr     dd ?
	.OEMProductRevisionPtr dd ?
	.Reserved              db 222 dup(?)
	.OEMData               db 256 dup(?)
}

struc VesaModeInfo
{
	.ModeAttributes    dw ?
	.WindowAAttributes db ?
	.WindowBAttributes db ?
	.WindowGranularity dw ?
	.WindowSize        dw ?
	.SegmentWindowA    dw ?
	.SegmentWindowB    dw ?
	.WindowPositioning dd ?
	.BytesPerScanLine  dw ?

	; --
	.Width            dw ?
	.Height           dw ?
	.WidthChar        db ?
	.HeightChar       db ?
	.PlanesCount      db ?
	.BitsPerPixel     db ?
	.BanksCount       db ?
	.MemoryModel      db ?
	.BankSize         db ?
	.ImagePagesCount  db ?
	.Reserved         db ?

	.RedMaskSize          db ?
	.RedFieldPosition     db ?
	.GreenMaskSize        db ?
	.GreenFieldPosition   db ?
	.BlueMaskSize         db ?
	.BlueFieldPosition    db ?
	.ReservedMaskSize     db ?
	.ReservedMaskPosition db ?
	.DirectColorModeInfo  db ?

	; -- VBE v1.2+
	.Framebuffer           dd ?
	.OffScreenMemoryOffset dd ?
	.OffScreenMemorySize   dd ?
	.Reserved2             db 206 dup(?)
}

	;; Function: video_setup
video_setup:
	mov si, szMsgDetectVideo
	call bios_log
	clc
	xor ax, ax
	mov es, ax
	mov di, vesa_block_buffer
	mov ax, 0x4F00
	int 0x10
	cmp ax, 0x004F
	jne .err
	cmp [vesa_block_buffer.Signature], 'VESA'
	jne .err
	; XXX: fix me
	push word [vesa_block_buffer.OEMProductNamePtr]
	push word [vesa_block_buffer.OEMVendorNamePtr]
	push word [vesa_block_buffer.CountOf64KBlocks]
	push word [vesa_block_buffer.OEMNamePtr]
	xor ecx, ecx
	mov cx, [vesa_block_buffer.Version]
	push ecx
	mov si, szMsgVesaInfo
	call bios_log

	mov di, vesa_info_block_buffer
	mov ebx, [vesa_block_buffer.VideoModesPtr]
	mov cx, [bx]
	cmp cx, 0xFFFF
	je .err

	clc
	mov ax, 0x4F01
	int 0x10
	cmp ax, 0x004F
	jne .err

	push dword [vesa_info_block_buffer.Framebuffer]
	mov si, szMsgFramebuffer
	call bios_log
	ret
.err:
	mov si, szMsgErrorVesa
	call bios_log
	stc
	ret

align 4
vesa_block_buffer VesaInfo
vesa_info_block_buffer VesaModeInfo

szMsgVesaInfo db "Version: %x", CR, LF, "OEM Name: %s", CR, LF, "Total Memory: %d", CR, LF, "Vendor name: %s", CR, LF, "Product name: %s", 0
szMsgVesaModeInfo db "%dx%dx%d", CR, LF, 0
szMsgDetectVideo db "Fetch video information.", 0
szMsgFramebuffer db "Fb: %x", 0
szMsgErrorVesa db "Failed to detect VBE mode", 0
