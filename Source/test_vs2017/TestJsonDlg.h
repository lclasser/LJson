
// TestJsonDlg.h: 헤더 파일
//

#pragma once
#include "../include/LJson.h"
#include "TSCtrl/TSCtrl.h"

// CTestJsonDlg 대화 상자
class CTestJsonDlg : public CDialogEx
{
// 생성입니다.
public:
	CTestJsonDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTJSON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON	m_hIcon;
	CRect	m_rcDialog;

	static CString m_text;

	CString m_JsonText;
	CString m_EditError;
	CString m_EditOutput;
	CTSCtrl* m_pTreeOutput;

	void OutputToEdit(LJsonObject* pROOT);
	void OutputToTree(LJsonObject* pROOT);

	BOOL Tree_Initialize();
	void Tree_Destroy();
	int RecursiveTree(LJsonObject* ptr_object, CTSCtrlItem* pItem);

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBnClickedButtonConvert();
	afx_msg void OnBnClickedButtonCTest();
	afx_msg void OnBnClickedButtonCPPTest();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedCheckVtext();
	afx_msg void OnBnClickedCheckVtree();
	afx_msg void OnDestroy();
};
