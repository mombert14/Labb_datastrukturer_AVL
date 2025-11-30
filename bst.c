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

	return	!T            	?	new_BST(v)                            :
		v < get_val(T)	?	cons(add(get_LC(T), v), T, get_RC(T)) :
		v > get_val(T)	?	cons(get_LC(T), T, add(get_RC(T), v)) :
		/* duplicate */		T;
}
//-----------------------------------------------------------------------------
// bst_rem: removes the value val from the BST (if it exists)
//-----------------------------------------------------------------------------

static BST find_successor(BST T)
{
    // Successor är det minsta värdet i det högra underträdet.
    // Gå en gång till höger, sedan så långt till vänster som möjligt.
    T = get_RC(T);
    while (get_LC(T) != NULL) {
        T = get_LC(T);
    }
    return T;
}

BST bst_rem(BST T, int val)
{
	// Basfall 1: Trädet är tomt eller värdet finns inte.
    if (T == NULL) {
        return T;
    }

    // 1. Sök efter noden rekursivt
    if (val < get_val(T)) {
        // Värdet är mindre, rekursivt anrop till vänster underträd.
        // Observera att vi använder 'rem' (funktionspekaren)
        T->LC = rem(get_LC(T), val);
    } 
    else if (val > get_val(T)) {
        // Värdet är större, rekursivt anrop till höger underträd.
        T->RC = rem(get_RC(T), val);
    } 
    else {
        if (get_LC(T) == NULL) {
            BST temp = get_RC(T);
            free(T);
            return temp; 
		} 
        else if (get_RC(T) == NULL) {
            BST temp = get_LC(T);
            free(T);
            return temp; // Returnera vänster barn som den nya sub-roten
        } 
        else {
            // FALL B: Noden har Två Barn (det svåraste fallet)
            
            // Hitta Successorn (minsta värdet i höger underträd)
            BST successor = find_successor(T);
            
            // Kopiera Successorns värde till den aktuella noden T
            T->val = get_val(successor); // Kopiera värdet
            
            // Ta bort Successorn rekursivt från det högra underträdet.
            // Observera att vi nu tar bort SUCCESSOR-värdet, inte det ursprungliga 'val'.
            // Successorn har garanterat noll eller ett barn (därför är den lätt att ta bort).
            T->RC = rem(get_RC(T), get_val(successor));
        }
    }
    
    // Om vi har gått rekursivt, returnera den (potentiellt modifierade) sub-roten T
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
	// max är den *maximala* arraystorleken (pow(2,height(T)) - 1, eller liknande)
    
    // Simulerad kö (Queue) med BST-pekare
    // Vi använder MAX (128) som maxstorlek för kön
    BST queue[MAX]; 
    int front = 0;   // Index för Dequeue (första elementet)
    int rear = 0;    // Index för Enqueue (nästa plats)
    int array_pos = 0; // Index för utmatnings-arrayen 'a'

    // Fyll utmatnings-arrayen med non-value X (t.ex. -42)
    for (int i = 0; i < max; i++) {
        a[i] = X;
    }

    // Basfall: Om trädet är tomt, gör ingenting (arrayen är redan fylld med X)
    if (T == NULL) {
        return;
    }

    // Start: Lägg till rotnoden i kön
    if (rear < MAX) {
        queue[rear++] = T;
    }

    // Kör BFS så länge kön inte är tom
    while (front < rear && array_pos < max)
    {
        // Steg 1: Ta ut nod från fronten (Dequeue)
        BST current = queue[front++];

        // Steg 2: Spara värdet i utmatnings-arrayen 'a'
        if (current != NULL)
        {
            // Spara nodens värde
            a[array_pos++] = get_val(current);

            // Steg 3: Lägg till barnen i kön (Enqueue)
            // Vi måste lägga till båda barnen, ÄVEN om de är NULL, för att fylla ut nivån korrekt.
            
            // Lägg till Vänster Barn
            if (rear < MAX) {
                queue[rear++] = get_LC(current);
            }
            // Lägg till Höger Barn
            if (rear < MAX) {
                queue[rear++] = get_RC(current);
            }
        }
        else 
        {
            // Detta är en NULL-pekare (tomt barn).
            // Spara non-value X (som är din * i utskriften) i arrayen.
            a[array_pos++] = X;

            // OBS! Lägg inte till barn för en NULL-nod.
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


