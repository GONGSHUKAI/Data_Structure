#include <iostream>

typedef struct treenode{
    int data;
    struct treenode *left;
    struct treenode *right;
    treenode(int x) : data(x), left(nullptr), right(nullptr) {}
}treenode;

bool judgeSymmetric(treenode *root1, treenode *root2){
    if (root1 == nullptr && root2 == nullptr){//Both are empty trees
        return true;
    }
    else if (root1 == nullptr || root2 == nullptr){//One of them is empty tree
        return false;
    }
    else if (root1->data != root2->data){//Two trees' roots' values are not equal
        return false;
    }
    else{//Two trees' roots' values are equal
        //Judge whether the left subtree of the first tree is symmetric to the right subtree of the second tree
        return judgeSymmetric(root1->left, root2->right) && judgeSymmetric(root1->right, root2->left);
    }
}

treenode* unitTest(){
    treenode *root = new treenode(1);
    root->left = new treenode(2);
    root->right = new treenode(3);

    root->left->right = new treenode(4);
    root->right->left = new treenode(5);
    root->right->right = new treenode(6);
    root->right->right->left = new treenode(7);
    root->right->right->right = new treenode(8);
    return root;
}

treenode* unitTest2(){
    treenode *root = new treenode(1);
    root->left = new treenode(3);
    root->right = new treenode(2);
    root->left->left = new treenode(6);
    root->left->right = new treenode(5);
    root->right->left = new treenode(4);

    root->left->left->left = new treenode(8);
    root->left->left->right = new treenode(7);
    return root;
}

int main(){
    treenode *root1 = unitTest();
    treenode *root2 = unitTest2();
    if (judgeSymmetric(root1, root2)) std::cout << "Symmetric" << std::endl;
    else std::cout << "Asymmetric" << std::endl;
}
