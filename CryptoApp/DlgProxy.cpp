
// DlgProxy.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CryptoApp.h"
#include "DlgProxy.h"
#include "CryptoAppDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CCryptoAppDlgAutoProxy

IMPLEMENT_DYNCREATE(CCryptoAppDlgAutoProxy, CCmdTarget)

CCryptoAppDlgAutoProxy::CCryptoAppDlgAutoProxy()
{
	EnableAutomation();

	// To keep the application running as long as an automation
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CCryptoAppDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CCryptoAppDlg)))
		{
			m_pDialog = reinterpret_cast<CCryptoAppDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CCryptoAppDlgAutoProxy::~CCryptoAppDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != nullptr)
		m_pDialog->m_pAutoProxy = nullptr;
	AfxOleUnlockApp();
}

void CCryptoAppDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CCryptoAppDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CCryptoAppDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_ICryptoApp to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the
//  dispinterface in the .IDL file.

// {3ca5f104-8a37-4129-b3bf-e6dafae15d9b}
static const IID IID_ICryptoApp =
{0x3ca5f104,0x8a37,0x4129,{0xb3,0xbf,0xe6,0xda,0xfa,0xe1,0x5d,0x9b}};

BEGIN_INTERFACE_MAP(CCryptoAppDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CCryptoAppDlgAutoProxy, IID_ICryptoApp, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in pch.h of this project
// {a2b91fb7-61ad-461f-abd4-a96efc421fbd}
IMPLEMENT_OLECREATE2(CCryptoAppDlgAutoProxy, "CryptoApp.Application", 0xa2b91fb7,0x61ad,0x461f,0xab,0xd4,0xa9,0x6e,0xfc,0x42,0x1f,0xbd)


// CCryptoAppDlgAutoProxy message handlers
