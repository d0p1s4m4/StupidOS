#!/usr/bin/env bash
set -e

: "${IMG=stupidos_efi.img}"
: "${EFI_PART_SIZE=1000000}" # 1Mb

if [ ! -f Makefile ]
then
	exit 1
fi

. ./releasetools/image.defaults
. ./releasetools/image.functions

DESTDIR=${BUILDDIR}/hd
export DESTDIR

if [ -f "${IMG}" ]
then
	rm -f "${IMG}"
fi

mkdir -p "${BUILDDIR}" "${OBJ}"

mkdir -p "${DESTDIR}/boot"

create_stpdboot_ini "${DESTDIR}/boot"

make

echo ""
echo "Disk image at $(pwd)/${IMG}"
