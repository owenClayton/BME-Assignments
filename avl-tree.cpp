#include "avl-tree.h"
#include <cmath>
#include <stack>
#include <iostream>

using namespace std;

typedef BinarySearchTree::DataType DataType;
typedef BinarySearchTree::Node Node;

// check height of the node
int AVLTree::depthBelow(Node *n)
{
    // return 0 if empty list
       if (n == nullptr) return 0;
       // check depth below a node (AKA height), similar to lecture pseudocode
       else{
           int depthLeft = depthBelow(n->left);
           int depthRight = depthBelow(n->right);
           if (depthLeft>depthRight)
               return depthLeft+1;
           else
               return depthRight+1;
       }
}

int AVLTree::Difference(Node *n) {
    // A function I added to quickly get the difference (know which way to rotate)
    return(depthBelow(n->left)-depthBelow(n->right));
}

bool AVLTree::singleLeftRotation(Node *parentNode, bool isLeftChild)
{
    if (parentNode == nullptr){
        // rotating the root node, need to change the root node pointer
        Node* rightChild = getRootNode()->right;
        Node* leftofRightChild = rightChild->left;
        rightChild->left = getRootNode();
        rightChild->left->right = leftofRightChild;
        Node** root_ = getRootNodeAddress();
        *root_ = rightChild;
        return true;
    }
    if (isLeftChild) {
        // will need to point the parent->left to new tree.
        Node* parentLeft = parentNode->left;
        Node* leftOfLeftChild = parentLeft->right->left;
        parentNode->left = parentLeft->right;
        parentNode->left->left = parentLeft;
        parentLeft->right = leftOfLeftChild;
        return true;
    }
    else
    {
        // will need to point parent->right to new tree.
        Node* rightChild = parentNode->right;
        Node* rightOfRightChild = rightChild->right;
        Node* leftOfRRChild = rightOfRightChild->left;
        rightOfRightChild->left = rightChild;
        rightChild->right = leftOfRRChild;
        parentNode->right = rightOfRightChild;
        return true;
    }
    return false;
}

bool AVLTree::singleRightRotation(Node *parentNode, bool isLeftChild)
{
    if (!isLeftChild) {
        // change parent->right pointer to new tree after RR rotation.
        Node* rightChild = parentNode->right;
        Node* leftOfRightChild = rightChild->left;
        Node* rightOfLOfRChild = leftOfRightChild->right;
        leftOfRightChild->right = rightChild;
        rightChild->left = rightOfLOfRChild;
        parentNode->right = leftOfRightChild;
        return true;
    }
    else if (parentNode == nullptr){
        // rotating the root, change root pointer.
        Node* leftChild = getRootNode()->left;
        Node* rightOfLeftChild = leftChild->right;
        leftChild->right = getRootNode();
        leftChild->right->left = rightOfLeftChild;
        Node** root_ = getRootNodeAddress();
        *root_ = leftChild;
        return true;
    }
    else{
        // rotating left subtree, change parent
        Node* rightChild = parentNode->right;
        Node* rightOfRightChild = rightChild->right;
        parentNode->right = rightChild->left;
        rightChild->left->right = rightChild;
        rightChild->right = rightOfRightChild;
        rightChild->left = nullptr;
        return true;
    }
    return false;
}

bool AVLTree::leftRightRotation(Node *parentNode, bool isLeftChild)
{
    if (parentNode == nullptr){
        //need to balance the root
        singleLeftRotation(getRootNode(), true);
        singleRightRotation(nullptr, true);
        return true;
    }
    if (isLeftChild){
        // balancing the left child of a parent
        Node* leftChild = parentNode->left;
        singleLeftRotation(leftChild, isLeftChild);
        singleRightRotation(parentNode, isLeftChild);
        return true;
    }
    else
    {
        //balancing the right child of a parent.
            Node* rightChild = parentNode->right;
            singleLeftRotation(rightChild, true);
            singleRightRotation(parentNode, isLeftChild);
            return true;
    }
    return false;
}

