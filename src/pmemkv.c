#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "pmkv.h"
#include "libpmemkv.h"

#define MAX_VAL_LEN 64

pmkv* pmkv_open(char *path, size_t pool_size, int force_create)
{
	pmemkv_config *cfg = pmemkv_config_new();
	assert(cfg != NULL);

	int s = pmemkv_config_put_string(cfg, "path", path);
	assert(s == PMEMKV_STATUS_OK);
	s = pmemkv_config_put_uint64(cfg, "size", pool_size);
	assert(s == PMEMKV_STATUS_OK);
	s = pmemkv_config_put_uint64(cfg, "force_create", force_create);
	assert(s == PMEMKV_STATUS_OK);

	pmemkv_db *db = NULL;
	s = pmemkv_open("cmap", cfg, &db);
	assert(s == PMEMKV_STATUS_OK);
	assert(db != NULL);

	return (pmkv*)db;
}

void* pmkv_get(pmkv *kv, const char *key)
{
	pmemkv_db *db = (pmemkv_db*)kv;
	char *val = malloc(MAX_VAL_LEN);
	int s = pmemkv_get_copy(db, key, strlen(key), val, MAX_VAL_LEN, NULL);
	assert(s == PMEMKV_STATUS_OK);
	return val;
}

void pmkv_put(pmkv *kv, const char *key, const char *value)
{
	pmemkv_db *db = (pmemkv_db*)kv;
	int s = pmemkv_put(db, key, strlen(key), value, strlen(value));
	assert(s == PMEMKV_STATUS_OK);
}

void pmkv_del(pmkv *kv, const char *key)
{
	pmemkv_db *db = (pmemkv_db*)kv;
	int s = pmemkv_remove(db, key, strlen(key));
	assert(s == PMEMKV_STATUS_OK);
}

void pmkv_close(pmkv *kv)
{
	pmemkv_close((pmemkv_db*)kv);
}
