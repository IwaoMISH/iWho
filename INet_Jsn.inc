// **************************************************************************
//  @file       INET_JSN.INC
//  @brief      ipwho.is レスポンス解析用 JSON 抽出モジュール (VC6/C++98 互換)
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
#include <cstdlib>
#include "i_types.inc"
#include "INet_Geo.INC"

////
// **************************************************************************
//  JSON 文字列から指定キーの文字列値を抽出する内部補助関数
//  作成日    :    2026-09-07
// **************************************************************************
inline bool JSON_ExtractValue(const tstring& strJson, LPCTSTR lpszKey, tstring& strVal)
{
	strVal = _T("");

	tstring strTargetKey = _T("\"");
	strTargetKey += lpszKey;
	strTargetKey += _T("\"");

	tstring::size_type nPos = strJson.find(strTargetKey);
	if (nPos == tstring::npos) {
		return false;
	}

	nPos = strJson.find(_T(":"), nPos + strTargetKey.length());
	if (nPos == tstring::npos) {
		return false;
	}

	// 値の開始位置までスキップ
	nPos = strJson.find_first_not_of(_T(" \t\r\n"), nPos + 1);
	if (nPos == tstring::npos) {
		return false;
	}

	// 文字列値 ("...") の場合
	if (strJson[nPos] == _T('"')) {
		tstring::size_type nEnd = strJson.find(_T('"'), nPos + 1);
		if (nEnd != tstring::npos) {
			strVal = strJson.substr(nPos + 1, nEnd - (nPos + 1));
			return true;
		}
	}
	// 数値・真偽値 (123, true, false 等) の場合
	else {
		tstring::size_type nEnd = strJson.find_first_of(_T(",}\r\n"), nPos);
		if (nEnd != tstring::npos) {
			strVal = strJson.substr(nPos, nEnd - nPos);
			return true;
		}
	}

	return false;
}

////
// **************************************************************************
//  ipwho.is の JSON レスポンスを GEOIP_INFO 構造体へ展開
//  作成日    :    2026-09-07
// **************************************************************************
inline bool JSON_ParseGeoIP(LPCTSTR lpszJson, GEOIP_INFO& geoInfo)
{
	geoInfo.Clear();

	if (lpszJson == NULL || ::_tcslen(lpszJson) == 0) {
		return false;
	}

	tstring strJson = lpszJson;
	tstring strValue;

	// 成否判定
	if (::JSON_ExtractValue(strJson, _T("success"), strValue)) {
		if (strValue == _T("true")) {
			geoInfo.bSuccess = true;
		}
	}

	if (!geoInfo.bSuccess) {
		::JSON_ExtractValue(strJson, _T("message"), geoInfo.strMessage);
		return false;
	}

	// 各フィールドの抽出
	::JSON_ExtractValue(strJson, _T("ip"),      geoInfo.strIP);
	::JSON_ExtractValue(strJson, _T("country"), geoInfo.strCountry);
	::JSON_ExtractValue(strJson, _T("region"),  geoInfo.strRegion);
	::JSON_ExtractValue(strJson, _T("city"),    geoInfo.strCity);
	::JSON_ExtractValue(strJson, _T("postal"),  geoInfo.strPostal);
	::JSON_ExtractValue(strJson, _T("isp"),     geoInfo.strISP);
	::JSON_ExtractValue(strJson, _T("asn"),     geoInfo.strASN);

	// timezone オブジェクトのブロックを探し、その内部の "id" を抽出
	tstring::size_type nTzPos = strJson.find(_T("\"timezone\""));
	if (nTzPos != tstring::npos) {
		tstring strTzSub = strJson.substr(nTzPos);
		::JSON_ExtractValue(strTzSub, _T("id"), geoInfo.strTimezone);
	}

	// 緯度・経度の抽出と変換
	if (::JSON_ExtractValue(strJson, _T("latitude"), strValue)) {
		geoInfo.dLatitude = ::_tcstod(strValue.c_str(), NULL);
	}
	if (::JSON_ExtractValue(strJson, _T("longitude"), strValue)) {
		geoInfo.dLongitude = ::_tcstod(strValue.c_str(), NULL);
	}

	return true;
}