bool AVLTree::rightLeftRotation(Node *parentNode, bool isLeftChild)
{
    if (parentNode == nullptr) {
        //balancing the root node.
        singleRightRotation(getRootNode(), false);
        singleLeftRotation(nullptr, true);
        return true;
    }
    else if (isLeftChild){
        // balance left child of parent
        Node* leftChild = parentNode->left;
        singleRightRotation(leftChild, isLeftChild);
        singleLeftRotation(parentNode, isLeftChild);
        return true;
    }
    else{
        //balance right child of parent
        Node* rightChild = parentNode->right;
        singleRightRotation(rightChild, isLeftChild);
        singleLeftRotation(parentNode, isLeftChild);
        return true;
    }


    return false;
}

// stack of path to inserted/deleted Node
std::stack<BinarySearchTree::Node*> *AVLTree::pathToNodeStack(DataType val)
{
    stack<BinarySearchTree::Node*>* path = new stack<BinarySearchTree::Node*>;

    //start at root
    Node* current = *getRootNodeAddress();

    //empty tree
    if (getRootNode() == nullptr)
        return path;

    //step through the tree in order to get to the desired value node, push each step onto the stack.
    while (current->val != val)
    {
        if (current->val < val)
        {
            path->push(current);
            if (current->right == nullptr)
                break;
            else
            current = current-> right;
        }
        else
        {
            path->push(current);
            if (current->left == nullptr )
                break;
            else
            current = current -> left;
        }
    }
    return path;
}

// generic function to updated tree balance of insertion/deletion
bool AVLTree::updateNodeBalance(std::stack<BinarySearchTree::Node*> *pathToNode, DataType val)
{
    //go through the entire stack and check that each node is balanced (bottom of tree to top)
    while (!(pathToNode->empty()))
    {
        Node* checkBalance = pathToNode->top();
        pathToNode->pop();
        int balanceFactor = Difference(checkBalance);
        if(balanceFactor > 1)
        {
            if(Difference(checkBalance->left)>0)
                if (pathToNode->empty())
                    return singleRightRotation(nullptr, true);
                else
                return singleRightRotation(pathToNode->top(), true);
            else
                if(pathToNode->empty())
                    return leftRightRotation(nullptr, true);
                else
                    if (pathToNode->top()->left == checkBalance)
                        return leftRightRotation(pathToNode->top(), true);
                    else
                        return leftRightRotation(pathToNode->top(), false); //check is leftChild's
        }
        else if (balanceFactor<-1){
            if (Difference(checkBalance->right)<0)
                if(pathToNode->empty())
                    return (singleLeftRotation(nullptr, false));
                else
                    return (singleLeftRotation(pathToNode->top(), false));
            else if (Difference(checkBalance->right)==0)
                return (singleLeftRotation(nullptr, false));
            else{
                if (pathToNode->empty())
                    return rightLeftRotation(nullptr, true);
                else
                    if (pathToNode->top()->left == checkBalance)
                        return rightLeftRotation(pathToNode->top(), true);
                    else
                        return rightLeftRotation(pathToNode->top(), false);
            }
        }
    }
    return true;
}

bool AVLTree::insert(DataType val)
{
    // call on BST insert, make the stack of path, and then check balance.
  bool inserted = BinarySearchTree::insert(val);
  if(inserted){
      stack<Node*> *ptr = pathToNodeStack(val);
      updateNodeBalance(ptr, val);
      delete ptr;
      ptr = nullptr;
      return true;
  }
  else{
      return false;
  }
}

bool AVLTree::remove(DataType val)
{
    // call BST remove, make stack of path, and check the balance.
    bool removed = BinarySearchTree::remove(val);
    if(removed){
        stack<Node*> *ptr = pathToNodeStack(val);
        updateNodeBalance(ptr, val);
        delete ptr;
        ptr = nullptr;
        return true;
    }
    else{
        return false;
    }
}
