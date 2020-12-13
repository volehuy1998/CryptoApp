#pragma once

#include <openssl/err.h>
#include <openssl/evp.h>

#include "CryptoMode.h"

int perform_aes(unsigned char* input, int input_len, unsigned char* output, int* output_len, unsigned char* aes_key, unsigned char* aes_iv, int type_crypto_bit, int type_crypto_mode, int perform_mode, unsigned long* error);