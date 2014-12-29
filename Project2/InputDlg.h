#pragma once
#include "Frames.h"
#include "InputStr.h"

// CInputDlg dialog

class CInputDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDlg)

public:
	CInputDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CInputDlg();
	CString m_inputstr;
	int m_NOFrames;
	

// Dialog Data
	enum { IDD = IDD_INPUTDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};
