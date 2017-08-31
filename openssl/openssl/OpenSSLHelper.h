#ifndef __OPENSSL_HELPER_H__
#define __OPENSSL_HELPER_H__

#ifdef _DEBUG
#pragma comment(lib, "libeay32MTd")
#else
#pragma comment(lib, "libeay32MT")
#endif // _DEBUG

#include <string>
using namespace std;

#include <openssl/rand.h>
#include <openssl/evp.h>
#ifndef OPENSSL_NO_AES
#include <openssl/aes.h>
#endif // OPENSSL_NO_AES
#ifndef OPENSSL_NO_DES
#include <openssl/des.h>
#endif
#ifndef OPENSSL_NO_MD5
#include <openssl/md5.h>
#endif // OPENSSL_NO_MD5
#ifndef OPENSSL_NO_RSA
#include <openssl/rsa.h>
#endif
#include <openssl/x509.h>
#include <openssl/pem.h>

class OpensslHelper
{
public:
#ifndef OPENSSL_NO_AES
	static string AesEncrypt(string Msg, const char * szKey, int bit, bool bPKCS5Padding = false)
	{
		AES_KEY AesKey;
		string ret;
		if (bPKCS5Padding)
		{
			// PKCS5 padding
			const char padding = AES_BLOCK_SIZE - (Msg.size() % AES_BLOCK_SIZE);
			Msg.append(padding, padding);
		}
		else
		{
			int n = Msg.size() % AES_BLOCK_SIZE;
			if (n)
				Msg.append(AES_BLOCK_SIZE - n, '\x00');
		}

		AES_set_encrypt_key(reinterpret_cast<const unsigned char *>(szKey), bit, &AesKey);

		int round = Msg.size() / AES_BLOCK_SIZE;
		const unsigned char *in = reinterpret_cast<const unsigned char *>(Msg.c_str());

		while (round--)
		{
			unsigned char tmp [AES_BLOCK_SIZE];
			AES_ecb_encrypt(in, tmp, &AesKey, AES_ENCRYPT);
			ret.append(reinterpret_cast<char*>(tmp), AES_BLOCK_SIZE);
			in += AES_BLOCK_SIZE;
		}

		return ret;
	}
	static string AesDecrypt( const string & Msg, const char * szKey, int bit )
	{
		AES_KEY AesKey;
		string ret;

		AES_set_decrypt_key(reinterpret_cast<const unsigned char *>(szKey), bit, &AesKey);

		int round = Msg.size() / AES_BLOCK_SIZE;
		const unsigned char *in = reinterpret_cast<const unsigned char *>(Msg.c_str());

		while (round--)
		{
			unsigned char tmp [AES_BLOCK_SIZE];
			AES_ecb_encrypt(in, tmp, &AesKey, AES_DECRYPT);
			ret.append(reinterpret_cast<char*>(tmp), AES_BLOCK_SIZE);
			in += AES_BLOCK_SIZE;
		}

		return ret;
	}
#endif // OPENSSL_NO_AES
#ifndef OPENSSL_NO_DES
	static string DesEncrypt( string Key, string Msg)
	{
		if (Msg.empty())
			return "";
		DES_cblock Key2;
		DES_key_schedule schedule;

		/* Prepare the key for use with DES_cfb64_encrypt */
		Key.resize(8);
		memcpy( Key2, Key.c_str(), 8);

		DES_set_odd_parity( &Key2 );
		if (DES_set_key_checked( &Key2, &schedule ) == 0)
		{
			// PKCS5 padding
			const char padding = 8 - (Msg.size() % 8);
			Msg.append(padding, padding);
			// reuse Key as output
			Key.resize(Msg.size());

			const_DES_cblock *input = (const_DES_cblock *)Msg.c_str();
			DES_cblock *output = (DES_cblock *)Key.c_str();

			int round = Msg.size() / 8;
			while (round--)
			{
				DES_ecb_encrypt(input, output, &schedule, DES_ENCRYPT);
				input++;
				output++;
			}
		}
		else
			Key.clear();
		return Key;
	}
#endif // OPENSSL_NO_DES
#ifndef OPENSSL_NO_RSA
	static string RsaEncrypt( EVP_PKEY * pubkey, const string & Msg, int pad = RSA_PKCS1_PADDING )
	{
		string ret;
		EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
		if (ctx)
		{
			if (EVP_PKEY_encrypt_init(ctx) > 0)
			{
				if (EVP_PKEY_CTX_set_rsa_padding(ctx, pad)> 0)
				{
					unsigned char *in = (unsigned char *)Msg.c_str();
					size_t outlen = 0, inlen = Msg.length();

					if (EVP_PKEY_encrypt(ctx, NULL, &outlen, in, inlen) > 0)
					{
						unsigned char * out = (unsigned char*)OPENSSL_malloc(outlen);
						if (out)
						{
							memset(out, 0, outlen);
							if (EVP_PKEY_encrypt(ctx, out, &outlen, in, inlen) > 0)
							{
								ret.assign(reinterpret_cast<char*>(out), outlen);
							}
							OPENSSL_free(out);
						}
					}
				}
			}
			EVP_PKEY_CTX_free(ctx);
		}
		return ret;
	}
	static string RsaDecrypt( EVP_PKEY * pubkey, const string & Msg, int pad = RSA_PKCS1_PADDING )
	{
		string ret;
		EVP_PKEY_CTX *ctx = EVP_PKEY_CTX_new(pubkey, NULL);
		if (ctx)
		{
			if (EVP_PKEY_decrypt_init(ctx) > 0)
			{
				if (EVP_PKEY_CTX_set_rsa_padding(ctx, pad)> 0)
				{
					unsigned char *in = (unsigned char *)Msg.c_str();
					size_t outlen = 0, inlen = Msg.length();

					if (EVP_PKEY_decrypt(ctx, NULL, &outlen, in, inlen) > 0)
					{
						unsigned char * out = (unsigned char*)OPENSSL_malloc(outlen);
						if (out)
						{
							memset(out, 0, outlen);
							if (EVP_PKEY_decrypt(ctx, out, &outlen, in, inlen) > 0)
							{
								ret.assign(reinterpret_cast<char*>(out), outlen);
							}
							OPENSSL_free(out);
						}
					}
				}
			}
			EVP_PKEY_CTX_free(ctx);
		}
		return ret;
	}
	static string RsaEncrypt(const string & PemCert, const string & Msg, int pad = RSA_PKCS1_PADDING)
	{
		string ret;
		if (Msg.empty() || PemCert.empty())
			return ret;
		BIO *b = BIO_new(BIO_s_mem());
		if (b)
		{
			if (BIO_write(b, PemCert.c_str(), PemCert.size())>0)
			{
				X509 *x = PEM_read_bio_X509(b, NULL, NULL, NULL);
				if (x)
				{
					EVP_PKEY *pubkey = X509_get_pubkey(x);
					if (pubkey)
					{
						ret = RsaEncrypt(pubkey, Msg, pad);
						EVP_PKEY_free(pubkey);
					}
					X509_free(x);
				}
			}
			BIO_free(b);
		}
		return ret;
	}
#endif // OPENSSL_NO_RSA
	static string Base64Encode(const string & Msg)
	{
		string ret;
		if (Msg.size())
		{
			unsigned char * outbase64 = (unsigned char*)OPENSSL_malloc(Msg.size() * 2);
			if (outbase64)
			{
				int n = EVP_EncodeBlock(outbase64, reinterpret_cast<const unsigned char *>(Msg.c_str()), Msg.size());
				if (n>0)
				{
					ret.assign((char*)outbase64, n);
				}
				OPENSSL_free(outbase64);
			}
		}
		return ret;
	}
	static string Base64Decode(const string & Msg)
	{
		string ret;
		if (Msg.size())
		{
			unsigned char * outbase64 = (unsigned char*)OPENSSL_malloc(Msg.size() * 2);
			if (outbase64)
			{
				int n = EVP_DecodeBlock(outbase64, reinterpret_cast<const unsigned char *>(Msg.c_str()), Msg.size());
				if (n>0)
				{
					// fix real size
					string::const_iterator it = Msg.end();
					it--;
					while (*it == '=')
					{
						n--;
						it--;
					}
					ret.assign((char*)outbase64, n);
				}
				OPENSSL_free(outbase64);
			}
		}
		return ret;
	}
#ifndef OPENSSL_NO_MD5
	static string Md5Hash(const string & Msg, bool bLower = true, bool bHex = true)
	{
		string ret;
		if (Msg.size())
		{
			MD5_CTX ctx;
			unsigned char md[16];

			MD5_Init(&ctx);
			MD5_Update(&ctx, Msg.c_str(), Msg.size());
			MD5_Final(md,&ctx);

			if (bHex)
			{
				char tmp[3]={'\0'};
				const char * szFormat;
				if (bLower)
					szFormat = "%02x";
				else
					szFormat = "%02X";
				for( int i=0; i<16; i++ )
				{
#if defined(_WINDOWS) || defined(_WIN32)
					_snprintf_s(tmp, 3, szFormat, md[i]);
#else
					snprintf(tmp, 3, szFormat, md[i]);
#endif
					ret.append(tmp);
				}
			}
			else
			{
				ret.assign(reinterpret_cast<char*>(md), 16);
			}
		}
		return ret;
	}
#endif // OPENSSL_NO_MD5
#ifndef OPENSSL_NO_SHA1
	static string Sha1Hash(const string & Msg, bool bHex = true, bool bLower = true)
	{
		string ret;
		if (Msg.size())
		{
			unsigned char md[20];
			SHA1(reinterpret_cast<const unsigned char *>(Msg.c_str()), Msg.size(), md);
			if (bHex)
			{
				char tmp[3] = { '\0' };
				const char * szFormat;
				if (bLower)
					szFormat = "%02x";
				else
					szFormat = "%02X";
				for (int i = 0; i < 20; i++)
				{
#if defined(_WINDOWS) || defined(_WIN32)
					_snprintf_s(tmp, 3, szFormat, md[i]);
#else
					snprintf(tmp, 3, szFormat, md[i]);
#endif
					ret.append(tmp);
				}
			}
			else
			{
				ret.assign(reinterpret_cast<char*>(md), 20);
			}
		}
		return ret;
	}
#endif // OPENSSL_NO_SHA1
};

