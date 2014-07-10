#                                              -*- Autoconf -*-
# Main configure script for BEEBS

# Copyright (C) 2013-2014 Embecosm Limited and the University of Bristol

# Contributor James Pallister <james.pallister@embecosm.com>
# Contributor Jeremy Bennett <jeremy.bennett@embecosm.com>

# This file is part of BEEBS

# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the Free
# Software Foundation; either version 3 of the License, or (at your option)
# any later version.

# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
# more details.

# You should have received a copy of the GNU General Public License along
# with this program.  If not, see <http://www.gnu.org/licenses/>.

# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([beebs], [git-dejagnu2], [beebs@mageec.org])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_SILENT_RULES([yes])

# Option for selecting the target board
AC_ARG_WITH([board],
    [AS_HELP_STRING([--with-board], [Select the board to target])],
    [],
    [with_board=none]
    )

# Option for selecting the target board
AC_ARG_WITH([chip],
[AS_HELP_STRING([--with-chip], [Select the chip to target])],
    [],
    [with_chip=none]
    )

# The host is where we will run the tests.
# @note This is what will determine the cross compiler to use
AC_CANONICAL_HOST

# Work out which architecture, board and chip we need
# Also pass this through to be substituted
arch=$host_cpu
board=$with_board
chip=$with_chip

# Some platforms cannot compile certain benchmarks, so we exclude them
# from the build

# @todo FIXME. Why is this a VAR and not a --with option.
AC_ARG_VAR(EXCLUDED_BENCHMARKS, exclude benchmarks from the build)

if test -f $srcdir/config/$arch/arch.cfg; then
    source $srcdir/config/$arch/arch.cfg
fi

if ! test -d $srcdir/config/$arch/boards/$board; then
    AC_MSG_ERROR([Config directory for board "$board" does not exist])
fi

if test -f $srcdir/config/$arch/boards/$board/board.cfg; then
    source $srcdir/config/$arch/boards/$board/board.cfg
fi


if ! test -d $srcdir/config/$arch/chips/$chip; then
    AC_MSG_ERROR([Config directory for chip "$chip" does not exist])
fi

if test -f $srcdir/config/$arch/chips/$chip/chip.cfg; then
    source $srcdir/config/$arch/chips/$chip/chip.cfg
fi

AM_CONDITIONAL([CHIPSUPPORT_C],
	       [test -f $srcdir/config/$arch/chips/$chip/chipsupport.c])

if test -f $srcdir/config/$arch/boards/$board/boardsupport.h; then
    AC_DEFINE([HAVE_BOARDSUPPORT_H], [1], [Board support header is present])
fi
if test -f $srcdir/config/$arch/chips/$chip/chipsupport.h; then
    AC_DEFINE([HAVE_CHIPSUPPORT_H], [1], [Chip support header is present])
fi

AC_SUBST(ARCH, $arch)
AC_SUBST(BOARD, $board)
AC_SUBST(CHIP, $chip)

CFLAGS="$CFLAGS $ARCH_CFLAGS $CHIP_CFLAGS $BOARD_CFLAGS"
LDFLAGS="$LDFLAGS $ARCH_LDFLAGS $CHIP_LDFLAGS $BOARD_LDFLAGS"

AM_INIT_AUTOMAKE([1.11])
LT_INIT
AC_SUBST([LIBTOOL_DEPS])

# If we set --host, this will select the cross-compiler.
AC_PROG_CC
AM_PROG_AS
AC_PROG_CC_C99

# List of the benchmarks. This list gets expanded into a variable passed
# into the makefile, and configures the various makefiles. This allows an
# individual benchmark to be excluded from the compilation in a easy way.
m4_define([benchmarklist],[dnl
[2dfir],
[adpcm],
[blowfish],
[bs],
[bsort100],
[bubblesort],
[cnt],
[compress],
[cover],
[crc],
[crc32],
[ctl-string],
[ctl-stack],
[ctl-vector],
[cubic],
[dhrystone],
[dijkstra],
[dtoa],
[duff],
[edn],
[expint],
[fac],
[fasta],
[fdct],
[fft],
[fibcall],
[fir],
[float_matmult],
[frac],
[gdb-advance],
[gdb-all-types],
[gdb-anon],
[gdb-arrayidx],
[gdb-async],
[gdb-bitfields],
[gdb-constvars],
[gdb-store],
[huffbench],
[insertsort],
[int_matmult],
[janne_complex],
[jfdctint],
[lcdnum],
[levenshtein],
[lms],
[ludcmp],
[mergesort],
[miniz],
[minver],
[nbody],
[ndes],
[nettle-arcfour],
[nettle-cast128],
[nettle-des],
[nettle-md5],
[newlib-exp],
[newlib-log],
[newlib-mod],
[newlib-sqrt],
[ns],
[nsichneu],
[prime],
[qsort],
[qurt],
[recursion],
[rijndael],
[select],
[sglib-arraybinsearch],
[sglib-arraysort],
[sglib-arraysort1],
[sglib-dllist],
[sglib-hashtable],
[sglib-listinsertsort],
[sglib-listinsertsort1],
[sglib-listsort],
[sglib-queue],
[sglib-rbtree],
[sha],
[slre],
[stringsearch1],
[strstr],
[sqrt],
[st],
[statemate],
[tarai],
[trio-snprintf],
[ud],
[wikisort],
[whetstone]])

BENCHMARKS=""
BENCHMARKS_SUBDIRS=""

m4_foreach([benchmark], [benchmarklist], [
    bmark=benchmark
    if ! echo "$EXCLUDED_BENCHMARKS" | egrep -q "\b$bmark\b" ; then
        AC_CONFIG_FILES(src/benchmark/Makefile)
        BENCHMARKS+="benchmark "
        BENCHMARKS_SUBDIRS+="src/benchmark "
    fi
])

AC_SUBST(BENCHMARKS,$BENCHMARKS)
AC_SUBST(BENCHMARKS_SUBDIRS,$BENCHMARKS_SUBDIRS)

AC_CONFIG_SRCDIR([src/])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_FILES([dejagnu/site.exp])
AC_CONFIG_FILES([src/template/Makefile])
AC_CONFIG_FILES([doc/Makefile])
AC_CONFIG_FILES([Makefile])
AC_OUTPUT

echo
echo \*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*\*
echo
echo "Host arch                   $arch"
echo "Host board                  $board"
echo "Host chip                   $chip"
echo "Host triple                 $target"

if test "x$EXCLUDED_BENCHMARKS" == x ; then
    echo "Excluded benchmarks           none"
else
    echo "Excluded benchmarks         $EXCLUDED_BENCHMARKS"
fi
