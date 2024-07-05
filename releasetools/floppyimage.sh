#!/usr/bin/env bash
set -e

if [ ! -f Makefile ]
then
	exit 1
fi

. ./releasetools/image.defaults
. ./releasetools/image.functions

DESTDIR=${BUILDDIR}/floppy
export DESTDIR

mkdir -p "${BUILDDIR}" "${OBJ}"

mkdir -p "${DESTDIR}/boot"

create_stpdboot_ini "${DESTDIR}/boot"

make

echo ""
echo "floppy image at $(pwd)/${IMG}"
