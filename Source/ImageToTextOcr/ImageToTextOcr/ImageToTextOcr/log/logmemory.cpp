#include "logmemory.h"

__int64 getCurrentMemory()
{
    HANDLE handle = GetCurrentProcess();
    PROCESS_MEMORY_COUNTERS pmc;
    GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
    return (__int64)pmc.WorkingSetSize / 1024;
}