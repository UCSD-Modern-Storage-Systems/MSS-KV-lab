#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <libpmemobj.h>

#define LAYOUT "example"
#define POOL_SIZE 1024*1024*1024

int main(int argc, char **argv)
{
	PMEMobjpool* pop;
	const char *path;
	int ret;

	if (argc < 2) {
		printf("Usage: %s poolfile\n", argv[0]);
		exit(1);
	}

	path = argv[1];

	// pool creation
	pop = pmemobj_open(path, LAYOUT);
	if (!pop) {
		pop = pmemobj_create(path, LAYOUT, POOL_SIZE, 0777);
		if (!pop) {
			printf("Error: failed to create a pool at %s (%lu): %s\n", path, (size_t)POOL_SIZE, strerror(errno));
			exit(1);
		}
	}

	// pool deletion
	pmemobj_close(pop);

	// pool consistency check
	ret = pmemobj_check(path, LAYOUT);
	if (!ret) {
		printf("Error: pool is not consistent.\n");
		exit(1);
	} else if (ret < 0) {
		printf("Error: pmemobj_check failed: %s\n", strerror(errno));
		exit(1);
	}


	return 0;
}
