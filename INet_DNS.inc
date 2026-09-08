// **************************************************************************
//  @file       INET_DNS.INC
//  @brief      Winsock2 を使用した DNS 名前解決モジュール
//
//  @author     Iwao ( https://mish.work/ )
//  @date       2026-09-07
//
//  @modify
//  2026-09-07  Newly created by AI assistant
//
//  @disclaimer
//  本コードの使用により生じたいかなる損害についても著作者は責任を負いません
//  引用時は上記 URL を明記してください
//
//  (C) 2026 Iwao. All Rights Reserved.
// **************************************************************************

#pragma     once

//#include <afx.h>
#include <tchar.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>

#pragma comment(lib, "ws2_32.lib")

#include "i_types.inc"

// VC6 ヘッダ未定義定数・構造体の補完
#ifndef AF_INET6
#define AF_INET6 23
#endif

#ifndef NI_MAXHOST
#define NI_MAXHOST 1025
#endif

#ifndef NI_NUMERICHOST
#define NI_NUMERICHOST 0x02
#endif

#ifndef NI_NAMEREQD
#define NI_NAMEREQD 0x04
#endif

typedef int  (WSAAPI *FN_GET_ADDRINFO)(const char*, const char*, const void*, void**);
typedef int  (WSAAPI *FN_GET_NAMEINFO)(const void*, int, char*, DWORD, char*, DWORD, int);
typedef void (WSAAPI *FN_FREEADDRINFO)(void*);

////
// **************************************************************************
//  Winsock2 初期化クラス (RAII パターン)
//  作成日    :    2026-09-07
// **************************************************************************
class CWSAinit
{
public:
	CWSAinit() : m_bInitialized(false)
	{
		WSADATA wsaData;
		if (::WSAStartup(MAKEWORD(2, 2), &wsaData) == 0) {
			m_bInitialized = true;
		}
	}

	~CWSAinit()
	{
		if (m_bInitialized) {
			::WSACleanup();
			m_bInitialized = false;
		}
	}

	bool IsOK() const { return m_bInitialized; }

private:
	bool m_bInitialized;
};

////
// **************************************************************************
//  ドメイン名/ホスト名から IP アドレスを取得 (IPv4/IPv6 正引き)
//  作成日    :    2026-09-07
// **************************************************************************
inline bool DNS_ResolveToIP(LPCTSTR lpszHost, tstring& strIP)
{
	strIP = _T("");

	if (lpszHost == NULL || ::_tcslen(lpszHost) == 0) {
		return false;
	}

	#ifdef _UNICODE
		int nLen = ::WideCharToMultiByte(CP_ACP, 0, lpszHost, -1, NULL, 0, NULL, NULL);
		std::vector<char> vHost(nLen);
		::WideCharToMultiByte(CP_ACP, 0, lpszHost, -1, &vHost[0], nLen, NULL, NULL);
		const char* szHost = &vHost[0];
	#else
		const char* szHost = lpszHost;
	#endif

	HMODULE hModule = ::GetModuleHandle(_T("ws2_32.dll"));
	if (hModule == NULL) {
		hModule = ::LoadLibrary(_T("ws2_32.dll"));
	}

	FN_GET_ADDRINFO pfnGet_AddrInfo = NULL;
	FN_GET_NAMEINFO pfnGet_NameInfo = NULL;
	FN_FREEADDRINFO pfnFreeAddrInfo = NULL;

	if (hModule != NULL) {
		pfnGet_AddrInfo = (FN_GET_ADDRINFO)::GetProcAddress(hModule,  "getaddrinfo");
		pfnGet_NameInfo = (FN_GET_NAMEINFO)::GetProcAddress(hModule,  "getnameinfo");
		pfnFreeAddrInfo = (FN_FREEADDRINFO)::GetProcAddress(hModule, "freeaddrinfo");
	}

	// getaddrinfo が利用可能な場合（IPv4/IPv6 両対応）
	if (pfnGet_AddrInfo != NULL && pfnGet_NameInfo != NULL && pfnFreeAddrInfo != NULL) {
		// VC6 用の疑似 addrinfo 構造体（メモリレイアウト互換）
		struct {
			int flags;
			int family;
			int socktype;
			int protocol;
			size_t addrlen;
			char* canonname;
			void* addr;
			void* next;
		} hints, *pResult = NULL;

		::memset(&hints, 0, sizeof(hints));
		hints.family = AF_UNSPEC;

		if (pfnGet_AddrInfo(szHost, NULL, &hints, (void**)&pResult) == 0 && pResult != NULL) {
			char szIPBuf[128] = {0};

			pfnGet_NameInfo(pResult->addr, (int)pResult->addrlen, szIPBuf, sizeof(szIPBuf), NULL, 0, NI_NUMERICHOST);
			pfnFreeAddrInfo(pResult);

			if (::strlen(szIPBuf) > 0) {
				#ifdef _UNICODE
					int nWLen = ::MultiByteToWideChar(CP_ACP, 0, szIPBuf, -1, NULL, 0);
					std::vector<wchar_t> wIP(nWLen);
					::MultiByteToWideChar(CP_ACP, 0, szIPBuf, -1, &wIP[0], nWLen);
					strIP = &wIP[0];
				#else
					strIP = szIPBuf;
				#endif
				return true;
			}
		}
	}

	// フォールバック: 旧来の gethostbyname (IPv4 のみ)
	hostent* pHost = ::gethostbyname(szHost);
	if (pHost != NULL && pHost->h_addr_list[0] != NULL) {
		in_addr addr;
		::memcpy(&addr, pHost->h_addr_list[0], sizeof(in_addr));
		char* szIP = ::inet_ntoa(addr);
		if (szIP != NULL) {
			#ifdef _UNICODE
				int nWLen = ::MultiByteToWideChar(CP_ACP, 0, szIP, -1, NULL, 0);
				std::vector<wchar_t> wIP(nWLen);
				::MultiByteToWideChar(CP_ACP, 0, szIP, -1, &wIP[0], nWLen);
				strIP = &wIP[0];
			#else
				strIP = szIP;
			#endif
			return true;
		}
	}

	return false;
}

