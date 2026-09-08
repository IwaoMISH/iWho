// iWho.h : IWHO アプリケーションのメイン ヘッダー ファイルです。
//

#if !defined(AFX_IWHO_H__F0ABFA8C_E46F_49E6_8F71_F19D8BC10144__INCLUDED_)
#define AFX_IWHO_H__F0ABFA8C_E46F_49E6_8F71_F19D8BC10144__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// メイン シンボル

/////////////////////////////////////////////////////////////////////////////
// CIWhoApp:
// このクラスの動作の定義に関しては iWho.cpp ファイルを参照してください。
//

class CIWhoApp : public CWinApp
{
public:
	CIWhoApp();

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CIWhoApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// インプリメンテーション

	//{{AFX_MSG(CIWhoApp)
		// メモ - ClassWizard はこの位置にメンバ関数を追加または削除します。
		//        この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_IWHO_H__F0ABFA8C_E46F_49E6_8F71_F19D8BC10144__INCLUDED_)
