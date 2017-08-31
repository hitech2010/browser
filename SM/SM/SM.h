#ifndef _SM_H_
#define _SM_H_

#include <openssl/evp.h>
#include <openssl/ec.h>
#include <openssl/engine.h>

// based on openssl 1.0.1
#if OPENSSL_VERSION_NUMBER < 0x10001000L
#error "Require OpenSSL 1.0.1 or higher"
#endif

#ifdef  __cplusplus
extern "C" {
#if 0
}
#endif
#endif

// const define

#define NID_BASE	920//958//1.0.2g //920 1.0.1

#define OBJ_sm		1L,2L,156L,10197L

#define SN_sm2		"SM2"
#define LN_sm2		"sm2"
#define NID_sm2		(NID_BASE + 0)//101972
#define OBJ_sm2		OBJ_sm,1L,301L
#define STRING_sm2	"1.2.156.10197.1.301"

#define EVP_PKEY_SM2	NID_sm2

#define SN_sm3		"SM3"
#define LN_sm3		"sm3"
#define NID_sm3		(NID_BASE + 1)//101973
#define OBJ_sm3		OBJ_sm,1L,401L
#define STRING_sm3	"1.2.156.10197.1.401"

#define SN_sm4		"SM4"
#define LN_sm4		"sm4"
#define NID_sm4		(NID_BASE + 2)//101974
#define OBJ_sm4		OBJ_sm,1L,104L
#define STRING_sm4	"1.2.156.10197.1.104"


#define SN_sm2p7_data "SM2P7DATA"
#define LN_sm2p7_data "sm2p7data"
#define NID_sm2p7_data (NID_BASE + 3)//101975
#define OBJ_sm2p7_data OBJ_sm,6L,1L,4L,2L,1L
#define STRING_sm2p7_data	"1.2.156.10197.6.1.4.2.1"

#define SN_sm2p7_signed "SM2P7SIGNED"
#define LN_sm2p7_signed "sm2p7signed"
#define NID_sm2p7_signed (NID_BASE + 4)//101976
#define OBJ_sm2p7_signed OBJ_sm,6L,1L,4L,2L,2L
#define STRING_sm2p7_signed	"1.2.156.10197.6.1.4.2.2"

#define SN_sm2p7_enveloped "SM2P7ENVDATA"
#define LN_sm2p7_enveloped "sm2p7envdata"
#define NID_sm2p7_enveloped (NID_BASE + 5)//101977
#define OBJ_sm2p7_enveloped OBJ_sm,6L,1L,4L,2L,3L
#define STRING_sm2p7_enveloped	"1.2.156.10197.6.1.4.2.3"

#define SN_sm2p7_signedAndEnveloped "SM2P7SIGNENVDATA"
#define LN_sm2p7_signedAndEnveloped "sm2p7signenvdata"
#define NID_sm2p7_signedAndEnveloped (NID_BASE + 6)//101978
#define OBJ_sm2p7_signedAndEnveloped OBJ_sm,6L,1L,4L,2L,4L
#define STRING_sm2p7_signedAndEnveloped	"1.2.156.10197.6.1.4.2.4"

#define SN_sm2p7_digest "SM2P7DIGEST"
#define LN_sm2p7_digest "sm2p7digest"
#define NID_sm2p7_digest (NID_BASE + 7)//101979
#define OBJ_sm2p7_digest OBJ_sm,6L,1L,4L,2L,5L
#define STRING_sm2p7_digest	"1.2.156.10197.6.1.4.2.5"

#define SN_sm2p7_encrypted "SM2P7ENCRYPTED"
#define LN_sm2p7_encrypted "sm2p7encrypted"
#define NID_sm2p7_encrypted (NID_BASE + 8)//101980
#define OBJ_sm2p7_encrypted OBJ_sm,6L,1L,4L,2L,6L
#define STRING_sm2p7_encrypted	"1.2.156.10197.6.1.4.2.6"

#define SN_sm2_3 "SM2-3"
#define LN_sm2_3 "sm2-3"
#define NID_sm2_3 (NID_BASE + 9)//101981
#define OBJ_sm2_3		OBJ_sm,1L,301L,3L
#define STRING_sm2_3	"1.2.156.10197.1.301.3"

struct evp_pkey_asn1_method_st
{
	int pkey_id;
	int pkey_base_id;
	unsigned long pkey_flags;

	char *pem_str;
	char *info;

