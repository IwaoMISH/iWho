// **************************************************************************
//  @file       IWHODLG.H
//  @brief      CIWhoDlg ダイアログクラス ヘッダーファイル
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

#if !defined(AFX_IWHODLG_H__88367FAD_3E24_41D2_B4BA_CB594B536509__INCLUDED_)
#define AFX_IWHODLG_H__88367FAD_3E24_41D2_B4BA_CB594B536509__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "INet_DNS.inc"

/////////////////////////////////////////////////////////////////////////////
// CIWhoDlg ダイアログ

class CIWhoDlg : public CDialog
{
// 構築
public:
    CIWhoDlg(CWnd* pParent = NULL);    // 標準のコンストラクタ

// ダイアログ データ
    //{{AFX_DATA(CIWhoDlg)
    enum { IDD = IDD_IWHO_DIALOG };
    CEdit   m_editTarget;
    CEdit   m_editResult;
    CEdit   m_editInput;
    //}}AFX_DATA

    CStatusBarCtrl  m_StatusBar;

    // ClassWizard は仮想関数のオーバーライドを生成します。
    //{{AFX_VIRTUAL(CIWhoDlg)
    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
    //}}AFX_VIRTUAL

// インプリメンテーション
protected:
    HICON    m_hIcon;
    CWSAinit m_wsa;

    // 生成されたメッセージ マップ関数
    //{{AFX_MSG(CIWhoDlg)
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    afx_msg void OnSize(UINT nType, int cx, int cy);
    virtual void OnOK();
    afx_msg void OnChangeEditInput();
    //}}AFX_MSG
    DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_IWHODLG_H__88367FAD_3E24_41D2_B4BA_CB594B536509__INCLUDED_)

