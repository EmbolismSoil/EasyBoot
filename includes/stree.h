#ifndef __SEARCH_STREE_H__
#define __SERRCH_STREE_H__

#define IS_ROOT -2

#include <linux/stddef.h>
        
typedef enum{gt,lt,eq}node_logic;

typedef struct __stree_node{
	struct __stree_node *lh,*rh,*parent;
}stree_node;

typedef node_logic (*cmp_t)(stree_node *, void *);

typedef struct{
    cmp_t   cmp;
	void* key;
}node_cmp;

extern stree_node *
stree_search(stree_node *root, const node_cmp *key_cmp);

extern int stree_add(stree_node *node,stree_node *root,const node_cmp *key_cmp);
extern int stree_del(stree_node *node);

extern int node_move(stree_node *node, 
		stree_node* root,const node_cmp *key_cmp);
static inline int stree_lh_empty(stree_node *root)
{
	return root->lh == NULL;
}
static inline int stree_rh_empty(stree_node *root)
{
	return root->rh == NULL;
}

static inline int is_node_lh(stree_node *root,stree_node *node)
{
	return root->lh == node;
}

static inline int is_node_rh(stree_node *root,stree_node *node)
{
	return root->rh == node;
}

#define container_of(ptr, type, member) ({                      \
        const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
        (type *)( (char *)__mptr - offsetof(type,member) );})
        
#define stree_node_entry(ptr, type, member) \
	container_of(ptr, type, member)
	
static inline void INIT_STREE_NODE(stree_node *node) \
{
	node->parent = NULL;
	node->lh = NULL;
	node->rh = NULL;
}
	
#endif
