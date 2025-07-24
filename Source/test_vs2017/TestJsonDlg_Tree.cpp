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


enum {
	HDRCOL_NAME = 0,
	HDRCOL_VALUE = 1,
	HDRCOL_TYPE = 2,
};

BOOL CTestJsonDlg::Tree_Initialize()
{
	CRect rcList(0, 0, 0, 0);
	GetDlgItem(IDC_EDIT_OUTPUT)->GetWindowRect(&rcList);
	ScreenToClient(&rcList);

	// m_ImageList.Create(16, 16, ILC_MASK | ILC_COLOR32, 0, 1);

	m_pTreeOutput = new CTSCtrl;
	if( m_pTreeOutput->Create(WS_CHILD | /*WS_VISIBLE |*/ WS_TABSTOP | WS_BORDER, rcList, this, IDC_TREE_OUTPUT) == FALSE ) {
		TRACE("ListConn create failed...\n");
		return FALSE;
	}

	m_pTreeOutput->SetHeader_Sort(FALSE);
	m_pTreeOutput->SetVisibleTLine(TRUE);
	m_pTreeOutput->SetReadOnly(TRUE);
	// m_pTreeOutput->SetImageList(&m_ImageList);

	TSHeaderItem tsHeader[6] = {
		{_T("Name")		, 150	, CMNTYPE_STATIC, CMNFLAG_NOCTRL_HDR | CMNFLAG_VALSTRING, 0, 0, 0, 0},
		{_T("Value")	, 250	, CMNTYPE_STATIC, CMNFLAG_NOCTRL_HDR | CMNFLAG_VALSTRING, 0, 0, 0, 0},
		{_T("Type")		, 100	, CMNTYPE_STATIC, CMNFLAG_NOCTRL_HDR | CMNFLAG_VALSTRING, 0, 0, 0, 0},
	};
	// pListCtrl->SetExtendedStyle(pListCtrl->GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_pTreeOutput->InsertColumn(&tsHeader[HDRCOL_NAME]);
	m_pTreeOutput->InsertColumn(&tsHeader[HDRCOL_VALUE]);
	m_pTreeOutput->InsertColumn(&tsHeader[HDRCOL_TYPE]);

	CMFCDynamicLayout* dynamicLayout = this->GetDynamicLayout();
	if( dynamicLayout )
	{
		CMFCDynamicLayout::MoveSettings moveSettings = CMFCDynamicLayout::MoveNone();
		CMFCDynamicLayout::SizeSettings sizeSettings = CMFCDynamicLayout::SizeHorizontalAndVertical(100,100);

		dynamicLayout->AddItem(m_pTreeOutput->GetSafeHwnd(),
			moveSettings,
			sizeSettings);
	}

	return TRUE;
}

void CTestJsonDlg::Tree_Destroy()
{
	if( m_pTreeOutput )
	{
		delete m_pTreeOutput;
		m_pTreeOutput = NULL;
	}
}

int CTestJsonDlg::RecursiveTree(LJsonObject* ptr_parent, CTSCtrlItem* pParent)
{
	if( ptr_parent )
	{
		int opos = 0;
		void* ptr_next = ljson_get_object_head(ptr_parent);
		while( ptr_next )
		{
			LJsonObject* ptr_object;
			ptr_object = ljson_get_object_next(ptr_parent, (LJLinkItem**)&ptr_next);

			CString s_name = (LPCTSTR)ljson_get_name(ptr_object);

			CTSCtrlItem* pItem = m_pTreeOutput->InsertItem(pParent, -1, s_name);
			switch( ljson_get_type(ptr_parent) )
			{
			case LJSON_TYPE_ARRAY:
				s_name.Format(_T("[%d]"), opos + 1);
				pItem->SetText(HDRCOL_NAME, s_name);
				break;
			default:
				pItem->SetText(HDRCOL_NAME, s_name);
				break;
			}

			CString s_value;
			switch( ljson_get_type(ptr_object) )
			{
			case LJSON_TYPE_NULL:
				pItem->SetText(HDRCOL_VALUE, (LPCTSTR)_T("(NULL)"));
				pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("Null"));
				break;
			case LJSON_TYPE_STRING:
				s_value.Format(_T("'%s'"), (LPCTSTR)ljson_get_value(ptr_object));
				pItem->SetText(HDRCOL_VALUE, s_value);
				pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("String"));
				break;
			case LJSON_TYPE_SCRIPT:
				s_value.Format(_T("'%s'"), (LPCTSTR)ljson_get_value(ptr_object));
				pItem->SetText(HDRCOL_VALUE, s_value);
				pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("Script"));
				break;
			case LJSON_TYPE_INTEGER:
				s_value.Format(_T("%s"), (LPCTSTR)ljson_get_value(ptr_object));
				pItem->SetText(HDRCOL_VALUE, s_value);
				pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("Integer"));
				break;
			case LJSON_TYPE_FLOAT:
				s_value.Format(_T("%s"), (LPCTSTR)ljson_get_value(ptr_object));
				pItem->SetText(HDRCOL_VALUE, s_value);
				pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("Float"));
				break;
			case LJSON_TYPE_OBJECT:
				// pItem->SetText(HDRCOL_VALUE, (LPCTSTR)_T("(OBJECT)"));
				pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("Object"));
				RecursiveTree(ptr_object, pItem);
				break;
			case LJSON_TYPE_ARRAY:
				// pItem->SetText(HDRCOL_VALUE, (LPCTSTR)_T("(ARRAY)"));
				pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("Array"));
				RecursiveTree(ptr_object, pItem);
				break;
			}
			opos++;
		}
	}
	return 0;
}

void CTestJsonDlg::OutputToTree(LJsonObject* pROOT)
{
	BOOL bShow = m_pTreeOutput->IsWindowVisible();
	if( bShow )
	{
		m_pTreeOutput->SetRedraw(FALSE);
	}

	m_pTreeOutput->DeleteAllItems();

	CTSCtrlItem* pItem = m_pTreeOutput->InsertItem(NULL, -1, _T("Root"));
	CString s_value;
	switch( ljson_get_type(pROOT) )
	{
	case LJSON_TYPE_OBJECT:
		// pItem->SetText(HDRCOL_VALUE, (LPCTSTR)_T("(OBJECT)"));
		pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("Object"));
		RecursiveTree(pROOT, pItem);
		break;
	case LJSON_TYPE_ARRAY:
		// pItem->SetText(HDRCOL_VALUE, (LPCTSTR)_T("(ARRAY)"));
		pItem->SetText(HDRCOL_TYPE, (LPCTSTR)_T("Array"));
		RecursiveTree(pROOT, pItem);
		break;
	}

	if( bShow )
	{
		m_pTreeOutput->SetRedraw(TRUE);
		m_pTreeOutput->Invalidate(FALSE);
	}
	else
	{
		m_pTreeOutput->ShowWindow(SW_HIDE);
	}
}
