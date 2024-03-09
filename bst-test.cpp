#include <iostream>
#include <map>
#include "bst.h"
#include "avlbst.h"

using namespace std;


int main(int argc, char *argv[])
{
    // Binary Search Tree tests
    // BinarySearchTree<char,int> bt1;
    // bt1.insert(std::make_pair('a',1));
    // bt1.insert(std::make_pair('b',2));
    // bt1.insert(std::make_pair('c',3));

    // bt1.print();
    // bt1.clear();
    
    // bt1.insert(std::make_pair('b',5));
    // bt1.print();

    AVLTree<int,int> avl;
    avl.insert(std::make_pair(10,4));
    avl.insert(std::make_pair(14,2));
    avl.insert(std::make_pair(12,6));
    
    avl.insert(std::make_pair(6,3));
    // avl.insert(std::make_pair(3,3));
    avl.print();
    // avl.insert(std::make_pair(4,3));


    // avl.print();
    // avl.remove(10);
    // avl.print();
    avl.remove(14);
    avl.print();
    // bt2.print();
    // bt2.insert(std::make_pair(9,5));
    // bt2.print();
    // cout << "Binary Search Tree contents:" << endl;
    // for(BinarySearchTree<int,int>::iterator it = bt2.begin(); it != bt2.end(); ++it) {
    //   // cout <<"Iterating in main" <<endl;
    //   cout << it->first << " " << it->second << endl;
    // }
    // if(bt.find('b') != bt.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;
    // bt.remove('b');

    // BinarySearchTree<string,string> bt3;
    // bt3.insert(std::make_pair("Hello World", "!!!"));

    // bt3.print();

    // // AVL Tree Tests
    // AVLTree<char,int> at;
    // at.insert(std::make_pair('a',1));
    // at.insert(std::make_pair('b',2));

    // cout << "\nAVLTree contents:" << endl;
    // for(AVLTree<char,int>::iterator it = at.begin(); it != at.end(); ++it) {
    //     cout << it->first << " " << it->second << endl;
    // }
    // if(at.find('b') != at.end()) {
    //     cout << "Found b" << endl;
    // }
    // else {
    //     cout << "Did not find b" << endl;
    // }
    // cout << "Erasing b" << endl;
    // at.remove('b');

    return 0;
}
