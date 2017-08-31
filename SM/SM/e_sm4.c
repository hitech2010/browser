#include <stddef.h>
#include <memory.h>
#include "SM.h"
#include "sm4.h"
#include <openssl/evp.h>

#define data(ctx)	(SM4_KEY *)(ctx->cipher_data)

static int sm4_init_key(EVP_CIPHER_CTX *ctx, const unsigned char *key, const unsigned char *iv, int enc)
{
	SM4InitKey(key, data(ctx));
	return 1;
}

static int sm4_cbc_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out, const unsigned char *in, size_t inl)
{
	if (ctx->encrypt)
	{
		while (inl >= SM4_BLOCK_SIZE)
		{
			unsigned char block[SM4_BLOCK_SIZE];
			memcpy(block, in, SM4_BLOCK_SIZE);
			SM4Xor(block, ctx->iv);
			SM4EncryptBlock(block, out, data(ctx));
			memcpy(ctx->iv, out, SM4_BLOCK_SIZE);
			inl -= SM4_BLOCK_SIZE;
			in += SM4_BLOCK_SIZE;
			out += SM4_BLOCK_SIZE;
		}
	}
	else
	{
		while (inl >= SM4_BLOCK_SIZE)
		{
			SM4DecryptBlock(in, out, data(ctx));
			SM4Xor(out, ctx->iv);
			memcpy(ctx->iv, in, SM4_BLOCK_SIZE);
			inl -= SM4_BLOCK_SIZE;
			in += SM4_BLOCK_SIZE;
			out += SM4_BLOCK_SIZE;
		}

	}
	return 1;
}

static int sm4_ecb_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out, const unsigned char *in, size_t inl)
{
	if (ctx->encrypt)
	{
		while (inl >= SM4_BLOCK_SIZE)
		{
			SM4EncryptBlock(in, out, data(ctx));
			inl -= SM4_BLOCK_SIZE;
			in += SM4_BLOCK_SIZE;
			out += SM4_BLOCK_SIZE;
		}
	}
	else
	{
		while (inl >= SM4_BLOCK_SIZE)
		{
			SM4DecryptBlock(in, out, data(ctx));
			inl -= SM4_BLOCK_SIZE;
			in += SM4_BLOCK_SIZE;
			out += SM4_BLOCK_SIZE;
		}
	}
	return 1;
}

static int sm4_cfb_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out, const unsigned char *in, size_t inl)
{
	if (ctx->encrypt)
	{
		while (inl >= SM4_BLOCK_SIZE)
		{
			SM4EncryptBlock(ctx->iv, out, data(ctx));
			SM4Xor(out, in);
			memcpy(ctx->iv, out, SM4_BLOCK_SIZE);
			inl -= SM4_BLOCK_SIZE;
			in += SM4_BLOCK_SIZE;
			out += SM4_BLOCK_SIZE;
		}
	}
	else
	{
		while (inl >= SM4_BLOCK_SIZE)
		{
			SM4EncryptBlock(ctx->iv, out, data(ctx));
			SM4Xor(out, in);
			memcpy(ctx->iv, in, SM4_BLOCK_SIZE);
			inl -= SM4_BLOCK_SIZE;
			in += SM4_BLOCK_SIZE;
			out += SM4_BLOCK_SIZE;
		}
	}
	return 1;
}

static int sm4_ofb_cipher(EVP_CIPHER_CTX *ctx, unsigned char *out, const unsigned char *in, size_t inl)
{
	if (ctx->encrypt)
	{
		while (inl >= SM4_BLOCK_SIZE)
		{
			SM4EncryptBlock(ctx->iv, out, data(ctx));
			memcpy(ctx->iv, out, SM4_BLOCK_SIZE);
			SM4Xor(out, in);
			inl -= SM4_BLOCK_SIZE;
			in += SM4_BLOCK_SIZE;
			out += SM4_BLOCK_SIZE;
		}
	}
	else
	{
		while (inl >= SM4_BLOCK_SIZE)
		{
			SM4EncryptBlock(ctx->iv, out, data(ctx));
			memcpy(ctx->iv, out, SM4_BLOCK_SIZE);
			SM4Xor(out, in);
			inl -= SM4_BLOCK_SIZE;
			in += SM4_BLOCK_SIZE;
			out += SM4_BLOCK_SIZE;
		}
	}
	return 1;
}

static const EVP_CIPHER sm4_cbc =
{
	NID_sm4,
	SM4_BLOCK_SIZE,
	16,	//key_len
	16,	//iv_len
	EVP_CIPH_CBC_MODE,
	sm4_init_key,
	sm4_cbc_cipher,
	NULL,
	sizeof(SM4_KEY),
	EVP_CIPHER_set_asn1_iv,
	EVP_CIPHER_get_asn1_iv,
	NULL,
	NULL
};

const EVP_CIPHER * EVP_sm4_cbc()
{
	return &sm4_cbc;
}

static const EVP_CIPHER sm4_ecb =
{
	NID_sm4,
	SM4_BLOCK_SIZE,
	16,
	0,
	EVP_CIPH_ECB_MODE,
	sm4_init_key,
	sm4_ecb_cipher,
	NULL,
	sizeof(SM4_KEY),
	EVP_CIPHER_set_asn1_iv,
	EVP_CIPHER_get_asn1_iv,
	NULL,
	NULL
};

const EVP_CIPHER * EVP_sm4_ecb()
{
	return &sm4_ecb;
}

static const EVP_CIPHER sm4_cfb =
{
	NID_sm4,
	1,
	16,
	16,
	EVP_CIPH_CFB_MODE,
	sm4_init_key,
	sm4_cfb_cipher,
	NULL,
	sizeof(SM4_KEY),
	EVP_CIPHER_set_asn1_iv,
	EVP_CIPHER_get_asn1_iv,
	NULL,
	NULL
};

const EVP_CIPHER * EVP_sm4_cfb()
{
	return &sm4_cfb;
}

static const EVP_CIPHER sm4_ofb =
{
	NID_sm4,
	1,
	16,
	16,
	EVP_CIPH_OFB_MODE,
	sm4_init_key,
	sm4_ofb_cipher,
	NULL,
	sizeof(SM4_KEY),
	EVP_CIPHER_set_asn1_iv,
	EVP_CIPHER_get_asn1_iv,
	NULL,
	NULL
};

const EVP_CIPHER * EVP_sm4_ofb()
{
	return &sm4_ofb;
}