////
// **************************************************************************
//  IP アドレスからドメイン名を取得 (IPv4/IPv6 逆引き)
//  作成日    :    2026-09-07
// **************************************************************************
inline bool DNS_ResolveToHost(LPCTSTR lpszIP, tstring& strHost)
{
	strHost = _T("");

	if (lpszIP == NULL || ::_tcslen(lpszIP) == 0) {
		return false;
	}

	#ifdef _UNICODE
		int nLen = ::WideCharToMultiByte(CP_ACP, 0, lpszIP, -1, NULL, 0, NULL, NULL);
		std::vector<char> vIP(nLen);
		::WideCharToMultiByte(CP_ACP, 0, lpszIP, -1, &vIP[0], nLen, NULL, NULL);
		const char* szIP = &vIP[0];
	#else
		const char* szIP = lpszIP;
	#endif

	HMODULE hModule = ::GetModuleHandle(_T("ws2_32.dll"));
	if (hModule == NULL) {
		hModule = ::LoadLibrary(_T("ws2_32.dll"));
	}

	FN_GET_ADDRINFO pfnGet_AddrInfo = NULL;
	FN_GET_NAMEINFO pfnGet_NameInfo = NULL;
	FN_FREEADDRINFO pfnFreeAddrInfo = NULL;

	if (hModule != NULL) {
		pfnGet_AddrInfo = (FN_GET_ADDRINFO)::GetProcAddress(hModule,  "getaddrinfo");
		pfnGet_NameInfo = (FN_GET_NAMEINFO)::GetProcAddress(hModule,  "getnameinfo");
		pfnFreeAddrInfo = (FN_FREEADDRINFO)::GetProcAddress(hModule, "freeaddrinfo");
	}

	if (pfnGet_AddrInfo != NULL && pfnGet_NameInfo != NULL && pfnFreeAddrInfo != NULL) {
		struct {
			int flags;
			int family;
			int socktype;
			int protocol;
			size_t addrlen;
			char* canonname;
			void* addr;
			void* next;
		} hints, *pResult = NULL;

		::memset(&hints, 0, sizeof(hints));
		hints.family = AF_UNSPEC;
		hints.flags = NI_NUMERICHOST;

		if (pfnGet_AddrInfo(szIP, NULL, &hints, (void**)&pResult) == 0 && pResult != NULL) {
			char szHostBuf[NI_MAXHOST] = {0};
			int nRet = pfnGet_NameInfo(pResult->addr, (int)pResult->addrlen, szHostBuf, sizeof(szHostBuf), NULL, 0, NI_NAMEREQD);

			pfnFreeAddrInfo(pResult);

			if (nRet == 0 && ::strlen(szHostBuf) > 0) {
				#ifdef _UNICODE
					int nWLen = ::MultiByteToWideChar(CP_ACP, 0, szHostBuf, -1, NULL, 0);
					std::vector<wchar_t> wHost(nWLen);
					::MultiByteToWideChar(CP_ACP, 0, szHostBuf, -1, &wHost[0], nWLen);
					strHost = &wHost[0];
				#else
					strHost = szHostBuf;
				#endif
				return true;
			}
		}
	}

	// フォールバック: 旧来の gethostbyaddr (IPv4 のみ)
	unsigned long ulAddr = ::inet_addr(szIP);
	if (ulAddr != INADDR_NONE) {
		hostent* pHost = ::gethostbyaddr((const char*)&ulAddr, sizeof(ulAddr), AF_INET);
		if (pHost != NULL && pHost->h_name != NULL) {
			#ifdef _UNICODE
				int nWLen = ::MultiByteToWideChar(CP_ACP, 0, pHost->h_name, -1, NULL, 0);
				std::vector<wchar_t> wHost(nWLen);
				::MultiByteToWideChar(CP_ACP, 0, pHost->h_name, -1, &wHost[0], nWLen);
				strHost = &wHost[0];
			#else
				strHost = pHost->h_name;
			#endif
			return true;
		}
	}

	return false;
}

