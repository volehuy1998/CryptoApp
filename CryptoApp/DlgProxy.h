
// DlgProxy.h: header file
//

#pragma once

class CCryptoAppDlg;


// CCryptoAppDlgAutoProxy command target

class CCryptoAppDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CCryptoAppDlgAutoProxy)

	CCryptoAppDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CCryptoAppDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CCryptoAppDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CCryptoAppDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

