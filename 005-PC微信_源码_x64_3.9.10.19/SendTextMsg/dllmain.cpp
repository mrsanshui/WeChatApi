// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <string>
using namespace std;

class WxStruct
{
public:
	wchar_t* pStr;
	DWORD minLen;
	DWORD maxLen;
	DWORD64 full1;
	DWORD full2;
	DWORD full3;
public:
	WxStruct(wchar_t* _pStr)
	{
		pStr = _pStr;
		minLen = wcslen(pStr);
		maxLen = minLen * 2;
		full1 = 0;
		full2 = 0;
		full3 = 0;
	}
};

void SendText();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
		HANDLE Thandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendText, NULL, NULL, NULL);
		if (Thandle != 0) CloseHandle(Thandle);
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void SendText()
{
	DWORD64 baseAddr = (DWORD64)LoadLibraryA("WeChatWin.dll");
	DWORD64 callAddr = baseAddr + 0x238EC70;

	typedef DWORD64(*PCall1)(DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64);
	PCall1 Call1 = (PCall1)callAddr;

	WxStruct sWxid((wchar_t*)L"filehelper");
	WxStruct sMsg((wchar_t*)L"我是测试消息");

	char buf1[0x18] = { 0 };
	char buf2[0x1024] = { 0 };

	Call1(
		(DWORD64)&buf2,
		(DWORD64)&sWxid,
		(DWORD64)&sMsg,
		(DWORD64)&buf1,
		1, 1, 0, 0
	);
	OutputDebugStringA("ok");
}
