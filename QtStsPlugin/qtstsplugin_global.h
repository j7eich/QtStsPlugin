#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(QTSTSPLUGIN_LIB)
#  define QTSTSPLUGIN_EXPORT Q_DECL_EXPORT
# else
#  define QTSTSPLUGIN_EXPORT Q_DECL_IMPORT
# endif
#else
# define QTSTSPLUGIN_EXPORT
#endif
