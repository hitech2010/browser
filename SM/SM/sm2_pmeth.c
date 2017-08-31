#include "SM.h"
#include "sm2.h"


const char * pkey_sm2_get_default_id()
{
	return NULL;
}

const EVP_MD * pkey_sm2_get_default_md()
{
	return EVP_sm3();
}

int pkey_sm2_init(EVP_PKEY_CTX *ctx)
{
	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
	if (rctx == NULL)
	{
		rctx = (EC_SM2_PKEY_CTX *)OPENSSL_malloc(sizeof(EC_SM2_PKEY_CTX));
		rctx->id = pkey_sm2_get_default_id();
		rctx->md = pkey_sm2_get_default_md();
		EVP_PKEY_CTX_set_data(ctx, rctx);
	}
	return 1;
}

void pkey_sm2_cleanup(EVP_PKEY_CTX *ctx)
{
	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
	if (rctx)
	{
		OPENSSL_free(rctx);
		EVP_PKEY_CTX_set_data(ctx, NULL);
	}
}

int pkey_sm2_encrypt_init(EVP_PKEY_CTX *ctx)
{
	EC_KEY * sm2 = EVP_PKEY_get1_EC_KEY(EVP_PKEY_CTX_get0_pkey(ctx));
	if (sm2)
	{
		int ret = 0;
		if (EC_KEY_get0_public_key(sm2))
		{
			if (EC_SM2_check_key(sm2))
				ret = 1;
		}
		EC_KEY_free(sm2);
		return ret;
	}
	return 0;
}

int pkey_sm2_encrypt(EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen, const unsigned char *in, size_t inlen)
{
	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
	EC_KEY * sm2 = EVP_PKEY_get1_EC_KEY(EVP_PKEY_CTX_get0_pkey(ctx));
	if (sm2)
	{
		*outlen = EC_SM2_encrypt(sm2, in, inlen, out, rctx->md);
		EC_KEY_free(sm2);
		return 1;
	}	
	return 0;
}

int pkey_sm2_decrypt_init(EVP_PKEY_CTX *ctx)
{
	EC_KEY * sm2 = EVP_PKEY_get1_EC_KEY(EVP_PKEY_CTX_get0_pkey(ctx));
	if (sm2)
	{
		int ret = 0;
		if (EC_KEY_get0_private_key(sm2))
		{
			ret = 1;
		}
		EC_KEY_free(sm2);
		return ret;
	}
	return 0;
}

int pkey_sm2_decrypt(EVP_PKEY_CTX *ctx, unsigned char *out, size_t *outlen, const unsigned char *in, size_t inlen)
{
	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
	EC_KEY * sm2 = EVP_PKEY_get1_EC_KEY(EVP_PKEY_CTX_get0_pkey(ctx));
	if (sm2)
	{
		*outlen = EC_SM2_decrypt(sm2, in, inlen, out, rctx->md);
		EC_KEY_free(sm2);
		return 1;
	}	
	return 0;
}

int pkey_sm2_sign(EVP_PKEY_CTX *ctx, unsigned char *sig, size_t *siglen, const unsigned char *tbs, size_t tbslen)
{
	unsigned char * sigout = NULL;
	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
	EC_KEY * sm2 = EVP_PKEY_get1_EC_KEY(EVP_PKEY_CTX_get0_pkey(ctx));
	EC_SM2_SIG * sm2sig = EC_SM2_SIG_new();
	if (sm2)
	{
		int ret = EC_SM2_sign(sm2, rctx->id, tbs, tbslen, sm2sig, rctx->md);
		EC_KEY_free(sm2);
		if (ret > 0)
		{
			int sigoutlen = i2d_EC_SM2_SIG(sm2sig, &sigout);
			EC_SM2_SIG_free(sm2sig);
			if (sig)
			{
				ret = 0;
				if (siglen)
				{
					if ((int)*siglen >= sigoutlen)
					{
						memcpy(sig, sigout, sigoutlen);
						ret = 1;
					}
				}
			}
			if (siglen) *siglen = sigoutlen;
			OPENSSL_free(sigout);
		}
		return ret;
	}
	return 0;
}

