#include "pch.h"
#include "CryptoAES.h"

int perform_aes(unsigned char* input, int input_len, unsigned char* output, int* output_len, unsigned char* aes_key, unsigned char* aes_iv, int type_crypto_bit, int type_crypto_mode, int perform_mode, unsigned long* error)
{
	int result_flag = 0;
	int remain_len = 0;
	EVP_CIPHER_CTX* ctx = NULL;
	EVP_CIPHER* cipher = NULL;

	do
	{
		cipher = get_aes_crypto_mode(type_crypto_bit, type_crypto_mode);
		if (cipher == NULL)
		{
			*error = ERR_get_error();
			break;
		}

		ctx = EVP_CIPHER_CTX_new();
		if (ctx == NULL)
		{
			*error = ERR_get_error();
			break;
		}

		result_flag = EVP_CipherInit_ex(ctx, cipher, NULL, aes_key, aes_iv, perform_mode);
		if (result_flag != 1)
		{
			*error = ERR_get_error();
			break;
		}

		result_flag = EVP_CipherUpdate(ctx, output, output_len, input, input_len);
		if (result_flag != 1)
		{
			*error = ERR_get_error();
			break;
		}

		result_flag = EVP_CipherFinal_ex(ctx, output + *output_len, &remain_len);
		if (result_flag != 1)
		{
			*error = ERR_get_error();
			break;
		}

		*output_len = *output_len + remain_len;

	} while (false);

	return result_flag;
}