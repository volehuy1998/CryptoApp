#pragma once

#include <openssl/evp.h>

EVP_CIPHER* get_des_crypto_mode(int mode);
EVP_CIPHER* get_aes_crypto_mode(int type_bit_crypto, int type_crypto_mode);
