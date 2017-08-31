#pragma once
#include <openssl/asn1.h>
#include <openssl/ossl_typ.h>

#include "SM.h"

/*
#define OBJ_sm		1L,2L,156L,10197L

#define SN_sm2		"SM2"
#define LN_sm2		"sm2"
#define NID_sm2		893//101972
#define OBJ_sm2		OBJ_sm,1L,301L
#define STRING_sm2	"1.2.156.10197.1.301"

#define SN_sm3		"SM3"
#define LN_sm3		"sm3"
#define NID_sm3		894//101973
#define OBJ_sm3		OBJ_sm,1L,401L
#define STRING_sm3	"1.2.156.10197.1.401"

#define SN_sm4		"SM4"
#define LN_sm4		"sm4"
#define NID_sm4		895//101974
#define OBJ_sm4		OBJ_sm,1L,104L
#define STRING_sm4	"1.2.156.10197.1.104"


#define SN_sm2p7_data "SM2P7DATA"
#define LN_sm2p7_data "sm2p7data"
#define NID_sm2p7_data 896//101975
#define OBJ_sm2p7_data OBJ_sm,6L,1L,4L,2L,1L
#define STRING_sm2p7_data	"1.2.156.10197.6.1.4.2.1"

#define SN_sm2p7_signed "SM2P7SIGNED"
#define LN_sm2p7_signed "sm2p7signed"
#define NID_sm2p7_signed 897//101976
#define OBJ_sm2p7_signed OBJ_sm,6L,1L,4L,2L,2L
#define STRING_sm2p7_signed	"1.2.156.10197.6.1.4.2.2"

#define SN_sm2p7_enveloped "SM2P7ENVDATA"
#define LN_sm2p7_enveloped "sm2p7envdata"
#define NID_sm2p7_enveloped 898//101977
#define OBJ_sm2p7_enveloped OBJ_sm,6L,1L,4L,2L,3L
#define STRING_sm2p7_enveloped	"1.2.156.10197.6.1.4.2.3"

#define SN_sm2p7_signedAndEnveloped "SM2P7SIGNENVDATA"
#define LN_sm2p7_signedAndEnveloped "sm2p7signenvdata"
#define NID_sm2p7_signedAndEnveloped 899//101978
#define OBJ_sm2p7_signedAndEnveloped OBJ_sm,6L,1L,4L,2L,4L
#define STRING_sm2p7_signedAndEnveloped	"1.2.156.10197.6.1.4.2.4"

#define SN_sm2p7_digest "SM2P7DIGEST"
#define LN_sm2p7_digest "sm2p7digest"
#define NID_sm2p7_digest 900//101979
#define OBJ_sm2p7_digest OBJ_sm,6L,1L,4L,2L,5L
#define STRING_sm2p7_digest	"1.2.156.10197.6.1.4.2.5"

#define SN_sm2p7_encrypted "SM2P7ENCRYPTED"
#define LN_sm2p7_encrypted "sm2p7encrypted"
#define NID_sm2p7_encrypted 901//101980
#define OBJ_sm2p7_encrypted OBJ_sm,6L,1L,4L,2L,6L
#define STRING_sm2p7_encrypted	"1.2.156.10197.6.1.4.2.6"

*/

#define SM2_MAX_XCOORDINATE_BITS_LEN 512
#define SM2_MAX_YCOORDINATE_BITS_LEN 512
#define SM2_MAX_MODULUS_BITS_LEN 512
#define ALG_TYPE_SM2 (15 << 9)
#define ALG_SID_SM2_ANY 0
#define ALG_SID_SM3 15
#define CALG_SM2_SIGN (ALG_CLASS_SIGNATURE | ALG_TYPE_SM2 | ALG_SID_SM2_ANY)
#define CALG_SM2_KEYX (ALG_CLASS_KEY_EXCHANGE | ALG_TYPE_SM2 | ALG_SID_SM2_ANY)
#define CALG_SM3 (ALG_CLASS_HASH| ALG_TYPE_ANY | ALG_SID_SM3)


