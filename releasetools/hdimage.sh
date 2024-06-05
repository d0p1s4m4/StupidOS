#!/usr/bin/env bash
set -e

: "${IMG=disk.img}"

if [ ! -f build.sh ]
then
	exit 1
fi

. ./releasetools/image.defaults
. ./releasetools/image.functions

if [ -f "${IMG}" ]
then
	rm -f "${IMG}"
fi

echo ""
echo "Disk image at $(pwd)/${IMG}"
