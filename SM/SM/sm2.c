#include <stdio.h>
#include <openssl/bn.h>
#include <openssl/bio.h>
#include <openssl/ec.h>
#include <openssl/evp.h>
#include <string.h>
#include "SM.h"
#include "sm2.h"

//#define _DEBUG_

#ifdef _DEBUG_

const unsigned char EC_SM2_a[32] = {
	0x78,0x79,0x68,0xB4,0xFA,0x32,0xC3,0xFD,0x24,0x17,0x84,0x2E,0x73,0xBB,0xFE,0xFF,
	0x2F,0x3C,0x84,0x8B,0x68,0x31,0xD7,0xE0,0xEC,0x65,0x22,0x8B,0x39,0x37,0xE4,0x98};
const unsigned char EC_SM2_b[32] = {
	0x63,0xE4,0xC6,0xD3,0xB2,0x3B,0x0C,0x84,0x9C,0xF8,0x42,0x41,0x48,0x4B,0xFE,0x48,
	0xF6,0x1D,0x59,0xA5,0xB1,0x6B,0xA0,0x6E,0x6E,0x12,0xD1,0xDA,0x27,0xC5,0x24,0x9A};
const unsigned char EC_SM2_p[32] = {
	0x85,0x42,0xD6,0x9E,0x4C,0x04,0x4F,0x18,0xE8,0xB9,0x24,0x35,0xBF,0x6F,0xF7,0xDE,
	0x45,0x72,0x83,0x91,0x5C,0x45,0x51,0x7D,0x72,0x2E,0xDB,0x8B,0x08,0xF1,0xDF,0xC3};
const unsigned char EC_SM2_n[32] = {
	0x85,0x42,0xD6,0x9E,0x4C,0x04,0x4F,0x18,0xE8,0xB9,0x24,0x35,0xBF,0x6F,0xF7,0xDD,
	0x29,0x77,0x20,0x63,0x04,0x85,0x62,0x8D,0x5A,0xE7,0x4E,0xE7,0xC3,0x2E,0x79,0xB7};
const unsigned char EC_SM2_Gx[32] = {
	0x42,0x1D,0xEB,0xD6,0x1B,0x62,0xEA,0xB6,0x74,0x64,0x34,0xEB,0xC3,0xCC,0x31,0x5E,
	0x32,0x22,0x0B,0x3B,0xAD,0xD5,0x0B,0xDC,0x4C,0x4E,0x6C,0x14,0x7F,0xED,0xD4,0x3D};
const unsigned char EC_SM2_Gy[32] = {
	0x06,0x80,0x51,0x2B,0xCB,0xB4,0x2C,0x07,0xD4,0x73,0x49,0xD2,0x15,0x3B,0x70,0xC4,
	0xE5,0xD7,0xFD,0xFC,0xBF,0xA3,0x6E,0xA1,0xA8,0x58,0x41,0xB9,0xE4,0x6E,0x09,0xA2};

#else

const unsigned char EC_SM2_a[32] = {
	0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFC};
const unsigned char EC_SM2_b[32] = {
	0x28,0xE9,0xFA,0x9E,0x9D,0x9F,0x5E,0x34,0x4D,0x5A,0x9E,0x4B,0xCF,0x65,0x09,0xA7,
	0xF3,0x97,0x89,0xF5,0x15,0xAB,0x8F,0x92,0xDD,0xBC,0xBD,0x41,0x4D,0x94,0x0E,0x93};
