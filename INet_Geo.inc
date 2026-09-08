// **************************************************************************
//  @file       INET_GEO.INC
//  @brief      GeoIP 情報の保持およびプライベート IP 判定モジュール (VC6/C++98 互換)
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

#include <tchar.h>
#include "i_types.inc"

////
// **************************************************************************
//  GeoIP 情報格納構造体
//  作成日    :    2026-09-07
// **************************************************************************
struct GEOIP_INFO
{
	bool    bSuccess;       // 取得成功フラグ
	tstring strIP;          // IP アドレス
	tstring strCountry;     // 国名
	tstring strRegion;      // 地域 / 都道府県
	tstring strCity;        // 市区町村
	tstring strPostal;      // 郵便番号
	double  dLatitude;      // 緯度
	double  dLongitude;     // 経度
	tstring strTimezone;    // タイムゾーン
	tstring strISP;         // ISP 名
	tstring strASN;         // ASN
	tstring strMessage;     // エラーメッセージ等

	GEOIP_INFO()
	{
		Clear();
	}

	void Clear()
	{
		bSuccess = false;
		strIP = _T("");
		strCountry = _T("");
		strRegion = _T("");
		strCity = _T("");
		strPostal = _T("");
		dLatitude = 0.0;
		dLongitude = 0.0;
		strTimezone = _T("");
		strISP = _T("");
		strASN = _T("");
		strMessage = _T("");
	}
};

////
// **************************************************************************
//  プライベート / ループバック IP アドレスの判定
//  作成日    :    2026-09-07
// **************************************************************************
inline bool IsPrivateIP(LPCTSTR lpszIP)
{
	if (lpszIP == NULL || ::_tcslen(lpszIP) == 0) {
		return true;
	}

	tstring strIP = lpszIP;

	// ループバック
	if (strIP == _T("127.0.0.1") || strIP == _T("::1")) {
		return true;
	}

	// IPv4 プライベート範囲の簡易判定
	if (strIP.find(_T("10.")) == 0 ||
		strIP.find(_T("192.168.")) == 0 ||
		strIP.find(_T("169.254.")) == 0) {
		return true;
	}

	// 172.16.0.0 ~ 172.31.255.255
	if (strIP.find(_T("172.")) == 0) {
		int nSecond = 0;
		if (::_stscanf(strIP.c_str(), _T("172.%d."), &nSecond) == 1) {
			if (nSecond >= 16 && nSecond <= 31) {
				return true;
			}
		}
	}

	// IPv6 リンクローカル / ユニークローカル
	if (strIP.find(_T("fe80:")) == 0 || strIP.find(_T("FE80:")) == 0 ||
		strIP.find(_T("fc00:")) == 0 || strIP.find(_T("FC00:")) == 0 ||
		strIP.find(_T("fd00:")) == 0 || strIP.find(_T("FD00:")) == 0) {
		return true;
	}

	return false;
}

