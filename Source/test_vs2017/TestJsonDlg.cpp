
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
#include "../include/LJson.h"
#include "SimpleJson.h"



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
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_EditOutput);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_EditText);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_JsonText);
	DDX_Text(pDX, IDC_EDIT_OUTPUT, m_JsonOutput);
}

BEGIN_MESSAGE_MAP(CTestJsonDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, &OnBnClickedButtonConvert)
	ON_BN_CLICKED(IDC_BUTTON1, &OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &OnBnClickedButton2)
	ON_WM_GETMINMAXINFO()
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
#ifdef _DEBUG
	m_JsonText = m_text;
	UpdateData(FALSE);
#else
	GetDlgItem(IDC_BUTTON1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_BUTTON2)->ShowWindow(SW_HIDE);
#endif // _DEBUG

	GetWindowRect(&m_rcDialog);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
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
		TRACE("Key:%x", pMsg->wParam);
		if( ::GetKeyState(VK_CONTROL) & 0x8000 )
		{
			if( (pMsg->wParam == 0x41) || (pMsg->wParam == 0x61) )
			{
				CWnd* pWnd = CWnd::FromHandle(pMsg->hwnd);
				if( pWnd )
				{
					CString strName = (LPCTSTR)pWnd->GetRuntimeClass()->m_lpszClassName;
					CString strEdit = (LPCTSTR)RUNTIME_CLASS(CEdit)->m_lpszClassName;
					if( strName == strEdit ) {
						CEdit* pEdit = (CEdit*)pWnd;
						pEdit->SetSel(0, -1);
					}
				}
			}
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTestJsonDlg::OnBnClickedButtonConvert()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	CString stext = m_JsonText.GetBuffer();
	m_JsonOutput = "";

	LJsonStruct jsons;
	LJsonStruct* pJSON = ljson_initialize(&jsons);
	if( pJSON == NULL )
	{
		m_JsonOutput = CStringA("초기화 오류가 발생했습니다.");
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

		m_JsonOutput.Format(_T("[%d] 오류가 발생했습니다. pos(%d:%*.*s)"), rtn, jsons.rpos, 20, 20, tmp_text);
		UpdateData(FALSE);
		return;
	}

	LJsonObject* pROOT = ljson_get_root(pJSON);
	if( pROOT == NULL )
	{
		m_JsonOutput = "Root가 없습니다.";
		UpdateData(FALSE);
		return;
	}

	m_JsonOutput = (LPCTSTR)ljson_write_tostring(pROOT);

	// PrintObject(pJSON, pROOT, 0);
	ljson_destroy(pJSON);

	UpdateData(FALSE);
}

void CTestJsonDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	CString stext = m_JsonText.GetBuffer();
	m_JsonOutput = "";

	SimpleJson json;
	if( json.fromString(stext.GetBuffer()) < 0 )
	{
		m_JsonOutput = json.getError();
	}
	else
	{
		m_JsonOutput = json.toString();
	}

	LJItem root = json.GetRoot();
	LJItem item_1 = root.GetObject((CString)"array_1");
	CString val = item_1.GetString(1);
	TRACE(L"VAL:%s\n", val);
	UpdateData(FALSE);
}

