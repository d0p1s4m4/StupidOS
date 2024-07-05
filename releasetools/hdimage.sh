#!/usr/bin/env bash
set -e

: "${IMG=disk.img}"

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
