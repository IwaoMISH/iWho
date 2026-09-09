// **************************************************************************
//  @file       IWHODLG.CPP
//  @brief      CIWhoDlg ダイアログクラス インプリメンテーションファイル
//
//  @author     Iwao ( https://mish.work/ )
//  @date       2026-09-09
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

#include "stdafx.h"
#pragma warning(disable: 4786)
#include "iWho.h"
#include "iWhoDlg.h"

#include "INet_Trg.inc"
#include "INet_Jsn.inc"
#include "INet_MFC.inc"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// アプリケーションのバージョン情報で使われている CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ダイアログ データ
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard は仮想関数のオーバーライドを生成します
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV のサポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// メッセージ ハンドラがありません。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIWhoDlg ダイアログ

CIWhoDlg::CIWhoDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIWhoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIWhoDlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CIWhoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIWhoDlg)
	DDX_Control(pDX, IDC_EDIT_TARGET, m_editTarget);
	DDX_Control(pDX, IDC_EDIT_RESULT, m_editResult);
	DDX_Control(pDX, IDC_EDIT_INPUT,  m_editInput);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIWhoDlg, CDialog)
	//{{AFX_MSG_MAP(CIWhoDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_INPUT, OnChangeEditInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIWhoDlg メッセージ ハンドラ

BOOL CIWhoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// "バージョン情報..." メニュー項目をシステム メニューへ追加します。

	// IDM_ABOUTBOX はコマンド メニューの範囲でなければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
    if (m_StatusBar.Create(WS_CHILD | WS_VISIBLE /*| SBT_NOBORDERS*/,
            CRect(0,0,0,0), this, AFX_IDW_STATUS_BAR)) {
        int parts[] = { -1 };
        m_StatusBar.SetParts(1, parts);
        if (m_wsa.IsOK()) {
            m_StatusBar.SetText(_T(" 準備完了 "), 0, 0);
        }
        else {
            m_StatusBar.SetText(_T(" エラー: Winsock の初期化に失敗しました "), 0, 0);
        }
    }

	return TRUE;  // TRUE を返すとコントロールに設定したフォーカスは失われません。
}

void CIWhoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CIWhoDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CIWhoDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIWhoDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
    if (::IsWindow(m_StatusBar.m_hWnd))    {
        m_StatusBar.SendMessage(WM_SIZE, 0, 0);
        }
    }	

void CIWhoDlg::OnChangeEditInput() 
{
    // 入力テキストの取得
    tstring strInput;
    int nLen = ::GetWindowTextLength(m_editInput.m_hWnd);
    if (nLen > 0) {
		strInput.resize(nLen + 1) ;
        LPTSTR pBuf = &strInput[0] ;
        ::GetWindowText(m_editInput.m_hWnd, pBuf, nLen + 1);
        strInput = tstring(pBuf) ;
    }

    // 抽出ターゲットおよび解析結果の初期化・設定
    if (strInput.empty()) {
        ::SetWindowText(m_editTarget.m_hWnd, _T(""));
        ::SetWindowText(m_editResult.m_hWnd, _T(""));
        if (::IsWindow(m_StatusBar.m_hWnd)) {
            m_StatusBar.SetText(_T(" 準備完了 "), 0, 0);
        }
    }
    else {
        // TODO: 抽出ターゲットの解析ロジックをここに実装
		tstring strTarget = ::ExtractTarget(strInput);
        ::SetWindowText(m_editTarget.m_hWnd, strTarget.c_str());

        if (::IsWindow(m_StatusBar.m_hWnd)) {
            m_StatusBar.SetText(_T(" 入力テキストを受信しました "), 0, 0);
            if (!strTarget.empty()) {
                tstring strStatus = _T(" 抽出ターゲット: ") + strTarget;
                m_StatusBar.SetText(strStatus.c_str(), 0, 0);
            }
            else {
                m_StatusBar.SetText(_T(" ターゲットを検出できませんでした "), 0, 0);
            }
        }
    }
}

