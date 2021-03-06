﻿
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

#include "../Native-Des-Lib/NativeDesDLL/NativeDes.h"
#include <sstream>
#include <vector>
#include <openssl/rand.h>
#include "CryptoReady.h"
#include "CryptoAES.h"

int crypto_map[] = { DES_READY, AES_128_READY, AES_192_READY, AES_256_READY, RC2_READY };
int mode_map[] = { ECB_READY, CBC_READY, OFB_READY, CFB_READY, EDE_READY };
int block_map[] = { DES_BLOCK, AES_128_BLOCK, AES_192_BLOCK, RC2_BLOCK };

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
	ON_BN_CLICKED(button_gen_iv, &CCryptoAppDlg::OnBnClickedGenIV)
	ON_BN_CLICKED(radio_aes128, &CCryptoAppDlg::OnBnClickedaes128)
	ON_BN_CLICKED(radio_des, &CCryptoAppDlg::OnBnClickeddes)
	ON_BN_CLICKED(radio_aes192, &CCryptoAppDlg::OnBnClickedaes192)
	ON_BN_CLICKED(radio_aes256, &CCryptoAppDlg::OnBnClickedaes256)
	ON_BN_CLICKED(radio_rc2, &CCryptoAppDlg::OnBnClickedrc2)
	ON_BN_CLICKED(radio_ecb, &CCryptoAppDlg::OnBnClickedecb)
	ON_BN_CLICKED(radio_ncbc, &CCryptoAppDlg::OnBnClickedncbc)
	ON_BN_CLICKED(radio_ofb, &CCryptoAppDlg::OnBnClickedofb)
	ON_BN_CLICKED(radio_cfb, &CCryptoAppDlg::OnBnClickedcfb)
	ON_BN_CLICKED(radio_ede, &CCryptoAppDlg::OnBnClickedede)
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

	// make default encrypt mode
	CDialogEx::CheckRadioButton(radio_encrypt, radio_decrypt, radio_encrypt);

	// make default crypto and mode *ecb, cbc, ofb
	CDialogEx::CheckRadioButton(radio_des, radio_rc2, radio_des);
	CDialogEx::CheckRadioButton(radio_ecb, radio_ofb, radio_ecb);

	std::vector<CWnd*> radio_cryptos;
	radio_cryptos.push_back(CDialogEx::GetDlgItem(radio_des));
	radio_cryptos.push_back(CDialogEx::GetDlgItem(radio_aes128));
	radio_cryptos.push_back(CDialogEx::GetDlgItem(radio_aes192));
	radio_cryptos.push_back(CDialogEx::GetDlgItem(radio_aes256));
	radio_cryptos.push_back(CDialogEx::GetDlgItem(radio_rc2));

	std::vector<CWnd*> radio_modes;
	radio_modes.push_back(CDialogEx::GetDlgItem(radio_ecb));
	radio_modes.push_back(CDialogEx::GetDlgItem(radio_ncbc));
	radio_modes.push_back(CDialogEx::GetDlgItem(radio_ofb));
	radio_modes.push_back(CDialogEx::GetDlgItem(radio_cfb));
	radio_modes.push_back(CDialogEx::GetDlgItem(radio_ede));

	// disable/enable radio crypto category
	for (int i = 0; i < radio_cryptos.size(); i++)
		radio_cryptos[i]->EnableWindow(crypto_map[i]);
	radio_cryptos.clear();

	// disable/enable radio crypto mode
	for (int i = 0; i < radio_modes.size(); i++)
		radio_modes[i]->EnableWindow(mode_map[i]);
	radio_modes.clear();

	// disable button generate iv if mode ecb
	if (CDialogEx::GetCheckedRadioButton(radio_ecb, radio_ede) == radio_ecb)
	{
		// disable button
		CWnd* pbutton_gen_iv = CDialogEx::GetDlgItem(button_gen_iv);
		pbutton_gen_iv->EnableWindow(FALSE);
	}

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

