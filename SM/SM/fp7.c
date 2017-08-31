#include "fp7.h"
#include <openssl/asn1t.h>
#include <openssl/obj_mac.h>
#include <openssl/x509.h>

static int internalsm2oid = -1;
static int internalsm3oid = -1;
static int internalsm4oid = -1;

static int internalsm2dataoid = -1;
static int internalsm2signedoid = -1;
static int internalsm2envoid = -1;
static int internalsm2signenvoid = -1;
static int internalsm2digestoid = -1;
static int internalsm2encoid = -1;

static int internalsm2_3 = -1;


/* PKCS#7 ASN1 module */
/* This is the ANY DEFINED BY table for the top level PKCS#7 structure */
ASN1_ADB_TEMPLATE(sm2p7default) = ASN1_EXP_OPT(SM2_P7, d.other, ASN1_ANY, 0);

ASN1_ADB(SM2_P7) = {
	ADB_ENTRY(NID_sm2p7_data, ASN1_NDEF_EXP_OPT(SM2_P7, d.data, ASN1_OCTET_STRING_NDEF, 0)),
	ADB_ENTRY(NID_sm2p7_signed, ASN1_NDEF_EXP_OPT(SM2_P7, d.sign, SM2_SIGNED, 0)),
	ADB_ENTRY(NID_sm2p7_enveloped, ASN1_NDEF_EXP_OPT(SM2_P7, d.enveloped, SM2_ENVELOPE, 0)),
	ADB_ENTRY(NID_sm2p7_signedAndEnveloped, ASN1_NDEF_EXP_OPT(SM2_P7, d.signed_and_enveloped, SM2_SIGN_ENVELOPE, 0)),
	ADB_ENTRY(NID_sm2p7_digest, ASN1_NDEF_EXP_OPT(SM2_P7, d.digest, SM2_DIGEST, 0)),
	ADB_ENTRY(NID_sm2p7_encrypted, ASN1_NDEF_EXP_OPT(SM2_P7, d.encrypted, SM2_ENCRYPT, 0))
} ASN1_ADB_END(SM2_P7, 0, type, 0, &sm2p7default_tt, NULL);

/* PKCS#7 streaming support */
static int sm2_cb(int operation, ASN1_VALUE **pval, const ASN1_ITEM *it,
	void *exarg)
{
	/*
	ASN1_STREAM_ARG *sarg = exarg;
	SM2 **pp7 = (SM2 **)pval;

	switch (operation)
	{

	case ASN1_OP_STREAM_PRE:
		if (SM2_stream(&sarg->boundary, *pp7) <= 0)
			return 0;
	case ASN1_OP_DETACHED_PRE:
		sarg->ndef_bio = SM2_dataInit(*pp7, sarg->out);
		if (!sarg->ndef_bio)
			return 0;
		break;

	case ASN1_OP_STREAM_POST:
	case ASN1_OP_DETACHED_POST:
		if (SM2_dataFinal(*pp7, sarg->ndef_bio) <= 0)
			return 0;
		break;

	}*/
	return 1;
}

ASN1_NDEF_SEQUENCE_cb(SM2_P7, NULL) = {
	ASN1_SIMPLE(SM2_P7, type, ASN1_OBJECT),
	ASN1_ADB_OBJECT(SM2_P7)
}ASN1_NDEF_SEQUENCE_END_cb(SM2_P7, SM2_P7)

IMPLEMENT_ASN1_FUNCTIONS(SM2_P7)
IMPLEMENT_ASN1_NDEF_FUNCTION(SM2_P7)
IMPLEMENT_ASN1_DUP_FUNCTION(SM2_P7)

ASN1_NDEF_SEQUENCE(SM2_SIGNED) = {
	ASN1_SIMPLE(SM2_SIGNED, version, ASN1_INTEGER),
	ASN1_SET_OF(SM2_SIGNED, md_algs, X509_ALGOR),
	ASN1_SIMPLE(SM2_SIGNED, contents, SM2_P7),
	ASN1_IMP_SEQUENCE_OF_OPT(SM2_SIGNED, cert, X509, 0),
	ASN1_IMP_SET_OF_OPT(SM2_SIGNED, crl, X509_CRL, 1),
	ASN1_SET_OF(SM2_SIGNED, signer_info, SM2_SIGNER_INFO)
} ASN1_NDEF_SEQUENCE_END(SM2_SIGNED)

IMPLEMENT_ASN1_FUNCTIONS(SM2_SIGNED)

