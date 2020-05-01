/*****************************************************************************
QtStsPlugin - Qt-based library for using StellwerkSim's plugin interface
Copyright (C) 2020  Jan Siebeneich

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published
by the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/
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
