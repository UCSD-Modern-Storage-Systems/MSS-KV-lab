#ifndef __PMKV_H
#define __PMKV_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {} pmkv;

pmkv* pmkv_open(const char *path, size_t pool_size, int force_create);
void pmkv_close(pmkv *kv);
void* pmkv_get(pmkv *kv, const char *key);
void pmkv_put(pmkv *kv, const char *key, const char *value);
void pmkv_del(pmkv *kv, const char *key);

#ifdef __cplusplus
}
#endif
#endif