void CIWhoDlg::OnOK() 
{
	CWaitCursor	wait ;

    if (!m_wsa.IsOK()) {
        ::AfxMessageBox(_T("Winsock が初期化されていません"), MB_OK | MB_ICONERROR);
        return;
    }

    bool	bShouldMoveFocus = false ;
	{
		HWND	hWndFocus = ::GetFocus();
	    // OK ボタンのウィンドウハンドルを取得
    	CWnd* pBtnOK = GetDlgItem(IDOK);
    	HWND hWndOK = pBtnOK ? pBtnOK->m_hWnd : NULL;

	    // m_editInput または OK ボタンにフォーカスがあるか判定
    	bShouldMoveFocus = (hWndFocus == m_editInput.m_hWnd || hWndFocus == hWndOK);
	}

	DWORD	ei_sel = m_editInput.GetSel() ;

    // 1. 入力テキストの取得
    tstring strInput;
    int nLen = ::GetWindowTextLength(m_editInput.m_hWnd);
    if (nLen > 0) {
        strInput.resize(nLen + 1);
        LPTSTR pBuf = &strInput[0];
        ::GetWindowText(m_editInput.m_hWnd, pBuf, nLen + 1);
        strInput = tstring(pBuf) ;
    }

    if (strInput.empty()) {
        ::AfxMessageBox(_T("入力テキストを入力してください"), MB_OK | MB_ICONEXCLAMATION);
	    if (bShouldMoveFocus) {
	        m_editInput.SetFocus();
	    }
        return;
    }

    // 2. 抽出ターゲットの取得
    tstring strTarget = ::ExtractTarget(strInput);
    if (strTarget.empty()) {
        ::SetWindowText(m_editResult.m_hWnd, _T("エラー: 対象となる IP アドレスやホスト名を抽出できませんでした"));
        if (::IsWindow(m_StatusBar.m_hWnd)) {
            m_StatusBar.SetText(_T(" エラー: ターゲット抽出失敗 "), 0, 0);
        }
	    if (bShouldMoveFocus) {
	        m_editInput.SetFocus();
	    }
        return;
    }

    ::SetWindowText(m_editTarget.m_hWnd, strTarget.c_str());

    if (::IsWindow(m_StatusBar.m_hWnd)) {
        m_StatusBar.SetText(_T(" 名前解決中... "), 0, 0);
    }

    tstring strResult;

    // 3. 名前解決 (正引き)
    tstring strTargetIP;
    if (!::DNS_ResolveToIP(strTarget.c_str(), strTargetIP)) {
        strResult = _T("エラー: 名前解決（正引き）に失敗しました");
        ::SetWindowText(m_editResult.m_hWnd, strResult.c_str());
        if (::IsWindow(m_StatusBar.m_hWnd)) {
            m_StatusBar.SetText(_T(" エラー: 正引き失敗 "), 0, 0);
        }
	    if (bShouldMoveFocus) {
	        m_editInput.SetFocus();
	    }
        return;
    }

    strResult +=     _T("[正引き] ターゲット IP :  ") + strTargetIP + _T("\r\n");

    // 4. 名前解決 (逆引き)
    tstring strHostName;
    if (::DNS_ResolveToHost(strTargetIP.c_str(), strHostName)) {
        strResult += _T("[逆引き] ホスト名      :  ") + strHostName + _T("\r\n");
    }
    else {
        strResult += _T("[逆引き] ホスト名         取得に失敗しました\r\n");
    }

    // 5. プライベート IP 判定
    if (::IsPrivateIP(strTargetIP.c_str())) {
        strResult += _T("[判定] プライベート / ループバック IP のため API 呼び出しをスキップします\r\n");
        ::SetWindowText(m_editResult.m_hWnd, strResult.c_str());
        if (::IsWindow(m_StatusBar.m_hWnd)) {
            m_StatusBar.SetText(_T(" 処理完了 (プライベート IP) "), 0, 0);
        }
        if (bShouldMoveFocus) {
            m_editInput.SetFocus();
        }
        return;
    }

    if (::IsWindow(m_StatusBar.m_hWnd)) {
        m_StatusBar.SetText(_T(" GeoIP 情報取得中... "), 0, 0);
    }

    // 6. GeoIP API 呼び出し
    tstring strApiUrl = _T("https://ipwho.is/") + strTargetIP;
    tstring strJson;

    if (!::IN_GetText(strApiUrl.c_str(), strJson)) {
        strResult += _T("\r\nエラー: GeoIP API へのリクエストに失敗しました");
        ::SetWindowText(m_editResult.m_hWnd, strResult.c_str());
        if (::IsWindow(m_StatusBar.m_hWnd)) {
            m_StatusBar.SetText(_T(" エラー: API 呼び出し失敗 "), 0, 0);
        }
        if (bShouldMoveFocus) {
            m_editInput.SetFocus();
        }
        return;
    }

    // 7. JSON 解析
    GEOIP_INFO geoInfo;
    if (::JSON_ParseGeoIP(strJson.c_str(), geoInfo)) {
	        strResult += _T("\r\n");
	        strResult += _T("\r\n--- GeoIP 解析結果 ---\r\n");
	        strResult += _T("IP アドレス  :  ")  + geoInfo.strIP       + _T("\r\n");
	        strResult += _T("国名         :  ")  + geoInfo.strCountry  + _T("\r\n");
	        strResult += _T("地域/県      :  ")  + geoInfo.strRegion   + _T("\r\n");
	        strResult += _T("都市         :  ")  + geoInfo.strCity     + _T("\r\n");
	        strResult += _T("郵便番号     :  ")  + geoInfo.strPostal   + _T("\r\n");
	        strResult += _T("タイムゾーン :  ")  + geoInfo.strTimezone + _T("\r\n");
	        strResult += _T("ISP          :  ")  + geoInfo.strISP      + _T("\r\n");
	        strResult += _T("ASN          :  ")  + geoInfo.strASN      + _T("\r\n");

        if (!strHostName.empty()) {
            strResult += _T("ホスト名     :  ")  + strHostName         + _T("\r\n");
        }

        if (::IsWindow(m_StatusBar.m_hWnd)) {
            m_StatusBar.SetText(_T(" 処理完了 "), 0, 0);
        }
    }
    else {
        strResult += _T("\r\nエラー: GeoIP データの解析に失敗しました (") + geoInfo.strMessage + _T(")\r\n");
        if (::IsWindow(m_StatusBar.m_hWnd)) {
            m_StatusBar.SetText(_T(" エラー: JSON 解析失敗 "), 0, 0);
        }
    }

    ::SetWindowText(m_editResult.m_hWnd, strResult.c_str());

	m_editInput.SetSel(ei_sel) ;
    // 呼び出し元が m_editInput だった場合のみフォーカスを再設定
    if (bShouldMoveFocus) {
        m_editInput.SetFocus();
    }

}

