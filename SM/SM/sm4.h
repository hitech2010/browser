#ifndef _SM4_H_
#define _SM4_H_

#ifdef  __cplusplus
extern "C" {
#if 0
}
#endif
#endif

enum { SM4_BLOCK_SIZE = 16 };

#define RT_PARAM_ERR  -1
#define RT_OK 0

typedef unsigned char	UINT8;
typedef unsigned short	UINT16;
typedef unsigned int 	UINT32;

typedef struct
{
	unsigned int RK_ADDR[32];
} SM4_KEY;

UINT8 SM4InitKey(const unsigned char * pEK, SM4_KEY * key);
void SM4Xor(unsigned char * buff, const unsigned char * chain);
void SM4EncryptBlock(const unsigned char * pIn, unsigned char * pOut, const SM4_KEY * key);
void SM4DecryptBlock(const unsigned char * pIn, unsigned char * pOut, const SM4_KEY * key);

#ifdef  __cplusplus
}
#endif

#endif
