#pragma once
#include "TObject.h"

typedef enum class ThreadType
{
    tt_main,
    tt_regular,
    tt_interpretor,
    tt_animaion
};

typedef struct ThreadDetails
{
    DWORD threadId;
    HANDLE handle;
    LPTHREAD_START_ROUTINE function;
    LPVOID functionParams;
    bool sleep;
    ThreadType type;
}ThreadDetails;

class _TREC_LIB_DLL TThread :
    public TObject
{
public:
    TThread();
    TThread(LPTHREAD_START_ROUTINE routine, LPVOID params);
    TThread(const TThread& orig);

    bool Run();

    /**
     * Method: TThread::SetMainThread
     * Purpose: Establishes the main Thread so that it can be monitored by Anagame
     * Parameters: void
     * Returns: void
     * 
     * Note: This Method Must be called in the Window beteen the calling of winMain and the start of the message loop.
     *      If this Mehtod is not called, then any attempt to create new Threads through this interface will throw an exception
     *      Another Exception will be thrown in this method is called twice during an Anagame Applications runtime
     */
    static void SetMainThread();

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