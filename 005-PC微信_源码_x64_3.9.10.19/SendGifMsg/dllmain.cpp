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

void SendGif();

BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SendGif, NULL, NULL, NULL);
		if (hThread) CloseHandle(hThread);
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}


void SendGif()
{
	DWORD64 baseAddr = (DWORD64)LoadLibraryA("WeChatWin.dll");
	DWORD64 callAddr1 = baseAddr + 0x1C988B0;
	DWORD64 callAddr2 = baseAddr + 0x227CA40;

	typedef DWORD64(*PCall1)();
	PCall1 Call1 = (PCall1)(callAddr1);

	typedef DWORD64(*PCall2)(DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64);
	PCall2 Call2 = (PCall2)(callAddr2);

	wchar_t* _wxId = (wchar_t*)L"filehelper";
	size_t wxIdLen = wcslen(_wxId) + 1;
	wchar_t* p_wxId = (wchar_t*)HeapAlloc(GetProcessHeap(), 0, wxIdLen * 2);
	if (!p_wxId) return;
	wmemcpy(p_wxId, _wxId, wxIdLen);
	WxStruct recvWxId(p_wxId);

	wchar_t* _filePath = (wchar_t*)L"C:\\Users\\Administrator\\Desktop\\1.gif";
	size_t filePathLen = wcslen(_filePath) + 1;
	wchar_t* p_filePath = (wchar_t*)HeapAlloc(GetProcessHeap(), 0, filePathLen * 2);
	if (!p_filePath) return;
	wmemcpy(p_filePath, _filePath, filePathLen);
	WxStruct filePath(p_filePath);

	//缓冲区
	char buf[0x450] = { 0 };
	char temp1[0x20] = { 0 };
	char temp2[0x20] = { 0 };
	Call2(Call1(), (DWORD64)&filePath, (DWORD64)&buf, (DWORD64)&recvWxId, 0x2, (DWORD64)&temp1, 0x0, (DWORD64)&temp2);
	
	OutputDebugStringA("OK");
}

