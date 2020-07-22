#pragma once
#ifndef _TREC_LIB_DLL
#define _TREC_LIB_DLL __declspec(dllexport)
#else
#define _TREC_LIB_DLL __declspec(dllimport)
#endif

// Need for debugging heap

#if _DEBUG
#define _CRTDBG_MAP_ALLOC
#endif

#include <Windows.h>
