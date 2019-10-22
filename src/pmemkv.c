#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "pmkv.h"
#include "libpmemkv.h"

pmkv* pmkv_open(const char *path, size_t pool_size, int force_create)
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

void pmkv_close(pmkv *kv)
{
	pmemkv_close((pmemkv_db*)kv);
}

int pmkv_get(pmkv *kv, const char *key, size_t key_size, char *val, size_t *out_val_size)
{
	pmemkv_db *db = (pmemkv_db*)kv;
	return pmemkv_get_copy(db, key, key_size, val, MAX_VAL_LEN, out_val_size);
}

int pmkv_put(pmkv *kv, const char *key, size_t key_size, const char *val, size_t val_size)
{
	pmemkv_db *db = (pmemkv_db*)kv;
	return pmemkv_put(db, key, key_size, val, val_size);
}

int pmkv_delete(pmkv *kv, const char *key, size_t key_size)
{
	pmemkv_db *db = (pmemkv_db*)kv;
	return pmemkv_remove(db, key, key_size);
}

int pmkv_count_all(pmkv *kv, size_t *out_cnt)
{
	pmemkv_db *db = (pmemkv_db*)kv;
	return pmemkv_count_all(db, out_cnt);
}

int pmkv_exists(pmkv *kv, const char *key, size_t key_size)
{
	pmemkv_db *db = (pmemkv_db*)kv;
	int s = pmemkv_exists(db, key, key_size);
	return s == PMEMKV_STATUS_OK ? 1 : 0;
}