typedef struct sm2_issuer_and_serial_st
{
	X509_NAME *issuer;
	ASN1_INTEGER *serial;
} SM2_ISSUER_AND_SERIAL;

typedef struct sm2_signer_info_st
{
	ASN1_INTEGER 			*version;	/* version 1 */
	SM2_ISSUER_AND_SERIAL		*issuer_and_serial;
	X509_ALGOR			*digest_alg;
	STACK_OF(X509_ATTRIBUTE)	*auth_attr;	/* [ 0 ] */
	X509_ALGOR			*digest_enc_alg;
	ASN1_OCTET_STRING		*enc_digest;
	STACK_OF(X509_ATTRIBUTE)	*unauth_attr;	/* [ 1 ] */

	/* The private key to sign with */
	EVP_PKEY			*pkey;
} SM2_SIGNER_INFO;

DECLARE_STACK_OF(SM2_SIGNER_INFO)
DECLARE_ASN1_SET_OF(SM2_SIGNER_INFO)

typedef struct sm2_recip_info_st
{
	ASN1_INTEGER			*version;	/* version 0 */
	SM2_ISSUER_AND_SERIAL		*issuer_and_serial;
	X509_ALGOR			*key_enc_algor;
	ASN1_OCTET_STRING		*enc_key;
	X509				*cert; /* get the pub-key from this */
} SM2_RECIP_INFO;

DECLARE_STACK_OF(SM2_RECIP_INFO)
DECLARE_ASN1_SET_OF(SM2_RECIP_INFO)

typedef struct sm2_signed_st
{
	ASN1_INTEGER			*version;	/* version 1 */
	STACK_OF(X509_ALGOR)		*md_algs;	/* md used */
	STACK_OF(X509)			*cert;		/* [ 0 ] */
	STACK_OF(X509_CRL)		*crl;		/* [ 1 ] */
	STACK_OF(SM2_SIGNER_INFO)	*signer_info;

	struct sm2_p7_st			*contents;
} SM2_SIGNED;
/* The above structure is very very similar to PKCS7_SIGN_ENVELOPE.
* How about merging the two */

typedef struct sm2_enc_content_st
{
	ASN1_OBJECT			*content_type;
	X509_ALGOR			*algorithm;
	ASN1_OCTET_STRING		*enc_data;	/* [ 0 ] */
	const EVP_CIPHER		*cipher;
} SM2_ENC_CONTENT;

typedef struct sm2_enveloped_st
{
	ASN1_INTEGER			*version;	/* version 0 */
	STACK_OF(SM2_RECIP_INFO)	*recipientinfo;
	SM2_ENC_CONTENT		*enc_data;
} SM2_ENVELOPE;

typedef struct sm2_signedandenveloped_st
{
	ASN1_INTEGER			*version;	/* version 1 */
	STACK_OF(X509_ALGOR)		*md_algs;	/* md used */
	STACK_OF(X509)			*cert;		/* [ 0 ] */
	STACK_OF(X509_CRL)		*crl;		/* [ 1 ] */
	STACK_OF(SM2_SIGNER_INFO)	*signer_info;

	SM2_ENC_CONTENT		*enc_data;
	STACK_OF(SM2_RECIP_INFO)	*recipientinfo;
} SM2_SIGN_ENVELOPE;

typedef struct sm2_digest_st
{
	ASN1_INTEGER			*version;	/* version 0 */
	X509_ALGOR			*md;		/* md used */
	struct sm2_st 		*contents;
	ASN1_OCTET_STRING		*digest;
} SM2_DIGEST;

typedef struct sm2_encrypted_st
{
	ASN1_INTEGER			*version;	/* version 0 */
	SM2_ENC_CONTENT		*enc_data;
} SM2_ENCRYPT;

