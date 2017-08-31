#ifndef _SM2_H_
#define _SM2_H_

#include <openssl/evp.h>
#include <openssl/ec.h>

#ifdef  __cplusplus
extern "C" {
#if 0
}
#endif
#endif

#define EC_FLAGS_IS_SM2_KEY		0x10000

typedef struct  
{
	const char *name;
//	ECDSA_SIG *(*ecdsa_do_sign)(const unsigned char *dgst, int dgst_len, const BIGNUM *inv, const BIGNUM *rp, EC_KEY *eckey);
//	int (*ecdsa_sign_setup)(EC_KEY *eckey, BN_CTX *ctx, BIGNUM **kinv, BIGNUM **r);
//	int (*ecdsa_do_verify)(const unsigned char *dgst, int dgst_len, const ECDSA_SIG *sig, EC_KEY *eckey);
#if 0
	int (*init)(EC_KEY *eckey);
	int (*finish)(EC_KEY *eckey);
#endif
	int flags;
	char *app_data;
} EC_SM2_METHOD;

/*
typedef struct  
{
	/ * EC_KEY_METH_DATA part * /
	int (*init)(EC_KEY *);
	/ * method (ECDSA) specific part * /
	ENGINE	*engine;
	int	flags;
	const EC_SM2_METHOD *meth;
	CRYPTO_EX_DATA ex_data;
} EC_SM2_DATA;
*/


EVP_PKEY_ASN1_METHOD* sm2_pameth_new();
EVP_PKEY_METHOD * sm2_pmeth_new();
ENGINE *engine_sm2(void);

typedef struct  
{
	BIGNUM * EC_p;
	BIGNUM * EC_n;
	EC_POINT * BASE_POINT;
	EC_GROUP * EC_Group;
} EC_SM2_DATA;

EC_SM2_DATA *ec_sm2_check(EC_KEY *eckey);

typedef struct
{
	const char * id;
	/* message digest */
	const EVP_MD *md;
} EC_SM2_PKEY_CTX;

int EC_SM2_generate_key(EC_KEY * sm2);

int EC_SM2_check_key(EC_KEY * sm2);

int EC_SM2_check_key_ex(EC_KEY * sm2);

int EC_SM2_sign(EC_KEY * sm2, const char *id, const unsigned char *message, size_t msgLen, EC_SM2_SIG * sig, const EVP_MD * md);

int EC_SM2_verify(EC_KEY * sm2, const char *id, const unsigned char *message, size_t msgLen, const EC_SM2_SIG * sig, const EVP_MD * md);

int EC_SM2_encrypt(EC_KEY * sm2, const unsigned char *msg, size_t msgLen, unsigned char *oct, const EVP_MD * md);

int EC_SM2_decrypt(EC_KEY * sm2, const unsigned char *ct, size_t ctLen, unsigned char *pt, const EVP_MD * md);


#ifdef  __cplusplus
}
#endif

#endif
