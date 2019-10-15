#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "pmkv.h"

#define MAX_VAL_LEN 64

pmkv* pmkv_open(const char *path, size_t pool_size, int force_create)
{
	// TODO: implement this
	return NULL;
}

void* pmkv_get(pmkv *kv, const char *key)
{
	// TODO: implement this
	return NULL;
}

void pmkv_put(pmkv *kv, const char *key, const char *value)
{
	// TODO: implement this
}

void pmkv_del(pmkv *kv, const char *key)
{
	// TODO: implement this
}

void pmkv_close(pmkv *kv)
{
	// TODO: implement this
}

void pmkv_count_all(pmkv *kv, size_t *cnt)
{
	// TODO: implement this
}

int pmkv_exists(pmkv *kv, const char *key)
{
	// TODO: implement this
	return 0;
}
