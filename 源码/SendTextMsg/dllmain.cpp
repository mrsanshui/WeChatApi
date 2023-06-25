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
	DWORD callAddr = baseAddr + 0x4BE7B0;

	//构造数据
	WxStruct sWxid((wchar_t*)L"filehelper");
	WxStruct sMsg((wchar_t*)L"我是测试消息");
	char* pWxid = (char*)&sWxid.pStr;
	char* pMsg = (char*)&sMsg.pStr;

	// 被@人的缓冲区
	char buf1[0x10] = { 0 };

	// 空得缓冲区【不知道是干嘛的，反正测试置空也不影响发送消息，尽量给大】
	char buf2[0x1000] = { 0 };

	__asm
	{
		lea eax, buf1;
		push 0x1;
		push eax;
		mov edi, pMsg;
		push edi;
		mov edx, pWxid;
		lea ecx, buf2;
		call callAddr;
		add esp, 0xC;
	}
}
