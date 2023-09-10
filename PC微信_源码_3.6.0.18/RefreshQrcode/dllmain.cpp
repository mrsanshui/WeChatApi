// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"

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

//刷新登录二维码
void Hook()
{
	DWORD baseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
	DWORD qrCodeAddr1 = baseAddr + 0x31E4F8;
	DWORD qrCodeAddr2 = baseAddr + 0x31E4FF;

	__asm
	{
		call qrCodeAddr1;
		mov ecx, eax;
		call qrCodeAddr2;
	}
}

