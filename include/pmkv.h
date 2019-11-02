#ifndef __PMKV_H
#define __PMKV_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_VAL_LEN 1048576

typedef struct {} pmkv;

pmkv* pmkv_open(const char *path, size_t pool_size, int force_create);
void pmkv_close(pmkv *kv);
int pmkv_get(pmkv *kv, const char *key, size_t key_size, char *out_val, size_t *out_val_size);
int pmkv_put(pmkv *kv, const char *key, size_t key_size, const char *val, size_t val_size);
int pmkv_delete(pmkv *kv, const char *key, size_t key_size);
int pmkv_count_all(pmkv *kv, size_t *out_cnt);
int pmkv_exists(pmkv *kv, const char *key, size_t key_size);

#ifdef __cplusplus
}
#endif
#endif
