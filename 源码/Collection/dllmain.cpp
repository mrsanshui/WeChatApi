// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <string>
using namespace std;

class WxStruct
{
public:
	wchar_t* pStr;
	int minLen;
	int maxLen;
	int full1 = 0;
	int full2 = 0;
public:
	WxStruct(wchar_t* str)
	{
		pStr = str;
		minLen = wcslen(pStr);
		maxLen = minLen * 2;
	}
};


void Hook();


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Hook, NULL, NULL, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}
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
	//模块基址
	DWORD baseAddr = (DWORD)LoadLibraryA("WeChatWin.dll");
	DWORD callAddr1 = baseAddr + 0x701DC0;
	DWORD callAddr2 = baseAddr + 0xC2ED80;

	//对方的微信ID
	WxStruct wxId((wchar_t*)L"wxid_xxx");

	//转账ID
	WxStruct transferId((wchar_t*)L"10000500012000");

	__asm
	{
		sub esp, 0x30;
		mov esi, esp;
		mov ecx, esi;
		lea eax, transferId;
		push eax;
		call callAddr1;

		lea eax, wxId;
		push eax;
		lea ecx, [esi + 0x14];
		call callAddr1;

		call callAddr2;
		add esp, 0x30;
	}
}
