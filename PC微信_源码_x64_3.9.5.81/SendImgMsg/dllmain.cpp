// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <string>
using namespace std;

class WxStruct
{
public:
	wchar_t* pStr;  //文本数据的指针
	DWORD strLen;   //字符串的长度【低位】
	DWORD maxLen;   //字符串的最大长度【高位】
	DWORD64 full1;
	DWORD full2;
public:
	WxStruct(wchar_t* _pStr)
	{
		pStr = _pStr;
		strLen = wcslen(pStr);
		maxLen = strLen * 2;
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
	DWORD64 baseAddr = (DWORD64)LoadLibraryA("WeChatWin.dll");
	DWORD64 callAddr1 = baseAddr + 0x8C00E0;
	DWORD64 callAddr2 = baseAddr + 0xFCCF10;

	//构造函数指针
	typedef DWORD64(*Call1)();
	typedef DWORD64(*Call2)(DWORD64, DWORD64, DWORD64, DWORD64, DWORD64);

	//构造数据
	WxStruct sWxid((wchar_t*)L"filehelper");
	WxStruct imagefilepath((wchar_t*)L"C:\\Users\\Administrator\\Desktop\\1.png");

	//缓冲区
	char buf1[0x450] = { 0 };
	char buf2[0x450] = { 0 };

	//指针数组
	DWORD64* flag[10] = { 0 };
	DWORD64 temp1 = 0;
	DWORD64 temp2 = 0;
	flag[1] = (DWORD64*)&buf2;
	flag[8] = &temp1;
	flag[9] = &temp2;

	//获取句柄
	Call1 GetHandle = (Call1)callAddr1;
	DWORD64 h = GetHandle();

	//发送图片消息
	Call2 SendImg = (Call2)callAddr2;
	SendImg(
		h,
		(DWORD64)&buf1,
		(DWORD64)&sWxid,
		(DWORD64)&imagefilepath,
		(DWORD64)&flag
	);

	OutputDebugStringA("OK");
}