typedef struct sm2_p7_st
{
	/* The following is non NULL if it contains ASN1 encoding of
	* this structure */
	unsigned char *asn1;
	long length;

#define SM2_S_HEADER	0
#define SM2_S_BODY	1
#define SM2_S_TAIL	2
	int state; /* used during processing */

	int detached;

	ASN1_OBJECT *type;
	/* content as defined by the type */
	/* all encryption/message digests are applied to the 'contents',
	* leaving out the 'type' field. */
	union	{
		char *ptr;

		/* NID_pkcs7_data */
		ASN1_OCTET_STRING *data;

		/* NID_pkcs7_signed */
		SM2_SIGNED *sign;

		/* NID_pkcs7_enveloped */
		SM2_ENVELOPE *enveloped;

		/* NID_pkcs7_signedAndEnveloped */
		SM2_SIGN_ENVELOPE *signed_and_enveloped;

		/* NID_pkcs7_digest */
		SM2_DIGEST *digest;

		/* NID_pkcs7_encrypted */
		SM2_ENCRYPT *encrypted;

		/* Anything else */
		ASN1_TYPE *other;
	} d;
} SM2_P7;

DECLARE_STACK_OF(SM2_P7)
DECLARE_ASN1_SET_OF(SM2_P7)
DECLARE_PKCS12_STACK_OF(SM2_P7)


#define SM2_P7_OP_SET_DETACHED_SIGNATURE	1
#define SM2_P7_OP_GET_DETACHED_SIGNATURE	2

#define SM2_P7_get_signed_attributes(si)	((si)->auth_attr)
#define SM2_P7_get_attributes(si)	((si)->unauth_attr)

#define SM2_P7_type_is_signed(a) (OBJ_obj2nid((a)->type) == NID_sm2p7_signed)
#define SM2_P7_type_is_encrypted(a) (OBJ_obj2nid((a)->type) == NID_sm2p7_encrypted)
#define SM2_P7_type_is_enveloped(a) (OBJ_obj2nid((a)->type) == NID_sm2p7_enveloped)
#define SM2_P7_type_is_signedAndEnveloped(a) \
	(OBJ_obj2nid((a)->type) == NID_sm2p7_signedAndEnveloped)
#define SM2_P7_type_is_data(a)   (OBJ_obj2nid((a)->type) == NID_sm2p7_data)
#define SM2_P7_type_is_digest(a)   (OBJ_obj2nid((a)->type) == NID_sm2p7_digest)
#define SM2_P7_type_is_encrypted(a) \
	(OBJ_obj2nid((a)->type) == NID_sm2p7_encrypted)

#define SM2_P7_type_is_digest(a)   (OBJ_obj2nid((a)->type) == NID_sm2p7_digest)

#define SM2_P7_set_detached(p,v) \
	SM2_P7_ctrl(p, SM2_P7_OP_SET_DETACHED_SIGNATURE, v, NULL)
#define SM2_P7_get_detached(p) \
	SM2_P7_ctrl(p, SM2_P7_OP_GET_DETACHED_SIGNATURE, 0, NULL)

#define SM2_P7_is_detached(p7) (SM2_P7_type_is_signed(p7) && SM2_P7_get_detached(p7))

DECLARE_ASN1_FUNCTIONS(SM2_ISSUER_AND_SERIAL)

DECLARE_ASN1_FUNCTIONS(SM2_SIGNER_INFO)
DECLARE_ASN1_FUNCTIONS(SM2_RECIP_INFO)
DECLARE_ASN1_FUNCTIONS(SM2_SIGNED)
DECLARE_ASN1_FUNCTIONS(SM2_ENC_CONTENT)
DECLARE_ASN1_FUNCTIONS(SM2_ENVELOPE)
DECLARE_ASN1_FUNCTIONS(SM2_SIGN_ENVELOPE)
DECLARE_ASN1_FUNCTIONS(SM2_DIGEST)
DECLARE_ASN1_FUNCTIONS(SM2_ENCRYPT)
DECLARE_ASN1_FUNCTIONS(SM2_P7)

