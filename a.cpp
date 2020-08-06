#include <iostream>
#include <string>
using namespace std;

struct node *insert(node* ,string ,int );
struct node *deleteNode(node* , int );
struct node *rrrotation(node *);
struct node *llrotation(node *);
struct node *inorder(node *);
struct node *minValueNode(node *);
int max(int , int );
int avlheight(node *);
int getbalnfactor(node *);
int serialNumber;

struct node {
  int serial; //! Changed: Using this variable as Position in subtree(i.e. No. of nodes in the left of that node + 1), not the actual serial number
  string task;
  int info;
  struct node *left;
  struct node *right;
  int height;
};

int avlheight(node *ptr) {
  if (ptr == NULL)
      return 0;

  return ptr->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

node *minValueNode(node* ptr)
{
    node* current = ptr;

    /* loop down to find the leftmost leaf */
    while (current->left != NULL)
        current = current->left;

    return current;
}

node *insert(node* newp,string newt ,int item) {
  node* new_node = new node;
  new_node->task = newt;
  new_node->info = item;
  new_node->left = NULL;
  new_node->right = NULL;
  new_node->height = 1;
  new_node->serial = 1;//! Newly added

    if (newp == NULL) {
      return new_node;
    }

    if (item < newp->info) {
        newp->serial++;//! Newly added: as item is inserted in the left, serial i.e, No. of nodes in left increases
        newp->left = insert(newp->left,newt ,item);
    }
    else if (item >= newp->info)
        newp->right = insert(newp->right,newt ,item);
    else
        return newp;

    newp->height = 1 + max(avlheight(newp->left), avlheight(newp->right));

    int balance = getbalnfactor(newp);

    // For RR Rotation
    if (balance > 1 && item < newp->left->info)
        return rrrotation(newp);

    // For LL Rotation
    if (balance < -1 && item >= newp->right->info)
        return llrotation(newp);

    // For LR Rotation
    if (balance > 1 && item > newp->left->info)
    {
        newp->left = llrotation(newp->left);
        return rrrotation(newp);
    }

    // For RL Rotation
    if (balance < -1 && item < newp->right->info)
    {
        newp->right = rrrotation(newp->right);
        return llrotation(newp);
    }

    return newp;
}

node *deleteNode(node* root, int val)
{

    // STEP 1: PERFORM STANDARD BST DELETE
    if (root == NULL)
        return root;

    // If the val to be deleted is smaller
    // than the root's val, then it lies
    // in left subtree
    if ( val < root->serial ) {
        root->serial--;//! Newly added
        root->left = deleteNode(root->left, val);
    }

    // If the val to be deleted is greater
    // than the root's val, then it lies
    // in right subtree
    else if( val > root->serial ) {
        root->right = deleteNode(root->right, val - root->serial);//! Changed
    }

    // if val is same as root's val, then
    // This is the node to be deleted
    else
    {
        // node with only one child or no child
        if((root->left == NULL) || (root->right == NULL))
        {
            node *temp = root->left ?
                         root->left :
                         root->right;  // CHeck This

            // No child case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else // One child case
            *root = *temp; // Copy the contents of
                           // the non-empty child
            delete temp;
        }
        else
        {
            // node with two children: Get the inorder
            // successor (smallest in the right subtree)
            node* temp = minValueNode(root->right);

            // Copy the inorder successor's
            // data to this node
            
            root->info = temp->info;//! Changed
            root->task = temp->task;//!Newly added

            // Delete the inorder successor
            root->right = deleteNode(root->right, 1);//! Changed
        }
    }

    // If the tree had only one node
    // then return
    if (root == NULL)
    return root;

    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE
    root->height = 1 + max(avlheight(root->left),
                           avlheight(root->right));

    // STEP 3: GET THE BALANCE FACTOR OF
    // THIS NODE (to check whether this
    // node became unbalanced)
    int balance = getbalnfactor(root);

    // If this node becomes unbalanced,
    // then there are 4 cases

    // Left Left Case
    if (balance > 1 &&
        getbalnfactor(root->left) >= 0)
        return rrrotation(root);

    // Left Right Case
    if (balance > 1 &&
        getbalnfactor(root->left) < 0)
    {
        root->left = llrotation(root->left);
        return rrrotation(root);
    }

    // Right Right Case
    if (balance < -1 &&
        getbalnfactor(root->right) <= 0)
        return llrotation(root);

    // Right Left Case
    if (balance < -1 &&
        getbalnfactor(root->right) > 0)
    {
        root->right = rrrotation(root->right);
        return llrotation(root);
    }

    return root;
}

node *rrrotation(node *y) {

    /*
     *
     *                y
     *               / \
     *   BEFORE     x   T3
     *             / \
     *            T1  T2
     * 
     *            RR ROTATION
     * 
     *                x
     *               / \
     *   AFTER      T1  y
     *                 / \
     *                T2  T3
     * 
     */

  node *x = y->left;
  node *T2 = x->right;

  x->right = y;
  y->left = T2;

    // serial(y) changes
    // serial(y)[before] = No. of nodes in T1 + No. of nodes in T2 + 2;
    // serial(y)[after] = No. of nodes in T2 + 1;
    // serial(x) = No. of nodes in T1 + 1
    // Therefore
    //          serial(y) = serial(y) - serial(x)

  y->serial -= x->serial;//! Newly added

  y->height = max(avlheight(y->left), avlheight(y->right)) + 1;
  x->height = max(avlheight(x->left), avlheight(x->right)) + 1;

  return x;
}

node *llrotation(node *x) {

    /*
     *
     *                x
     *               / \
     *   BEFORE     T1  y
     *                 / \
     *                T2  T3
     * 
     *              LL ROTATION
     * 
     *                y
     *               / \
     *   AFTER      x   T3
     *             / \
     *            T1  T2
     * 
     */

  node *y = x->right;
  node *T2 = y->left;

  y->left = x;
  x->right = T2;

    // serial(y) changes
    // serial(y)[before] = No. of nodes in T2 + 1;
    // serial(y)[after] = No. of nodes in T1 + No. of nodes in T2 + 2;
    // serial(x) = No. of nodes in T1 + 1
    // Therefore
    //          serial(y) = serial(y) + serial(x)

  
  y->serial += x->serial;//! Newly added

  x->height = max(avlheight(x->left), avlheight(x->right)) + 1;
  y->height = max(avlheight(y->left), avlheight(y->right)) + 1;

  return y;
}

int getbalnfactor(node *nptr) {
  if (nptr == NULL)
      return 0;

  return avlheight(nptr->left) - avlheight(nptr->right);
}

node *inorder(node *root) {
  if(root != NULL) {
    inorder(root->left);
    cout<<serialNumber<<". "<<root->task<<" "<<root->info<<endl;//! Changed
    serialNumber++;//! Newly added
    inorder(root->right);
  }
  return root;
}

int main()
{
    node *root = NULL;
    int option, value;
    string task;
    bool flag = 1;
    while (flag) {
        cout << "\nPlease choose option:\n\n";
        cout << "1. Add task to todo list with priority.\n";
        cout << "2. Mark given task no as completed and remove from list.\n";
        cout << "3. Print list with decreasing priority.\n";
        cout << "4. Exit\n";
        cin >> option;
        switch (option) {
            case 1:
                cin.ignore();
                getline(cin, task);
                cin >> value;
                root = insert(root,task,value);
                break;
            case 2:
                cin >> value;
                root = deleteNode(root, value);
                break;
            case 3:
                serialNumber = 1;
                root = inorder(root);
                break;
            case 4:
                flag = 0;
        }
    }
}
