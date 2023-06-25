// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <string>
using namespace std;

class WxStruct
{
public:
	wchar_t* pStr;  // 文本数据的指针
	int minLen = 0;  // 字符串的长度
	int maxLen = 0;  // 字符串的最大长度
	int full1 = 0;
	int full2 = 0;
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

LPCWSTR GetMsgByAddress(DWORD MsgAddr)
{
	DWORD MsgLen = *(DWORD*)(MsgAddr + 4);
	if (MsgLen == 0)
	{
		WCHAR* msg = new WCHAR[1];
		msg[0] = 0;
		return msg;
	}
	WCHAR* msg = new WCHAR[MsgLen + 1];
	ZeroMemory(msg, MsgLen + 1);
	wmemcpy_s(msg, MsgLen + 1, (WCHAR*)(*(DWORD*)MsgAddr), MsgLen + 1);
	return msg;
}

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
	DWORD baseAddr = (DWORD)LoadLibraryA("WeChatWin.dll");
	DWORD callAddr0 = baseAddr + 0x9A000;
	DWORD callAddr1 = baseAddr + 0x701DC0;
	DWORD callAddr2 = baseAddr + 0x4024A0;
	DWORD callAddr3 = baseAddr + 0x64BBB0;
	DWORD callAddr4 = baseAddr + 0xBF550;
	DWORD callAddr5 = baseAddr + 0x3E46D0;

	DWORD paramAddr1 = baseAddr + 0x222F3BC;

	char buffer1[0xFFF] = { 0 };
	char buffer2[0xFFF] = { 0 };

	WxStruct chatRoomId((wchar_t*)L"123@chatroom");

	__asm
	{
		mov eax, [paramAddr1]; 
		mov esi, [eax]; 
		lea eax, buffer1;
		push eax;
		sub esp, 0x14;
		lea ebx, chatRoomId;
		mov ecx, esp;
		push ebx;
		call callAddr1;
		mov ecx, esi;
		call callAddr2;
		lea ecx, buffer2;
		call callAddr3;
		call callAddr4;
		lea eax, buffer2;
		push eax;
		push ebx;
		call callAddr5;
	}

	string memberListStr = (char*)(*(DWORD*)(buffer2 + 0x1C));
	OutputDebugStringA(memberListStr.c_str());

	//群主ID
	wstring qzId = GetMsgByAddress((DWORD)buffer2 + 0x4C);
	OutputDebugStringW(qzId.c_str());

	//群成员数量
	int memberNum = *(int*)(buffer2 + 0x9C);
	OutputDebugStringA(to_string(memberNum).c_str());
	OutputDebugStringA("**************************");
}

