#include "SM.h"
#include "sm3.h"

static int init(EVP_MD_CTX *ctx)
{
	return SM3_Init(ctx->md_data);
}

static int update(EVP_MD_CTX *ctx, const void *data, size_t count)
{
	return SM3_Update(ctx->md_data, data, count);
}

static int final(EVP_MD_CTX *ctx, unsigned char *md)
{
	return SM3_Final(ctx->md_data, md);
}

static const EVP_MD sm3_md =
{
	NID_sm3,
	0/*NID_md5WithRSAEncryption*/,
	SM3_DIGEST_LENGTH,
	0,
	init,
	update,
	final,
	NULL,
	NULL,
	EVP_PKEY_NULL_method,
	64/*MD5_CBLOCK*/,
	sizeof(EVP_MD *)+sizeof(SM3_CTX),
	NULL
};

const EVP_MD * EVP_sm3()
{
	return &sm3_md;
}
