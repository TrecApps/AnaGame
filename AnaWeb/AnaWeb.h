#pragma once

#ifndef ANA_WEB_DLL
#define ANA_WEB_DLL __declspec(dllexport)
#else
#define ANA_WEB_DLL __declspec(dllimport)
#endif