
// CryptoAppDlg.h : header file
//

#pragma once

class CCryptoAppDlgAutoProxy;


// CCryptoAppDlg dialog
class CCryptoAppDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCryptoAppDlg);
	friend class CCryptoAppDlgAutoProxy;

// Construction
public:
	CCryptoAppDlg(CWnd* pParent = nullptr);	// standard constructor
	virtual ~CCryptoAppDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CryptoApp_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	CCryptoAppDlgAutoProxy* m_pAutoProxy;
	HICON m_hIcon;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPerform();
	afx_msg void OnBnClickedGenkey();
	afx_msg void OnEnChange();
	afx_msg void OnBnClickedGenIV();
};
