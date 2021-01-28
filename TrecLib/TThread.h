#pragma once
#include "TObject.h"

typedef struct ThreadDetails
{
    DWORD threadId;
    HANDLE handle;
    LPTHREAD_START_ROUTINE function;
    LPVOID functionParams;
    bool sleep;
}ThreadDetails;

class _TREC_LIB_DLL TThread :
    public TObject
{
public:
    TThread();
    TThread(LPTHREAD_START_ROUTINE routine, LPVOID params);
    TThread(const TThread& orig);

    bool Run();

    static bool Suspend(DWORD id);
    static bool Resume(DWORD id);
    static DWORD Sleep(DWORD dwMilliseconds);
    static DWORD CreateTThread(LPTHREAD_START_ROUTINE routine, LPVOID params);
    static HANDLE GetHandleFromId(DWORD id);
    static void WakableSleep();
    static void WakeFromSleep(DWORD id);

private:

    void Initialize();

    ThreadDetails details;
};