unsigned char* ConvertCString2Pointer(CString cstring)
{
	CT2CA pszConvertedAnsiString(cstring);
	std::string str_input(pszConvertedAnsiString);

	return (unsigned char*)calloc(str_input.length(), 1);
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
	unsigned long long input_iv_ull = 0;
	int output_performed_len = 0;
	std::string output_native_string = "";
	std::string output_hex_string = "";
	CString output_hex_cstring;
	CString output_native_cstring;
	unsigned long error = 1;
	std::string error_msg = "";
	int error_key = 1;
	int crypto_category = 1;
	int crypto_mode = 1;
	int radio_crypto_category = CDialogEx::GetCheckedRadioButton(radio_des, radio_rc2);
	int radio_crypto_mode = CDialogEx::GetCheckedRadioButton(radio_ecb, radio_ede);
	int blocks = 0;

	int type_crypto_bit = 0;
	int type_crypto_mode = 0;
	unsigned char* key = NULL;
	unsigned char* iv = NULL;
	CString key_cstring;
	CString iv_cstring;

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

	if (radio_crypto_category == radio_des)
	{
		crypto_category = 1;
		blocks = block_map[DES_IDX];
		key = (unsigned char*)calloc(8, 1);
		iv = (unsigned char*)calloc(8, 1);
	}
	else if (radio_crypto_category == radio_aes128)
	{
		crypto_category = 2;
		blocks = block_map[AES_128_IDX];
		type_crypto_bit = AES_128;
		key = (unsigned char*)calloc(16, 1);
		iv = (unsigned char*)calloc(16, 1);
	}
	else if (radio_crypto_category == radio_aes192)
	{
		crypto_category = 3;
		blocks = block_map[AES_192_IDX];
		type_crypto_bit = AES_192;
		key = (unsigned char*)calloc(24, 1);
		iv = (unsigned char*)calloc(24, 1);
	}
	else if (radio_crypto_category == radio_aes256)
	{
		crypto_category = 4;
		blocks = block_map[AES_256_IDX];
		type_crypto_bit = AES_256;
		key = (unsigned char*)calloc(32, 1);
		iv = (unsigned char*)calloc(32, 1);
	}
	else if (radio_crypto_category == radio_rc2)
	{
		crypto_category = 5;
		blocks = block_map[RC2_IDX];
	}

	if (radio_crypto_mode == radio_ecb)
	{
		crypto_mode = 1;
		type_crypto_mode = ECB_MODE;
	}
	else if (radio_crypto_mode == radio_ncbc)
	{
		crypto_mode = 2;
		type_crypto_mode = CBC_MODE;
	}
	else if (radio_crypto_mode == radio_ofb)
	{
		crypto_mode = 3;
		type_crypto_mode = OFB_MODE;
	}
	else if (radio_crypto_mode == radio_cfb)
	{
		crypto_mode = 4;
		type_crypto_mode = CFB_MODE;
	}
	else if (radio_crypto_mode == radio_ede)
	{
		crypto_mode = 5;
		type_crypto_mode = EDE_MODE;
	}

	CDialogEx::SetDlgItemTextW(edit_control_error_state_cstring, CString(std::string("").c_str()));

	do
	{
		// get key cstring
		// convert key cstring to key ull
		key = set_key(edit_control_key_hex_cstring);

		if (type_crypto_mode != ECB_MODE)
		{
			// ecb mode no need iv

			// get key cstring
			// convert key cstring to key ull
			iv = set_key(edit_control_iv_hex_cstring);
		}

		// convert hex cstring to hex string
		input_hex_string = ConvertCString2String(input_hex_cstring);

		// convert hex string to native string
		input_native_string = hex_to_string(input_hex_string);

		// init output native
		input_native_len = input_native_string.length();

		output_native_len = ((input_native_len / (DES_KEY_SZ * blocks)) + 1) * (DES_KEY_SZ * blocks);
		output_native = (unsigned char*)calloc(output_native_len + 1, 1);

		if (crypto_category == 1)
		{
			// des

			// perform
			perform_des((unsigned char*)input_native_string.c_str(), input_native_len, key, iv, output_native, &output_performed_len, is_check_encrypt, crypto_category, crypto_mode, &error);
		}
		else if (crypto_category == 2 || crypto_category == 3 || crypto_category == 4)
		{
			// aes
			perform_aes((unsigned char*)input_native_string.c_str(), input_native_len, output_native, &output_performed_len, key, iv, type_crypto_bit, type_crypto_mode, is_check_encrypt, &error);
		}

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

	free(key);
	free(iv);

	CString error_msg_cstring = CString(error_msg.c_str());
	CDialogEx::SetDlgItemTextW(edit_control_error_state_cstring, error_msg_cstring);
}


void CCryptoAppDlg::OnBnClickedGenkey()
{
	this->put_key_or_iv(edit_control_key_hex_cstring);
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


void CCryptoAppDlg::OnBnClickedGenIV()
{
	this->put_key_or_iv(edit_control_iv_hex_cstring);
}

unsigned char* CCryptoAppDlg::set_key(int edit_control_id)
{
	CString input_key_cstring;
	CDialogEx::GetDlgItemTextW(edit_control_id, input_key_cstring);
	CT2CA pszConvertedAnsiString(input_key_cstring);
	std::string str_input(pszConvertedAnsiString);
	unsigned char* buff = (unsigned char*)calloc(str_input.length(), 1);
	memcpy_s(buff, str_input.length(), str_input.c_str(), str_input.length());
	return buff;
}

void CCryptoAppDlg::enable_item(int item, BOOL enable)
{
	CWnd* pbutton_gen_iv = CDialogEx::GetDlgItem(item);
	pbutton_gen_iv->EnableWindow(enable);
}

bool CCryptoAppDlg::check_crypto_category(int item_id)
{
	return CDialogEx::GetCheckedRadioButton(radio_des, radio_rc2) == item_id;
}

void CCryptoAppDlg::put_key_or_iv(int edit_control_id)
{
	int blocks = 0;
	int total_byte = 0;
	unsigned char* key = NULL;
	CString output_key_cstring;

	if (this->check_crypto_category(radio_des))
		blocks = 1;
	else if (this->check_crypto_category(radio_aes128))
		blocks = 2;
	else if (this->check_crypto_category(radio_aes192))
		blocks = 3;
	else if (this->check_crypto_category(radio_aes256))
		blocks = 4;

	total_byte = blocks * 8;
	key = (unsigned char*)calloc(total_byte, 1);
	RAND_bytes(key, total_byte);
	std::string result = string_to_hex(std::string((const char*)key, total_byte));
	output_key_cstring = CString(result.c_str());
	CDialogEx::SetDlgItemTextW(edit_control_id, output_key_cstring);
	free(key);
}

void CCryptoAppDlg::OnBnClickedaes128()
{
	// TODO: Add your control notification handler code here
	this->enable_item(radio_ede, FALSE);
}


void CCryptoAppDlg::OnBnClickeddes()
{
	// TODO: Add your control notification handler code here
	this->enable_item(radio_ede, TRUE);
}


void CCryptoAppDlg::OnBnClickedaes192()
{
	// TODO: Add your control notification handler code here
	this->enable_item(radio_ede, FALSE);
}



void CCryptoAppDlg::OnBnClickedaes256()
{
	// TODO: Add your control notification handler code here
	this->enable_item(radio_ede, FALSE);
}



void CCryptoAppDlg::OnBnClickedrc2()
{
	// TODO: Add your control notification handler code here
	this->enable_item(radio_ede, FALSE);
}


void CCryptoAppDlg::OnBnClickedecb()
{
	// TODO: Add your control notification handler code here
	this->enable_item(button_gen_iv, FALSE);
}


void CCryptoAppDlg::OnBnClickedncbc()
{
	// TODO: Add your control notification handler code here
	this->enable_item(button_gen_iv, TRUE);
}


void CCryptoAppDlg::OnBnClickedofb()
{
	// TODO: Add your control notification handler code here
	this->enable_item(button_gen_iv, TRUE);
}


void CCryptoAppDlg::OnBnClickedcfb()
{
	// TODO: Add your control notification handler code here
	this->enable_item(button_gen_iv, TRUE);
}


void CCryptoAppDlg::OnBnClickedede()
{
	// TODO: Add your control notification handler code here
	this->enable_item(button_gen_iv, TRUE);
}