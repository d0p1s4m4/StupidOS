#!/usr/bin/env bash
set -e

if [ ! -f Makefile ]
then
	exit 1
fi

. ./releasetools/image.defaults
. ./releasetools/image.functions

DESTDIR=${BUILDDIR}/iso

mkdir -p "${OBJ}"

(cd "${OBJ}"; get_grub)


make
