#include "binary-search-tree.h"
#include <iostream>
#include <queue>
#include <stack>

typedef BinarySearchTree::DataType DataType;
typedef BinarySearchTree::Node Node;

using namespace std;

BinarySearchTree::Node::Node(DataType newval)
{
    left = nullptr;
    right = nullptr;
    val = newval;
}

int BinarySearchTree::getNodeDepth(Node* n) const
{
    // Root being null means tree doesn't exist, only having root should return 0
    if (n == nullptr || (n->left == nullptr && n->right == nullptr))
        return 0;

    // Get the depth of the left and right subtree
    // using recursion.
    int leftDepth = getNodeDepth(n->left);
    int rightDepth = getNodeDepth(n->right);

    // Choose the larger one and add one
    if (leftDepth > rightDepth)
        return leftDepth + 1;
    else
        return rightDepth + 1;
}

BinarySearchTree::BinarySearchTree()
{
    root_ = nullptr;
    size_ = 0;
}

BinarySearchTree::~BinarySearchTree()
{
    queue <Node*> bft;
    if(root_ != nullptr){
        bft.push(root_);
    }
    while (!bft.empty()){
        Node* current = bft.front();
        if (current->left != nullptr)
        {
            bft.push(current->left);
        }
        if (current->right != nullptr)
        {
            bft.push(current->right);
        }
        bft.pop();
        delete current;
    }

}


unsigned int BinarySearchTree::size() const
{
    return size_;
}


DataType BinarySearchTree::max() const
{
    Node* current = root_;

    // go as far right as possible to get max.
    while (current->right != nullptr)
    {
        current = current->right;
    }
    return current->val;
}

DataType BinarySearchTree::min() const
{
    Node* current = root_;

    // go as far left as possible to get min
    while (current->left != nullptr)
    {
        current = current->left;
    }
    return current->val;
}

unsigned int BinarySearchTree::depth() const
{
    // call getNodeDepth on the root to get the depth of the entire tree.
    return (getNodeDepth(root_));
}

void BinarySearchTree::print() const
{
    // pre order print.

    stack<Node*> stk;

    // Add the first val.
    if (root_) // not null
        stk.push(root_);

    // Loop while we still have work.
    while (!stk.empty()) {

        // Get the top item and remove it.
        Node* cur = stk.top();
        stk.pop();

        cout << cur->val << " ";

        // Add the children if they exist.
        if (cur->right)
            stk.push(cur->right);
        if (cur->left)
            stk.push(cur->left);
    }

    cout << endl;
}

bool BinarySearchTree::exists(DataType val) const
{
    // check if given value is root.
    if (root_->val == val) return true;
    else
    {
        // step through the tree, going the appropriate direction each comparison (ie left for  val < current val)
        Node* current = root_;
        while ( current != nullptr) {
            if (current->val == val) return true;
            else if (val < current->val) {
                current = current->left;
            } else {
                current = current->right;
            }
        }
        // not found
        return false;
    }
}

Node* BinarySearchTree::getRootNode()
{
    return root_;
}

Node** BinarySearchTree::getRootNodeAddress()
{
    return &root_;
}

bool BinarySearchTree::insert(DataType val)
{
    Node* newNode = new Node (val); // create new node to insert

    // inserting as root.
    if (size_ == 0)
    {
        root_ = newNode;
        size_ ++;
        newNode = nullptr;
        return true;
    }
    else
    {
        Node* current = root_;

        // iterate through until proper leaf location is found or same value is found
        while ( current->val != val) {
            if (val < current->val) {
                // can place node in the left due to it being empty.
                if(current->left == nullptr )
                {
                   current -> left = newNode;
                    size_++;
                    newNode = nullptr;
                    return true;
                }
                else {
                    current = current->left;
                }
            }
            else if ( val > current->val)
            {
                // can place node in the right since it is empty and satisfies condition.
                if (current->right == nullptr)
                {

                    current->right = newNode;
                    size_ ++;
                    newNode = nullptr;
                    return true;
                }
                else{
                    current = current->right;
                }
            }
        }
        // identical value was found, cannot insert.
        delete newNode;
        newNode = nullptr;
        return false;
    }
}

Node* findPredecessor(Node* ptr)
{
}

bool BinarySearchTree::remove(DataType val)
{
    // nothing to remove, empty tree.
   if (size_ == 0 || root_ == nullptr) return false;

   // value to be removed does not exist.
   if (!exists(val)) return false;
   bool isLeftChild;

   Node* current = root_;
   Node* parent = nullptr; // will follow current through in order to change pointers when value is found.

   // find the value to be removed. Leave current pointing to it, and parent pointing to its parent.
   while(current->val != val )
   {
       if (val < current->val )
       {
           parent = current;
           current = current->left;
           isLeftChild = true;
       }
       else
       {
           parent = current;
           current = current->right;
           isLeftChild = false;
       }
   }

   // only the root node to remove, change root pointer to null;
   if (size_ == 1 && val == root_->val)
   {
       delete root_;
       root_ = nullptr;
       size_--;
       return true;
   }

   // a leaf node, simple delete and change the parent pointer.
   if (current->left == nullptr && current->right == nullptr)
   {
       delete current;
       current = nullptr;
       if (isLeftChild) parent->left = nullptr;
       else parent->right = nullptr;
       size_--;
       return true;
   }

   // only a right child / right subtree. must point parent to the right child of current.
   if (current->left == nullptr && current->right != nullptr )
   {
       if (current == root_)
       {
           root_ = current->right;
            delete current;
            current = nullptr;
       }
       else
       {
           if (isLeftChild)
           {
               parent->left = current->right;
           }
           else
           {
               parent->right = current->right;
           }
           delete current;
           current = nullptr;
       }
       size_--;
       return true;
   }

   // only a left child / left subtree
    if (current->left != nullptr && current->right == nullptr )
    {
        if (current == root_)
        {
            root_ = current->left;
            delete current;
            current = nullptr;
        }
        else
        {
            if (isLeftChild)
            {
                parent->left = current->left;
            }
            else
            {
                parent->right = current->left;
            }
            delete current;
            current = nullptr;
        }
        size_--;
        return true;
    }

    // two children/subtrees
    if (current->left != nullptr && current->right != nullptr)
    {
        // find max in left subtree (predecessor)
        Node* predecessor = current->left;
        Node* parentPredecessor = current; // keep track of parent to predecessor to change pointers.

        // loop while no greater value in tree
        while (predecessor -> right != nullptr)
        {
            parentPredecessor = predecessor;
            predecessor = predecessor->right;
        }

        // replace current with the predecessor
        current->val = predecessor->val;

        // ensure left subtree of predecessor is not lost
        if (predecessor->left != nullptr) {
            parentPredecessor->right = predecessor->left;
        }

        // only a single replacement (predecessor is child of current)
        else if (parentPredecessor == current)
        {
            parentPredecessor->left = nullptr;
        }
        else {
            parentPredecessor->right = nullptr;
        }
        delete predecessor;
        predecessor = nullptr;
        size_--;
        return true;
    }
 return false;
}
