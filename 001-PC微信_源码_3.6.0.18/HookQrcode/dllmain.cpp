// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <iostream>

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


// 模块基址
DWORD baseAddr = (DWORD)GetModuleHandle(L"WeChatWin.dll");
DWORD hookAddr = baseAddr + 0x6FFAE9;
DWORD returnAddr = hookAddr + 5;
DWORD callAddr = baseAddr + 0x19F8DB0;
void __stdcall downQrcode(DWORD esi)
{
	DWORD imgSizeAddr = esi + 0x4;
	size_t imgSize = (size_t) * ((LPVOID*)imgSizeAddr);

	// 拷贝图片
	char imgData[0xFFF] = { 0 };
	memcpy(imgData, *((LPVOID*)esi), imgSize);

	// 创建文件句柄
	HANDLE hFile = CreateFileA("C:\\Users\\Administrator\\Desktop\\qrcode666.png", GENERIC_ALL, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile == NULL)
	{
		OutputDebugStringA("创建文件句柄失败");
		return;
	}

	// 写文件
	DWORD dwRead = 0;
	if (!WriteFile(hFile, imgData, imgSize, &dwRead, NULL))
	{
		OutputDebugStringA("写入失败");
		return;
	}
	CloseHandle(hFile);
}

__declspec(naked) void myCall()
{
	__asm
	{
		call callAddr;
		pushad;
		pushfd;
		push esi;
		call downQrcode;
		popfd;
		popad;
		jmp returnAddr;
	}
}

void Hook()
{
	BYTE jmpCode[5] = { 0 };
	jmpCode[0] = 0xE9;
	*(DWORD*)&jmpCode[1] = (DWORD)myCall - hookAddr - 5;
	DWORD OldProtext = 0;
	VirtualProtect((LPVOID)hookAddr, 5, PAGE_EXECUTE_READWRITE, &OldProtext);
	memcpy((void*)hookAddr, jmpCode, 5);
	VirtualProtect((LPVOID)hookAddr, 5, OldProtext, &OldProtext);
}
