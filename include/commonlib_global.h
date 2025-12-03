#ifndef COMMONLIB_GLOBAL_H
#define COMMONLIB_GLOBAL_H

#include <QtCore/qglobal.h>
#define APP_NAME            "DDC"
#define ORG_NAME            "DDC"

#define COMMON_LIBRARY
#if defined(COMMON_LIBRARY)
#  define COMMONSHARED_EXPORT Q_DECL_EXPORT
#else
#  define COMMONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMONLIB_GLOBAL_H