const unsigned char EC_SM2_p[32] = {
	0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
const unsigned char EC_SM2_n[32] = {
	0xFF,0xFF,0xFF,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,
	0x72,0x03,0xDF,0x6B,0x21,0xC6,0x05,0x2B,0x53,0xBB,0xF4,0x09,0x39,0xD5,0x41,0x23};
const unsigned char EC_SM2_Gx[32] = {
	0x32,0xC4,0xAE,0x2C,0x1F,0x19,0x81,0x19,0x5F,0x99,0x04,0x46,0x6A,0x39,0xC9,0x94,
	0x8F,0xE3,0x0B,0xBF,0xF2,0x66,0x0B,0xE1,0x71,0x5A,0x45,0x89,0x33,0x4C,0x74,0xC7};
const unsigned char EC_SM2_Gy[32] = {
	0xBC,0x37,0x36,0xA2,0xF4,0xF6,0x77,0x9C,0x59,0xBD,0xCE,0xE3,0x6B,0x69,0x21,0x53,
	0xD0,0xA9,0x87,0x7C,0xC6,0x2A,0x47,0x40,0x02,0xDF,0x32,0xE5,0x21,0x39,0xF0,0xA0};

#endif

static int IsArrayZero(unsigned char *array, size_t len)
{
	while (len--)
	{
		if (*array++)
			return 0;
	}
	return 1;
}

int KDF(const unsigned char *Z, size_t zLen, unsigned long byteLen, unsigned char *K, const EVP_MD * md)
{
	unsigned int ct = 0;
	unsigned int md_len;
	unsigned int md_size;
	unsigned char *p = (unsigned char *)&ct;

	EVP_MD_CTX * mdctx = EVP_MD_CTX_create();
	if (mdctx)
	{
		md_size = EVP_MD_size(md);
		while (byteLen)
		{
			unsigned char t3, t2, t1, t0;
			EVP_MD_CTX_init(mdctx);
			EVP_DigestInit(mdctx, md);
			EVP_DigestUpdate(mdctx, Z, zLen);
			++ct;
			EVP_DigestUpdate(mdctx, p+3, 1);
			t3 = *(p+3);
			EVP_DigestUpdate(mdctx, p+2, 1);
			t2 = *(p+2);
			EVP_DigestUpdate(mdctx, p+1, 1);
			t1 = *(p+1);
			EVP_DigestUpdate(mdctx, p, 1);
			t0 = *p;
			if (byteLen >= md_size)
			{
				EVP_DigestFinal(mdctx, K, &md_len);
				K += md_len;
				byteLen -= md_len;
			}
			else
			{
				unsigned char tmp[EVP_MAX_MD_SIZE];
				EVP_DigestFinal(mdctx, tmp, &md_len);
				memcpy(K, tmp, byteLen);
				K += byteLen;
				byteLen = 0;
			}
			EVP_MD_CTX_cleanup(mdctx);
		}
		EVP_MD_CTX_destroy(mdctx);
	}
	return 1;
}

int Md_bn2bin(const BIGNUM *b, unsigned char *array, int size)
{
	int ret = BN_num_bytes(b);
	memset(array, 0, size);
	if (ret <= size)
	{
		ret = BN_bn2bin(b, array + size - ret);
	}
	return ret;
}

void * ec_sm2_data_new()
{
	EC_SM2_DATA * q = (EC_SM2_DATA*)OPENSSL_malloc(sizeof(EC_SM2_DATA));
	if (q)
	{
		BIGNUM * EC_a = BN_bin2bn(EC_SM2_a, sizeof(EC_SM2_a), NULL);
		BIGNUM * EC_b = BN_bin2bn(EC_SM2_b, sizeof(EC_SM2_b), NULL);
		BIGNUM * EC_Gx = BN_bin2bn(EC_SM2_Gx, sizeof(EC_SM2_Gx), NULL);
		BIGNUM * EC_Gy = BN_bin2bn(EC_SM2_Gy, sizeof(EC_SM2_Gy), NULL);
		q->EC_p = BN_bin2bn(EC_SM2_p, sizeof(EC_SM2_p), NULL);
		q->EC_n = BN_bin2bn(EC_SM2_n, sizeof(EC_SM2_n), NULL);
		q->EC_Group = EC_GROUP_new_curve_GFp(q->EC_p, EC_a, EC_b, NULL);
		q->BASE_POINT = EC_POINT_new(q->EC_Group);
		EC_POINT_set_affine_coordinates_GFp(q->EC_Group, q->BASE_POINT, EC_Gx, EC_Gy, NULL);
		BN_free(EC_a);
		BN_free(EC_b);
		BN_free(EC_Gx);
		BN_free(EC_Gy);
	}
	return q;
}

void * ec_sm2_data_dup(void * p)
{
	if (p == NULL)
		return NULL;
	return ec_sm2_data_new();
}

void ec_sm2_data_free(void * p)
{
	EC_SM2_DATA * q = (EC_SM2_DATA*)p;
	if (q)
	{
		if (q->EC_p)
			BN_free(q->EC_p);
		if (q->EC_n)
			BN_free(q->EC_n);
		if (q->BASE_POINT)
			EC_POINT_free(q->BASE_POINT);
		if (q->EC_Group)
			EC_GROUP_clear_free(q->EC_Group);
	}
	OPENSSL_free(q);
}

EC_SM2_DATA *ec_sm2_check(EC_KEY *key)
{
	EC_SM2_DATA *ec_sm2_data;

	void *data = EC_KEY_get_key_method_data(key, ec_sm2_data_dup, ec_sm2_data_free, ec_sm2_data_free);
	if (data == NULL)
	{
		ec_sm2_data = (EC_SM2_DATA *)ec_sm2_data_new();
		if (ec_sm2_data == NULL)
			return NULL;
		EC_KEY_insert_key_method_data(key, (void *)ec_sm2_data, ec_sm2_data_dup, ec_sm2_data_free, ec_sm2_data_free);
	}
	else
		ec_sm2_data = (EC_SM2_DATA *)data;

	return ec_sm2_data;
}

int EC_SM2_check_key(EC_KEY * sm2)
{
	int ret;
	BIGNUM *x = BN_new();
	BIGNUM *y = BN_new();
	BIGNUM *zero = BN_new();
	EC_POINT *p = EC_POINT_new(EC_KEY_get0_group(sm2));
	EC_SM2_DATA * data = ec_sm2_check(sm2);
	BN_zero(zero);
	ret = EC_POINT_get_affine_coordinates_GFp(EC_KEY_get0_group(sm2), EC_KEY_get0_public_key(sm2), x, y, NULL);

	if (BN_cmp(x, data->EC_p) >= 0)
	{
		ret = 0;
		goto END;
	}
	if (BN_cmp(x, zero) < 0)
	{
		ret = 0;
		goto END;
	}

	if (BN_cmp(y, data->EC_p) >= 0)
	{
		ret = 0;
		goto END;
	}
	if (BN_cmp(y, zero) < 0)
	{
		ret = 0;
		goto END;
	}

	ret = EC_POINT_is_on_curve(EC_KEY_get0_group(sm2), EC_KEY_get0_public_key(sm2), NULL);
	if (ret == 0)
		goto END;

	ret = EC_POINT_is_at_infinity(EC_KEY_get0_group(sm2), EC_KEY_get0_public_key(sm2));
	if (ret == 1)
	{
		ret = 0;
		goto END;
	}

	ret = EC_POINT_mul(EC_KEY_get0_group(sm2), p, NULL, EC_KEY_get0_public_key(sm2), data->EC_n, NULL);
	if (ret == 0)
		goto END;

	ret = EC_POINT_is_at_infinity(EC_KEY_get0_group(sm2), p);

END:
	BN_free(x);
	BN_free(y);
	BN_free(zero);
	EC_POINT_free(p);
	return ret;
}

int EC_SM2_check_key_ex(EC_KEY * sm2)
{
	int ret;
	EC_SM2_DATA * data = ec_sm2_check(sm2);
	EC_POINT *p = EC_POINT_new(EC_KEY_get0_group(sm2));

	ret = EC_POINT_is_at_infinity(EC_KEY_get0_group(sm2), p);

	EC_POINT_free(p);

	return ret;
}

int EC_SM2_generate_key(EC_KEY * sm2)
{
	int ret = 0;
	BIGNUM *up = BN_new();
	BIGNUM *priv_key = BN_new();
	EC_POINT *pub_key = EC_POINT_new(EC_KEY_get0_group(sm2));
	EC_SM2_DATA * data = ec_sm2_check(sm2);

	if (EC_KEY_get0_private_key(sm2))
		goto err;
	if (EC_KEY_get0_public_key(sm2))
		goto err;
	if ((up == NULL) || (priv_key == NULL) || (pub_key == NULL))
		goto err;

	BN_sub(up, data->EC_n, BN_value_one());
	BN_rand_range(priv_key, up);

	ret = EC_POINT_mul(EC_KEY_get0_group(sm2), pub_key, NULL, data->BASE_POINT, priv_key, NULL);
	if (ret == 0)
		goto err;

	EC_KEY_set_private_key(sm2, priv_key);
	EC_KEY_set_public_key(sm2, pub_key);
	ret = 1;
err:
	BN_free(up);
	BN_free(priv_key);
	EC_POINT_free(pub_key);
	return ret;
}

int EC_SM2_HashIdMessage_PreData(X509* cert, const char *id, const unsigned char *message, size_t msgLen, const EVP_MD * md, unsigned char* puchin, int inlen)
{
	unsigned short len;
	unsigned char array[EVP_MAX_MD_SIZE];
	unsigned int md_len = 0;
	EC_KEY * sm2 = EC_KEY_from_X509(cert);

	if (id == NULL)
		id = "1234567812345678";
	len = strlen(id) * 8;

	if (puchin
		&&inlen>0)
	{
		BIGNUM * x = BN_new();
		BIGNUM * y = BN_new();
		BIGNUM * ret = NULL;
		EVP_MD_CTX * mdctx = EVP_MD_CTX_create();

		if (mdctx)
		{
			EVP_DigestInit(mdctx, md);
			EVP_DigestUpdate(mdctx, (char*)(&len) + 1, 1);
			EVP_DigestUpdate(mdctx, &len, 1);
			EVP_DigestUpdate(mdctx, id, strlen(id));
			EVP_DigestUpdate(mdctx, EC_SM2_a, sizeof(EC_SM2_a));
			EVP_DigestUpdate(mdctx, EC_SM2_b, sizeof(EC_SM2_b));
			EVP_DigestUpdate(mdctx, EC_SM2_Gx, sizeof(EC_SM2_Gx));
			EVP_DigestUpdate(mdctx, EC_SM2_Gy, sizeof(EC_SM2_Gy));
			EC_POINT_get_affine_coordinates_GFp(EC_KEY_get0_group(sm2), EC_KEY_get0_public_key(sm2), x, y, NULL);
			Md_bn2bin(x, array, 32);
			BN_free(x);
			EVP_DigestUpdate(mdctx, array, 32);
			Md_bn2bin(y, array, 32);
			BN_free(y);
			EVP_DigestUpdate(mdctx, array, 32);
			EVP_DigestFinal(mdctx, array, &md_len);


			memcpy(puchin, array, md_len);
			memcpy(puchin + md_len, message, msgLen);
			return 1;
		}
		
	}

	return 0;
}

BIGNUM * EC_SM2_HashIdMessage(EC_KEY * sm2, const char *id, const unsigned char *message, size_t msgLen, const EVP_MD * md)
{
	unsigned short len;
	unsigned char array[EVP_MAX_MD_SIZE];
	unsigned int md_len = 0;
	if (id == NULL)
		id = "1234567812345678";
	len = strlen(id) * 8;
	{
		BIGNUM * x = BN_new();
		BIGNUM * y = BN_new();
		BIGNUM * ret = NULL;
		EVP_MD_CTX * mdctx = EVP_MD_CTX_create();
		if (mdctx)
		{
			EVP_DigestInit(mdctx, md);
			EVP_DigestUpdate(mdctx, (char*)(&len)+1, 1);
			EVP_DigestUpdate(mdctx, &len, 1);
			EVP_DigestUpdate(mdctx, id, strlen(id));
			EVP_DigestUpdate(mdctx, EC_SM2_a, sizeof(EC_SM2_a));
			EVP_DigestUpdate(mdctx, EC_SM2_b, sizeof(EC_SM2_b));
			EVP_DigestUpdate(mdctx, EC_SM2_Gx, sizeof(EC_SM2_Gx));
			EVP_DigestUpdate(mdctx, EC_SM2_Gy, sizeof(EC_SM2_Gy));
			EC_POINT_get_affine_coordinates_GFp(EC_KEY_get0_group(sm2), EC_KEY_get0_public_key(sm2), x, y, NULL);
			Md_bn2bin(x, array, 32);
			BN_free(x);
			EVP_DigestUpdate(mdctx, array, 32);
			Md_bn2bin(y, array, 32);
			BN_free(y);
			EVP_DigestUpdate(mdctx, array, 32);
			EVP_DigestFinal(mdctx, array, &md_len);

			EVP_MD_CTX_init(mdctx);
			EVP_DigestInit(mdctx, md);
			EVP_DigestUpdate(mdctx, array, md_len);
			EVP_DigestUpdate(mdctx, message, msgLen);
			EVP_DigestFinal(mdctx, array, &md_len);
			EVP_MD_CTX_destroy(mdctx);
			ret = BN_bin2bn(array, md_len, NULL);
		}
		return ret;
	}
}

int EC_SM2_sign(EC_KEY * sm2, const char *id, const unsigned char *message, size_t msgLen, EC_SM2_SIG * sig, const EVP_MD * md)
{
    BIGNUM *e = NULL;
    BIGNUM *k = NULL;
    BIGNUM *x1 = NULL;
    EC_POINT *G1 = NULL;
    BN_CTX *ctx= NULL;
	EC_SM2_DATA * data = ec_sm2_check(sm2);
    int ret;

    // A1, A2 计算e = Hash(Za || M)
	e = EC_SM2_HashIdMessage(sm2, id, message, msgLen, md);
    
    // A3:用随机数发生器产生随机数k is [1,n-1];
STEP_A3:
#ifdef _DEBUG_
    {
        const char *kHexStr =   "6CB28D99385C175C94F94E934817663FC176D925DD72B727260DBAAE1FB2F96F";
        BN_hex2bn(&k, kHexStr);
    }
#else
    if (k == NULL)
		k = BN_new();
    BN_rand_range(k, data->EC_n);
    if (BN_is_zero(k))
	{
        goto STEP_A3;
    }
#endif
    // A4:计算椭圆曲线点(x1,y1)=[k]G,按本文本第1部分4.2.7给出的细节将x1的数据类型转换为整数;
    if (G1 == NULL)
		G1 = EC_POINT_new(EC_KEY_get0_group(sm2));
    ret = EC_POINT_mul(EC_KEY_get0_group(sm2), G1, NULL, data->BASE_POINT, k, NULL);
    if (ret == 0) {
        ret = -2;
        goto END;// 设置基点坐标失败
    }
    else {
#ifdef _DEBUG_
        //char *g1Str = EC_POINT_point2hex(EC_KEY_get0_group(sm2), G1, POINT_CONVERSION_UNCOMPRESSED, NULL);
        //printf("[k]G is: %s\n", g1Str);
#endif
    }
    if (x1 == NULL) x1 = BN_new();
    ret = EC_POINT_get_affine_coordinates_GFp(EC_KEY_get0_group(sm2), G1, x1, NULL, NULL);
    if (ret == 0) {
        ret = -3;  // 设置基点坐标失败
        goto END;
    }
    
    // A5:计算r=(e+x1) modn,若r=0或r+k=n则返回A3;
    BN_mod_add_quick(sig->r, e, x1, data->EC_n);
    
    // 判断生成的r是否有效
    if (BN_is_zero(sig->r))
	{
        // 重新生成随机数
        goto STEP_A3;
    }
    else
	{
        BIGNUM *temp = BN_new();
        BN_add(temp, sig->r, k);
        if (!BN_cmp(temp, data->EC_n))
		{
            // 重新生成随机数
            BN_free(temp);
            temp = NULL;
            goto STEP_A3;
        }
        BN_free(temp);
        temp = NULL;
    }
    
    // A6:计算s = ((1 + dA)?1 · (k ? r · dA)) modn,若s=0则返回A3;
    {
		BIGNUM *num = BN_new();
		BIGNUM *den = BN_new();
        if (ctx == NULL)
		{
            ctx = BN_CTX_new();
            BN_CTX_start(ctx);
        }
        
        BN_copy(den, EC_KEY_get0_private_key(sm2));
        BN_add_word(den, 1);
        BN_mod_inverse(den, den, data->EC_n, ctx);
        
        BN_mod_mul(num, sig->r, EC_KEY_get0_private_key(sm2), data->EC_n, ctx);
        BN_sub(num, k, num);
        BN_mod_mul(sig->s, num, den, data->EC_n, ctx);
        
        BN_free(num);
        BN_free(den);
        num = NULL;
        den = NULL;
        if (BN_is_zero(sig->s))
		{
            // 重新生成随机数
            goto STEP_A3;
        }
    }
    ret = 1;

END:
    if (ctx != NULL)
	{
        BN_CTX_end(ctx);
        BN_CTX_free(ctx);
        ctx = NULL;
    }
    EC_POINT_free(G1);
    BN_free(e);
    BN_free(x1);
    BN_free(k);
    return ret;
}

int EC_SM2_verify(EC_KEY * sm2, const char *id, const unsigned char *message, size_t msgLen, const EC_SM2_SIG * sig, const EVP_MD * md)
{
	int ret = 0;
	BIGNUM *e = NULL;
	BIGNUM *t = NULL;
	BIGNUM *R = NULL;
	EC_POINT *G1 = NULL;
	EC_POINT *G2 = NULL;
	BN_CTX *ctx = NULL;
	EC_SM2_DATA * data = ec_sm2_check(sm2);

	// B1:检验r′ ∈[1,n-1]是否成立,若不成立则验证不通过;
    if (sig == NULL)
	{
        ret = -1;
        goto END;
    }
    if (BN_is_zero(sig->r))
	{
        ret = 0;
        goto END;
    }
	else if (BN_cmp(sig->r, data->EC_n) >= 0)
	{
        ret = 0;
        goto END;
    }
    
    // B2:检验s′ ∈[1,n-1]是否成立,若不成立则验证不通过;
    if (BN_is_zero(sig->s))
	{
        ret = 0;
        goto END;
    }
	else if (BN_cmp(sig->s, data->EC_n) >= 0)
	{
        ret = 0;
        goto END;
    }
    
    // B3:置M′=ZA ∥ M′;
    // B4:计算e′ = Hv(M′),按本文本第1部分4.2.3和4.2.2给出的细节将e′的数据类型转换为整数;
	e = EC_SM2_HashIdMessage(sm2, id, message, msgLen, md);
    
    // B5:按本文本第1部分4.2.2给出的细节将r′、s′的数据类型转换为整数,计算t = (r′ + s′) modn, 若t = 0,则验证不通过;
    if (t == NULL)
	{
        t = BN_new();
    }
    BN_mod_add_quick(t, sig->r, sig->s, data->EC_n);
    if (BN_is_zero(t))
	{
        ret = 0;
        goto END;
    }
    
    // B6:计算椭圆曲线点(x1′, y1′)=[s′]G + [t]PA;
    if (ctx == NULL)
		ctx = BN_CTX_new();
    BN_CTX_start(ctx);
    if (G1 == NULL)
		G1 = EC_POINT_new(EC_KEY_get0_group(sm2));
    EC_POINT_mul(EC_KEY_get0_group(sm2), G1, NULL, data->BASE_POINT, sig->s, ctx);
    
    if (G2 == NULL)
		G2 = EC_POINT_new(EC_KEY_get0_group(sm2));

    ret = EC_POINT_mul(EC_KEY_get0_group(sm2), G2, NULL, EC_KEY_get0_public_key(sm2), t, ctx);
    if (ret == 0)
	{
        goto END;
    }
    ret = EC_POINT_add(EC_KEY_get0_group(sm2), G1, G1, G2, ctx);
    if (ret == 0)
	{
        goto END;
    }
    ret = EC_POINT_get_affine_coordinates_GFp(EC_KEY_get0_group(sm2), G1, sig->s, NULL, ctx);
    if (ret == 0)
	{
        goto END;
    }
    
    // B7:按本文本第1部分4.2.7给出的细节将x′1的数据类型转换为整数,计算R = (e′ + x′1) modn,检 验R=r′是否成立,若成立则验证通过;否则验证不通过。
    if (R == NULL)
		R = BN_new();
    ret = BN_mod_add(R, e, sig->s, data->EC_n, ctx);
    if (ret == 0)
	{
        goto END;
    }
    
    if (BN_cmp(R, sig->r) == 0)
	{
        ret = 1;
        goto END;
    }
	else
	{
        ret = 0;
        goto END;
    }
    
END:
    if (ctx != NULL)
		BN_CTX_end(ctx);
    BN_CTX_free(ctx);
    BN_free(R);
    BN_free(t);
    BN_free(e);
    EC_POINT_free(G1);
    EC_POINT_free(G2);
    return ret;
}

int EC_SM2_encrypt(EC_KEY * sm2, const unsigned char *msg, size_t msgLen, unsigned char *oct, const EVP_MD * md)
{
	int ret = 0;
	size_t i;
	BIGNUM *k = NULL;
	EC_POINT *C1 = NULL;
	EC_POINT *kPB = NULL;
	BN_CTX *ctx = NULL;
	unsigned char *xy2 = NULL;
	unsigned char *t = NULL;
	EC_SM2_DATA * data = ec_sm2_check(sm2);

	if (oct == NULL)
	{
		ret = (int)(msgLen + 65 + EVP_MD_size(md));
		goto END;
	}
    
    // A1:用随机数发生器产生随机数k∈[1,n-1];
STEP_A1:
#ifdef _DEBUG_
    {
        const char *kHexStr =   "4C62EEFD6ECFC2B95B92FD6C3D9575148AFA17425546D49018E5388D49DD7B4F";
        BN_hex2bn(&k, kHexStr);
    }
#else
    if (k == NULL) k = BN_new();
    BN_rand_range(k, data->EC_n);
    if (BN_is_zero(k)) {
        goto STEP_A1;
    }
#endif
    
    ctx = BN_CTX_new();
    BN_CTX_start(ctx);
    // A2: 计算椭圆曲线点C1=[k]G=(x1,y1),按本文本第1部分4.2.8和4.2.4给出的细节,将C1的数据类型转换为比特串;
    if (C1 == NULL)
		C1 = EC_POINT_new(EC_KEY_get0_group(sm2));
    ret = EC_POINT_mul(EC_KEY_get0_group(sm2), C1, NULL, data->BASE_POINT, k, ctx);
    if (ret == 0) goto END;
    
    // A3:计算椭圆曲线点S=[h]PB ,若S是无穷远点,则报错并退出;
    if (EC_POINT_is_at_infinity(EC_KEY_get0_group(sm2), EC_KEY_get0_public_key(sm2))) {
        ret = 0;
        goto END;
    }
    
    // A4:计算椭圆曲线点[k]PB=(x2,y2),按本文本第1部分4.2.5和4.2.4给出的细节,将坐标x2、y2的数据类型转换为比特串;
    if (kPB == NULL)
		kPB = EC_POINT_new(EC_KEY_get0_group(sm2));
    ret = EC_POINT_mul(EC_KEY_get0_group(sm2), kPB, NULL, EC_KEY_get0_public_key(sm2), k, ctx);
    if (ret == 0)
		goto END;
    if (xy2 == NULL)
		xy2 = (unsigned char *)OPENSSL_malloc(32 * 2 + 1);
    ret = (int)EC_POINT_point2oct(EC_KEY_get0_group(sm2), kPB, POINT_CONVERSION_UNCOMPRESSED, xy2, 32 * 2 + 1, ctx);
    if (ret == 0)
		goto END;
    
    // A5:计算t=KDF (x2||y2, klen),若t为全0比特串,则返回A1;
    if (t == NULL)
		t = (unsigned char *)OPENSSL_malloc(msgLen);
    KDF(xy2 + 1, 32 * 2, msgLen, t, md);
    ret = IsArrayZero(t, msgLen);
    if (ret == 1)
	{
        ret = 0;
        goto STEP_A1;
    }
    
    // A6:计算C2 = M ⊕ t;
    for (i = 0; i < msgLen; ++i)
	{
        t[i] ^= msg[i];
    }
    
    // A7:计算C3 = Hash(x2||M||y2);
	{
		unsigned int md_len;
		EVP_MD_CTX * mdctx = EVP_MD_CTX_create();
		if (mdctx)
		{
			EVP_DigestInit(mdctx, md);
			EVP_DigestUpdate(mdctx, xy2+1, 32);
			EVP_DigestUpdate(mdctx, msg, msgLen);
			EVP_DigestUpdate(mdctx, xy2+33, 32);
			EVP_DigestFinal(mdctx, oct + 65, &md_len);
			EVP_MD_CTX_destroy(mdctx);
		}
	}

    // A8:输出密文C = C1||C3||C2
    EC_POINT_point2oct(EC_KEY_get0_group(sm2), C1, POINT_CONVERSION_UNCOMPRESSED, oct, 32 * 2 + 1, ctx);
	memcpy(oct+65 + EVP_MD_size(md), t, msgLen);
    ret = 65 + EVP_MD_size(md) + msgLen;
    
END:
    
    BN_free(k);
    EC_POINT_free(C1);
    EC_POINT_free(kPB);
    if (ctx != NULL)
	{
        BN_CTX_end(ctx);
        BN_CTX_free(ctx);
    };
    OPENSSL_free(xy2);
    OPENSSL_free(t);
    return ret;
}

int EC_SM2_decrypt(EC_KEY * sm2, const unsigned char *ct, size_t ctLen, unsigned char *pt, const EVP_MD * md)
{
    int ret = 0;
    size_t i;
	unsigned char *t = NULL;
    unsigned char *c2xy = NULL;
    unsigned char hash[EVP_MAX_MD_SIZE];
    BN_CTX *ctx = BN_CTX_new();
    EC_POINT *C1 = NULL;
    
    BN_CTX_start(ctx);
    
    if (ctLen < 65 + (unsigned int)EVP_MD_size(md))
	{
        goto END;
    }
    
    if (pt == NULL)
	{
        ret = (int)(ctLen - 65 - EVP_MD_size(md));
        goto END;
    }
    
    // B1:从C中取出比特串C1,按本文本第1部分4.2.3和4.2.9给出的细节,将C1的数据类型转换为椭圆曲线上的点,验证C1是否满足椭圆曲线方程,若不满足则报错并退出;
    if (C1 == NULL) C1 = EC_POINT_new(EC_KEY_get0_group(sm2));
    ret = EC_POINT_oct2point(EC_KEY_get0_group(sm2), C1, ct, 65, ctx);
    if (ret == 0)
		goto END;
    ret = EC_POINT_is_on_curve(EC_KEY_get0_group(sm2), C1, ctx);
    if (ret == 0)
		goto END;
    
    // B2:计算椭圆曲线点S=[h]C1,若S是无穷远点,则报错并退出;
    ret = EC_POINT_is_at_infinity(EC_KEY_get0_group(sm2), C1);
    if (ret == 1)
	{
        ret = 0;
        goto END;
    }
    
    // B3:计算[dB]C1=(x2,y2),按本文本第1部分4.2.5和4.2.4给出的细节,将坐标x2、y2的数据类型转换为比特串;
    ret = EC_POINT_mul(EC_KEY_get0_group(sm2), C1, NULL, C1, EC_KEY_get0_private_key(sm2), ctx);
    if (ret == 0)
		goto END;
    if (c2xy == NULL)
		c2xy = (unsigned char *)OPENSSL_malloc(65);
    ret = (int)EC_POINT_point2oct(EC_KEY_get0_group(sm2), C1, POINT_CONVERSION_UNCOMPRESSED, c2xy, 65, ctx);
    if (ret == 0)
		goto END;
    
    // B4:计算t=KDF (x2||y2, klen),若t为全0比特串,则报错并退出;
    ctLen -= 97;
    if (t == NULL) t = (unsigned char *)OPENSSL_malloc(ctLen);
    KDF(c2xy + 1, 64, ctLen, t, md);
    ret = IsArrayZero(t, ctLen);
    if (ret == 1)
	{
        ret = 0;
        goto END;
    }
    
    // B5:从C中取出比特串C2,计算M' = C2 ⊕ t;
	{
		const unsigned char * tmp = ct + 65 + EVP_MD_size(md);
		for (i = 0; i < ctLen; ++i)
		{
			t[i] ^= tmp[i];
		}
	}
    
    // B6:计算u = Hash(x2||M'||y2),从C中取出比特串C3,若u =? C3,则报错并退出;
	{
		unsigned int md_len = 0;
		EVP_MD_CTX * mdctx = EVP_MD_CTX_create();
		if (mdctx)
		{
			EVP_DigestInit(mdctx, md);
			EVP_DigestUpdate(mdctx, c2xy+1, 32);
			EVP_DigestUpdate(mdctx, t, ctLen);
			EVP_DigestUpdate(mdctx, c2xy+33, 32);
			EVP_DigestFinal(mdctx, hash, &md_len);
			EVP_MD_CTX_destroy(mdctx);
		}
		if (memcmp(hash, ct + 65, md_len))
		{
			ret = 0;
			goto END;
		}
	}
    
    // B7:输出明文M'.
	memcpy(pt, t, ctLen);
    ret = ctLen;
    
END:
	OPENSSL_free(t);
	OPENSSL_free(c2xy);
	EC_POINT_free(C1);
    if (ctx != NULL)
	{
        BN_CTX_end(ctx);
        BN_CTX_free(ctx);
    }
    return ret;
}


