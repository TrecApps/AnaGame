#pragma once
#ifndef _ANAFACE_DLL2
#define _ANAFACE_DLL2 __declspec(dllexport)
#else
#define _ANAFACE_DLL2 __declspec(dllimport)
#endif