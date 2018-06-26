#pragma once
//////////////////////////////////////////////////////////////////////////
#include <QVector>
typedef QVector<int> SColumnOrders;

#define freeAndNull(p) {if (p) {delete p; p = NULL;}}

#define FIRST_SHEET 1
