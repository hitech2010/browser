#include <openssl/crypto.h>
#include "SM.h"
#include "sm2.h"

void EVP_sm2_cleanup(void)
{
	CRYPTO_cleanup_all_ex_data();
}

EC_KEY * EC_KEY_SM2_init(EC_KEY * sm2)
{
	if (sm2 == NULL)
		sm2 = EC_KEY_new();
	if (sm2)
	{
		EC_SM2_DATA * data = ec_sm2_check(sm2);
		EC_KEY_set_group(sm2, data->EC_Group);
		// should set sm2 flag
		EC_KEY_set_flags(sm2, EC_FLAGS_IS_SM2_KEY);
	}
	return sm2;
}

int EC_KEY_is_sm2(EC_KEY * ec)
{
	if (EC_KEY_get_flags(ec) & EC_FLAGS_IS_SM2_KEY)
		return 1;
	return 0;
}

EC_KEY * EC_KEY_from_BN_EX(BIGNUM * priv_key)
{
	EC_KEY * sm2 = EC_KEY_SM2_init(NULL);
	if (sm2)
	{
		if (priv_key)
		{
			EC_KEY_set_private_key(sm2, priv_key);
		}
		if (EC_SM2_check_key_ex(sm2) == 0)
		{
			EC_KEY_free(sm2);
			sm2 = NULL;
		}
	}
	return sm2;
}

EC_KEY * EC_KEY_from_BN(BIGNUM * x, BIGNUM * y, BIGNUM * priv_key)
{
	EC_KEY * sm2 = EC_KEY_SM2_init(NULL);
	if (sm2)
	{
		EC_POINT *p = EC_POINT_new(EC_KEY_get0_group(sm2));
		if (p)
		{
			EC_POINT_set_affine_coordinates_GFp(EC_KEY_get0_group(sm2), p, x, y, NULL);
			EC_KEY_set_public_key(sm2, p);
			EC_POINT_free(p);
		}
		if (priv_key)
		{
			EC_KEY_set_private_key(sm2, priv_key);
		}
		if (EC_SM2_check_key(sm2) == 0)
		{
			EC_KEY_free(sm2);
			sm2 = NULL;
		}
	}
	return sm2;
}


/*
int eckey_param2type(int *pptype, void **ppval, EC_KEY *ec_key)
{
	const EC_GROUP  *group;
	int nid;
	if (ec_key == NULL || (group = EC_KEY_get0_group(ec_key)) == NULL)
	{
		ECerr(EC_F_ECKEY_PARAM2TYPE, EC_R_MISSING_PARAMETERS);
		return 0;
	}
	nid = EC_GROUP_get_curve_name(group);
	if (EC_KEY_is_sm2(ec_key))
	{
		*ppval = SM2_OBJ_nid2obj(NID_sm2);
		*pptype = V_ASN1_OBJECT;
	}
	else	
	{
		ASN1_STRING *pstr = NULL;
		pstr = ASN1_STRING_new();
		if (!pstr)
			return 0;
		pstr->length = i2d_ECParameters(ec_key, &pstr->data);
		if (pstr->length < 0)
		{
			ASN1_STRING_free(pstr);
			ECerr(EC_F_ECKEY_PARAM2TYPE, ERR_R_EC_LIB);
			return 0;
		}
		*ppval = pstr;
		*pptype = V_ASN1_SEQUENCE;
	}
	return 1;
}

int pkey_asn1_pub_encode(X509_PUBKEY *pk, const EVP_PKEY *pkey)
{
	EC_KEY *ec_key = pkey->pkey.ec;
	void *pval = NULL;
	int ptype;
	unsigned char *penc = NULL, *p;
	int penclen;

	if (!eckey_param2type(&ptype, &pval, ec_key))
	{
		ECerr(EC_F_ECKEY_PUB_ENCODE, ERR_R_EC_LIB);
		return 0;
	}
	penclen = i2o_ECPublicKey(ec_key, NULL);
	if (penclen <= 0)
		goto err;
	penc = OPENSSL_malloc(penclen);
	if (!penc)
		goto err;
	p = penc;
	penclen = i2o_ECPublicKey(ec_key, &p);
	if (penclen <= 0)
		goto err;
	if (X509_PUBKEY_set0_param(pk, OBJ_nid2obj(EVP_PKEY_EC),
		ptype, pval, penc, penclen))
		return 1;
err:
	if (ptype == V_ASN1_OBJECT)
		ASN1_OBJECT_free(pval);
	else
		ASN1_STRING_free(pval);
	if (penc)
		OPENSSL_free(penc);
	return 0;
}

int X509_PUBKEY_sm2_set(X509_PUBKEY **x, EVP_PKEY *pkey)
{
	X509_PUBKEY *pk = NULL;

	if (x == NULL) return(0);

	if ((pk = X509_PUBKEY_new()) == NULL) goto error;

	if (!pkey_asn1_pub_encode(pk, pkey))
	{
		X509err(X509_F_X509_PUBKEY_SET,
			X509_R_PUBLIC_KEY_ENCODE_ERROR);
		goto error;
	}

	if (*x != NULL)
		X509_PUBKEY_free(*x);

	*x = pk;

	return 1;
error:
	if (pk != NULL) X509_PUBKEY_free(pk);
	return 0;
}

int X509_REQ_set_sm2_pubkey(X509_REQ *x, EVP_PKEY *pkey)
{
	if ((x == NULL) || (x->req_info == NULL)) return(0);
	return (X509_PUBKEY_sm2_set(&x->req_info->pubkey, pkey));
}
*/

EVP_PKEY * EVP_PKEY_SM2_init(EC_KEY * sm2)
{
	EVP_PKEY * pkey = EVP_PKEY_new();
	if (pkey)
	{
		EVP_PKEY_assign_EC_KEY(pkey, sm2);
		pkey->engine = engine_sm2();
		pkey->ameth = sm2_pameth_new();
		//////////////////////////////////////////////////
		//((struct evp_pkey_asn1_method_st *)(pkey->ameth))->pub_encode = pkey_asn1_pub_encode;
		//////////////////////////////////////////////////
	}
	return pkey;
}

void EVP_PKEY_CTX_set_md(EVP_PKEY_CTX * ctx, const EVP_MD * md)
{
	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
	rctx->md = md;
}

void EVP_PKEY_CTX_set_id(EVP_PKEY_CTX * ctx, const char * id)
{
	EC_SM2_PKEY_CTX *rctx = (EC_SM2_PKEY_CTX*)EVP_PKEY_CTX_get_data(ctx);
	rctx->id = id;
}