void CTestJsonDlg::OnBnClickedButton2()
{

	LJsonStruct jsons;
	LJsonStruct* pStruct = ljson_initialize(&jsons);
	if( pStruct == NULL )
	{
		TRACE(L"초기화 오류가 발생했습니다.");
		return;
	}

	pStruct->config.is_ownercheck = 1;

	LJsonObject* ptr_root = ljson_get_root(pStruct);
	if( ptr_root )
	{
		LJsonObject* ptr_obj1 = ljson_make_object(pStruct, NULL, LJSON_TYPE_OBJECT);
		if( ptr_obj1 )
		{
			// ljson_set_object(ptr_obj1, "prop", ptr_test);
			ljson_set_string (ptr_obj1, (LJChar*)L"name", (LJChar*)L"test");
			ljson_set_integer(ptr_obj1, (LJChar*)L"int", 1);
			ljson_set_double (ptr_obj1, (LJChar*)L"dbl", 2.3);

			ljson_set_object(ptr_root, (LJChar*)L"ptr_obj1", ptr_obj1);
		}

		LJsonObject* ptr_array = ljson_make_object(pStruct, NULL, LJSON_TYPE_ARRAY);
		if( ptr_array )
		{
			ljson_add_array_object(ptr_array, ptr_obj1);
			ljson_add_array_string(ptr_array, (LJChar*)L"array_2");
			ljson_add_array_integer(ptr_array, 12);
			ljson_add_array_double(ptr_array, 34.56);

			ljson_set_object(ptr_root, (LJChar*)CHAR_SET("array"), ptr_array);
		}
	}

	TRACE(L"Output: \n%s \n", ljson_write_tostring(ptr_root));

	ptr_root = ljson_get_root(pStruct);
	if( ptr_root )
	{
		LJsonObject* ptr_obj1 = ljson_get_object(ptr_root, (LJChar*)CHAR_SET("ptr_obj1"));
		if( ptr_obj1 )
		{
			LJChar* v1 = ljson_get_string(ptr_obj1, (LJChar*)CHAR_SET("name"));
			int     v2 = ljson_get_integer(ptr_obj1, (LJChar*)CHAR_SET("int"));
			TRACE(L"[%s] %s = %d \n", L"name", v1, v2);
		}

		LJsonObject* ptr_array = ljson_get_object(ptr_root, (LJChar*)CHAR_SET("array"));
		if( ptr_array )
		{
			LJsonObject*  v1 = ljson_get_array_object(ptr_array, 0);
			LJChar*       v2 = ljson_get_array_string(ptr_array, 1);
			LJChar*		  v3 = ljson_get_array_string(ptr_array, 2);

			TRACE(CHAR_SET("%s = %s, %s, %s\n"), CHAR_SET("array"),
				   ljson_get_string(v1, (LJChar*)CHAR_SET("dbl")),
				   v2, v3
			);
		}
	}

	{
		int pos;
		int cnt = pStruct->mem_str.cnt;
		LJChar* ptr = (LJChar*)pStruct->mem_str.val;
		for( pos = 0; pos < cnt; pos++ )
		{
			TRACE(L"%3d STR: '%s'\n",
				  pos, ptr
			);
			ptr += wcslen((wchar_t*)ptr) + 1;
		}
	}
	ljson_destroy(pStruct);
}

