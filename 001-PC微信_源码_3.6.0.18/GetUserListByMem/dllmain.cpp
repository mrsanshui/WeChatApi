// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <string>
#include <map>
#include <list>
using namespace std;

void Hook();

map<DWORD, BOOL>isOk;
map<DWORD, BOOL>isOk2;

wstring StringToWstring(string str)
{
	wstring result;
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';
	result.append(buffer);
	delete[] buffer;
	return result;
}

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

string DecIntToHexStr(DWORD num)
{
	try
	{
		string str;
		DWORD Temp = num / 16;
		DWORD left = num % 16;
		if (Temp > 0)
			str += DecIntToHexStr(Temp);
		if (left < 10)
			str += (left + '0');
		else
			str += ('A' + left - 10);
		return str;
	}
	catch (...)
	{
		return "0x??????";
	}
}

BOOL APIENTRY DllMain( HMODULE hModule,
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

void PrintUserList()
{
	for (auto it = isOk2.begin(); it != isOk2.end(); it++)
	{
		DWORD curAddr = it->first;

		wstring addr = StringToWstring(DecIntToHexStr(curAddr));
		wstring wxId = GetMsgByAddress(curAddr + 0x30);
		wstring wxAccount = GetMsgByAddress(curAddr + 0x44);
		wstring wxRemark = GetMsgByAddress(curAddr + 0x78);
		wstring wxNickName = GetMsgByAddress(curAddr + 0x8C);
		wstring wxAvatar = GetMsgByAddress(curAddr + 0x11C);

		OutputDebugStringW((L"地  址：" + addr).c_str());
		OutputDebugStringW((L"微信ID：" + wxId).c_str());
		OutputDebugStringW((L"账  号：" + wxAccount).c_str());
		OutputDebugStringW((L"备  注：" + wxRemark).c_str());
		OutputDebugStringW((L"昵  称：" + wxNickName).c_str());
		OutputDebugStringW((L"头  像：" + wxAvatar).c_str());
		OutputDebugStringA("################################");
	}
}

void GetInfo(DWORD curAddr, string src)
{
	if (isOk.count(curAddr))
	{
		return;
	}
	isOk[curAddr] = TRUE;
	isOk2[curAddr] = TRUE;

	DWORD header1 = *(DWORD*)curAddr;
	DWORD header2 = *(DWORD*)(curAddr + 0x4);

	string log = "";
	GetInfo(header1, log);
	GetInfo(header2, log);
}

void Hook()
{
    DWORD baseAddr = (DWORD)LoadLibraryA("WeChatWin.dll");

    // 链表头指针
    DWORD startAddr = *(DWORD*)(baseAddr + 0x222F3BC) + 0x4C;

    //头节点地址
    DWORD rootAddr = *(DWORD*)startAddr;
	OutputDebugStringA(("头结点地址：" + DecIntToHexStr(rootAddr)).c_str());

    //联系人数量
    DWORD count = *(DWORD*)(startAddr + 0x4);
    OutputDebugStringA(to_string(count).c_str());

    isOk[rootAddr] = TRUE;

	DWORD header1 = *(DWORD*)rootAddr;
	DWORD header2 = *(DWORD*)(rootAddr + 0x4);

	OutputDebugStringA(("分支1：" + DecIntToHexStr(header1)).c_str());
	OutputDebugStringA(("分支2：" + DecIntToHexStr(header2)).c_str());
	OutputDebugStringA("===========================================");

    GetInfo(header1, DecIntToHexStr(rootAddr));
    GetInfo(header2, DecIntToHexStr(rootAddr));

	//输出
	PrintUserList();
}

