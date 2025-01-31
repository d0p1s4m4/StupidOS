#include <stdlib.h>
#include <stdio.h>

#include "coff.h"
#include "ld.h"

int
coff_output(LDState *state, FILE *fp)
{
	FILHDR file_header;
	AOUTHDR opt_header;

	file_header.f_magic = F_MACH_I386;
	file_header.f_timdat = 0;
	file_header.f_opthdr = AOUTHSZ;
	file_header.f_flags = F_RELFLG | F_EXEC | F_LNNO | F_LSYMS | F_LITTLE;

	opt_header.magic = ZMAGIC;
	opt_header.vstamp = 0;

	fwrite(&file_header, FILHSZ, 1, fp);
	fwrite(&opt_header, AOUTHSZ, 1, fp);

	return (0);
}

int coff_load(LDState *state, FILE *fp)
{
	FILHDR file_header;
	AOUTHDR opt_header;

	fread(&file_header, FILHSZ, 1, fp);
	
	return (0);
}
