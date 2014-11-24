#include <linux/list>

/*这是类似与内核链表的通用数据结构，我并不强制这个查找树存放的是什么内容，用户仅仅需要将其放入自己的结构中即可组成自己的查找树，用户需要作的事情就是，定义一个判断节点的函数，然后将stree_node加入自己的结构中即可*/



typedef enum{gt,lt,eq}node_logic;

struct stree_node{
	struct stree_node *lh,*rh;
};

static typedef node_logic (*cmp_t)(stree_node *, void *);

typedef struct{
    cmp_t   cmp;
	void* arg;
}node_cmp;

static inline int stree_lh_empty(stree_node *root)
{
	return root->lh == NULL;
}
static inline int stree_lh_empty(stree_node *root)
{
	return root->rh == NULL;
}

static inline int stree_rh_empty(stree_node *root)
{
	return list_empty(&root->rh);
}

#define stree_node_entry(ptr, type, member) \
	container_of(ptr, type, member)
	
static stree_node *
stree_search(stree_node *root, node_cmp *key_cmp)
{
	DEBUG_CHECK(!root || !key_cmp, NULL);
	
   stree_node *tmp;
   while (root){
       switch (key_cmp(root,key_cmp->arg)){
       	case gt :
       		root = root->rh;
       		break;
       	case lt :
       		root = root->lh;
       	case eq :
       		return root;
       }
   }
   return NULL;   	
}

static stree_node* 
stree_add(stree_node *root, node_cmp *key_cmp)
{
    DEBUG_CHECK(!root || !key_cmp, NULL);
	stree_node*  tmp = stree_search(root,key_cmp);
	if (tmp){
		
	}else
}

