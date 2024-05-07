// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include <Windows.h>
#include <string>
using namespace std;

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
        Hook();
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
	DWORD isLoginAddr = baseAddr + 0x222EC90;

	//登录状态
	int isLogin = *(DWORD*)isLoginAddr;
	OutputDebugStringA(to_string(isLogin).c_str());

	char tempContent[0x300];

	//微信号【字符串】【ASCII】
	memset(tempContent, 0, 0x300);
	DWORD wxAccountAddr = isLoginAddr + 0xA0;
	sprintf_s(tempContent, "%s", (char*)wxAccountAddr);
	OutputDebugStringA(tempContent);

	//微信ID【新版本的wxid是指针，老版本的wxid是字符串】
	memset(tempContent, 0, 0x300);
	DWORD wxIdAddr = isLoginAddr + 0x390;
	sprintf_s(tempContent, "%s", (char*)wxIdAddr);
	if (strlen(tempContent) < 0x6 || strlen(tempContent) > 0x14)
	{
		sprintf_s(tempContent, "%s", (char*)(*(DWORD*)wxIdAddr));
	}
	OutputDebugStringA(tempContent);

	//小头像【指针】【ASCII】
	memset(tempContent, 0, 0x300);
	DWORD smallAvatarAddr = isLoginAddr + 0x204;
	sprintf_s(tempContent, "%s", (char*)(*(DWORD*)smallAvatarAddr));
	OutputDebugStringA(tempContent);

	//国家【字符串】【ASCII】
	memset(tempContent, 0, 0x300);
	DWORD gjAddr = isLoginAddr + 0x118;
	sprintf_s(tempContent, "%s", (char*)gjAddr);
	OutputDebugStringA(tempContent);

	//省份【字符串】【ASCII】
	memset(tempContent, 0, 0x300);
	DWORD sfAddr = isLoginAddr + 0x2C;
	sprintf_s(tempContent, "%s", (char*)sfAddr);
	OutputDebugStringA(tempContent);

	//城市【字符串】【ASCII】
	memset(tempContent, 0, 0x300);
	DWORD cityAddr = isLoginAddr + 0x44;
	sprintf_s(tempContent, "%s", (char*)cityAddr);
	OutputDebugStringA(tempContent);

	//手机号【字符串】【ASCII】
	memset(tempContent, 0, 0x300);
	DWORD mobileAddr = isLoginAddr - 0xA8;
	sprintf_s(tempContent, "%s", (char*)mobileAddr);
	OutputDebugStringA(tempContent);

	//昵称【字符串】【ASCII】
	memset(tempContent, 0, 0x300);
	DWORD nicknameAddr = isLoginAddr - 0xDC;
	sprintf_s(tempContent, "%s", (char*)nicknameAddr);
	OutputDebugStringA(tempContent);

	//缓存目录【Unicode】【指针】
	DWORD cacheAddr = isLoginAddr + 0x304;
	wstring cacheDir = GetMsgByAddress(cacheAddr);
	OutputDebugStringW(cacheDir.c_str());

	//登录设备【字符串】【ASCII】
	memset(tempContent, 0, 0x300);
	DWORD sbAddr = isLoginAddr + 0x3C8;
	sprintf_s(tempContent, "%s", (char*)sbAddr);
	OutputDebugStringA(tempContent);

	//性别
	DWORD sexAddr = isLoginAddr + 0x28;
	BYTE sex = *(BYTE*)sexAddr;
	OutputDebugStringA(to_string(sex).c_str());
}
