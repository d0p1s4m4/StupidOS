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
	ret
