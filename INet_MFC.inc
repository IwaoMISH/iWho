// **************************************************************************
//  @file       INET_MFC.INC
//  @brief      WinINet を使用した HTTP GET 通信モジュール (MFC 版)
//
//  @author     Iwao ( https://mish.work/ )
//  @date       2026-09-07
//
//  @modify
//  2026-09-07  新規作成
//
//  @disclaimer
//  本コードの使用により生じたいかなる損害についても著作者は責任を負いません
//  引用時は上記 URL を明記してください
//
//  (C) 2026 Iwao. All Rights Reserved.
// **************************************************************************

#pragma     once

#ifdef      _MSC_VER
	#pragma warning (disable    :   4786)
#endif

#include    <afxinet.h>
#include    <fstream>
#include    <vector>
#include    "i_types.inc"

////
// **************************************************************************
//  URL からテキスト（JSON 等）を取得する
//  作成日    :    2026-09-07
// **************************************************************************
inline bool IN_GetText(LPCTSTR lpszUrl, tstring& strResponse)
{
	strResponse.erase();

	if (lpszUrl == NULL || ::_tcslen(lpszUrl) == 0) {
		return false;
	}

	// 1. URL の解析
	DWORD	dwServiceType = 0;
	CString	strServer;
	CString	strObject;
	INTERNET_PORT nPort = 0;

	if (!::AfxParseURL(lpszUrl, dwServiceType, strServer, strObject, nPort)) {
		return false;
	}

	// 2. HTTP / HTTPS セッションの開始
	CInternetSession session(_T("INet_MFC_Client"));
	CHttpConnection* pConnection = NULL;
	CHttpFile* pFile = NULL;

	bool bSuccess = false;

	try {
		pConnection = session.GetHttpConnection(strServer, nPort);
		if (pConnection == NULL) {
			session.Close();
			return false;
		}

		DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE;
		if (dwServiceType == AFX_INET_SERVICE_HTTPS) {
			dwFlags |= INTERNET_FLAG_SECURE;
		}

		pFile = pConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, dwFlags);
		if (pFile == NULL) {
			pConnection->Close();
			delete pConnection;
			session.Close();
			return false;
		}

		// リクエスト送信
		pFile->SendRequest();

		// ステータスコードチェック
		DWORD dwStatusCode = 0;
		pFile->QueryInfoStatusCode(dwStatusCode);

		if (dwStatusCode == HTTP_STATUS_OK) {
			// 生のバイトデータをバッファへ読み込み
			std::vector<char> rawBuffer;
			char buffer[1024];
			UINT nRead = 0;

			while ((nRead = pFile->Read(buffer, sizeof(buffer))) > 0) {
				rawBuffer.insert(rawBuffer.end(), buffer, buffer + nRead);
			}

			if (!rawBuffer.empty()) {
				rawBuffer.push_back('\0'); // ヌル終端

				#ifdef _UNICODE
					// UTF-8 から ワイド文字列 (wchar_t) への変換
					int nLen = ::MultiByteToWideChar(CP_UTF8, 0, &rawBuffer[0], -1, NULL, 0);
					if (nLen > 0) {
						std::vector<wchar_t> wBuffer(nLen);
						::MultiByteToWideChar(CP_UTF8, 0, &rawBuffer[0], -1, &wBuffer[0], nLen);
						strResponse = &wBuffer[0];
						bSuccess = true;
					}
				#else
					// マルチバイト環境の場合はそのまま代入
					strResponse = &rawBuffer[0];
					bSuccess = true;
				#endif
			}
		}

		pFile->Close();
		delete pFile;
		pConnection->Close();
		delete pConnection;
		session.Close();
	}
	catch (CInternetException* pEx) {
		pEx->Delete();
		if (pFile != NULL) { pFile->Close(); delete pFile; }
		if (pConnection != NULL) { pConnection->Close(); delete pConnection; }
		session.Close();
		return false;
	}

	return bSuccess;
}

////
// **************************************************************************
//  URL からレスポンスをファイルへ保存する
//  作成日    :    2026-09-07
// **************************************************************************
inline bool IN_GetFile(LPCTSTR lpszUrl, LPCTSTR lpszFilePath)
{
	if (lpszFilePath == NULL || ::_tcslen(lpszFilePath) == 0) {
		return false;
	}

	tstring strResponse;
	if (!::IN_GetText(lpszUrl, strResponse)) {
		return false;
	}

	// ANSI 文字列に変換してファイルへ書き出し
	#ifdef _UNICODE
		int nLen = ::WideCharToMultiByte(CP_ACP, 0, strResponse.c_str(), -1, NULL, 0, NULL, NULL);
		std::vector<char> vBuffer(nLen);
		::WideCharToMultiByte(CP_ACP, 0, strResponse.c_str(), -1, &vBuffer[0], nLen, NULL, NULL);
		std::ofstream ofs(vBuffer.empty() ? "" : &vBuffer[0], std::ios::out | std::ios::binary | std::ios::trunc);
	#else
		std::ofstream ofs(lpszFilePath, std::ios::out | std::ios::binary | std::ios::trunc);
	#endif

	if (!ofs.is_open()) {
		return false;
	}

	#ifdef _UNICODE
		int nMbLen = ::WideCharToMultiByte(CP_UTF8, 0, strResponse.c_str(), -1, NULL, 0, NULL, NULL);
		if (nMbLen > 0) {
			std::vector<char> utf8Buf(nMbLen);
			::WideCharToMultiByte(CP_UTF8, 0, strResponse.c_str(), -1, &utf8Buf[0], nMbLen, NULL, NULL);
			ofs.write(&utf8Buf[0], nMbLen - 1);
		}
	#else
		ofs << strResponse.c_str();
	#endif

	ofs.close();
	return true;
}
