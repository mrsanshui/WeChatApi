// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <string>
#include <time.h>
using namespace std;

DWORD GetBaseAddr()
{
	DWORD baseAddr = (DWORD)LoadLibraryA("WeChatWin.dll");
	return baseAddr;
}

class WxStruct
{
public:
	wchar_t* pStr;
	int minLen;
	int maxLen;
	int full1;
	int full2;
public:
	WxStruct(wchar_t* _pStr)
	{
		pStr = _pStr;
		minLen = wcslen(pStr);
		maxLen = minLen * 2;
		full1 = 0;
		full2 = 0;
	}
};

void UpdateRemark();

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		UpdateRemark();
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

void UpdateRemark()
{
	DWORD callAddr = GetBaseAddr() + 0x404740;
	DWORD paramAddr = GetBaseAddr() + 0x222F3BC;
	char buff[0x1024] = { 0 };
	WxStruct wxId((wchar_t*)L"wxid_765f5t7o179u32");
	WxStruct newRemark((wchar_t*)L"你好1");
	memcpy(&buff[0x10], &wxId, sizeof(WxStruct));
	*(DWORD*)(buff + 0x50) = 0x43;
	memcpy(&buff[0x58], &newRemark, sizeof(WxStruct));
	*(DWORD*)(buff + 0x230) = 0x43;
	__asm
	{
		mov ecx, paramAddr;
		mov ecx, [ecx];
		push ecx;
		lea eax, buff;
		push eax;
		call callAddr;
	}
	OutputDebugStringA("OK");
}
