#ifndef LOGMEMORY_H
#define LOGMEMORY_H
#include "export.h"
#include "windows.h"
#include <psapi.h>

#pragma comment(lib,"psapi.lib")

// using KB unit
DT_UTIL_EXPORT inline __int64 getCurrentMemory();

#endif