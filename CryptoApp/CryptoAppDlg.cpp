
// CryptoAppDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "CryptoApp.h"
#include "CryptoAppDlg.h"
#include "DlgProxy.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "../../NativeDesLibrary/NativeDesDLL/NativeDes.cpp"
#include <sstream>

void StringUpper(std::string& str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = std::toupper(str[i]);
}

std::string string_to_hex(const std::string& input)
{
	static const char hex_digits[] = "0123456789ABCDEF";

	std::string output;
	output.reserve(input.length() * 2);
	for (unsigned char c : input)
	{
		output.push_back(hex_digits[c >> 4]);
		output.push_back(hex_digits[c & 15]);
	}
	return output;
}

int hex_value(unsigned char hex_digit)
{
	static const signed char hex_values[256] = {
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, -1, -1, -1, -1, -1, -1,
		-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
		-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	};
	int value = hex_values[hex_digit];
	if (value == -1) throw std::invalid_argument("invalid hex digit");
	return value;
}

std::string hex_to_string(const std::string& input)
{
	const auto len = input.length();
	if (len & 1) throw std::invalid_argument("odd length");

	std::string output;
	output.reserve(len / 2);
	for (auto it = input.begin(); it != input.end(); )
	{
		int hi = hex_value(*it++);
		int lo = hex_value(*it++);
		output.push_back(hi << 4 | lo);
	}
	return output;
}

// CCryptoAppDlg dialog


IMPLEMENT_DYNAMIC(CCryptoAppDlg, CDialogEx);

CCryptoAppDlg::CCryptoAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CryptoApp_DIALOG, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_pAutoProxy = nullptr;
}

CCryptoAppDlg::~CCryptoAppDlg()
{
	// If there is an automation proxy for this dialog, set
	//  its back pointer to this dialog to null, so it knows
	//  the dialog has been deleted.
	if (m_pAutoProxy != nullptr)
		m_pAutoProxy->m_pDialog = nullptr;
}

void CCryptoAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CCryptoAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(button_perform, &CCryptoAppDlg::OnBnClickedPerform)
	ON_BN_CLICKED(button_gen_key, &CCryptoAppDlg::OnBnClickedGenkey)
	ON_EN_CHANGE(edit_control_input_native_cstring, &CCryptoAppDlg::OnEnChange)
END_MESSAGE_MAP()


// CCryptoAppDlg message handlers

BOOL CCryptoAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
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

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CenterWindow();

	ShowWindow(SW_NORMAL);

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CCryptoAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CCryptoAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CCryptoAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CCryptoAppDlg::OnClose()
{
	if (CanExit())
		CDialogEx::OnClose();
}

void CCryptoAppDlg::OnOK()
{
	if (CanExit())
		CDialogEx::OnOK();
}

void CCryptoAppDlg::OnCancel()
{
	if (CanExit())
		CDialogEx::OnCancel();
}

BOOL CCryptoAppDlg::CanExit()
{
	// If the proxy object is still around, then the automation
	//  controller is still holding on to this application.  Leave
	//  the dialog around, but hide its UI.
	if (m_pAutoProxy != nullptr)
	{
		ShowWindow(SW_HIDE);
		return FALSE;
	}

	return TRUE;
}

std::string ConvertCString2String(CString cstring)
{
	CT2CA pszConvertedAnsiString(cstring);
	std::string str_input(pszConvertedAnsiString);

	return str_input;
}

unsigned long long ConvertKeyCStringToULL(CString cstring)
{
	CT2A my_key(cstring.GetBuffer());
	unsigned long long key = std::strtoull(my_key, NULL, 16);

	return key;
}