CString CTestJsonDlg::m_text =
L"{ \
	\"array_1\" : [ \
		\"value 1\", \
		\"value 2\", \
		\"value 3\", \
		{ \
			\"obj_1\" : { \
				\"txt 1\" : 1, \
				\"txt 2\" : 2.3, \
			} \
		} \
	], \
\"web-app\":{ \
\"servlet\": [ \
		{ \
	\"servlet-name\": \"cofaxCDS\", \
		\"servlet-class\" : \"org.cofax.cds.CDSServlet\", \
		\"init-param\" : { \
		\"configGlossary:installationAt\": \"Philadelphia, PA\", \
		\"configGlossary:adminEmail\" : \"ksm@pobox.com\", \
		\"configGlossary:poweredBy\" : \"Cofax\", \
		\"configGlossary:poweredByIconv\" : \"/images/cofax.gifv\", \
					\"configGlossary:staticPath\" : \"/content/static\", \
					\"templateProcessorClass\" : \"org.cofax.WysiwygTemplate\", \
					\"templateLoaderClass\" : \"org.cofax.FilesTemplateLoader\", \
					\"templatePath\" : \"templates\", \
					\"templateOverridePath\" : \"\", \
					\"defaultListTemplate\" : \"listTemplate.htm\", \
					\"defaultFileTemplate\" : \"articleTemplate.htm\", \
					\"useJSP\" : false, \
					\"jspListTemplate\" : \"listTemplate.jsp\", \
					\"jspFileTemplate\" : \"articleTemplate.jsp\", \
					\"cachePackageTagsTrack\" : 200, \
					\"cachePackageTagsStore\" : 200, \
					\"cachePackageTagsRefresh\" : 60, \
					\"cacheTemplatesTrack\" : 100, \
					\"cacheTemplatesStore\" : 50, \
					\"cacheTemplatesRefresh\" : 15, \
					\"cachePagesTrack\" : 200, \
					\"cachePagesStore\" : 100, \
					\"cachePagesRefresh\" : 10, \
					\"cachePagesDirtyRead\" : 10, \
					\"searchEngineListTemplate\" : \"forSearchEnginesList.htm\", \
					\"searchEngineFileTemplate\" : \"forSearchEngines.htm\", \
					\"searchEngineRobotsDb\" : \"WEB-INF/robots.db\", \
					\"useDataStore\" : true, \
					\"dataStoreClass\" : \"org.cofax.SqlDataStore\", \
					\"redirectionClass\" : \"org.cofax.SqlRedirection\", \
					\"dataStoreName\" : \"cofax\", \
					\"dataStoreDriver\" : \"com.microsoft.jdbc.sqlserver.SQLServerDriver\", \
					\"dataStoreUrl\" : \"jdbc:microsoft:sqlserver://LOCALHOST:1433;DatabaseName=goon\", \
					\"dataStoreUser\" : \"sa\", \
					\"dataStorePassword\" : \"dataStoreTestQuery\", \
					\"dataStoreTestQuery\" : \"SET NOCOUNT ON;select test='test';\", \
					\"dataStoreLogFile\" : \"/usr/local/tomcat/logs/datastore.log\", \
					\"dataStoreInitConns\" : 10, \
					\"dataStoreMaxConns\" : 100, \
					\"dataStoreConnUsageLimit\" : 100, \
					\"dataStoreLogLevel\" : \"debug\", \
					\"maxUrlLength\" : 500 \
			} \
		}, \
			{ \
				\"servlet-name\": \"cofaxEmail\", \
				\"servlet-class\" : \"org.cofax.cds.EmailServlet\", \
				\"init-param\" : { \
					\"mailHost\": \"mail1\", \
					\"mailHostOverride\" : \"mail2\" \
				} \
			}, \
			{ \
				\"servlet-name\": \"cofaxAdmin\", \
				\"servlet-class\" : \"org.cofax.cds.AdminServlet\" \
			}, \
			{ \
				\"servlet-name\": \"fileServlet\", \
				\"servlet-class\" : \"org.cofax.cds.FileServlet\" \
			}, \
			{ \
				\"servlet-name\": \"cofaxTools\", \
				\"servlet-class\" : \"org.cofax.cms.CofaxToolsServlet\", \
				\"init-param\" : { \
					\"templatePath\": \"toolstemplates/\", \
					\"log\" : 1, \
					\"logLocation\" : \"/usr/local/tomcat/logs/CofaxTools.log\", \
					\"logMaxSize\" : \"\", \
					\"dataLog\" : 1, \
					\"dataLogLocation\" : \"/usr/local/tomcat/logs/dataLog.log\", \
					\"dataLogMaxSize\" : \"\", \
					\"removePageCache\" : \"/content/admin/remove?cache=pages&id=\", \
					\"removeTemplateCache\" : \"/content/admin/remove?cache=templates&id=\", \
					\"fileTransferFolder\" : \"/usr/local/tomcat/webapps/content/fileTransferFolder\", \
					\"lookInContext\" : 1, \
					\"adminGroupID\" : 4, \
					\"betaServer\" : true \
				}  \
			} \
		], \
			\"servlet-mapping\": { \
				\"cofaxCDS\": \"/\", \
					\"cofaxEmail\" : \"/cofaxutil/aemail/*\", \
					\"cofaxAdmin\" : \"/admin/*\", \
					\"fileServlet\" : \"/static/*\", \
					\"cofaxTools\" : \"/tools/*\" \
			}, \
				\"taglib\": { \
					\"taglib-uri\": \"cofax.tld\", \
						\"taglib-location\" : \"/WEB-INF/tlds/cofax.tld\" \
				} \
	} \
} \
";
