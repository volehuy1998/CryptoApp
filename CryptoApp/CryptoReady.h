#pragma once

#define DES_IDX		0
#define AES_128_IDX 1
#define AES_192_IDX 2
#define RC2_IDX		3

#define DES_READY	  1
#define AES_128_READY 1
#define AES_192_READY 1
#define RC2_READY	  0

#define DES_BLOCK	  1
#define AES_128_BLOCK 2
#define AES_192_BLOCK 3
#define RC2_BLOCK	  4

#define ECB_READY 1
#define CBC_READY 1
#define OFB_READY 1
#define CFB_READY 1
#define EDE_READY 1

#define AES_128 2
#define AES_192 3

#define ECB_MODE 1
#define CBC_MODE 2
#define CFB_MODE 3
#define OFB_MODE 4
#define EDE_MODE 5