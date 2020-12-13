#include "pch.h"
#include "CryptoMode.h"
#include "CryptoReady.h"

EVP_CIPHER* get_des_crypto_mode(int mode)
{
    return nullptr;
}

EVP_CIPHER* get_aes_crypto_mode(int type_crypto_bit, int type_crypto_mode)
{
	EVP_CIPHER* cipher = NULL;

	if (type_crypto_bit == AES_128)
	{
		switch (type_crypto_mode)
		{
		case ECB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_128_ecb();
			break;
		case CBC_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_128_cbc();
			break;
		case OFB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_128_ofb();
			break;
		case CFB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_128_cfb();
		}
	}
	else if (type_crypto_bit == AES_192)
	{
		switch (type_crypto_mode)
		{
		case ECB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_192_ecb();
			break;
		case CBC_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_192_cbc();
			break;
		case OFB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_192_ofb();
			break;
		case CFB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_192_cfb();
		}
	}
	else if (type_crypto_bit == AES_256)
	{
		switch (type_crypto_mode)
		{
		case ECB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_256_ecb();
			break;
		case CBC_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_256_cbc();
			break;
		case OFB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_256_ofb();
			break;
		case CFB_MODE:
			cipher = (EVP_CIPHER*)EVP_aes_256_cfb();
		}
	}

	return cipher;
}
