// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"


class WxStruct
{
public:
	wchar_t* pStr;  //文本数据的指针
	int minLen;     //字符串的长度
	int maxLen;     //字符串的最大长度
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

BOOL APIENTRY DllMain( HMODULE hModule,
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
	DWORD callAddr = baseAddr + 0xCE6C80;

	//构造数据
	WxStruct sWxid((wchar_t*)L"filehelper");
	WxStruct sMsg((wchar_t*)L"我是测试消息");
	
	//缓存区
	char buf1[0x10] = { 0 };
	char buf2[0x1000] = { 0 };

	__asm
	{
		push 0x0;
		push 0x0;
		lea eax, buf1;
		push 0x0;
		push 0x1;
		push eax;
		lea eax, sMsg;
		push eax;
		lea edx, sWxid;
		lea ecx, buf2;
		call callAddr;
		add esp, 0x18;
	}
}