/* Minor tweak to operation: free up EVP_PKEY */
static int si_cb(int operation, ASN1_VALUE **pval, const ASN1_ITEM *it,
void *exarg)
	{
	if (operation == ASN1_OP_FREE_POST) {
		SM2_SIGNER_INFO *si = (SM2_SIGNER_INFO *)*pval;
		EVP_PKEY_free(si->pkey);
	}
	return 1;
}

ASN1_SEQUENCE_cb(SM2_SIGNER_INFO, si_cb) = {
	ASN1_SIMPLE(SM2_SIGNER_INFO, version, ASN1_INTEGER),
	ASN1_SIMPLE(SM2_SIGNER_INFO, issuer_and_serial, SM2_ISSUER_AND_SERIAL),
	ASN1_SIMPLE(SM2_SIGNER_INFO, digest_alg, X509_ALGOR),
	/* NB this should be a SET OF but we use a SEQUENCE OF so the
	* original order * is retained when the structure is reencoded.
	* Since the attributes are implicitly tagged this will not affect
	* the encoding.
	*/
	ASN1_IMP_SEQUENCE_OF_OPT(SM2_SIGNER_INFO, auth_attr, X509_ATTRIBUTE, 0),
	ASN1_SIMPLE(SM2_SIGNER_INFO, digest_enc_alg, X509_ALGOR),
	ASN1_SIMPLE(SM2_SIGNER_INFO, enc_digest, ASN1_OCTET_STRING),
	ASN1_IMP_SET_OF_OPT(SM2_SIGNER_INFO, unauth_attr, X509_ATTRIBUTE, 1)
} ASN1_SEQUENCE_END_cb(SM2_SIGNER_INFO, SM2_SIGNER_INFO)

IMPLEMENT_ASN1_FUNCTIONS(SM2_SIGNER_INFO)

ASN1_SEQUENCE(SM2_ISSUER_AND_SERIAL) = {
	ASN1_SIMPLE(SM2_ISSUER_AND_SERIAL, issuer, X509_NAME),
	ASN1_SIMPLE(SM2_ISSUER_AND_SERIAL, serial, ASN1_INTEGER)
} ASN1_SEQUENCE_END(SM2_ISSUER_AND_SERIAL)

IMPLEMENT_ASN1_FUNCTIONS(SM2_ISSUER_AND_SERIAL)

ASN1_NDEF_SEQUENCE(SM2_ENVELOPE) = {
	ASN1_SIMPLE(SM2_ENVELOPE, version, ASN1_INTEGER),
	ASN1_SET_OF(SM2_ENVELOPE, recipientinfo, SM2_RECIP_INFO),
	ASN1_SIMPLE(SM2_ENVELOPE, enc_data, SM2_ENC_CONTENT)
} ASN1_NDEF_SEQUENCE_END(SM2_ENVELOPE)

IMPLEMENT_ASN1_FUNCTIONS(SM2_ENVELOPE)

/* Minor tweak to operation: free up X509 */
static int ri_cb(int operation, ASN1_VALUE **pval, const ASN1_ITEM *it,
void *exarg)
	{
	if (operation == ASN1_OP_FREE_POST) {
		SM2_RECIP_INFO *ri = (SM2_RECIP_INFO *)*pval;
		X509_free(ri->cert);
	}
	return 1;
}

