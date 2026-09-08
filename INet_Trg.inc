// **************************************************************************
//  @file       INET_TRG.INC
//  @brief      入力文字列からの IP アドレス・ドメイン名・ホスト名抽出モジュール (VC6/C++98 互換)
//
//  @author     Iwao ( https://mish.work/ )
//  @date       2026-09-08
//
//  @modify
//  2026-09-08  Newly created by AI assistant
//
//  @disclaimer
//  本コードの使用により生じたいかなる損害についても著作者は責任を負いません
//  引用時は上記 URL を明記してください
//
//  (C) 2026 Iwao. All Rights Reserved.
// **************************************************************************

#pragma     once

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <windows.h>
#include <tchar.h>

#include "i_types.inc"

#pragma comment(lib, "ws2_32.lib")

////
// **************************************************************************
//  IPv6 アドレス文字列の判定
//  作成日    :    2026-09-08
// **************************************************************************
inline bool IsIPv6Address(const tstring& strToken)
{
	int nColonCount = 0;
	size_t nBlockLen = 0;
	bool bHasHexAlpha = false;
	size_t nMaxBlockLen = 0;

	for (size_t i = 0; i < strToken.length(); ++i) {
		TCHAR c = strToken[i];
		if (c == _T(':')) {
			nColonCount++;
			if (nBlockLen > nMaxBlockLen) {
				nMaxBlockLen = nBlockLen;
			}
			if (nBlockLen > 4) {
				return false;
			}
			nBlockLen = 0;
		} else if (c >= _T('0') && c <= _T('9')) {
			nBlockLen++;
		} else if ((c >= _T('a') && c <= _T('f')) || (c >= _T('A') && c <= _T('F'))) {
			bHasHexAlpha = true;
			nBlockLen++;
		} else {
			return false;
		}
	}
	if (nBlockLen > nMaxBlockLen) {
		nMaxBlockLen = nBlockLen;
	}
	if (nBlockLen > 4) {
		return false;
	}

	if (nColonCount == 2 && !bHasHexAlpha && nMaxBlockLen <= 2) {
		return false;
	}

	return (nColonCount >= 2);
}

////
// **************************************************************************
//  入力文字列から IP アドレス・ドメイン名・ホスト名を抽出
//  作成日    :    2026-09-08
// **************************************************************************
inline tstring ExtractTarget(const tstring& strInput)
{
	v_tstring vTokens;
	size_t nStart = 0;

	// 1. 空白やタブでトークンに分割
	while (nStart < strInput.length()) {
		size_t nEnd = strInput.find_first_of(_T(" \t\r\n"), nStart);
		if (nEnd == tstring::npos) {
			vTokens.push_back(strInput.substr(nStart));
			break;
		}
		if (nEnd > nStart) {
			vTokens.push_back(strInput.substr(nStart, nEnd - nStart));
		}
		nStart = nEnd + 1;
	}

	// 2. 各トークンからターゲットを検証・解析
	for (size_t i = 0; i < vTokens.size(); ++i) {
		tstring strToken = vTokens[i];

		// プロトコル (https:// 等) が含まれている場合のみパス区切り (/) を処理
		size_t nSchemePos = strToken.find(_T("://"));
		if (nSchemePos != tstring::npos) {
			strToken = strToken.substr(nSchemePos + 3);
			size_t nSlashPos = strToken.find(_T('/'));
			if (nSlashPos != tstring::npos) {
				strToken = strToken.substr(0, nSlashPos);
			}
		}

		// [IPv6]:port 形式の括弧解除処理
		if (strToken.length() > 0 && strToken[0] == _T('[')) {
			size_t nClosePos = strToken.find(_T(']'));
			if (nClosePos != tstring::npos) {
				strToken = strToken.substr(1, nClosePos - 1);
			}
		} else {
			// ドメイン/IPv4 のポート番号 (:443 等) 除去
			size_t nLastColon = strToken.rfind(_T(':'));
			if (nLastColon != tstring::npos && strToken.find(_T('.')) != tstring::npos) {
				bool bIsPort = true;
				for (size_t k = nLastColon + 1; k < strToken.length(); ++k) {
					TCHAR c = strToken[k];
					if (c < _T('0') || c > _T('9')) {
						bIsPort = false;
						break;
					}
				}
				if (bIsPort && nLastColon + 1 < strToken.length()) {
					strToken = strToken.substr(0, nLastColon);
				}
			}
		}

		// パス指定 (/ で始まるトークン) は除外
		if (strToken.length() > 0 && strToken[0] == _T('/')) {
			continue;
		}

		// 日付形式 (2026/09/08 等) は除外
		if (strToken.find(_T('/')) != tstring::npos) {
			continue;
		}

		// HTTP メソッド等の標準キーワードを除外
		if (strToken == _T("GET") || strToken == _T("POST") || strToken == _T("PUT") ||
		    strToken == _T("DELETE") || strToken == _T("HEAD") || strToken == _T("OPTIONS")) {
			continue;
		}

		// HTTP ステータスコード（数字 3 桁）を除外
		if (strToken.length() == 3) {
			bool bIsNumeric = true;
			for (size_t k = 0; k < 3; ++k) {
				if (strToken[k] < _T('0') || strToken[k] > _T('9')) {
					bIsNumeric = false;
					break;
				}
			}
			if (bIsNumeric) {
				continue;
			}
		}

		// IPv6 アドレスの判定
		if (::IsIPv6Address(strToken)) {
			return strToken;
		}

		// 時刻文字列（HH:MM:SS）の除外判定
		if (strToken.find(_T(':')) != tstring::npos) {
			continue;
		}

		// ドットが含まれる場合 (ドメイン名または IPv4)
		if (strToken.find(_T('.')) != tstring::npos) {
			bool bHasNonHexChar = false;
			for (size_t j = 0; j < strToken.length(); ++j) {
				TCHAR c = strToken[j];
				if ((c >= _T('g') && c <= _T('z')) || (c >= _T('G') && c <= _T('Z'))) {
					bHasNonHexChar = true;
					break;
				}
			}

			if (bHasNonHexChar) {
				return strToken;
			}

#if defined(_UNICODE) || defined(UNICODE)
			char szAnsi[256] = {0};
			::WideCharToMultiByte(CP_ACP, 0, strToken.c_str(), -1, szAnsi, sizeof(szAnsi), NULL, NULL);
			if (::inet_addr(szAnsi) != INADDR_NONE) {
				return strToken;
			}
#else
			if (::inet_addr(strToken.c_str()) != INADDR_NONE) {
				return strToken;
			}
#endif
		} else {
			// ドットもコロンも含まないトークン (DS220, WDCloud 等) はホスト名・識別子として返却
			return strToken;
		}
	}

	return _T("");
}

