#pragma once

#define DES_IDX		0
#define AES_128_IDX 1
#define AES_192_IDX 2
#define RC2_IDX		3

#define DES		1
#define AES_128 1
#define AES_192 1
#define RC2		0

#define DES_BLOCK	  1
#define AES_128_BLOCK 2
#define AES_192_BLOCK 3
#define RC2_BLOCK	  4

#define ECB 1
#define CBC 1
#define OFB 1
#define CFB 1
#define EDE 1


int crypto_map[] = { DES, AES_128, AES_192, RC2 };
int mode_map[] = { ECB, CBC, OFB, CFB, EDE };
int block_map[] = { DES_BLOCK, AES_128_BLOCK, AES_192_BLOCK, RC2_BLOCK };