ASN1_SEQUENCE_cb(SM2_RECIP_INFO, ri_cb) = {
	ASN1_SIMPLE(SM2_RECIP_INFO, version, ASN1_INTEGER),
	ASN1_SIMPLE(SM2_RECIP_INFO, issuer_and_serial, SM2_ISSUER_AND_SERIAL),
	ASN1_SIMPLE(SM2_RECIP_INFO, key_enc_algor, X509_ALGOR),
	ASN1_SIMPLE(SM2_RECIP_INFO, enc_key, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END_cb(SM2_RECIP_INFO, SM2_RECIP_INFO)

IMPLEMENT_ASN1_FUNCTIONS(SM2_RECIP_INFO)

ASN1_NDEF_SEQUENCE(SM2_ENC_CONTENT) = {
	ASN1_SIMPLE(SM2_ENC_CONTENT, content_type, ASN1_OBJECT),
	ASN1_SIMPLE(SM2_ENC_CONTENT, algorithm, X509_ALGOR),
	ASN1_IMP_OPT(SM2_ENC_CONTENT, enc_data, ASN1_OCTET_STRING_NDEF, 0)
} ASN1_NDEF_SEQUENCE_END(SM2_ENC_CONTENT)

IMPLEMENT_ASN1_FUNCTIONS(SM2_ENC_CONTENT)

ASN1_NDEF_SEQUENCE(SM2_SIGN_ENVELOPE) = {
	ASN1_SIMPLE(SM2_SIGN_ENVELOPE, version, ASN1_INTEGER),
	ASN1_SET_OF(SM2_SIGN_ENVELOPE, recipientinfo, SM2_RECIP_INFO),
	ASN1_SET_OF(SM2_SIGN_ENVELOPE, md_algs, X509_ALGOR),
	ASN1_SIMPLE(SM2_SIGN_ENVELOPE, enc_data, SM2_ENC_CONTENT),
	ASN1_IMP_SET_OF_OPT(SM2_SIGN_ENVELOPE, cert, X509, 0),
	ASN1_IMP_SET_OF_OPT(SM2_SIGN_ENVELOPE, crl, X509_CRL, 1),
	ASN1_SET_OF(SM2_SIGN_ENVELOPE, signer_info, SM2_SIGNER_INFO)
} ASN1_NDEF_SEQUENCE_END(SM2_SIGN_ENVELOPE)

IMPLEMENT_ASN1_FUNCTIONS(SM2_SIGN_ENVELOPE)

ASN1_NDEF_SEQUENCE(SM2_ENCRYPT) = {
	ASN1_SIMPLE(SM2_ENCRYPT, version, ASN1_INTEGER),
	ASN1_SIMPLE(SM2_ENCRYPT, enc_data, SM2_ENC_CONTENT)
} ASN1_NDEF_SEQUENCE_END(SM2_ENCRYPT)

IMPLEMENT_ASN1_FUNCTIONS(SM2_ENCRYPT)

ASN1_NDEF_SEQUENCE(SM2_DIGEST) = {
	ASN1_SIMPLE(SM2_DIGEST, version, ASN1_INTEGER),
	ASN1_SIMPLE(SM2_DIGEST, md, X509_ALGOR),
	ASN1_SIMPLE(SM2_DIGEST, contents, SM2_P7),
	ASN1_SIMPLE(SM2_DIGEST, digest, ASN1_OCTET_STRING)
} ASN1_NDEF_SEQUENCE_END(SM2_DIGEST)

IMPLEMENT_ASN1_FUNCTIONS(SM2_DIGEST)

/* Specials for authenticated attributes */

/* When signing attributes we want to reorder them to match the sorted
* encoding.
*/

ASN1_ITEM_TEMPLATE(SM2_ATTR_SIGN) =
ASN1_EX_TEMPLATE_TYPE(ASN1_TFLG_SET_ORDER, 0, SM2_ATTRIBUTES, X509_ATTRIBUTE)
ASN1_ITEM_TEMPLATE_END(SM2_ATTR_SIGN)

/* When verifying attributes we need to use the received order. So
* we use SEQUENCE OF and tag it to SET OF
*/

ASN1_ITEM_TEMPLATE(SM2_ATTR_VERIFY) =
ASN1_EX_TEMPLATE_TYPE(ASN1_TFLG_SEQUENCE_OF | ASN1_TFLG_IMPTAG | ASN1_TFLG_UNIVERSAL,
V_ASN1_SET, SM2_ATTRIBUTES, X509_ATTRIBUTE)
ASN1_ITEM_TEMPLATE_END(SM2_ATTR_VERIFY)

IMPLEMENT_ASN1_PRINT_FUNCTION(SM2_P7)


/////////////////////////////////////////////////////////////////////////////////////
ASN1_SEQUENCE(EC_SM2_CIPHER) = {
	ASN1_SIMPLE(EC_SM2_CIPHER, x, CBIGNUM),
	ASN1_SIMPLE(EC_SM2_CIPHER, y, CBIGNUM),
	ASN1_SIMPLE(EC_SM2_CIPHER, hash, ASN1_OCTET_STRING),
	ASN1_SIMPLE(EC_SM2_CIPHER, ciphertext, ASN1_OCTET_STRING)
} ASN1_SEQUENCE_END(EC_SM2_CIPHER)


IMPLEMENT_ASN1_FUNCTIONS_const(EC_SM2_CIPHER)
/////////////////////////////////////////////////////////////////////////////////////

ASN1_CHOICE(SM2PKPARAMETERS) = {
	ASN1_SIMPLE(SM2PKPARAMETERS, value.named_curve, ASN1_OBJECT),
	ASN1_SIMPLE(SM2PKPARAMETERS, value.implicitlyCA, ASN1_NULL)
} ASN1_CHOICE_END(SM2PKPARAMETERS)

DECLARE_ASN1_FUNCTIONS_const(SM2PKPARAMETERS)
DECLARE_ASN1_ENCODE_FUNCTIONS_const(SM2PKPARAMETERS, SM2PKPARAMETERS)
IMPLEMENT_ASN1_FUNCTIONS_const(SM2PKPARAMETERS)

ASN1_SEQUENCE(SM2_PRIVATEKEY) = {
	ASN1_SIMPLE(SM2_PRIVATEKEY, version, LONG),
	ASN1_SIMPLE(SM2_PRIVATEKEY, privateKey, ASN1_OCTET_STRING),
	ASN1_EXP_OPT(SM2_PRIVATEKEY, parameters, SM2PKPARAMETERS, 0),
	//ASN1_EXP_OPT(SM2_PRIVATEKEY, parameters, ASN1_OBJECT, 0),
	ASN1_EXP_OPT(SM2_PRIVATEKEY, publicKey, ASN1_BIT_STRING, 1)
} ASN1_SEQUENCE_END(SM2_PRIVATEKEY)

DECLARE_ASN1_FUNCTIONS_const(SM2_PRIVATEKEY)
DECLARE_ASN1_ENCODE_FUNCTIONS_const(SM2_PRIVATEKEY, SM2_PRIVATEKEY)
IMPLEMENT_ASN1_FUNCTIONS_const(SM2_PRIVATEKEY)


/////////////////////////////////////////////////////////////////////////////////////

int translateinternalnid(int iid)
{
	int iret = -1;

	if (iid == internalsm2oid)
	{
		iret = NID_sm2;
	}
	else if (iid == internalsm3oid)
	{
		iret = NID_sm3;
	}
	else if (iid == internalsm4oid)
	{
		iret = NID_sm4;
	}
	else if (iid == internalsm2dataoid)
	{
		iret = NID_sm2p7_data;
	}
	else if (iid == internalsm2signedoid)
	{
		iret = NID_sm2p7_signed;
	}
	else if (iid == internalsm2envoid)
	{
		iret = NID_sm2p7_enveloped;
	}
	else if (iid == internalsm2signenvoid)
	{
		iret = NID_sm2p7_signedAndEnveloped;
	}
	else if (iid == internalsm2digestoid)
	{
		iret = NID_sm2p7_digest;
	}
	else if (iid == internalsm2encoid)
	{
		iret = NID_sm2p7_encrypted;
	}
	//internalsm2_3
	else if (iid == internalsm2_3)
	{
		iret = NID_sm2_3;
	}

	return iret;
}


long SM2_P7_ctrl(SM2_P7 *p7, int cmd, long larg, char *parg)
{
	int nid;
	long ret = 0;

	int nidouter = translateinternalnid(p7->type->nid);
	nid = p7->type->nid;

	switch (cmd)
	{
	case SM2_P7_OP_SET_DETACHED_SIGNATURE:
		if (nidouter == NID_sm2p7_signed)
		{
			ret = p7->detached = (int)larg;
			if (ret && SM2_P7_type_is_data(p7->d.sign->contents))
			{
				ASN1_OCTET_STRING *os;
				os = p7->d.sign->contents->d.data;
				ASN1_OCTET_STRING_free(os);
				p7->d.sign->contents->d.data = NULL;
			}
		}
		break;
	case SM2_P7_OP_GET_DETACHED_SIGNATURE:
		if (nidouter == NID_sm2p7_signed)
		{
			if (!p7->d.sign || !p7->d.sign->contents->d.ptr)
				ret = 1;
			else ret = 0;

			p7->detached = ret;
		}
		break;
	}

	return(ret);
}

int SM2_P7_content_new(SM2_P7 *p7, int type)
{
	SM2_P7 *ret = NULL;

	if ((ret = SM2_P7_new()) == NULL) goto err;
	if (!SM2_P7_set_type(ret, type)) goto err;
	if (!SM2_P7_set_content(p7, ret)) goto err;

	return(1);
err:
	if (ret != NULL) SM2_P7_free(ret);

	return(0);
}


int SM2_P7_set_content(SM2_P7 *p7, SM2_P7 *p7_data)
{
	int i;
	int nidouter = translateinternalnid(p7->type->nid);
	i = p7->type->nid;

	switch (nidouter)
	{
	case NID_sm2p7_signed:
		if (p7->d.sign->contents != NULL)
			SM2_P7_free(p7->d.sign->contents);
		p7->d.sign->contents = p7_data;
		break;
	case NID_sm2p7_data:
	default:
		goto err;
	}
	return(1);

err:
	return(0);
}

int SM2_P7_set_type(SM2_P7 *p7, int type)
{
	ASN1_OBJECT *obj;

	obj = SM2_OBJ_nid2obj(type);

	switch (type)
	{
	case NID_sm2p7_signed:
		p7->type = obj;
		if ((p7->d.sign = SM2_SIGNED_new()) == NULL)
			goto err;
		if (!ASN1_INTEGER_set(p7->d.sign->version, 1))
		{
			SM2_SIGNED_free(p7->d.sign);
			p7->d.sign = NULL;
			goto err;
		}
		break;
	case NID_sm2p7_data:
		p7->type = obj;
		if ((p7->d.data = M_ASN1_OCTET_STRING_new()) == NULL)
			goto err;
		break;
	default:
	case NID_sm2p7_enveloped:
		p7->type = obj;
		if ((p7->d.enveloped = SM2_ENVELOPE_new()) == NULL)
			goto err;
		break;
		goto err;
	}
	return(1);
err:
	return(0);
}

int SM2_P7_set0_type_other(SM2_P7 *p7, int type, ASN1_TYPE *other)
{
	p7->type = SM2_OBJ_nid2obj(type);
	p7->d.other = other;
	return 1;
}

int SM2_P7_add_signer(SM2_P7 *p7, SM2_SIGNER_INFO *psi)
{
	int i, j, nid, nidouter;
	X509_ALGOR *alg;
	STACK_OF(SM2_SIGNER_INFO) *signer_sk;
	STACK_OF(X509_ALGOR) *md_sk;

	nidouter = translateinternalnid(p7->type->nid);
	i = p7->type->nid;

	switch (nidouter)
	{
	case NID_sm2p7_signed:
		signer_sk = p7->d.sign->signer_info;
		md_sk = p7->d.sign->md_algs;
		break;
	default:
		return(0);
	}

	nid = OBJ_obj2nid((ASN1_OBJECT *)(psi->digest_alg->algorithm));


	j = 0;
	for (i = 0; i<sk_X509_ALGOR_num(md_sk); i++)
	{
		alg = sk_X509_ALGOR_value(md_sk, i);
		if (OBJ_obj2nid(alg->algorithm) == nid)
		{
			j = 1;
			break;
		}
	}
	if (!j)
	{
		if (!(alg = X509_ALGOR_new())
			|| !(alg->parameter = ASN1_TYPE_new()))
		{
			X509_ALGOR_free(alg);

			return(0);
		}
		alg->algorithm = SM2_OBJ_nid2obj(nid);
		alg->parameter->type = V_ASN1_NULL;
		if (!sk_X509_ALGOR_push(md_sk, alg))
		{
			X509_ALGOR_free(alg);
			return 0;
		}
	}

	if (!sk_SM2_SIGNER_INFO_push(signer_sk, psi))
		return 0;

	return(1);
}

int SM2_P7_add_certificate(SM2_P7 *p7, X509 *x509)
{
	int i, nidouter;
	STACK_OF(X509) **sk;
	nidouter = translateinternalnid(p7->type->nid);
	i = p7->type->nid;

	switch (nidouter)
	{
	case NID_sm2p7_signed:
		sk = &(p7->d.sign->cert);
		break;
	default:
		return(0);
	}

	if (*sk == NULL)
		*sk = sk_X509_new_null();
	if (*sk == NULL)
	{
		return 0;
	}

	CRYPTO_add(&x509->references, 1, CRYPTO_LOCK_X509);

	if (!sk_X509_push(*sk, x509))
	{
		X509_free(x509);
		return 0;
	}

	return(1);
}

int SM2_P7_add_crl(SM2_P7 *p7, X509_CRL *crl)
{
	int i, nidouter;
	STACK_OF(X509_CRL) **sk;
	nidouter = translateinternalnid(p7->type->nid);
	i = p7->type->nid;

	switch (nidouter)
	{
	case NID_sm2p7_signed:
		sk = &(p7->d.sign->crl);
		break;
	default:
		return(0);
	}

	if (*sk == NULL)
		*sk = sk_X509_CRL_new_null();
	if (*sk == NULL)
	{
		return 0;
	}

	CRYPTO_add(&crl->references, 1, CRYPTO_LOCK_X509_CRL);

	if (!sk_X509_CRL_push(*sk, crl))
	{
		X509_CRL_free(crl);
		return 0;
	}

	return(1);
}

ASN1_OBJECT *	SM2_OBJ_nid2obj(int n)
{
	ASN1_OBJECT *	retoid = OBJ_nid2obj(n);

	if (retoid == NULL)
	{
		unsigned char bufoid0[1024] = { 0 };
		
		int ilen = 0;
		switch (n)
		{
		case NID_sm2:
			if (internalsm2oid == -1)
			{
				internalsm2oid = OBJ_create(STRING_sm2, SN_sm2, LN_sm2);
			}
			retoid = OBJ_nid2obj(internalsm2oid);
			break;
		case NID_sm3:
			if (internalsm3oid == -1)
			{
				internalsm3oid = OBJ_create(STRING_sm3, SN_sm3, LN_sm3);
			}
			retoid = OBJ_nid2obj(internalsm3oid);
			break;
		case NID_sm4:
			if (internalsm4oid == -1)
			{
				internalsm4oid = OBJ_create(STRING_sm4, SN_sm4, LN_sm4);
			}
			retoid = OBJ_nid2obj(internalsm4oid);
			break;
		case NID_sm2p7_data:
			if (internalsm2dataoid == -1)
			{
				internalsm2dataoid = OBJ_create(STRING_sm2p7_data, SN_sm2p7_data, LN_sm2p7_data);
			}
			retoid = OBJ_nid2obj(internalsm2dataoid);
			break;
		case NID_sm2p7_signed:
			if (internalsm2signedoid == -1)
			{
				internalsm2signedoid = OBJ_create(STRING_sm2p7_signed, SN_sm2p7_signed, LN_sm2p7_signed);
			}
			retoid = OBJ_nid2obj(internalsm2signedoid);
			break;
		case NID_sm2p7_enveloped:
			if (internalsm2envoid == -1)
			{
				internalsm2envoid = OBJ_create(STRING_sm2p7_enveloped, SN_sm2p7_enveloped, LN_sm2p7_enveloped);
			}
			retoid = OBJ_nid2obj(internalsm2envoid);
			break;
		case NID_sm2p7_signedAndEnveloped:
			if (internalsm2signenvoid == -1)
			{
				internalsm2signenvoid = OBJ_create(STRING_sm2p7_signedAndEnveloped, SN_sm2p7_signedAndEnveloped, LN_sm2p7_signedAndEnveloped);
			}
			retoid = OBJ_nid2obj(internalsm2signenvoid);
			break;
		case NID_sm2p7_digest:
			if (internalsm2digestoid == -1)
			{
				internalsm2digestoid = OBJ_create(STRING_sm2p7_digest, SN_sm2p7_digest, LN_sm2p7_digest);
			}
			retoid = OBJ_nid2obj(internalsm2digestoid);
			break;
		case NID_sm2p7_encrypted:
			if (internalsm2encoid == -1)
			{
				internalsm2encoid = OBJ_create(STRING_sm2p7_encrypted, SN_sm2p7_encrypted, LN_sm2p7_encrypted);
			}
			retoid = OBJ_nid2obj(internalsm2encoid);
			break;
		case NID_sm2_3:
			if (internalsm2encoid == -1)
			{
				internalsm2encoid = OBJ_create(STRING_sm2_3, SN_sm2_3, LN_sm2_3);
			}
			retoid = OBJ_nid2obj(internalsm2encoid);
			break;
		}
	}

	return retoid;
}

void initsm2internalnid()
{
	static int bflag = 0;

	if (0 == bflag)
	{
		bflag = 1;

		internalsm2oid = OBJ_create(STRING_sm2, SN_sm2, LN_sm2);
		internalsm3oid = OBJ_create(STRING_sm3, SN_sm3, LN_sm3);
		internalsm4oid = OBJ_create(STRING_sm4, SN_sm4, LN_sm4);

		internalsm2dataoid = OBJ_create(STRING_sm2p7_data, SN_sm2p7_data, LN_sm2p7_data);
		internalsm2signedoid = OBJ_create(STRING_sm2p7_signed, SN_sm2p7_signed, LN_sm2p7_signed);
		internalsm2envoid = OBJ_create(STRING_sm2p7_enveloped, SN_sm2p7_enveloped, LN_sm2p7_enveloped);
		internalsm2signenvoid = OBJ_create(STRING_sm2p7_signedAndEnveloped, SN_sm2p7_signedAndEnveloped, LN_sm2p7_signedAndEnveloped);
		internalsm2digestoid = OBJ_create(STRING_sm2p7_digest, SN_sm2p7_digest, LN_sm2p7_digest);
		internalsm2encoid = OBJ_create(STRING_sm2p7_encrypted, SN_sm2p7_encrypted, LN_sm2p7_encrypted);

		internalsm2encoid = OBJ_create(STRING_sm2_3, SN_sm2_3, LN_sm2_3);

		OpenSSL_add_all_algorithms();
	}
	
}


STACK_OF(SM2_SIGNER_INFO) *SM2_P7_get_signer_info(SM2_P7 *p7)
{
	if (SM2_P7_type_is_signed(p7))
	{
		return(p7->d.sign->signer_info);
	}
	else if (SM2_P7_type_is_signedAndEnveloped(p7))
	{
		return(p7->d.signed_and_enveloped->signer_info);
	}
	else
		return(NULL);
}


static ASN1_TYPE *get_attribute(STACK_OF(X509_ATTRIBUTE) *sk, int nid)
{
	int i;
	X509_ATTRIBUTE *xa;
	ASN1_OBJECT *o;

	o = OBJ_nid2obj(nid);
	if (!o || !sk) return(NULL);
	for (i = 0; i<sk_X509_ATTRIBUTE_num(sk); i++)
	{
		xa = sk_X509_ATTRIBUTE_value(sk, i);
		if (OBJ_cmp(xa->object, o) == 0)
		{
			if (!xa->single && sk_ASN1_TYPE_num(xa->value.set))
				return(sk_ASN1_TYPE_value(xa->value.set, 0));
			else
				return(NULL);
		}
	}
	return(NULL);
}

ASN1_OCTET_STRING *SM2_P7_digest_from_attributes(STACK_OF(X509_ATTRIBUTE) *sk)
{
	ASN1_TYPE *astype;
	if (!(astype = get_attribute(sk, NID_pkcs9_messageDigest))) return NULL;
	return astype->value.octet_string;
}

static void reversebyte(unsigned char* src, int istart, int ilen)
{
	if (src
		&&(istart>=0)
		&&(ilen>0)
		&&(istart < ilen))
	{
		int iround = ilen / 2;
		int i;
		for (i = 0; i < iround; i++)
		{
			unsigned char uchtmp = src[istart + i];
			src[istart + i] = src[istart + ilen - i - 1];
			src[istart + ilen - i - 1] = uchtmp;
		}
	}
}

/*
void translatetohex(unsigned char* psrc, char* pdest, int ilen)
{
	if (psrc
		&&pdest
		&&ilen)
	{
		for (int i = 0; i < ilen;i++)
		{
			_snprintf_s(pdest + i*2, 3, 3, "%02X", psrc[i]);
		}
	}
}
*/

int SM2Verify(X509* x, unsigned char* puchsig, int isiglen, unsigned char* puchmsg, int imsglen)
{
	int iret = 0;

	if (x&&puchsig&&puchmsg
		&&isiglen>0
		&& imsglen>0)
	{
		EVP_PKEY *pkey;
		EVP_PKEY_CTX * ctx;
		const EVP_MD* md = EVP_sm3();
		EC_KEY *eck = EC_KEY_from_X509(x);
		pkey = EVP_PKEY_SM2_init(eck);

		//////////////////////////////////////////////////////////////////////////////
		/*unsigned char md_value[EVP_MAX_MD_SIZE];
		unsigned int md_len;
		EVP_MD_CTX * mdctx = EVP_MD_CTX_create();
		if (mdctx)
		{
		EVP_DigestInit(mdctx, md);
		EVP_DigestUpdate(mdctx, puchmsg, imsglen);
		EVP_DigestFinal(mdctx, md_value, &md_len);
		EVP_MD_CTX_destroy(mdctx);
		}*/
		//////////////////////////////////////////////////////////////////////////////

		if (pkey)
		{
			ctx = EVP_PKEY_CTX_new(pkey, NULL);

			if (EVP_PKEY_verify_init(ctx) > 0)
			{
				if (EVP_PKEY_verify(ctx, puchsig, isiglen, puchmsg, imsglen) > 0)
				{
					iret = 1;
				}
				else
				{
					unsigned char buf[32] = { 0 };
					memcpy(buf, puchsig, 32);
					reversebyte(buf, 0, 32);
					memcpy(puchsig, buf, 32);

					memcpy(buf, puchsig + 32, 32);
					reversebyte(buf, 0, 32);
					memcpy(puchsig + 32, buf, 32);

					if (EVP_PKEY_verify(ctx, puchsig, isiglen, puchmsg, imsglen) > 0)
					{
						iret = 1;
					}
				}
			}
		}
	}

	return iret;
}


int SM2_P7_signatureVerify(SM2_P7 *p7, SM2_SIGNER_INFO *si, X509 *x509, unsigned char*msg, int msglen)
{
	ASN1_OCTET_STRING *os;
	const EVP_MD* md;
	int ret = 0, i;
	int md_type;
	STACK_OF(X509_ATTRIBUTE) *sk;
	EVP_PKEY *pkey;
	EVP_PKEY_CTX * ctx;
	BIGNUM *r = NULL;
	BIGNUM *s = NULL;

	if (msg
		&&msglen)
	{
		if (SM2_P7_type_is_signed(p7) ||
			SM2_P7_type_is_signedAndEnveloped(p7)) {
			md_type = si->digest_alg->algorithm->nid;

			switch (md_type)
			{
			case NID_sm3:
				md = EVP_sm3();
				break;
			case NID_md5:
				md = EVP_md5();
				break;
			case NID_sha1:
				md = EVP_sha1();
				break;
			case NID_sha256:
				md = EVP_sha256();
				break;
			default:
				md = EVP_sm3();
				break;
			}

			
			//////////////////////////////////////////////////////////////////////////////
			/*unsigned char md_value[EVP_MAX_MD_SIZE];
			unsigned int md_len;
			EVP_MD_CTX * mdctx = EVP_MD_CTX_create();
			if (mdctx)
			{
				EVP_DigestInit(mdctx, md);
				EVP_DigestUpdate(mdctx, msg, msglen);
				EVP_DigestFinal(mdctx, md_value, &md_len);
				EVP_MD_CTX_destroy(mdctx);
			}*/
			//////////////////////////////////////////////////////////////////////////////


			os = si->enc_digest;

			////////////////////////////////////////////////////////////
			if (os->length == 64)
			{
				/*
				EC_SM2_SIG *sm2signed = EC_SM2_SIG_new();
				unsigned char buf[32] = { 0 };
				char hex[65] = {0};
				memcpy(buf, os->data, 32);
				reversebyte(buf, 0, 32);
				//translatetohex(buf, hex, 32);
				r = BN_bin2bn(buf, 32, NULL);
				//BN_hex2bn(&r, hex);
				memcpy(buf, os->data + 32, 32);
				reversebyte(buf, 0, 32);
				//translatetohex(buf, hex, 32);
				s = BN_bin2bn(buf, 32, NULL);
				//BN_hex2bn(&s, hex);
				sm2signed->r = r;
				sm2signed->s = s;
				unsigned char * sigout = NULL;
				int sigoutlen = i2d_EC_SM2_SIG(sm2signed, &sigout);
				*/
				EC_KEY *eck = EC_KEY_from_X509(x509);
				pkey = EVP_PKEY_SM2_init(eck);

				if (pkey)
				{
					ctx = EVP_PKEY_CTX_new(pkey, NULL);
					
					if (EVP_PKEY_verify_init(ctx) > 0)
					{
						if (EVP_PKEY_verify(ctx, os->data, os->length, msg, msglen) > 0)
						{
							ret = 1;
						}
						else
						{
							unsigned char buf[32] = { 0 };
							memcpy(buf, os->data, 32);
							reversebyte(buf, 0, 32);
							memcpy(os->data, buf, 32);

							memcpy(buf, os->data + 32, 32);
							reversebyte(buf, 0, 32);
							memcpy(os->data + 32, buf, 32);

							if (EVP_PKEY_verify(ctx, os->data, os->length, msg, msglen) > 0)
							{
								ret = 1;
							}
						}
					}
				}


				//EC_SM2_SIG_free(sm2signed);
				//OPENSSL_free(sigout);
			}
			////////////////////////////////////////////////////////////
		}
	}
	
	return (ret);
}
