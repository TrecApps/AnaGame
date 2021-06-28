// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "TControl.h"
#include <TTextRenderer.h>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        textRenderer = new TTextRenderer();
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        textRenderer->Release();
        break;
    }
    return TRUE;
}

