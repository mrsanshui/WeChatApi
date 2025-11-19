// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <string>
using namespace std;

DWORD64 GetStringRealAddr(DWORD64 val)
{
#ifdef _DEBUG
	val += 0x8;
#endif
	return val;
}
typedef struct WxStruct
{
	union
	{
		CHAR szBuffer[0x10];
		PCSTR pBuffer;
	};
	DWORD64 minLen;
	DWORD64 maxLen;
}WX_STRUCT, * P_WX_STRUCT;

void AutoUpdateSwitch();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        OutputDebugStringA("进来了");
        AutoUpdateSwitch();
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void AutoUpdateSwitch()
{
	DWORD64 baseAddr = (DWORD64)LoadLibraryA("Weixin.dll");
	DWORD64 callAddr1 = baseAddr + 0x44D10;

	typedef DWORD64(*PCall1)(DWORD64, DWORD64, DWORD64);
	PCall1 Call1 = (PCall1)callAddr1;

    DWORD64 rcxArg = *(DWORD64*)((*(DWORD64*)(baseAddr + 0x9089080)) + 0x320);

    string cmdStr = "mmkv_key_is_auto_update";
	DWORD64 p_cmdStr = GetStringRealAddr((DWORD64)&cmdStr);

    //BOOL state = TRUE;
    BOOL state = FALSE;
    Call1(rcxArg, p_cmdStr, state);
    OutputDebugStringA("OK");
}
