// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"


class WxStruct
{
public:
	wchar_t* pStr;  // 文本数据的指针
	int minLen;  // 字符串的长度
	int maxLen;  // 字符串的最大长度
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
		HANDLE Thandle = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Hook, NULL, NULL, NULL);
		if (Thandle != 0)
		{
			CloseHandle(Thandle);
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
	DWORD eaxAddr = baseAddr + 0x222F0F0;
	DWORD callAddr1 = baseAddr + 0x701DC0;
	DWORD callAddr2 = baseAddr + 0x4BE160;

	//构造数据
	WxStruct sWxid((wchar_t*)L"filehelper");
	WxStruct imagefilepath((wchar_t*)L"C:\\Users\\Administrator\\Desktop\\1.png");

	//空得缓冲区【未知的字符串，置空也不影响发送，尽量给大】
	char buff1[0x1000] = { 0 };

	//缓冲区
	char buff2[0x1000] = { 0 };

	__asm
	{
		sub esp, 0x14;
		lea eax, buff1;  // 空的缓冲区
		mov ecx, esp;
		lea edi, imagefilepath;  // 图片路径结构体
		push eax;
		call callAddr1;
		mov ecx, eaxAddr;
		lea eax, sWxid;
		push edi;
		push eax;
		lea eax, buff2;
		push eax;
		call callAddr2;
	}
}