DECLARE_ASN1_ITEM(SM2_ATTR_SIGN)
DECLARE_ASN1_ITEM(SM2_ATTR_VERIFY)

DECLARE_ASN1_NDEF_FUNCTION(SM2_P7)
DECLARE_ASN1_PRINT_FUNCTION(SM2_P7)


long SM2_P7_ctrl(SM2_P7 *p7, int cmd, long larg, char *parg);

int SM2_P7_set_type(SM2_P7 *p7, int type);
int SM2_P7_set0_type_other(SM2_P7 *p7, int type, ASN1_TYPE *other);
int SM2_P7_set_content(SM2_P7 *p7, SM2_P7 *p7_data);

int SM2_P7_add_signer(SM2_P7 *p7, SM2_SIGNER_INFO *p7i);
int SM2_P7_add_certificate(SM2_P7 *p7, X509 *x509);
int SM2_P7_add_crl(SM2_P7 *p7, X509_CRL *x509);
int SM2_P7_content_new(SM2_P7 *p7, int nid);

ASN1_OBJECT *	SM2_OBJ_nid2obj(int n);
int SM2_OBJ_obj2nid(const ASN1_OBJECT *a);

void initsm2internalnid();


STACK_OF(SM2_SIGNER_INFO) *SM2_P7_get_signer_info(SM2_P7 *p7);
int SM2_P7_signatureVerify(SM2_P7 *p7, SM2_SIGNER_INFO *si, X509 *x509, unsigned char*msg, int msglen);
ASN1_OCTET_STRING *SM2_P7_digest_from_attributes(STACK_OF(X509_ATTRIBUTE) *sk);
int SM2Verify(X509* x, unsigned char* puchsig, int isiglen, unsigned char* puchmsg, int imsglen);

#define sk_SM2_SIGNER_INFO_push(st, val) SKM_sk_push(SM2_SIGNER_INFO, (st), (val))
#define sk_SM2_SIGNER_INFO_num(st) SKM_sk_num(SM2_SIGNER_INFO, (st))
#define sk_SM2_SIGNER_INFO_value(st, i) SKM_sk_value(SM2_SIGNER_INFO, (st), (i))
#define sk_SM2_RECIP_INFO_num(st) SKM_sk_num(PKCS7_RECIP_INFO, (st))
#define sk_SM2_RECIP_INFO_value(st, i) SKM_sk_value(PKCS7_RECIP_INFO, (st), (i))

//////////////////////////////////////////////////////////////////////
typedef struct sm2_cipher_struct
{
	BIGNUM *x;
	BIGNUM *y;
	ASN1_OCTET_STRING *hash;
	ASN1_OCTET_STRING *ciphertext;
} EC_SM2_CIPHER;

DECLARE_ASN1_FUNCTIONS_const(EC_SM2_CIPHER)
///////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////
//SM2 private key
typedef struct sm2_parameters_st {
	int	type;
	union {
		ASN1_OBJECT  *named_curve;
		ASN1_NULL    *implicitlyCA;
	} value;
}SM2PKPARAMETERS;

DECLARE_ASN1_FUNCTIONS_const(SM2PKPARAMETERS)
DECLARE_ASN1_ENCODE_FUNCTIONS_const(SM2PKPARAMETERS, SM2PKPARAMETERS)

typedef struct sm2_privatekey_st {
	int              version;
	ASN1_OCTET_STRING *privateKey;
	//ASN1_OBJECT		  *parameters;
	SM2PKPARAMETERS    *parameters;
	ASN1_BIT_STRING   *publicKey;
} SM2_PRIVATEKEY;

DECLARE_ASN1_FUNCTIONS_const(SM2_PRIVATEKEY)
DECLARE_ASN1_ENCODE_FUNCTIONS_const(SM2_PRIVATEKEY, SM2_PRIVATEKEY)


////////////////////////////////////////////////////////////////////////