//int pkey_sm2_sign(EVP_PKEY_CTX *ctx, unsigned char *sig, size_t *siglen, const unsigned char *tbs, size_t tbslen)
//{
//	unsigned char * sigout = NULL;
//	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
//	EC_KEY * sm2 = EVP_PKEY_get1_EC_KEY(EVP_PKEY_CTX_get0_pkey(ctx));
//	EC_SM2_SIG * sm2sig = EC_SM2_SIG_new();
//	if (sm2)
//	{
//		int ret = EC_SM2_sign(sm2, rctx->id, tbs, tbslen, sm2sig, rctx->md);
//		EC_KEY_free(sm2);
//		if (ret > 0)
//		{
//			int sigoutlen = 64;//i2d_EC_SM2_SIG(sm2sig, &sigout);
//			sigout = (unsigned char*)OPENSSL_malloc(sigoutlen);
//			unsigned char buf[32] = { 0 };
//			BN_bn2bin(sm2sig->r, buf);
//			memcpy(sigout, buf, 32);
//
//			BN_bn2bin(sm2sig->s, buf);
//			memcpy(sigout + 32, buf, 32);
//			
//			EC_SM2_SIG_free(sm2sig);
//			if (sig)
//			{
//				ret = 0;
//				if (siglen)
//				{
//					if ((int)*siglen >= sigoutlen)
//					{
//						memcpy(sig, sigout, sigoutlen);
//						ret = 1;
//					}
//				}
//			}
//			if (siglen) *siglen = sigoutlen;
//			OPENSSL_free(sigout);
//		}
//		return ret;
//	}
//	return 0;
//}

int pkey_sm2_verify(EVP_PKEY_CTX *ctx, const unsigned char *sig, size_t siglen, const unsigned char *tbs, size_t tbslen)
{
	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
	EC_KEY * sm2 = EVP_PKEY_get1_EC_KEY(EVP_PKEY_CTX_get0_pkey(ctx));
	EC_SM2_SIG * sm2sig = d2i_EC_SM2_SIG(NULL, &sig, siglen); //EC_SM2_SIG_new();// d2i_EC_SM2_SIG(NULL, &sig, siglen);

	/*unsigned char buf[32] = { 0 };
	memcpy(buf, sig, 32);
	sm2sig->r = BN_bin2bn(buf, 32, NULL);
	
	memcpy(buf, sig + 32, 32);
	sm2sig->s = BN_bin2bn(buf, 32, NULL);*/

	int ret = 0;
	if (sm2)
	{
		ret = EC_SM2_verify(sm2, rctx->id, tbs, tbslen, sm2sig, rctx->md);
		EC_KEY_free(sm2);
	}
	if (sm2sig)
		EC_SM2_SIG_free(sm2sig);
	return ret;
}

EVP_PKEY_METHOD * sm2_pmeth_new()
{
	EVP_PKEY_METHOD * sm2_pmeth = EVP_PKEY_meth_new(NID_sm2, 0);
	if (sm2_pmeth)
	{
 		EVP_PKEY_meth_set_init(sm2_pmeth, pkey_sm2_init);
 		EVP_PKEY_meth_set_cleanup(sm2_pmeth, pkey_sm2_cleanup);
		EVP_PKEY_meth_set_encrypt(sm2_pmeth, pkey_sm2_encrypt_init, pkey_sm2_encrypt);
		EVP_PKEY_meth_set_decrypt(sm2_pmeth, pkey_sm2_decrypt_init, pkey_sm2_decrypt);
		EVP_PKEY_meth_set_sign(sm2_pmeth, pkey_sm2_decrypt_init, pkey_sm2_sign);
		EVP_PKEY_meth_set_verify(sm2_pmeth, pkey_sm2_encrypt_init, pkey_sm2_verify);

		// cause memory leak in openssl
//		EVP_PKEY_meth_add0(sm2_pmeth);
	}
	return sm2_pmeth;
}
