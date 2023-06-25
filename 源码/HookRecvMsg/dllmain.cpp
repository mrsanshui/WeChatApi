// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <iostream>
#include <string>
#include <time.h>
using namespace std;

void Hook();

wstring GetMsgByAddress(DWORD memAddress)
{
	wstring tmp;
	DWORD msgLength = *(DWORD*)(memAddress + 4);
	if (msgLength > 0) {
		WCHAR* msg = new WCHAR[msgLength + 1]{ 0 };
		wmemcpy_s(msg, msgLength + 1, (WCHAR*)(*(DWORD*)memAddress), msgLength + 1);
		tmp = msg;
		delete[]msg;
	}
	return tmp;
}

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

// 模块基址
DWORD baseAddr = (DWORD)LoadLibraryA("WeChatWin.dll");
DWORD hookAddr = baseAddr + 0x4C2DC0;
DWORD returnAddr = hookAddr + 5;
void __stdcall RecvMsg(DWORD eax)
{
	//消息类型
	DWORD msgType = *(DWORD*)(eax + 0x38);
	OutputDebugStringW(to_wstring(msgType).c_str());

	//是否是自己发送的消息【别人发送的是0  自己发送的是1】
	BYTE isRecv = *(BYTE*)(eax + 0x3C);
	OutputDebugStringA(isRecv == 0 ? "【别人发送的】" : "【自己发送的】");

	//微信ID
	wstring wxId = GetMsgByAddress(eax + 0x48);
	OutputDebugStringW(wxId.c_str());

	//消息内容
	wstring content = GetMsgByAddress(eax + 0x70);
	OutputDebugStringW(content.c_str());

	// 是否是PC端发送的消息【自己PC端发送的消息最后一个字节是0，  自己移动端或者别人发送的消息最后一个字节是1】
	BYTE isPcMsg = *(BYTE*)(eax + 0xD8);
	OutputDebugStringA(isPcMsg == 0 ? "【PC端消息】" : "【其他设备】");

	//群聊时发送者的微信ID【不是群聊 或者 自己在群里发送消息的时候：为0】
	wstring sender = GetMsgByAddress(eax + 0x170);
	if (sender.empty())
	{
		OutputDebugStringA("sender为：NULL");
	}
	else
	{
		OutputDebugStringW(sender.c_str());
	}

	//文件路径
	wstring filePath = GetMsgByAddress(eax + 0x198);
	if (filePath.empty())
	{
		OutputDebugStringA("文件路径为：NULL");
	}
	else
	{
		OutputDebugStringW(filePath.c_str());
	}

	//当前微信ID
	wstring selfWxId = GetMsgByAddress(eax + 0x298);
	OutputDebugStringW(selfWxId.c_str());

	//消息ID
	int msgId;

	OutputDebugStringA("***************");
}

__declspec(naked) void myCall()
{
	__asm
	{
		pushad;
		pushfd;
		push eax;
		call RecvMsg;
		popfd;
		popad;
		push ebp;
		mov ebp, esp;
		push - 0x1;
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
