#include <iostream>
#include <string>
#include <stack>
#include <cmath>
#define INF 999
using namespace std;

typedef struct treenode{
    char data;
    struct treenode *left;
    struct treenode *right;
    treenode(char x): data(x), left(nullptr), right(nullptr) {}
}treenode;

stack <char> OPRT;

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/';
}

int getPriority(char c) {
    if (c == '+' || c == '-') {
        return 1;
    } else if (c == '*' || c == '/') {
        return 2;
    } else {
        return 0;
    }
}

int findLastOperator(string s) {
    /*
    The logic is as followed: 
    E.g. "3+(5-2)*6"
    We want to find the last operator, which is '+'.
    Operands in between brackets should be ignored, 
    so we use a bracketCount to count the number of brackets.

    In the rule of Poland Expression computation,
    '*' and '/' have higher priority than '+' and '-',
    and should be firstly computed.

    The operator with minimal priority should be the last operator,
    in this case, '+'.
    We use a minPriority to record the operand with minimal priority
    */
    int opIndex = -1;
    int bracketCount = 0;
    int minPriority = INF;
    for (int i = s.size() - 1; i >= 0; i--){
        if (s[i] == ')') {
            bracketCount++;
        }
        else if (s[i] == '('){
            bracketCount--;
        }
        else if (bracketCount == 0 && isOperator(s[i])){
            int precedence = getPriority(s[i]);
            if (precedence <= minPriority) {
                minPriority = precedence;
                opIndex = i;
            }
        }
    }
    return opIndex;
}

treenode* infixToTree(string s) {
    /*
    E.g. "1+2*(3-4)""
    The last operator to be executed in the infix expression is the tree root.
    In this case, '+'.
    Devide the infix expression into two parts:
    the left child of the root is "1",
    the right child of the root is "2*(3-4)".
    the infix expression tree can then be generate by recursively apply the above steps to the left and right child.
    */
    if (s.size() == 0){
        return nullptr;
    }
    if (s.size() == 1){
        return new treenode(s[0]);
    }
    int opIndex = findLastOperator(s);
    //Special process: in cases like (3-4), the last operator is '-'
    //The brackets should therefore be removed to get the correct operator.
    if (opIndex == -1 && s[0] == '(' && s[s.size()-1] == ')'){
        s = s.substr(1, s.size()-2);
        opIndex = findLastOperator(s);
    }
    
    treenode* root = new treenode(s[opIndex]);
    string front = s.substr(0, opIndex);
    string back = s.substr(opIndex + 1);
    root->left = infixToTree(front);
    root->right = infixToTree(back);
    return root;
}

void preOrder(treenode *root){
    if (root){
        cout << root->data << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}
int main(){
    treenode *root = infixToTree("1+2*(3-4)");
    preOrder(root);
}
