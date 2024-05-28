#!/usr/bin/env bash

# -----------------------------------------------------------------------------
# log reporting
# -----------------------------------------------------------------------------
plain() {
	local mesg=$1; shift

	# shellcheck disable=SC2059
	printf "${BOLD}    ${mesg}${ALL_OFF}\n" "$@"
}

msg() {
	local mesg=$1; shift

	# shellcheck disable=SC2059
	printf "${MAGENTA}==>${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@"
}

msg2() {
	local mesg=$1; shift

	# shellcheck disable=SC2059
	printf "${BLUE} ->${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@"
}

error() {
	local mesg=$1; shift

	# shellcheck disable=SC2059
	printf "${RED}==> ERROR:${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@"
	exit 1
}

warning() {
	local mesg=$1; shift

	# shellcheck disable=SC2059
	printf "${YELLOW}==> WARNING:${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@"
}

success() {
	local mesg=$1; shift

	# shellcheck disable=SC2059
	printf "${GREEN}==> SUCCESS:${ALL_OFF}${BOLD} ${mesg}${ALL_OFF}\n" "$@"
}

# -----------------------------------------------------------------------------
#  entry 
# -----------------------------------------------------------------------------
LC_ALL=C
export LC_ALL

unset INFODIR
unset LESSCHARSET
unset MAKEFLAGS
unset TERMINFO

unset ALL_OFF BOLD RED GREEN BLUE MAGENTA YELLOW
if [ ! -v NO_COLOR ]; then
	ALL_OFF="\e[1;0m"
	BOLD="\e[1;1m"
	BLUE="${BOLD}\e[1;34m"
	GREEN="${BOLD}\e[1;32m"
	RED="${BOLD}\e[1;31m"
	MAGENTA="${BOLD}\e[1;35m"
	YELLOW="${BOLD}\e[1;33m"
fi
readonly ALL_OFF BOLD RED GREEN BLUE MAGENTA YELLOW

unset topdir prgname build_start
prgname="$(basename $0)"
topdir="$(realpath "$0")"
topdir="$(dirname "${topdir}")"
build_start="$(date)"
readonly topdir prgname build_start

SRC_DIR="${topdir}"
BUILD_DIR="${topdir}/.build"
TOOLS_DIR="${topdir}/.tools"
TOOLS_PREFIX="stpd-"

if [ $# -eq 0 ]; then
	printf "Try '%s -h' for more information.\n" "${prgname}" >&2
	exit 1
fi

