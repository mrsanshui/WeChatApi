// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

void Hook();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        Hook();
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void Hook()
{
    DWORD baseAddr = (DWORD)LoadLibraryA("WeChatWin.dll");
	DWORD callAddr = baseAddr + 0x60F840;
	__asm
	{
		push 0x0;
		call callAddr;
	}
}