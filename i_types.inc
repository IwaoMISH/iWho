// **************************************************************************
//  @file       I_TYPES.INC
//  @brief      Minimum 型定義ヘッダー (tstring / v_tstring)
//
//  @author     Iwao (https://mish.work/)
//  @date       2026-06-05
//
//  @modify
//  2026-06-05  新規作成
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

#include    <string>
#include    <vector>
#include    <tchar.h>

// 汎用文字列・動的配列型定義
typedef     std::basic_string <TCHAR, std::char_traits<TCHAR>, std::allocator<TCHAR> >       tstring ;

typedef           std::vector<tstring>          v_tstring ;
typedef     const std::vector<tstring>        c_v_tstring ;


