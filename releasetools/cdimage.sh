#!/usr/bin/env bash
set -e

if [ ! -f build.sh ]
then
	exit 1
fi

. ./releasetools/image.defaults
. ./releasetools/image.functions

DESTDIR=${BUILDDIR}/iso

