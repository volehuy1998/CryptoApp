#pragma once

#define DES		1
#define AES_128 0
#define AES_192 0
#define RC2		0

#define ECB 1
#define CBC 1
#define OFB 1
#define CFB 1
#define EDE 1


int crypto_map[] = { DES, AES_128, AES_192, RC2 };
int mode_map[] = { ECB, CBC, OFB, CFB, EDE };