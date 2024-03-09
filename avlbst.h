#ifndef AVLBST_H
#define AVLBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char

};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
    
{
}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    int calculateBalance(AVLNode<Key, Value>* node);
    void insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n);
    void removeFix(AVLNode<Key,Value>* n, int diff);
    void rotateRight(AVLNode<Key,Value>* p);
    void rotateLeft(AVLNode<Key,Value>* p);
};



/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::insert(const std::pair<const Key, Value> &new_item)
{
  if(this->internalFind(new_item.first) != NULL) {
    // std::cout << "Node already in tree"<< std::endl;
    Node<Key, Value>* found = this->internalFind(new_item.first);
    found->setValue(new_item.second);
    return;
  }
  // std::cout << "Completed internal find"<< std::endl;
  if(this->root_ == NULL) {
    // std::cout << "Root is null"<< std::endl;
    this->root_ = new AVLNode<Key, Value> (new_item.first, new_item.second, NULL);
    return; 
  }
  // std::cout << "Creating new node in non-empty tree" << std::endl;
  AVLNode<Key, Value> *parent = static_cast<AVLNode<Key, Value>*>(this->root_);
  AVLNode<Key, Value> *current = static_cast<AVLNode<Key, Value>*>(this->root_);
  while(current != NULL) {
    if(current->getKey() > new_item.first) {
      parent = current;
      current = current->getLeft();
    }
    else if(current->getKey() < new_item.first) {
      parent = current;
      current = current->getRight();
    }
  }
  AVLNode<Key, Value>* newNode = new AVLNode<Key, Value> (new_item.first, new_item.second, parent);
  if(parent->getKey() > new_item.first) {
    // std::cout << "Create new node to the left of previous" << std::endl;
    parent->setLeft(newNode);
  }
  else {
    //  std::cout << "Create new node to the right of previous" << std::endl;
    parent->setRight(newNode);
  }
  int8_t pBalance = parent->getBalance();
  if(pBalance == -1 || pBalance == 1) {
    // std::cout << "Tree is already balanced"<< std::endl;
    parent->setBalance(0);
    return;
  }
  else {
    // std::cout << "Tree is not balanced"<< std::endl;
    parent->setBalance(calculateBalance(parent));
    insertFix(parent, newNode);
  }
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
  // std::cout << "Removing node" << std::endl;
  if(this->internalFind(key) == NULL) {
    // std::cout << "Node not found" << std::endl;
    return;
  }
  AVLNode<Key,Value>* n = static_cast<AVLNode<Key, Value>*>(this->internalFind(key));
  if(static_cast<AVLNode<Key, Value>*>(this->root_) == n && n->getRight() == NULL && n->getLeft() == NULL) { // if it's the only node
    // std::cout << "This is the only node" << std::endl;
    delete n;
    this->root_ = NULL;
    return;
  }
  if(n->getRight() != NULL && n->getLeft() != NULL) {
    // std::cout << "Node has two children" << std::endl;
    // if two children, will swap down -> guarantees that there is a parent of n
    AVLNode<Key,Value>* toSwap = static_cast<AVLNode<Key, Value>*>(this->predecessor(n));
    nodeSwap(n, toSwap);
  }
  int diff;
  AVLNode<Key,Value>* p = n->getParent();
  if(p != NULL) {
    if(n == p->getLeft()) {
      diff = 1;
    }
    else {
      diff = -1;
    }
  }
  else { // the node is the root and there is only one child
    // std::cout << "The node is the root and there is only one child" << std::endl;
    if(n->getRight() != NULL) {
      this->root_ = n->getRight();
      delete n;
      this->root_->setParent(NULL);
      return;
    }
    if(n->getLeft() != NULL) {
      this->root_ = n->getLeft();
      delete n;
      this->root_->setParent(NULL);
      return;
    }
  }
  if(n->getRight() == NULL && n->getLeft() == NULL) {
    // std::cout << "Node has no children" << std::endl;
    if(n == p->getLeft()) {
      p->setLeft(NULL);
      // std::cout << "Node to be removed is left child" << std::endl;
    }
    else {
      // std::cout << "Node to be removed is right child" << std::endl;
      p->setRight(NULL);
      
    }
    delete n;
    // std::cout << "Deleted node" << std::endl;
  }
  else if(n->getRight() != NULL) {
    // std::cout << "Node has right child" << std::endl;
    Node<Key,Value>* c = n->getRight();
    if(n == p->getLeft()) {
      p->setLeft(c);
      // std::cout << "Node to be removed is left child" << std::endl;
    }
    else {
      // std::cout << "Node to be removed is right child" << std::endl;
      p->setRight(c);
    }
    c->setParent(p);
    delete n;
  }
  else if(n->getLeft() != NULL) {
    // std::cout << "Node has left child" << std::endl;
    Node<Key,Value>* c = n->getLeft();
    if(n == p->getLeft()) {
      p->setLeft(c);
      // std::cout << "Node to be removed is left child" << std::endl;
    }
    else {
      // std::cout << "Node to be removed is right child" << std::endl;
      p->setRight(c);
    }
    c->setParent(p);
    delete n;
  }
  // std::cout << "Calling removeFix" << std::endl;
  removeFix(p, diff);
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}

