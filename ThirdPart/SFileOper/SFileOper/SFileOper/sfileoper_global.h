#ifndef SFILEOPER_GLOBAL_H
#define SFILEOPER_GLOBAL_H

#include <QtCore/qglobal.h>
#include <QtGlobal>

#define QT_BEGIN_NAMESPACE_XLSX namespace QXlsx {
#define QT_END_NAMESPACE_XLSX }
#define QTXLSX_USE_NAMESPACE using namespace QXlsx;

#ifdef SFILEOPER_LIB
# define SFILEOPER_EXPORT Q_DECL_EXPORT
#else
# define SFILEOPER_EXPORT Q_DECL_IMPORT
#endif

#endif // SFILEOPER_GLOBAL_H