	int(*pub_decode)(EVP_PKEY *pk, X509_PUBKEY *pub);
	int(*pub_encode)(X509_PUBKEY *pub, const EVP_PKEY *pk);
	int(*pub_cmp)(const EVP_PKEY *a, const EVP_PKEY *b);
	int(*pub_print)(BIO *out, const EVP_PKEY *pkey, int indent,
		ASN1_PCTX *pctx);

	int(*priv_decode)(EVP_PKEY *pk, PKCS8_PRIV_KEY_INFO *p8inf);
	int(*priv_encode)(PKCS8_PRIV_KEY_INFO *p8, const EVP_PKEY *pk);
	int(*priv_print)(BIO *out, const EVP_PKEY *pkey, int indent,
		ASN1_PCTX *pctx);

	int(*pkey_size)(const EVP_PKEY *pk);
	int(*pkey_bits)(const EVP_PKEY *pk);

	int(*param_decode)(EVP_PKEY *pkey,
		const unsigned char **pder, int derlen);
	int(*param_encode)(const EVP_PKEY *pkey, unsigned char **pder);
	int(*param_missing)(const EVP_PKEY *pk);
	int(*param_copy)(EVP_PKEY *to, const EVP_PKEY *from);
	int(*param_cmp)(const EVP_PKEY *a, const EVP_PKEY *b);
	int(*param_print)(BIO *out, const EVP_PKEY *pkey, int indent,
		ASN1_PCTX *pctx);
	int(*sig_print)(BIO *out,
		const X509_ALGOR *sigalg, const ASN1_STRING *sig,
		int indent, ASN1_PCTX *pctx);


	void(*pkey_free)(EVP_PKEY *pkey);
	int(*pkey_ctrl)(EVP_PKEY *pkey, int op, long arg1, void *arg2);

	/* Legacy functions for old PEM */

	int(*old_priv_decode)(EVP_PKEY *pkey,
		const unsigned char **pder, int derlen);
	int(*old_priv_encode)(const EVP_PKEY *pkey, unsigned char **pder);
	/* Custom ASN1 signature verification */
	int(*item_verify)(EVP_MD_CTX *ctx, const ASN1_ITEM *it, void *asn,
		X509_ALGOR *a, ASN1_BIT_STRING *sig,
		EVP_PKEY *pkey);
	int(*item_sign)(EVP_MD_CTX *ctx, const ASN1_ITEM *it, void *asn,
		X509_ALGOR *alg1, X509_ALGOR *alg2,
		ASN1_BIT_STRING *sig);

} /* EVP_PKEY_ASN1_METHOD */;

// ASN1 Structure
typedef struct
{
	BIGNUM *r;
	BIGNUM *s;
} EC_SM2_SIG;

EC_SM2_SIG * EC_SM2_SIG_new();
void EC_SM2_SIG_free(EC_SM2_SIG * p);
DECLARE_ASN1_ENCODE_FUNCTIONS_const(EC_SM2_SIG, EC_SM2_SIG);


int EC_SM2_HashIdMessage_PreData(X509* cert,
									const char *id,
									const unsigned char *message,
									size_t msgLen,
									const EVP_MD * md,
									unsigned char* puchin,
									int inlen);

// SM2 EVP
// 将标准EC key转换为SM2 key
EC_KEY * EC_KEY_SM2_init(EC_KEY * sm2);

int EC_SM2_generate_key(EC_KEY * sm2);

// 从X509创建一个SM2 key
EC_KEY * EC_KEY_from_X509(X509 * x509);
// 从公私钥创建一个SM2 key
EC_KEY * EC_KEY_from_BN(BIGNUM * x, BIGNUM * y, BIGNUM * priv_key);

EC_KEY * EC_KEY_from_BN_EX(BIGNUM * priv_key);
// 返回是否是SM2 key
int EC_KEY_is_sm2(EC_KEY * ec);
// sm2不需要释放
EVP_PKEY * EVP_PKEY_SM2_init(EC_KEY * sm2);

void EVP_PKEY_CTX_set_md(EVP_PKEY_CTX * ctx, const EVP_MD * md);
void EVP_PKEY_CTX_set_id(EVP_PKEY_CTX * ctx, const char * id);

// 清楚SM2算法占用内存
void EVP_sm2_cleanup(void);

// SM3 EVP
const EVP_MD * EVP_sm3();

// SM4 EVP
const EVP_CIPHER * EVP_sm4_cbc();
const EVP_CIPHER * EVP_sm4_ecb();
const EVP_CIPHER * EVP_sm4_cfb();
const EVP_CIPHER * EVP_sm4_ofb();


#ifdef  __cplusplus
}
#endif

#endif // _SM_H_
