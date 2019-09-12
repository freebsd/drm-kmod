#!/bin/sh

# Cross compile test for https://github.com/FreeBSDDesktop/kms-drm
# Execute as non-root in root folder of repo

# Requires:
# pkg install devel/powerpc64-xtoolchain-gcc (gcc in base is too old)
# cd /usr/src; make kernel-toolchain TARGET_ARCH=aarch64


CLEAN=no
UPDATE=no
PWD=`pwd`
OBJDIRPREFIX=/tmp/kms-drm
_SYSDIR=/usr/src/sys

while [ "$1" != "" ]; do
    ARG=`echo $1 | awk -F= '{print $1}'`
    VAL=`echo $1 | awk -F= '{print $2}'`
    echo Got ${ARG}=${VAL}
    case $ARG in
        clean)
            CLEAN=yes
            ;;
        sysdir)
            _SYSDIR=${VAL}
            ;;
        update-toolchains)
            UPDATE=yes
            ;;
        *)
            echo "ERROR: unknown argument \"$ARG\""
            exit 1
            ;;
    esac
    shift
done

if [ ${CLEAN} == "no" ]; then
	CMD="/bin/sh -c 'cd ${PWD}; make -s -j8'"
else
	CMD="/bin/sh -c 'cd ${PWD}; make clean cleandepend -s -j16; make -s -j8'"
fi

if [ ${UPDATE} == "yes" ]; then
	sudo pkg install -y devel/powerpc64-xtoolchain-gcc
	cd /usr/src && sudo make -s -j8 kernel-toolchain TARGET_ARCH=aarch64 || exit 1
	cd ${PWD}
fi

SUCCESS=""
FAIL=""

dobuild()
{
	local target=$1
	local options=$2

	echo "Building ${target} ${options}..."

	(cd /usr/src; make buildenv MAKEOBJDIRPREFIX=${OBJDIRPREFIX}-${target}  \
		TARGET_ARCH=${target} ${options} BUILDENV_SHELL="${CMD}") && \
		SUCCESS="${SUCCESS} ${target}" || FAIL="${FAIL} ${target}"
}

#       TARGET		OPTIONS
dobuild "amd64"		"SYSDIR=${_SYSDIR}"
dobuild "i386"		"SYSDIR=${_SYSDIR}"
dobuild "powerpc64"	"SYSDIR=${_SYSDIR} CROSS_TOOLCHAIN=powerpc64-gcc"
dobuild "aarch64"	"SYSDIR=${_SYSDIR}"

echo
echo "SUCCESS: ${SUCCESS}"
echo "FAIL: ${FAIL}"

if [ ! -z "${FAIL}" ]; then exit 1; fi
exit 0
