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
