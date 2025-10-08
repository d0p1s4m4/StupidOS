#ifndef MTREE_H
# define MTREE_H 1

# include <sys/types.h>
# include <stdint.h>

# define NODE_PARENT(n)       ((n)->tree.parent)
# define NODE_CHILD_FIRST(n)  ((n)->tree.child)
# define NODE_CHILD_ADD(p, c) do {							\
		(c)->tree.parent = p;							\
		if (((c)->list.next = (p)->tree.child) != NULL) { 	\
			(p)->tree.child->list.prev = &(c)->list.next;	\
		}													\
		(p)->tree.child = (c);								\
		(c)->list.prev = (p)->tree.child;					\
	} while(0);

enum flags {
	FL_TYPE = (1<<0),
	FL_MODE = (1<<1),
	FL_UID  = (1<<2),
	FL_GID  = (1<<3),
};

struct node {
	struct {
		struct node *parent;
		struct node *child;
	} tree;
	struct {
		struct node *next;
		struct node **prev;
	} list;

	uint32_t flags;
	mode_t mode;
	uid_t uid;
	gid_t gid;
	char *name;
};

/* node.c */
struct node *node_new(void);
void node_free(struct *node);
void node_dump(struct *node);

#endif /* !MTREE_H */
