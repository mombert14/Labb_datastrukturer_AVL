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
		return balance_factor(get_LC(T)) >= 0 ? srr(T) : drr(T);
	
	if(bf < -1)
		return balance_factor(get_RC(T)) <= 0 ? slr(T) : dlr(T);

	return T;
}
//=============================================================================
// Private functions, for local use only
//-----------------------------------------------------------------------------
static AVL srr(AVL T) // Single right rotation
{
	if(DEBUG)printf("srr\n");
	AVL L = get_LC(T);
	AVL LR = get_RC(L);

	set_LC(T,LR);
	set_RC(L,T);
	return L;
}
static AVL slr(AVL T) // single left rotation
{
	if(DEBUG)printf("slr\n");
	AVL R = get_RC(T);
	AVL RL = get_LC(R);

	set_RC(T, RL);
	set_LC(R, T);
	
	return R;
}
static AVL drr(AVL T) // Double right rotation
{
	if(DEBUG)printf("drr\n");
	AVL L = get_LC(T);
	L = slr(L);

	set_LC(T, L);
	return srr(T);
}
static AVL dlr(AVL T) // Double left rotation
{
	AVL R = get_RC(T);
	R = srr(R);

	set_RC(T, R);
	return slr(T);
}

static int balance_factor(AVL T)
{
	if(!T)return 0;
	return height(get_LC(T)) - height(get_RC(T));
}
