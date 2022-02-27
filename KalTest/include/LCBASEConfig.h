#ifndef __LCBASE_CONFIG__
#define __LCBASE_CONFIG__
#define __ROOT_VERSION__ 6
#define __ROOT_MINORVERSION__ 24
#define __ROOT_PATCHLEVEL__ 4
#define __ROOT_FULLVERSION__ 62404
#ifndef __CINT__
#define __UNAME_MACHINE__ x86_64
#define __UNAME_SYSNAME__ Darwin
#define __G77EGCS_VERSION__  8
#define __G77EGCS_MINORVERSION__  2
#define __G77EGCS_PATCHLEVEL__  0
#define __G77EGCS_LIBNAME__ gfortran
#define JSF_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define JSF_VERSION_CODE JSF_VERSION(__JSF_VERSION__,__JSF_MINORVERSION__,__JSF_PATCHLEVEL__)
#define GNUC_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))
#define GNUC_VERSION_CODE GNUC_VERSION(__GNUC_VERSION__,__GNUC_MINOR__,__GNUC_PATCHLEVEL__)
#endif
#endif
