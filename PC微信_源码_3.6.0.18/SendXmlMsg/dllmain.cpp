// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "string"
using namespace std;

class WxStruct
{
public:
    wchar_t* pStr;
    int minLen;
    int maxLen;
    int full1;
    int full2;
public:
    WxStruct(wchar_t* str)
    {
        pStr = str;
        minLen = wcslen(pStr);
        maxLen = minLen * 2;
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
        DWORD cAddr = (DWORD)&Hook;
        //MessageBoxA(0, to_string(cAddr).c_str(), "TPS", 0);
        Hook();

		/*HANDLE hThread = CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Hook, NULL, NULL, NULL);
		if (hThread)
		{
			CloseHandle(hThread);
		}*/
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

    //缓冲区
    char ecxBuff[0x1024] = { 0 };

    //生成缓冲区的call
    DWORD callAdder1 = baseAddr + 0x954F0;
    DWORD callAdder2 = baseAddr + 0x614310;

    //发送call？
    DWORD callAdder3 = baseAddr + 0x391F80;

    //这个才是真正的发送call 29-3
    DWORD pushCallAddr = baseAddr + 0x223EC34;
    DWORD callAddr4 = baseAddr + 0x392150;

    wchar_t* _selfWxid = (wchar_t*)L"wxid_xxx";
    wchar_t* _cardTitle = (wchar_t*)L"我是标题信息";
    wchar_t* _cardDesc = (wchar_t*)L"我是描述信息";
    wchar_t* _targetUrl = (wchar_t*)L"https://www.baidu.com/";
    wchar_t* _headerUrl = (wchar_t*)L"http://wx.qlogo.cn/mmhead/ver_1/s7TdfrXLia8l8Z3u8uhssibB1ph2tEkx7zZiaSPeU9e0J7a1iahFfnBwbNGxJ8JQCW0l1cpSpdMMNEUADyG4GFPujqyaBpAibgkicoTiavak6KRdgc/132";
    
    wchar_t* _recvWxid = (wchar_t*)L"filehelper";

	//__asm
	//{
	//	// 生成缓冲区
	//	lea ecx, ecxBuff;
	//	call callAdder1;
	//}

    //构造数据【自身的微信ID】
    WxStruct selfWxid(_selfWxid);
    memcpy(ecxBuff + 0x3F8, &selfWxid, 20);

    //图片路径结构体【只关系到本地显示，可不传】
    //GenerateData imgPath((wchar_t*)L"C:\\Users\\Administrator\\Desktop\\RecvMsg\\1.png");
    //memcpy(ecxBuff + 0xEC4, &imgPath, 20);

    // xml结构体
    wchar_t xml[0x1024] = { 0 };
    swprintf_s(xml, L"<msg><fromusername>%s</fromusername><scene>0</scene><commenturl></commenturl><appmsg appid=\"\" sdkver=\"0\"><title>%s</title><des>%s</des><action>view</action><type>5</type><showtype>0</showtype><content></content><url>%s</url><dataurl></dataurl><lowurl></lowurl><lowdataurl></lowdataurl><recorditem></recorditem><thumburl>%s</thumburl><messageaction></messageaction><laninfo></laninfo><extinfo></extinfo><sourceusername></sourceusername><sourcedisplayname></sourcedisplayname><commenturl></commenturl><appattach><totallen>0</totallen><attachid></attachid><emoticonmd5></emoticonmd5><fileext></fileext><aeskey></aeskey><encryver>1</encryver><cdnthumblength>12051</cdnthumblength><cdnthumbheight>67</cdnthumbheight><cdnthumbwidth>120</cdnthumbwidth></appattach><weappinfo><pagepath></pagepath><username></username><appid></appid><appservicetype>0</appservicetype></weappinfo><websearch /></appmsg><appinfo><version>1</version><appname>Window wechat</appname></appinfo></msg>", _selfWxid, _cardTitle, _cardDesc, _targetUrl, _headerUrl);
    WxStruct xmlContent(xml);
    //GenerateData xmlContent((wchar_t*)L"<msg><fromusername>%s</fromusername><scene>0</scene><commenturl></commenturl><appmsg appid=\"\" sdkver=\"0\"><title>%s</title><des>%s</des><action>view</action><type>5</type><showtype>0</showtype><content></content><url>%s</url><dataurl></dataurl><lowurl></lowurl><lowdataurl></lowdataurl><recorditem></recorditem><thumburl>%s</thumburl><messageaction></messageaction><laninfo></laninfo><extinfo></extinfo><sourceusername></sourceusername><sourcedisplayname></sourcedisplayname><commenturl></commenturl><appattach><totallen>0</totallen><attachid></attachid><emoticonmd5></emoticonmd5><fileext></fileext><aeskey></aeskey><encryver>1</encryver><cdnthumblength>12051</cdnthumblength><cdnthumbheight>67</cdnthumbheight><cdnthumbwidth>120</cdnthumbwidth></appattach><weappinfo><pagepath></pagepath><username></username><appid></appid><appservicetype>0</appservicetype></weappinfo><websearch /></appmsg><appinfo><version>1</version><appname>Window wechat</appname></appinfo></msg>");
    //GenerateData xmlContent((wchar_t*)L"<msg><fromusername>wxid_6x0zjbwpvusu12</fromusername><scene>0</scene><commenturl></commenturl><appmsg appid=\"\" sdkver=\"0\"><title>百度一下，你就知道</title><des>https://www.baidu.com/</des><action>view</action><type>5</type><showtype>0</showtype><content></content><url>https://www.baidu.com/</url><dataurl></dataurl><lowurl></lowurl><lowdataurl></lowdataurl><recorditem></recorditem><thumburl>https://wx3.sinaimg.cn/mw690/002Qjjnugy1h9nt0qw49lj60qo0qogpi02.jpg</thumburl><messageaction></messageaction><extinfo></extinfo><sourceusername></sourceusername><sourcedisplayname></sourcedisplayname><commenturl></commenturl><appattach><totallen>0</totallen><attachid></attachid><emoticonmd5></emoticonmd5><fileext></fileext><cdnthumburl>3057020100044b304902010002043bb547c302032f5b0d0204307e0c7902046446bf93042433663865383735622d626438322d343361392d626466372d3564653438616639653130340204011808030201000405004c537500</cdnthumburl><aeskey></aeskey><cdnthumbaeskey>b8a91d895b639be35d516c7bde40300e</cdnthumbaeskey><cdnthumblength>3904</cdnthumblength><cdnthumbheight>150</cdnthumbheight><cdnthumbwidth>150</cdnthumbwidth></appattach><weappinfo><pagepath></pagepath><username></username><appid></appid><appservicetype>0</appservicetype></weappinfo><websearch /></appmsg><appinfo><version>1</version><appname>Window wechat</appname></appinfo></msg>");
    memcpy(ecxBuff + 0xE88, &xmlContent, 20);

    //构造数据【接收者微信ID】
    WxStruct recvWxid(_recvWxid);
    //memcpy(ecxBuff + 0xEF4, &recvWxid, 20);

    __asm
    {
		// 生成缓冲区
		lea ecx, ecxBuff;
		call callAdder1;

		/*push 0x1;
		lea ecx, [ecxBuff + 0x298];
		push ecx;
		lea ecx, ecxBuff;
		push ecx;
		lea ecx, [ecxBuff + 0xEAC];
		push ecx;
		mov ecx, eax;
		call callAdder2;*/

        lea eax, [ecxBuff + 0xEAC];
        push 0x5;
        lea edx, [ecxBuff + 0x3F8];
        push eax;
        lea eax, [ecxBuff + 0xEC4];
        push eax;
        lea eax, [ecxBuff + 0xE88];
        push eax;
        lea eax, recvWxid;  //lea eax, [ecxBuff + 0xEF4];
        push eax;
        lea ecx, ecxBuff;
        call callAdder3;
        add esp, 0x14;

		push pushCallAddr;
		push pushCallAddr;
		xor dl, dl;
		lea ecx, ecxBuff;
		call callAddr4;
		add esp, 0x8;
    }
}

