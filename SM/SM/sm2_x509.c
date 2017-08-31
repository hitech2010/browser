#include "SM.h"
#include "sm2.h"

EC_KEY * EC_KEY_from_X509(X509 * x509)
{
	ASN1_BIT_STRING * p = X509_get0_pubkey_bitstr(x509);
	BIGNUM * x = BN_bin2bn(p->data+1, 32, NULL);
	BIGNUM * y = BN_bin2bn(p->data+33, 32, NULL);
	EC_KEY * sm2 = EC_KEY_from_BN(x, y, NULL);
	if (x) BN_free(x);
	if (y) BN_free(y);
	return sm2;
}
