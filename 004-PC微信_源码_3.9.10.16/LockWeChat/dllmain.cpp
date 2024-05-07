// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

void LockWeChat();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        LockWeChat();
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void LockWeChat()
{
    DWORD64 baseAddr = (DWORD64)LoadLibraryA("WeChatWin.dll");
	DWORD64 callAddr1 = baseAddr + 0x13322B0;
	DWORD64 callAddr2 = baseAddr + 0x12FC5A0;
    __asm
    {
        call callAddr1;
        mov ecx, eax;
        call callAddr2;
    }
    OutputDebugStringA("OK");
}