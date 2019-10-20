#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <libpmemobj.h>

#define LAYOUT "linkedlist"
#define POOL_SIZE 1024*1024*1024

POBJ_LAYOUT_BEGIN(linkedlist);
POBJ_LAYOUT_ROOT(linkedlist, struct list_head);
POBJ_LAYOUT_TOID(linkedlist, struct list_elem);
POBJ_LAYOUT_END(linkedlist);

struct list_head {
	int val;
	TOID(struct list_elem) next;
};

struct list_elem {
	int val;
	PMEMoid next;
};

static int 
elem_constructor(PMEMobjpool *pop, void *ptr, void *arg)
{
        struct list_elem *obj = (struct list_elem*)ptr;
	memset(obj, 0, sizeof(*obj));
        pmemobj_persist(pop, obj, sizeof(*obj));

        return 0;
}

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
		printf("Pool %s is created with size %lu.\n", path, (size_t)POOL_SIZE);
	} else {
		printf("Pool %s is opened.\n", path);
	}

	// allocate a list_elem object
	PMEMoid elem;
	ret = pmemobj_alloc(pop, &elem, sizeof(struct list_elem), 1/*type_num*/, elem_constructor, NULL);
	assert(ret == 0);

	// free the list_elem object
	pmemobj_free(&elem);

	// pool deletion
	pmemobj_close(pop);
	printf("Pool %s is closed.\n", path);

	// pool consistency check
	ret = pmemobj_check(path, LAYOUT);
	if (ret == 1) {
		printf("Pool %s is consistent.\n", path);
	} else if (ret == 0) {
		printf("Error: pool is not consistent.\n");
		exit(1);
	} else {
		printf("Error: pmemobj_check failed: %s\n", strerror(errno));
		exit(1);
	}

	return 0;
}
