#ifndef COMMONLIB_GLOBAL_H
#define COMMONLIB_GLOBAL_H

/**
 * @file include/commonlib_global.h
 * @brief Export macros and application metadata for the common library.
 *
 * This header centralizes macros used by the common library of the DDC
 * application. It provides:
 *  - Application and organization name constants used by Qt (e.g. QSettings).
 *  - The COMMONSHARED_EXPORT macro used to mark symbols for export/import
 *    when building or consuming the shared library.
 *
 * The header depends on Qt's qglobal.h for Q_DECL_EXPORT / Q_DECL_IMPORT.
 */

#include <QtCore/qglobal.h>

/**
 * @def APP_NAME
 * @brief The application name used by the project.
 *
 * Typical use: QCoreApplication::setApplicationName(APP_NAME) or
 * when storing application-specific settings.
 */
#define APP_NAME            "DDC"

/**
 * @def ORG_NAME
 * @brief The organization name used by the project.
 *
 * Typical use: QCoreApplication::setOrganizationName(ORG_NAME) or
 * when storing organization-scoped settings.
 */
#define ORG_NAME            "DDC"

/**
 * @def COMMON_LIBRARY
 * @brief Define this when building the common library itself.
 *
 * When COMMON_LIBRARY is defined the COMMONSHARED_EXPORT macro expands to
 * Q_DECL_EXPORT so that symbols are exported from the shared library.
 * Consumers of the library should not define COMMON_LIBRARY, causing
 * COMMONSHARED_EXPORT to expand to Q_DECL_IMPORT instead.
 */
#define COMMON_LIBRARY

#if defined(COMMON_LIBRARY)
/// Export symbols when building the library
#  define COMMONSHARED_EXPORT Q_DECL_EXPORT
#else
/// Import symbols when using the library
#  define COMMONSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // COMMONLIB_GLOBAL_H
