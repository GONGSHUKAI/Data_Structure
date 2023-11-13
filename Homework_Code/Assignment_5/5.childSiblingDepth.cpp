#include <iostream>

typedef struct treeNode{
    char data;
    struct treeNode *child;
    struct treeNode *sibling;
    treeNode(char x): data(x), child(nullptr), sibling(nullptr) {}
}treeNode;

int treeDepth(treeNode *root){
    if (root == nullptr){
        return 0;
    }
    else{
        int childDepth = treeDepth(root->child);
        int sibDepth = treeDepth(root->sibling);
        return childDepth > sibDepth ? (childDepth + 1) : (sibDepth + 1);
    }
}

treeNode* myTree(){
    treeNode *root = new treeNode('A');
    root->child = new treeNode('B');
    root->child->child = new treeNode('D');
    root->child->sibling = new treeNode ('C');
    root->child->child->sibling = new treeNode ('E');
    root->child->child->sibling->sibling = new treeNode ('F');
    root->child->child->sibling->child = new treeNode ('G');
    return root;
}

int main(){
    treeNode *root = myTree();
    std::cout << treeDepth(root) << std::endl;
    return 0;
}
