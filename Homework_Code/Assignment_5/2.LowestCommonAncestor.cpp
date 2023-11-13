#include <iostream>

typedef struct treenode{
    int data;
    struct treenode *left;
    struct treenode *right;
    struct treenode *parent;
    treenode(int x): data(x), left(nullptr), right(nullptr) {}
}treenode;

treenode* lowestCommonAncestor(treenode *root, int u, int v){
    if (root == nullptr || root->data == u || root->data == v){
        return root;
    }
    treenode *left = lowestCommonAncestor(root->left, u, v);//Search from left subtree
    //After this step, left is either u or v or nullptr
    treenode *right = lowestCommonAncestor(root->right, u, v);//Search from right subtree
    //After this step, right is either u or v or nullptr

    if (left && right){//both left and right are not empty
        //u and v are in different sides of the root
        //root is the LCA
        return root;
    }
    else return left ? left : right;
    //either left or right is empty, 
    //meaning that the LCA is in the other subtree or its ancestors
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

int main(){
    treenode *myTree = unitTest();
    treenode *LCA = lowestCommonAncestor(myTree, 5, 6);
    std::cout << LCA->data << std::endl;
}