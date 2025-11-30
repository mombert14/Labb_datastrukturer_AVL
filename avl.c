//=============================================================================
// avl.c - AVL-tree based on binary search tree (bst.h/bst.c)
//=============================================================================
#include "avl.h"
#define DEBUG 0
//=============================================================================
// local prototypes
//-----------------------------------------------------------------------------
static AVL srr(AVL T);
static AVL slr(AVL T);
static AVL drr(AVL T);
static AVL dlr(AVL T);
static int balance_factor(AVL T);
//=============================================================================
// Public functions, exported via .h-file
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
// avl_add: adds the value val to AVL T in a balanced fashion
//-----------------------------------------------------------------------------
AVL avl_add(AVL T, int val)
{
	if(DEBUG)printf("avl_add (%d)\n",val);
	T = bst_add(T, val);
	return balance(T);
	return T;
}
//-----------------------------------------------------------------------------
// avl_rem: removes the value val from AVL T in a balanced fashion
//-----------------------------------------------------------------------------
AVL avl_rem(AVL T, int val)
{
	if(DEBUG)printf("avl_rem (%d)\n",val);
	T = bst_rem(T, val);
	return balance(T);
}
//-----------------------------------------------------------------------------
// balance: balances the AVL tree T if needed
//-----------------------------------------------------------------------------
AVL balance(AVL T)
{
	if(!T) return NULL;

	int bf = balance_factor(T);

	if(bf > 1)
		return balance_factor(T->LC) >= 0 ? srr(T) : drr(T);
	
	if(bf < -1)
		return balance_factor(T->RC) <= 0 ? slr(T) : dlr(T);

	return T;
}
//=============================================================================
// Private functions, for local use only
//-----------------------------------------------------------------------------
static AVL srr(AVL T) // Single right rotation
{
	if(DEBUG)printf("srr\n");
	AVL L = T->LC;
	T->LC = L->RC;
	L->RC = T;
	return L;
}
static AVL slr(AVL T) // single left rotation
{
	if(DEBUG)printf("slr\n");
	AVL R = T ->RC;
	T->RC = R->LC;
	R->LC = T;
	return R;
}
static AVL drr(AVL T) // Double right rotation
{
	if(DEBUG)printf("drr\n");
	T->LC = slr(T->LC);
	return srr(T);
}
static AVL dlr(AVL T) // Double left rotation
{
	if(DEBUG)printf("drr\n");
	T->RC = srr(T->RC);
	return slr(T);
}

static int balance_factor(AVL T)
{
	if(!T)return 0;
	return height(T->LC) - height(T->RC);
}
