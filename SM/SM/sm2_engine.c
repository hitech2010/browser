#include <openssl/engine.h>
#include "SM.h"
#include "sm2.h"

/* Initiator which is only present to make sure this engine looks available */
int init(ENGINE *e)
{
	// load string
	//ERR_load__strings();

	return 1;
}

/* Finisher which is only present to make sure this engine looks available */
int finish(ENGINE *e)
{
	return 1;
}

/* Destructor (complements the "ENGINE_ncipher()" constructor) */
int destroy(ENGINE *e)
{
	// unload string
	//ERR_unload__strings();
	return 1;
}

int sm2_nids[1] = {EVP_PKEY_EC};
//EVP_PKEY_METHOD * sm2_pmeth = NULL;

int pkey_meths(ENGINE * e, EVP_PKEY_METHOD ** pmeth, const int ** nids, int id)
{
	if (nids)
		*nids = sm2_nids;
	if (pmeth)
	{
		if (id == EVP_PKEY_EC)
		{
			*pmeth = sm2_pmeth_new();
		}
		else
			*pmeth = NULL;
	}
	return 1;
}

/*
int pkey_asn1_meths(ENGINE *e, EVP_PKEY_ASN1_METHOD **pameth, const int ** nids, int id)
{
	if (nids)
		*nids = sm2_nids;
	if (pameth)
	{
		if (id == EVP_PKEY_EC)
		{
			*pameth = sm2_pameth_new();
		}
		else
			*pameth = NULL;
	}
	return 1;
}
*/

static int bind_sm2(ENGINE *e)
{
	if (!e)
		return 0;
	if(!ENGINE_set_id(e, "sm2")
		|| !ENGINE_set_name(e, "sm2 support")
//		|| !ENGINE_set_ciphers(e, ciphers)
//		|| !ENGINE_set_digests(e, digests)
		|| !ENGINE_set_destroy_function(e, destroy)
		|| !ENGINE_set_init_function(e, init)
		|| !ENGINE_set_finish_function(e, finish)
//		|| !ENGINE_set_load_privkey_function(e, load_prikey)
//		|| !ENGINE_set_load_pubkey_function(e, load_pubkey)
// 		|| !ENGINE_set_pkey_asn1_meths(e, pkey_asn1_meths)
		|| !ENGINE_set_pkey_meths(e, pkey_meths)
//		|| !ENGINE_set_ctrl_function(e, ctrl)
//		|| !ENGINE_set_cmd_defns(e, cmd_defns)
		)
		return 0;

	/* Ensure the token error handling is set up */
//	TOKEN::ERR_load_strings();

	return 1;
}

ENGINE *engine_sm2(void)
{
	ENGINE *ret = ENGINE_new();
	if (ret)
	{
		if (!bind_sm2(ret))
		{
			ENGINE_free(ret);
			return NULL;
		}
	}
	return ret;
}

void ENGINE_load_sm2(void)
{
	ENGINE * toadd = engine_sm2();
	if (toadd)
	{
		ENGINE_add(toadd);
//		ENGINE_register_pkey_asn1_meths(toadd);
		ENGINE_free(toadd);
		ERR_clear_error();
	}
}