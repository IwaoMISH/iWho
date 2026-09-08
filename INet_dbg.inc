// **************************************************************************
//  @file       INET_DBG.INC
//  @brief      GeoIP 解析結果デバッグ出力モジュール (VC6/C++98 互換)
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

#include <tchar.h>
#include <cstdio>
#include "INet_Geo.inc"

////
// **************************************************************************
//  GeoIP 情報のコンソール出力処理
//  作成日    :    2026-09-07
// **************************************************************************
inline void PrintGeoInfo(const GEOIP_INFO& info, LPCTSTR lpszHostName)
{
	::_tprintf(_T("\n==========================================\n"));
	::_tprintf(_T(" [GeoIP 解析結果]\n"));
	::_tprintf(_T("==========================================\n"));
	::_tprintf(_T(" IP アドレス : %s\n"), info.strIP.c_str());
	::_tprintf(_T(" ホスト名    : %s\n"), (lpszHostName && ::_tcslen(lpszHostName) > 0) ? lpszHostName : _T("(逆引き不可)"));
	::_tprintf(_T(" 国名        : %s\n"), info.strCountry.c_str());
	::_tprintf(_T(" 地域/県     : %s\n"), info.strRegion.c_str());
	::_tprintf(_T(" 市区町村    : %s\n"), info.strCity.c_str());
	::_tprintf(_T(" 郵便番号    : %s\n"), info.strPostal.c_str());
	::_tprintf(_T(" 緯度 / 経度 : %.4f / %.4f\n"), info.dLatitude, info.dLongitude);
	::_tprintf(_T(" タイムゾーン: %s\n"), info.strTimezone.c_str());
	::_tprintf(_T(" ISP         : %s\n"), info.strISP.c_str());
	::_tprintf(_T(" ASN         : %s\n"), info.strASN.c_str());
	::_tprintf(_T("==========================================\n\n"));
}

