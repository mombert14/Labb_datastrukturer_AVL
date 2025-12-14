//=============================================================================
// bst.c - binary search tree, based on binary tree (bt.h/bt.c)
//=============================================================================
#include "global.h"
#include "bst.h"

//-----------------------------------------------------------------------------
// local function prototypes
//-----------------------------------------------------------------------------
static void _preorder(BST T, int* pos, int* a);

static void _inorder(BST T, int* pos, int* a);

static void _postorder(BST T, int* pos, int* a);

BST getSuccessor(BST T);
BST getPredecessor(BST T);
//-----------------------------------------------------------------------------
// public functions, exported through bst.h
//-----------------------------------------------------------------------------
// new_BST: creates a new BST
BST new_BST(int val)
{
	return new_BT(val);
}
//-----------------------------------------------------------------------------
// bst_add: adds the value v to the BST T (unless duplicate)
//-----------------------------------------------------------------------------
BST bst_add(BST T, int v)
{

	return	!T          ?	new_BST(v)                            :
		v < get_val(T)	?	cons(add(get_LC(T), v), T, get_RC(T)) :
		v > get_val(T)	?	cons(get_LC(T), T, add(get_RC(T), v)) :
		/* duplicate */		T;
}
//-----------------------------------------------------------------------------
// bst_rem: removes the value val from the BST (if it exists)
//-----------------------------------------------------------------------------

BST bst_rem(BST T, int val)
{
    BST left = get_LC(T);
    BST right = get_RC(T);
	// Basfall 1: Trädet är tomt eller värdet finns inte.
    if (T == NULL) {
        return T;
    }

    if (val< get_val(T))
    {
        T->LC = bst_rem(left, val);
    }
    else if (val >get_val(T))
    {
        T->RC = bst_rem(right, val);       
    }
    else{

        if (!left)
        {
            BST temp = get_RC(T);
            free(T);
            return temp;
        }
        else if (!right)
        {
            BST temp = get_LC(T);
            free(T);
            return temp;
        }
        else{
            if (height(left) >= height(right))
            {
                printf("trying to remove");
                BST predecessor = getPredecessor(T);
                set_val(T, get_val(predecessor));
                T->LC = bst_rem(get_LC(T), get_val(predecessor));

            }
            else{
                printf("trying to remove");
                BST successor = getSuccessor(T);
                set_val(T, get_val(successor));
                T->RC = bst_rem(get_RC(T), get_val(successor));  
            } 
        }     
    }
    return T;
}
    
BST getSuccessor(BST T){
    T = get_RC(T);
    while (T != NULL && get_LC(T)!= NULL)
    {
        T = get_LC(T);
    }
    return T;
    
}

BST getPredecessor(BST T){
    T = get_LC(T);
    while (get_RC(T))
    {
        T = get_RC(T);
    }
    return T;
}
//-----------------------------------------------------------------------------
// preorder: puts the BST T values into array a in preorder
//-----------------------------------------------------------------------------
//   2                    2
//  / \	--> [2,1,3]      / \  --> [2,3]
// 1  3                 *  3
//-----------------------------------------------------------------------------
void preorder(BST T, int* a)
{
	int pos = 0;
	_preorder(T, &pos, a);
}


//-----------------------------------------------------------------------------
// inorder: puts the BST T values into array a in inorder
//-----------------------------------------------------------------------------
//   2                    2
//  / \	--> [1,2,3]      / \  --> [2,3]
// 1  3                 *  3
//-----------------------------------------------------------------------------
void inorder(BST T, int* a)
{
	int pos = 0;
	_inorder(T, &pos, a);
}
//-----------------------------------------------------------------------------
// postorder: puts the BST T values into array a in postorder
//-----------------------------------------------------------------------------
//   2                    2
//  / \	--> [1,3,2]      / \  --> [3,2]
// 1  3                 *  3
//-----------------------------------------------------------------------------
void postorder(BST T, int* a)
{
	int pos = 0;
	_postorder(T, &pos, a);
}
//-----------------------------------------------------------------------------
// bfs: puts the BST T values into array a in bfs-order, non-nodes
// are indicated by X, as defined in global.h
//-----------------------------------------------------------------------------
//   2                    2
//  / \	--> [2,1,3]      / \  --> [2,X,3]
// 1  3                 *  3
//-----------------------------------------------------------------------------
void bfs(BST T, int* a, int max)
{
    
    BST queue[MAX]; 

    // Fyll utmatnings-arrayen med non-value X (t.ex. -42)
    for (int i = 0; i < max; i++) {
        a[i] = X;
    }

    // Basfall: Om trädet är tomt, gör ingenting (arrayen är redan fylld med X)
    if (T == NULL) {
        return;
    }
    int elements = 0 ;
    int front= 0;
    queue[0] = T;
    
    elements ++;
    int pos_in_arr = 0;
    //printf("Max %d \n", max);
    while (pos_in_arr< max)
    {
        BST current_element = queue[front++];
        if (current_element)
        {
            a[pos_in_arr++] = get_val(current_element);
            if(elements<max)queue[elements++] = get_LC(current_element);
            if(elements<max)queue[elements++] = get_RC(current_element);
        }    
        else{
            pos_in_arr ++;
            if (elements < MAX) queue[elements++] = NULL;
            if (elements < MAX) queue[elements++] = NULL;
        }
    }
    
}
//-----------------------------------------------------------------------------
// is_member: checks if value val is member of BST T
//-----------------------------------------------------------------------------
bool is_member(BST T, int val)
{
	int value_im_looking_4 = val;
	if (T)
	{
		//printf("on look out\n");
		return value_im_looking_4 < T->val ? is_member(T->LC, value_im_looking_4):
		value_im_looking_4> T->val ? is_member(T->RC, value_im_looking_4):
		value_im_looking_4 == T->val;
	}
	return 	false;
}
//-----------------------------------------------------------------------------
// height: returns height of BST T
//-----------------------------------------------------------------------------
int height(BST T)
{
	if(!T) return 0;
	int lheight = height(T->LC);
	int rheight = height(T->RC);
	int tot_height = (lheight > rheight ? lheight : rheight) + 1;
 	return tot_height;
}
//-----------------------------------------------------------------------------
// size: returns size of BST T
//-----------------------------------------------------------------------------
int size(BST T)
{
	if(!T) return 0;
	int left = size(T->LC);
	int right = size(T->RC);
	//printf("%d ", left + right + 1);
	return left + right + 1;
}
//-----------------------------------------------------------------------------
// private helper functions, not exported
//-----------------------------------------------------------------------------
static void _preorder(BST T, int* pos, int* a)
{
	if (T)
	{
		a[(*pos)++] = get_val(T);
		_preorder(get_LC(T), pos, a);
		_preorder(get_RC(T), pos, a);
	}
}


static void _inorder(BST T, int* pos, int* a)
{
	if (T)
	{
		
		_inorder(get_LC(T), pos, a);
		a[(*pos)++] = get_val(T);
		_inorder(get_RC(T), pos, a);
	}
}

static void _postorder(BST T, int* pos, int* a)
{
	if (T)
	{
		_postorder(get_LC(T), pos, a);
		_postorder(get_RC(T), pos, a);
		a[(*pos)++] = get_val(T);		
	}
}