void CCryptoAppDlg::OnBnClickedPerform()
{

	bool is_check_encrypt = CDialogEx::GetCheckedRadioButton(radio_encrypt, radio_encrypt) == radio_encrypt ? true : false;
	CString input_hex_cstring;
	std::string input_hex_string = "";
	std::string input_native_string = "";
	int input_native_len = 0;
	int output_native_len = 0;
	unsigned char* output_native = NULL;
	CString input_key_cstring;
	unsigned long long input_key_ull = 0;
	int output_performed_len = 0;
	std::string output_native_string = "";
	std::string output_hex_string = "";
	CString output_hex_cstring;
	CString output_native_cstring;
	unsigned long error = 1;
	std::string error_msg = "";
	int error_key = 1;

	if (is_check_encrypt)
	{
		// encrypt

		// get encrypt cstring
		CDialogEx::GetDlgItemTextW(edit_control_input_hex_cstring, input_hex_cstring);
	}
	else
	{
		// get decrypt cstring
		CDialogEx::GetDlgItemTextW(edit_control_output_hex_cstring, input_hex_cstring);
	}

	CDialogEx::SetDlgItemTextW(edit_control_error_state_cstring, CString(std::string("").c_str()));

	do
	{
		// get key cstring
		CDialogEx::GetDlgItemTextW(edit_control_key_hex_cstring, input_key_cstring);

		// convert key cstring to key ull
		input_key_ull = ConvertKeyCStringToULL(input_key_cstring);

		error_key = des_check_key(input_key_ull);
		if (error_key == -2)
		{
			error_msg = "Key weak";
			break;
		}

		if (error_key == -1)
		{
			error_msg = "Key check parity bit failed";
			break;
		}

		// convert hex cstring to hex string
		input_hex_string = ConvertCString2String(input_hex_cstring);

		// convert hex string to native string
		input_native_string = hex_to_string(input_hex_string);

		// init output native
		input_native_len = input_native_string.length();

		output_native_len = ((input_native_len / DES_KEY_SZ) + 1) * DES_KEY_SZ;
		output_native = (unsigned char*)calloc(output_native_len + 1, 1);

		// perform
		des_ecb((unsigned char*)input_native_string.c_str(), input_native_len, input_key_ull, output_native, &output_performed_len, is_check_encrypt, &error);

		if (error != 1)
		{
			error_msg = ERR_reason_error_string(error);
			break;
		}

		if (output_native != NULL)
		{
			// convert output native (pointer) to output native (C++) with performed length
			output_native_string = std::string((const char* const)output_native, output_performed_len);
		}

		// convert output native string to output native cstring
		output_native_cstring = CString(output_native_string.c_str());

		// convert output native to output hex
		output_hex_string = string_to_hex(output_native_string);

		// convert output hex string to output hex cstring
		output_hex_cstring = CString(output_hex_string.c_str());

		if (is_check_encrypt)
		{
			// set EDIT CONTROL 
			CDialogEx::SetDlgItemTextW(edit_control_output_native_cstring, output_native_cstring);
			CDialogEx::SetDlgItemTextW(edit_control_output_hex_cstring, output_hex_cstring);
		}
		else
		{
			// set EDIT CONTROL 
			CDialogEx::SetDlgItemTextW(edit_control_input_native_cstring, output_native_cstring);
			CDialogEx::SetDlgItemTextW(edit_control_input_hex_cstring, output_hex_cstring);
		}

		if (error != 1)
		{
			error_msg = ERR_reason_error_string(error);
			break;
		}

		error_msg = "Success";
	} while (false);

	if (output_native != NULL)
	{
		free(output_native);
	}

	CString error_msg_cstring = CString(error_msg.c_str());
	CDialogEx::SetDlgItemTextW(edit_control_error_state_cstring, error_msg_cstring);
}


void CCryptoAppDlg::OnBnClickedGenkey()
{
	// TODO: Add your control notification handler code here
	CString output_key;
	_DES_cblock _des_block;
	memset(_des_block.des_block, 0, DES_KEY_SZ);

	_des_block.des_ull = des_random_key();

	std::stringstream stream;
	stream << std::hex << _des_block.des_ull;
	std::string result(stream.str());
	StringUpper(result);

	output_key = CString(result.c_str());
	CDialogEx::SetDlgItemTextW(edit_control_key_hex_cstring, output_key);
}

void CCryptoAppDlg::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	CString input;
	CString hex_input;
	CDialogEx::GetDlgItemTextW(edit_control_input_native_cstring, input);

	CT2CA pszConvertedAnsiString(input);
	std::string str_input(pszConvertedAnsiString);
	str_input = string_to_hex(str_input);

	hex_input = CString(str_input.c_str());
	CDialogEx::SetDlgItemTextW(edit_control_input_hex_cstring, hex_input);
}
