
// TestJsonDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "TestJson.h"
#include "TestJsonDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _UNICODE
#define CHAR_SET(str) _T(str)
#else
#define CHAR_SET(str) str
#endif 


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestJsonDlg 대화 상자



CTestJsonDlg::CTestJsonDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTJSON_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestJsonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_JsonText);
	DDX_Text(pDX, IDC_EDIT_ERROR, m_EditError);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_EditOutput);
}

BEGIN_MESSAGE_MAP(CTestJsonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &OnBnClickedButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON_CTEST, &OnBnClickedButtonCTest)
	ON_BN_CLICKED(IDC_BUTTON_CPPTEST, &OnBnClickedButtonCPPTest)
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_CHECK_VTEXT, &CTestJsonDlg::OnBnClickedCheckVtext)
	ON_BN_CLICKED(IDC_CHECK_VTREE, &CTestJsonDlg::OnBnClickedCheckVtree)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CTestJsonDlg 메시지 처리기

BOOL CTestJsonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	((CEdit*)GetDlgItem(IDC_EDIT_TEXT))->SetLimitText(0);

#ifdef _DEBUG
	m_JsonText = m_text;
	UpdateData(FALSE);
#else
	GetDlgItem(IDC_BUTTON_CTEST)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON_CPPTEST)->ShowWindow(SW_HIDE);
#endif // _DEBUG
	if( Tree_Initialize() == FALSE )
	{
		TRACE("Tree_Initialize failed...\n");
		return FALSE;
	}

	// ((CButton*)GetDlgItem(IDC_CHECK_VTEXT))->SetCheck(TRUE);
	OnBnClickedCheckVtext();

	GetWindowRect(&m_rcDialog);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CTestJsonDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	Tree_Destroy();
}

void CTestJsonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CTestJsonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CTestJsonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestJsonDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	lpMMI->ptMinTrackSize = CPoint(m_rcDialog.Width(), m_rcDialog.Height());

	CDialogEx::OnGetMinMaxInfo(lpMMI);
}

BOOL CTestJsonDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if( pMsg->message == WM_KEYDOWN )
	{
		// TRACE("Key:%x", pMsg->wParam);
		if( ::GetKeyState(VK_CONTROL) & 0x8000 )
		{
			switch( pMsg->wParam )
			{
			case 0x41:
			case 0x61:
				{
					CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
					if( pWnd )
					{
						int nID = pWnd->GetDlgCtrlID();
						switch( pWnd->GetDlgCtrlID() )
						{
						case IDC_EDIT_TEXT:
						case IDC_EDIT_OUTPUT:
							CEdit* pEdit = (CEdit*)pWnd;
							pEdit->SetSel(0, -1);
							break;
						}
					}
				}
				break;
			}
		}
		else
		{
			switch( pMsg->wParam )
			{
			case VK_F5:
				OnBnClickedButtonConvert();
				break;
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTestJsonDlg::OnBnClickedCheckVtext()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_CHECK_VTEXT))->SetCheck(TRUE);
	((CButton*)GetDlgItem(IDC_CHECK_VTREE))->SetCheck(FALSE);

	GetDlgItem(IDC_EDIT_OUTPUT)->ShowWindow(SW_SHOW);
	m_pTreeOutput->ShowWindow(SW_HIDE);
}


void CTestJsonDlg::OnBnClickedCheckVtree()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	((CButton*)GetDlgItem(IDC_CHECK_VTEXT))->SetCheck(FALSE);
	((CButton*)GetDlgItem(IDC_CHECK_VTREE))->SetCheck(TRUE);

	GetDlgItem(IDC_EDIT_OUTPUT)->ShowWindow(SW_HIDE);
	m_pTreeOutput->ShowWindow(SW_SHOW);
}

void CTestJsonDlg::OnBnClickedButtonConvert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString stext = m_JsonText.GetBuffer();
	m_EditOutput = "";
	m_EditError = "";

	LJsonStruct jsons;
	LJsonStruct* pJSON = ljson_initialize(&jsons);
	if( pJSON == NULL )
	{
		m_EditError = CStringA("초기화 오류가 발생했습니다.");
		UpdateData(FALSE);
		return;
	}

	pJSON->config.is_indent = TRUE;
	pJSON->config.is_newline = TRUE;
	pJSON->config.is_tabindent = FALSE;
	pJSON->config.cnt_space = 4;

	int rtn = ljson_read_fromstring(&jsons, (LJChar*)(LPCTSTR)stext);
	if( rtn <= 0 )
	{
		int npos = jsons.rpos;
		if( npos < 10 )
			npos = 10;

		CString tmp_text = stext.Mid(npos - 10);

		m_EditError.Format(_T("[%d] 오류가 발생했습니다. pos(%d:%*.*s)"), rtn, jsons.rpos, 20, 20, tmp_text);
		UpdateData(FALSE);
		return;
	}

	TRACE("rest:'%s'\n", stext.Mid(rtn));

	LJsonObject* pROOT = ljson_get_root(pJSON);
	if( pROOT == NULL )
	{
		m_EditError = "Root가 없습니다.";
		UpdateData(FALSE);
		return;
	}

	// to Edit 
	OutputToEdit(pROOT);

	// to Tree
	OutputToTree(pROOT);

	// PrintObject(pJSON, pROOT, 0);
	ljson_destroy(pJSON);

	m_EditError.Format(_T("일단은 처리되었습니다.(len:%d)"), rtn);
	UpdateData(FALSE);
	Invalidate(FALSE);
}

void CTestJsonDlg::OutputToEdit(LJsonObject* pROOT)
{
	m_EditOutput = (LPCTSTR)ljson_write_tostring(pROOT);
}

