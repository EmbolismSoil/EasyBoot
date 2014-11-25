#include <linux/list.h>
#include <search_stree.h>
#include <debug.h>
/*这是类似与内核链表的通用数据结构，我并不强制这个查找树存放的是什么内容，用户仅仅需要将其放入自己的结构中即可组成自己的查找树，用户需要作的事情就是，定义一个判断节点的函数，然后将stree_node加入自己的结构中即可*/

/*
 * @function : stree_search ,search an key int the binary serach tree.key_cmp
 * is an method which define by user to decide which node is fit to the key 
 */	
stree_node *
stree_search(stree_node *root, const node_cmp *key_cmp)
{
	DEBUG_CHECK(!root || !key_cmp, NULL);
	
   stree_node *tmp;
   while (root){
       switch (key_cmp->cmp(root,key_cmp->key)){
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

static stree_node **
stree_modifiled_search(stree_node *root, const node_cmp *key_cmp)
{
    DEBUG_CHECK(!root || !key_cmp, NULL);
	
   stree_node *tmp;
   while (root){
       switch (key_cmp->cmp(root,key_cmp->key)){
       	case gt :
       		if (!root->rh)
       			return &(root->rh);
       		root = root->rh;
       		break;
       	case lt :
       		if (!root->lh)
       			return &(root->lh);
       		root = root->lh;
       	case eq :
       		return NULL;
       }
   }
   return NULL;   	
}

int stree_add(stree_node *node,stree_node *root,const node_cmp *key_cmp)
{
    DEBUG_CHECK(!root || !key_cmp, -1);
	stree_node**  tmp = stree_modifiled_search(root,key_cmp);
	if (tmp){
		*tmp = node;
	}else return -1;
	
	return 0;
}


int stree_del(stree_node *node)
{
	DEBUG_CHECK(!node->parent || !node, -1);
	
	stree_node *ch = node->lh;
	if (!ch){
		if(is_node_lh(node->parent,node)){
			node->parent->lh = node->rh;
			return 0;
		}	
	    node->parent->rh = node;
	    return 0;
	}
	
	if(ch->rh){
	    while (ch->rh)
	    	ch = ch->rh;
	    if(is_node_lh(node->parent,node))
	    	node->parent->lh = ch;
	    node->parent->rh = ch;
	    
	    ch->parent->rh = ch->lh;
		ch->lh = node->lh;
		ch->rh = node->rh;
		return 0;
	}else{
		if(is_node_lh(node->parent,node))
	    	node->parent->lh = ch;
	    node->parent->rh = ch;
	    ch->rh = node->rh;
	    return 0;
	}
	
	return -1;
}

/*
 * @function : move an node to anther tree
 */
int node_move(stree_node *node, 
		stree_node* root,const node_cmp *key_cmp)
{
	DEBUG_CHECK(!node || !root || !key_cmp, -1);
	stree_del(node);
	stree_add(node, root, key_cmp);
}
