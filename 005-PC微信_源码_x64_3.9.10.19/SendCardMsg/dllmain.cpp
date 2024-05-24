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

void SendFriendCard();

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        SendFriendCard();
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void SendFriendCard()
{
    DWORD64 baseAddr = (DWORD64)LoadLibraryA("WeChatWin.dll");
    DWORD64 callAddr1 = baseAddr + 0x25E4AE0;
    DWORD64 callAddr2 = baseAddr + 0x238EC70;

    typedef DWORD64(*PCall1)(DWORD64);
    PCall1 Call1 = PCall1(callAddr1);

    typedef DWORD64(*PCall2)(DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64, DWORD64);
    PCall2 Call2 = PCall2(callAddr2);

    WxStruct recvWxid((wchar_t*)L"filehelper");
    wchar_t* sendWxid = (wchar_t*)L"filehelper";
    wchar_t* nickName = (wchar_t*)L"测试";
	wchar_t xml[0x1024] = { 0 };
	swprintf_s(xml, L"<?xml version=\"1.0\"?><msg bigheadimgurl=\"\" username=\"%s\" nickname=\"%s\" fullpy=\"\" shortpy=\"\" alias=\"\" imagestatus=\"3\" scene=\"17\" province=\"\" city=\"\" sign=\"\" sex=\"\" certflag=\"0\" certinfo=\"\" brandIconUrl=\"\" brandHomeUrl=\"\" brandSubscriptConfigUrl= \"\" brandFlags=\"0\" regionCode=\"\" />", sendWxid, nickName);
    WxStruct xmlStr(xml);
    char buff[0x1024] = { 0 };

    Call1((DWORD64)&buff);
    Call2(
        (DWORD64)&buff,
        (DWORD64)&recvWxid,
        (DWORD64)&xmlStr,
        0x0, 0x2A, 0x1, 0x0, 0x0
    );
    OutputDebugStringA("OK");
}
