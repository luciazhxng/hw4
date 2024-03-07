#ifndef RECCHECK
//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{
  if(root == nullptr) {
    return true;
  }
  if(root->left != nullptr && root->right != nullptr) {
    if(getHeights(root->left) != getHeights(root->right)) {
      return false;
    }
    return equalPaths(root->right) && equalPaths(root->left);
  }
  if(root->left != nullptr) {
    root = root->left; 
    if(getHeights(root->left) != getHeights(root->right)) {
      return false;
    }
    return equalPaths(root->right) && equalPaths(root->left);
  }
  if(root->right != nullptr) {
    root = root->right; 
    if(getHeights(root->left) != getHeights(root->right)) {
      return false;
    }
    return equalPaths(root->right) && equalPaths(root->left);
  }
  return true;
}
int getHeights(Node * root) {
	if(root == nullptr) {
		return 0; 
	}
	int left = getHeights(root->left);
	int right = getHeights(root->right);
	if(left > right) {
		return 1 + left;
	}
	return 1 + right;
}