class CBio
{
public:
	CBio(BIO_METHOD * bm)
	{
		p = BIO_new(bm);
	}
	CBio(BIO * b)
	{
		p = b;
	}
	~CBio()
	{
		BIO_free(p);
	}
	operator BIO * const()
	{
		return p;
	}
private:
	BIO * p;
};

/*
string base64_encode( const string & data )
{
	CBio b64(BIO_f_base64());
	BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
	CBio mem(BIO_s_mem());
	BIO_push(b64, mem);
	while (true)
	{
		int res = BIO_write(b64, data.c_str(), data.size());
		if (res <= 0) // if failed
		{
			if (BIO_should_retry(b64))
				continue;
			else
				return "";
		}
		else
			break;
	}

	BIO_flush(b64);

	char* dt;
	long len = BIO_get_mem_data(mem, &dt);
	return std::string(dt, len);
}

string base64_decode( const string & data )
{
	string in,out;
	out.resize(data.size());
	in = data;
	in.append(1, '\n');
	CBio b64(BIO_f_base64());
	CBio mem(BIO_new_mem_buf(const_cast<char*>(in.c_str()), in.size()));
	BIO_push(b64, mem);
	out.resize(BIO_read(b64, const_cast<char*>(out.c_str()), out.size()));
	return out;
}
*/


#endif // OPENSSL_HELPER_H__
