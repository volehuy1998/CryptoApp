
// CryptoAppDlg.h : header file
//

#pragma once

#include <string>

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

protected:
	unsigned char* set_key(int edit_control_id);
	void enable_item(int item, BOOL enable);
	bool check_crypto_category(int item_id);
	void put_key_or_iv(int edit_control_id);
public:
	afx_msg void OnBnClickeddes();
	afx_msg void OnBnClickedaes128();
	afx_msg void OnBnClickedaes192();
	afx_msg void OnBnClickedaes256();
	afx_msg void OnBnClickedrc2();
	afx_msg void OnBnClickedecb();
	afx_msg void OnBnClickedncbc();
	afx_msg void OnBnClickedofb();
	afx_msg void OnBnClickedcfb();
	afx_msg void OnBnClickedede();
};
