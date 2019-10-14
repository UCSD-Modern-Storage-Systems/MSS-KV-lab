#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "pmkv.h"

int main(int argc, char *argv[])
{
	if (argc < 3) {
		fprintf(stderr, "Usage: ./%s kvfile size\n", argv[0]);
		exit(1);
	}
	char *path = argv[1];
	size_t pool_size = atol(argv[2]);

	pmkv *kv = pmkv_open(path, pool_size);
	pmkv_put(kv, "key1", "value1");
	char *val = (char*)pmkv_get(kv, "key1");
	assert(!strcmp(val, "value1"));
	//pmemkv_del(kv, "key1");
	pmkv_close(kv);
	return 0;
}
