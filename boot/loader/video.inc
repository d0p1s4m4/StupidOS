struc VesaInfo
{
	.Signature             dd ?
	.Version               dw ?
	.OEMNamePtr            dd ?
	.Capabilities          dd ?
	.VideoModesOffset      dw ?
	.VideoModesSegment     dw ?
	.CountOf64KBlocks      dw ?
	.OEMSoftwareRevision   dw ?
	.OEMVendorNamePtr      dd ?
	.OEMProductNamePtr     dd ?
	.OEMProductRevisionPtr dd ?
	.Reserved              db 222 dup(?)
	.OEMData               db 256 dup(?)
}
defn VesaInfo

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
defn VesaModeInfo

video_setup:
	clc
	mov di, [vesa_block_buffer]
	mov ax, 0x4F00
	int 0x10
	cmp ax, 0x004F
	jne .err

	push word [vesa_block_buffer + VesaInfo.VideoModesSegment]
	pop es
	mov di, vesa_info_block_buffer
	mov bx, [vesa_block_buffer + VesaInfo.VideoModesOffset]
	mov cx, [bx]
	cmp cx, 0xFFFF
	je .err

	mov ax, 0x4F01
	int 0x10
	cmp ax, 0x004F
	jne .err
	ret
.err:
	stc
	ret

vesa_block_buffer VesaInfo
vesa_info_block_buffer VesaModeInfo
