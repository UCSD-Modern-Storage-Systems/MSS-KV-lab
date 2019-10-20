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
	int num_elements;
	TOID(struct list_elem) head;
	TOID(struct list_elem) tail;
};

struct list_elem {
	int val;
	TOID(struct list_elem) next;
};

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

	/**********************************************
	* Create the head object of the linked list
	***********************************************/

	// allocate a head object (which is also the root object)
	TOID(struct list_head) root = POBJ_ROOT(pop, struct list_head);
	assert(!TOID_IS_NULL(root));
	
	/**********************************************
	* Append a new node to the linked list
	***********************************************/

	TX_BEGIN(pop) {

		// get tail
		TOID(struct list_elem) tail = D_RW(root)->tail;

		// allocate a elem object
		TOID(struct list_elem) node;
		node = TX_ZNEW(struct list_elem);

		// write value and persist it.
		D_RW(node)->val = 100;
		//pmemobj_persist(pop, &D_RW(node)->val, sizeof(int));

		// undo log the root
		TX_ADD(root);

		if (!TOID_IS_NULL(tail)) {
			// undo log the tail
			TX_ADD(tail);
			// link the new elem to the tail
			D_RW(tail)->next = node;
		} else {
			D_RW(root)->head = node;
		}

		// update the tail
		D_RW(root)->tail = node;

		// increment the list count in head
		D_RW(root)->num_elements++;

	} TX_ONCOMMIT {
		// execute on commit
		printf("A new object is appended to the linked list!\n");

	} TX_ONABORT {
		// execute on abort
		printf("Error: failed to append a new object to the linked list.\n");

	} TX_FINALLY {
		// execute this anyway
	} TX_END

	// scan all elements in the list
	TOID(struct list_elem) tmp = D_RO(root)->head;
	printf("linked list: ");
	while (!TOID_IS_NULL(tmp)) {
		int val = D_RO(tmp)->val;
		printf("%d ", val);
		tmp = D_RO(tmp)->next;
	}
	printf("\n");

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