template<class Key, class Value>
int AVLTree<Key, Value>::calculateBalance(AVLNode<Key, Value>* node) {
  if(node == nullptr) {
		return 0; 
	}
	int left = this->getHeights(node->getLeft());
	int right = this->getHeights(node->getRight());
  return right - left;
}

template<class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key,Value>* p, AVLNode<Key,Value>* n) {
  // std::cout << "Running insertFix"<< std::endl;
  if(p == NULL || p->getParent() == NULL) {
    return;
  }
  AVLNode<Key,Value>* g = p->getParent();
  if(p == g->getLeft()) {
    g->updateBalance(-1);
    if(g->getBalance() == 0) {
      // std::cout << "Grandparent is balanced"<< std::endl;
      return;
    }
    else if(g->getBalance() == -1) {
      // std::cout << "No rotation needed"<< std::endl;
      insertFix(g, p);
    }
    else if(g->getBalance() == -2) {
      // std::cout << "Rotation needed"<< std::endl;
      if(p->getLeft() == n) { // zig-zig
        // std::cout << "Zig-zig"<< std::endl;
        rotateRight(g);
        p->setBalance(0);
        g->setBalance(0);
        return;
      }
      else { //zig-zag
        // std::cout << "Zig-zag"<< std::endl;
        rotateLeft(p);
        rotateRight(g);
        int8_t nBalance = n->getBalance();
        if(nBalance == -1) {
          p->setBalance(0);
          g->setBalance(1);
          n->setBalance(0);
          return;
        }
        else if(nBalance == 0) {
          p->setBalance(0);
          g->setBalance(0);
          n->setBalance(0);
          return;
        }
        else if(nBalance == 1) {
          p->setBalance(-1);
          g->setBalance(0);
          n->setBalance(0);
          return;
        }
      }
    }
  }
  else {
    g->updateBalance(1);
    if(g->getBalance() == 0) {
      // std::cout << "Grandparent is balanced"<< std::endl;
      return;
    }
    else if(g->getBalance() == +1) {
      // std::cout << "No rotation needed"<< std::endl;
      insertFix(g, p);
    }
    else if(g->getBalance() == +2) {
      // std::cout << "Rotation needed"<< std::endl;
      if(p->getRight() == n) { // zig-zig
        // std::cout << "Zig-zig"<< std::endl;
        rotateLeft(g);
        p->setBalance(0);
        g->setBalance(0);
        return;
      }
      else { //zig-zag
        // std::cout << "Zig-zag"<< std::endl;
        rotateRight(p);
        // std::cout << "Rotated-right"<< std::endl;
        // this->print();
        rotateLeft(g);
        int8_t nBalance = n->getBalance();
        if(nBalance == 1) {
          p->setBalance(0);
          g->setBalance(-1);
          n->setBalance(0);
          return;
        }
        else if(nBalance == 0) {
          p->setBalance(0);
          g->setBalance(0);
          n->setBalance(0);
          return;
        }
        else if(nBalance == -1) {
          p->setBalance(1);
          g->setBalance(0);
          n->setBalance(0);
          return;
        }
      }
    }
  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key,Value>* n, int diff) {
  // std::cout << "Running removeFix"<< std::endl;
  if(n == NULL){
    return;
  }
  AVLNode<Key,Value>* p = n->getParent();
  int ndiff;
  if(p != NULL) {
    // std::cout << "Parent is not null"<< std::endl;
    if(p->getRight() == n) {
      ndiff = -1;
    }
    else {
      ndiff = 1;
    }
  }
  int8_t nBalance = n->getBalance();
  if(diff == -1) {
    // std::cout << "Node was a right child"<< std::endl;
    if(nBalance + diff == -2) {
      AVLNode<Key,Value>* c = n->getLeft();
      int8_t cBalance = c->getBalance();
      if(cBalance == -1) {
        rotateRight(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(cBalance == 0) {
        rotateRight(n);
        n->setBalance(-1);
        c->setBalance(1);
      }
      else if(cBalance == 1) {
        AVLNode<Key,Value>* g = c->getRight();
        int8_t gBalance = g->getBalance();
        rotateLeft(c);
        rotateRight(n);
        if(gBalance == 1) {
          n->setBalance(0);
          c->setBalance(-1);
          g->setBalance(0);
        }
        else if(gBalance == 0) {
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if(gBalance == -1) {
          n->setBalance(1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if(nBalance + diff == -1) {
      n->setBalance(-1);
      return;
    }
    else if(nBalance + diff == 0) {
      n->setBalance(0);
      removeFix(p, ndiff);
    }

  }
  else {
    // std::cout << "Node was a left child"<< std::endl;
    if(nBalance + diff == 2) {
      AVLNode<Key,Value>* c = n->getRight();
      int8_t cBalance = c->getBalance();
      if(cBalance == 1) {
        rotateLeft(n);
        n->setBalance(0);
        c->setBalance(0);
        removeFix(p, ndiff);
      }
      else if(cBalance == 0) {
        rotateLeft(n);
        n->setBalance(1);
        c->setBalance(-1);
      }
      else if(cBalance == -1) {
        AVLNode<Key,Value>* g = c->getLeft();
        int8_t gBalance = g->getBalance();
        rotateRight(c);
        rotateLeft(n);
        if(gBalance == -1) {
          n->setBalance(0);
          c->setBalance(1);
          g->setBalance(0);
        }
        else if(gBalance == 0) {
          n->setBalance(0);
          c->setBalance(0);
          g->setBalance(0);
        }
        else if(gBalance == 1) {
          n->setBalance(-1);
          c->setBalance(0);
          g->setBalance(0);
        }
        removeFix(p, ndiff);
      }
    }
    else if(nBalance + diff == 1) {
      n->setBalance(1);
      return;
    }
    else if(nBalance + diff == 0) {
      n->setBalance(0);
      removeFix(p, ndiff);
    }

  }
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* p) {
  // std::cout << "Running rotateRight" << std::endl;
  AVLNode<Key, Value>* n = p->getLeft();
  AVLNode<Key, Value>* g = p->getParent();
  AVLNode<Key, Value>* c = n->getRight();
  n->setRight(p);
  p->setLeft(c);
  if(g != NULL) {
    if(p == g->getRight()) {
      g->setRight(n);
    }
    else {
      g->setLeft(n);
    }
  }
  n->setParent(g);
  p->setParent(n);
  if(c != NULL) {
    c->setParent(p);
  }
  if(this->root_ == p) {
    // std::cout << "p is root" << std::endl;
    this->root_ = n;
  }
  
}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* p) {
  AVLNode<Key, Value>* n = p->getRight();
  AVLNode<Key, Value>* g = p->getParent();
  AVLNode<Key, Value>* c = n->getLeft();
  n->setLeft(p);
  p->setRight(c);
  if(g != NULL) {
    if(p == g->getRight()) {
      g->setRight(n);
    }
    else {
      g->setLeft(n);
    }
  }
  n->setParent(g);
  p->setParent(n);
  if(c != NULL) {
    c->setParent(p);
  }
  if(this->root_ == p) {
    // std::cout << "p is root" << std::endl;
    this->root_ = n;
  }
}
#endif
