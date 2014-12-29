// InputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Project2.h"
#include "InputDlg.h"
#include "afxdialogex.h"


// CInputDlg dialog

IMPLEMENT_DYNAMIC(CInputDlg, CDialogEx)

CInputDlg::CInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputDlg::IDD, pParent)
{
	m_inputstr="";
	m_NOFrames=NULL;
}

CInputDlg::~CInputDlg()
{
}

void CInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CInputDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CInputDlg message handlers




void CInputDlg::OnBnClickedOk()
{
	CEdit* GetDlg;
	CString temp;

	GetDlg=(CEdit*)GetDlgItem(IDC_INPUTSTR);
	GetDlg->GetWindowTextW(m_inputstr);

	GetDlg=(CEdit*)GetDlgItem(IDC_NOFRAMES);
    

	
	GetDlg->GetWindowTextW(temp);
	
	m_NOFrames=StrToIntW(temp);
	CDialogEx::OnOK();
}
