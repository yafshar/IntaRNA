# this is example-file: configure.ac

AC_PREREQ([2.65])
# 5 argument version only available with aclocal >= 2.64
AC_INIT( [IntaRNA], [2.1.0], [], [intaRNA], [http://www.bioinf.uni-freiburg.de] )


# minimal required version of the boost library 
BOOST_REQUIRED_VERSION=1.50.0
# minimal required version of the Vienna RNA library
VRNA_REQUIRED_VERSION=2.4.1


AC_CANONICAL_HOST
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([src/easylogging++.h])
AC_CONFIG_HEADERS([src/config.h])

# AC_CONFIG_MACRO_DIR([m4])
m4_include([m4/m4_ax_cxx_compile_stdcxx.m4])
m4_include([m4/m4_ax_openmp.m4])
m4_include([m4/m4_ax_boost_base.m4])

lt_enable_auto_import=""
case "$host_os" in 
	cygwin* | mingw* | cegcc*)
		AM_LDFLAGS="-Wl,--enable-auto-import $AM_LDFLAGS"
esac


# check for C++ compiler
# store current compiler flags to avoid default setup via AC_PROG_CXX and *_CC
OLD_CXXFLAGS=$CXXFLAGS
OLD_CFLAGS=$CFLAGS
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_RANLIB
# reset compiler flags to initial flags
CXXFLAGS=$OLD_CXXFLAGS
CFLAGS=$OLD_CFLAGS

# automake initialisation (mandatory) and check for minimal automake API version
AM_INIT_AUTOMAKE([1.11])

# use the C++ compiler for the following checks
AC_LANG([C++])

# ensure we are using c11 C++ standard
AX_CXX_COMPILE_STDCXX( [11], [noext], [mandatory])


###############################################################################
###############################################################################

############  PARAMETERS  ########################################

###############################################################################
# PKG-CONFIG SETUP
###############################################################################

AC_MSG_CHECKING([whether to use and provide pkg-config information])
pkgconfigEnabled=yes
AC_ARG_ENABLE([pkg-config],
	[AS_HELP_STRING([--disable-pkg-config],
	    [disable pkg-config support (def=enabled)])],
  	[pkgconfigEnabled="$enableval"],
  	[pkgconfigEnabled=yes])
AC_MSG_RESULT([$pkgconfigEnabled])
AS_IF([test x"$pkgconfigEnabled" = x"yes"], [
	# Checks for pkg-config
	AC_CHECK_PROG([HAVE_PKG_CONFIG],[pkg-config],[yes],[no])
	AS_IF([test "HAVE_PKG_CONFIG" == "yes"], [
		dnl Testing presence of pkg-config
		AC_MSG_CHECKING([pkg-config m4 macros])
		AS_IF([test m4_ifdef([PKG_CHECK_MODULES], [yes], [no]) == yes], [
			AC_MSG_RESULT([yes]);
		], [
			AC_MSG_RESULT([no]);
			HAVE_PKG_CONFIG=no
		])
	])
], [
	HAVE_PKG_CONFIG=no
])


###############################################################################
# DEBUG SUPPORT SETUP
###############################################################################

AC_MSG_CHECKING([whether to build with debug information])
debuger=no
AC_ARG_ENABLE([debug],
	[AS_HELP_STRING([--enable-debug],
	    [enable debug data generation (def=disabled)])],
  	[debuger="$enableval"])
AC_MSG_RESULT([$debuger])
AS_IF([test x"$debuger" = x"yes"], [
	AC_DEFINE([_DEBUG], [1], [Run in DEBUG mode with additional assertions and debug output])
	AM_CXXFLAGS="$AM_CXXFLAGS -g -O0 -Wno-uninitialized -Wno-deprecated" # -Wall"
], [
	AC_DEFINE([NDEBUG], [1], [Run in normal mode with minimal assertions])
	AM_CXXFLAGS="$AM_CXXFLAGS -O3 -fno-strict-aliasing -Wno-uninitialized -Wno-deprecated"
])


###############################################################################
# MULTI-THREADING SUPPORT SETUP
###############################################################################

AC_MSG_CHECKING([whether to enable multi-threading support])
multithreadingEnabled=yes
AC_ARG_ENABLE([multithreading],
	[AS_HELP_STRING([--disable-multithreading],
	    [disable multi-threading support (def=enabled)])],
  	[multithreadingEnabled="$enableval"],
  	[multithreadingEnabled=yes])
AC_MSG_RESULT([$multithreadingEnabled])
AS_IF([test x"$multithreadingEnabled" = x"yes"], [
	AC_DEFINE([INTARNA_MULITHREADING], [1], [Enabling multi-threading support])
	AC_SUBST([INTARNA_MULITHREADING],[1])
	# ensure OPENMP can be used
	AX_OPENMP([],[AC_MSG_ERROR([OPENMP support is mandatory for multi-threading compilation])])
	AM_CXXFLAGS="$AM_CXXFLAGS $OPENMP_CXXFLAGS"
], [
	AC_DEFINE([INTARNA_MULITHREADING], [0], [Disabling multi-threading support])
	AC_SUBST([INTARNA_MULITHREADING],[0])
	AM_CXXFLAGS="$AM_CXXFLAGS -pthread"
])

###############################################################################
# Vienna RNA package library path support, if not installed in usual directories
###############################################################################
AC_ARG_WITH([vrna],
	[AC_HELP_STRING(
	    [--with-vrna=PREFIX],
	    [alternative prefix path to Vienna RNA library]
	  )],
	  [RNAPATHSET=1],
	  [RNAPATHSET=0]
)
# handle user-defined path
AS_IF([test  $RNAPATHSET = 1 ], [
	AS_IF([test "x$HAVE_PKG_CONFIG" = "xyes"], [
		# use pkg-config data if available
		export PKG_CONFIG_PATH="$with_vrna/lib/pkgconfig/:$PKG_CONFIG_PATH"
	], [
		# guess compiler and linker flags if needed
		AM_CXXFLAGS="-I$with_vrna/include $AM_CXXFLAGS"
		AM_LDFLAGS="-L$with_vrna/lib $AM_LDFLAGS"
	])
])

###############################################################################
dnl generate doxygen documentation in Doxy
###############################################################################

AC_CHECK_PROG([HAVE_DOXYGEN],[doxygen],[yes],[no])

AS_IF([test "x$HAVE_DOXYGEN" = "xyes"], [
	# loading m4 macro
	m4_include([m4/doxygen.m4])
	# setup doxygen documentation to build
	DX_HTML_FEATURE(ON)
	DX_CHM_FEATURE(OFF)
	DX_CHI_FEATURE(OFF)
	DX_MAN_FEATURE(OFF)
	DX_RTF_FEATURE(OFF)
	DX_XML_FEATURE(OFF)
	DX_PDF_FEATURE(ON)
	DX_PS_FEATURE(OFF)
	# generate according options etc.
	DX_INIT_DOXYGEN($PACKAGE_NAME, [doc/doxygen.cfg], [doxygen-doc])
])

###############################################################################
# EASYLOGGING++
###############################################################################

# enable easylogging++ performance tracking
AM_CXXFLAGS="$AM_CXXFLAGS -DELPP_FEATURE_PERFORMANCE_TRACKING"
# disable default log file creation
AM_CXXFLAGS="$AM_CXXFLAGS -DELPP_NO_DEFAULT_LOG_FILE"

#############################
# COLORING SETUP
#############################

AC_MSG_CHECKING([whether to enable coloring of console log output])
logColoring=yes
AC_ARG_ENABLE([log-coloring],
	[AS_HELP_STRING([--disable-log-coloring],
	    [disable console log coloring (def=enabled)])],
  	[logColoring="$enableval"])
AC_MSG_RESULT([$logColoring])
AS_IF([test x"$logColoring" = x"yes"], [
	AC_DEFINE([INTARNA_LOG_COLORING], [1], [Enabling console log coloring])
	AC_SUBST([INTARNA_LOG_COLORING],[1])
], [
	AC_DEFINE([INTARNA_LOG_COLORING], [0], [Disabling console log coloring])
	AC_SUBST([INTARNA_LOG_COLORING],[0])
])



###############################################################################

###############################################################################
# BOOST CHECK
###############################################################################

AX_BOOST_BASE([$BOOST_REQUIRED_VERSION], [FOUND_BOOST=1;], [FOUND_BOOST=0;])


############  CHECKS  ############################################

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_SIZE_T

# Checks for header files.
AC_HEADER_STDC


##########################################################################
# check boost test results
##########################################################################

# FOUND_BOOST is only defined if want_boost is "yes"
AS_IF([test $want_boost = "no" || test $FOUND_BOOST != 1], [
	AC_MSG_NOTICE([])
	AC_MSG_NOTICE([The Boost Library was not found!])
	AC_MSG_NOTICE([ -> If installed in a non-standard path, please use '--with-boost=PREFIX'.])
	AC_MSG_NOTICE([])
 	DEPENDENCYNOTFOUND=1;
], [
	AM_CXXFLAGS="$BOOST_CPPFLAGS $AM_CXXFLAGS"
	AM_LDFLAGS="$BOOST_LDFLAGS $AM_LDFLAGS"
	LIBS="$LIBS -lboost_regex -lboost_program_options -lboost_filesystem -lboost_system"
])


###############################################################################
# BEGIN VIENNA CHECK
###############################################################################
# check for Vienna RNA headers

RNANOTFOUND=0
AS_IF([test "x$HAVE_PKG_CONFIG" = "xyes"], [
	AC_MSG_NOTICE([check if VRNA version >= $VRNA_REQUIRED_VERSION])
	PKG_CHECK_MODULES( [VRNA], [RNAlib2 >= $VRNA_REQUIRED_VERSION], [RNANOTFOUND=0], [RNANOTFOUND=1])
])

AS_IF([test "$RNANOTFOUND" = "0"], [
	AC_MSG_CHECKING([for compilation with Vienna RNA package headers])
	OLD_CPPFLAGS=$CPPFLAGS
	OLD_CXXFLAGS=$CXXFLAGS
	OLD_LDFLAGS=$LDFLAGS
	AC_LANG_PUSH([C])
	CPPFLAGS="$CPPFLAGS $AM_CXXFLAGS"
	LDFLAGS="$LDFLAGS $AM_LDFLAGS"
	AS_IF([test "x$HAVE_PKG_CONFIG" = "xyes"], [
		CPPFLAGS="$CPPFLAGS $VRNA_CFLAGS"
		LDFLAGS="$LDFLAGS $VRNA_LIBS"
	])
	AC_COMPILE_IFELSE(
	     [AC_LANG_PROGRAM([[#include <ViennaRNA/model.h>]],[[vrna_md_t tmp; vrna_md_copy(&tmp,&tmp);]])],
	     [ 
			AC_MSG_RESULT([yes])
			RNANOTFOUND=0;
	    ],
	     [
	       AC_MSG_RESULT([no])
		   AC_MSG_NOTICE([DEBUG : used CPPFLAGS = $CPPFLAGS])
		   AC_MSG_NOTICE([DEBUG : used  LDFLAGS = $LDFLAGS])
	       RNANOTFOUND=1;
	     ]
	)
	AC_LANG_POP([C])
	CPPFLAGS=$OLD_CPPFLAGS
	LDFLAGS=$OLD_LDFLAGS
])

# error output if ViennaRNA not found
AS_IF([test "$RNANOTFOUND" = "1"], [
	AC_MSG_NOTICE()
	AC_MSG_NOTICE([The Vienna RNA C library version >= $VRNA_REQUIRED_VERSION is required.])
	AC_MSG_NOTICE([ -> It can be obtained from http://www.tbi.univie.ac.at/.])
	AC_MSG_NOTICE()
 	AS_IF([test "$RNAPATHSET" = "1"], [
		AC_MSG_NOTICE([ -> Can't find the Vienna RNA library in given path '$with_vrna'.])
 	], [ # else
		AC_MSG_NOTICE([ -> If installed in a non-standard path, please use '--with-vrna=PREFIX'.])
 	])
 	DEPENDENCYNOTFOUND=1;
],[ # else
	# register Vienna RNA lib for linking
	AS_IF([test "x$HAVE_PKG_CONFIG" = "xyes"], [
		# apply pkg-config options for VRNA package for further build
		AM_CXXFLAGS="$AM_CXXFLAGS $VRNA_CFLAGS"
		LIBS="$LIBS $VRNA_LIBS"
 	], [ # else
 		# guess that only lib information missing
		LIBS="$LIBS -lRNA"
 	])
])


###############################################################################
# END VIENNA CHECK
###############################################################################

###############################################################################
# FINAL DEPENDENCY CHECK AND EXIT IF NEEDED
###############################################################################

# error ABORT if on of the libraries was not found
AS_IF([test "$DEPENDENCYNOTFOUND" = "1"], [
	AC_MSG_NOTICE()
	AC_MSG_ERROR([Some dependency was not met! See above for errors and relate to './configure --help'.])
])

##########################################################################



# distribute additional compiler and linker flags
# --> set these variables instead of CXXFLAGS or LDFLAGS
AC_SUBST([AM_CXXFLAGS])
AC_SUBST([AM_LDFLAGS])
AC_SUBST([LIBS])
AM_CONDITIONAL(enable_pkg_config, [test "x$HAVE_PKG_CONFIG" = "xyes"])
AM_CONDITIONAL(enable_doxygen, [test "x$HAVE_DOXYGEN" = "xyes"])


# files to generate via autotools (.am or .in source files)
AC_CONFIG_FILES([Makefile])
AC_CONFIG_FILES([src/Makefile])
AC_CONFIG_FILES([src/IntaRNA/Makefile])
AC_CONFIG_FILES([src/IntaRNA/intarna_config.h])
AC_CONFIG_FILES([src/bin/Makefile])
AC_CONFIG_FILES([perl/Makefile])
AC_CONFIG_FILES([tests/Makefile])
AC_CONFIG_FILES([doc/Makefile])
AC_CONFIG_FILES([IntaRNA.pc])

# generate the final Makefile etc.
AC_OUTPUT
