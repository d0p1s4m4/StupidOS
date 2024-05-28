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
# -----------------------------------------------------------------------------

# -----------------------------------------------------------------------------
#  build
# -----------------------------------------------------------------------------


# -----------------------------------------------------------------------------
#  tools
# -----------------------------------------------------------------------------

build_tools() {
	msg "Builing tools"
	plain "tools dir: %s" "${TOOLS_DIR}"

	make tools
}

# -----------------------------------------------------------------------------
#  doctor - check os dependancies
# -----------------------------------------------------------------------------
check_tool() {
	local tool=$1; shift

	printf "checking for %s " "${tool}"

	tool_path="$(which "${tool}" 2> /dev/null)"
	if [ "x${tool_path}" = "x" ]; then
		printf "${BOLD}${RED}NO${ALL_OFF}\n"
		false;
	else
		printf "${GREEN}${tool_path}${ALL_OFF}\n"
		true;
	fi
}

doctor() {
	local err=0

	check_tool fasm || ((err++))
	check_tool gcc || ((err++))
	check_tool bmake || ((err++))
	check_tool sha256sum || ((err++))
	check_tool python3 || ((err++))

	if [ $err -gt 0 ]; then error "Some tools are missing"; fi
}

# -----------------------------------------------------------------------------
#  usage & version
# -----------------------------------------------------------------------------

usage() {
	local status=$1

	if [ ${status} -eq 0 ]; then
		cat <<EOF
Usage: $prgname [-hV] [COMMANDS]
Flags:
	-h	display this menu.
	-V	display version information.
Options:
Commands:
	doctor
	tools
	build
EOF
	else
		printf "Try '%s -h' for more information.\n" "${prgname}" >&2
	fi

	exit ${status}
}

version() {
	VERSION_MAJOR="$(grep VERSION_MAJOR  kernel/const.inc  | cut -d' ' -f 3)"
	VERSION_MINOR="$(grep VERSION_MINOR  kernel/const.inc  | cut -d' ' -f 3)"

	printf "%s (StupidOS) %d.%d\n" "${prgname}" ${VERSION_MAJOR} ${VERSION_MINOR}

	exit 0
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

export PATH="$PATH:$TOOLS_DIR"

if [ ! -f Makefile ] || [ ! -f LICENSE ]; then
	error "Run this script at StupidOS root"
fi

if [ $# -eq 0 ]; then
	usage 1
fi

while [ $# -gt 0 ]; do
	op=$1; shift
	
	case "${op}" in
		help | --help | -h)
			usage 0
			;;
		--version | -V)
			version
			;;
		doctor)
			doctor
			;;
		tools)
			build_tools
			;;
		*)
			usage 1
			;;
	esac
done
