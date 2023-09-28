#include <iostream>
#define MAXSIZE 100
using namespace std;

typedef struct stack{
    int data[MAXSIZE];
    int top;
}stack;

typedef struct LNode{
    int data;
    struct LNode *next;
}LNode;

stack* InitStack(){
    stack *s = new stack;
    s->top = -1;//栈顶指针（即数组下标）赋初值-1
    return s;
}

int StackTop(stack *s){//返回栈顶元素
    if (s->top > -1 && s->top < MAXSIZE) return s->data[s->top];
    else return false;
}

void StackPush(stack *s, double value){//压入栈
    if (s->top == MAXSIZE - 1) return;
    else s->data[++s->top] = value;
}

void StackPop(stack *s){//弹出栈顶元素
    if (s->top == -1) return;
    else{
        s->top--;
    }
}

LNode* TailInsert(int n){
    LNode *head = new LNode;
    LNode *tail = new LNode;
    head->next = NULL;
    tail = head;
    for (int i = 0 ; i < n ; i++){
        LNode *p = new LNode;
        cin >> p->data;
        p->next = NULL;
        tail->next = p;
        tail = p;
    }
    return head;
}

int ListLength(LNode *head){
    int length = 0;
    LNode *p = head->next;
    while (p != NULL){
        length++;
        p = p->next;
    }
    return length;
}

void PrintList(LNode *head){
    LNode *p = head->next;
    while(p != NULL){
        cout << p->data << " ";
        p = p->next;
    }
    cout << endl;
}

void CentroSymmetry(LNode *head, stack *s){
    int length = ListLength(head);//先求链表的长度
    int loop;//链表需要入栈的长度
    if (length % 2 == 0) loop = length / 2;//链表长度为奇数的情况
    else loop = (length + 1)/2;//链表长度为偶数的情况
    LNode *p = head->next;
    //将一半的链表数据入栈
    for (int i = 0 ; i < loop ; i++){
        StackPush(s, p->data);
        p = p->next;
    }
    //如果链表长度为奇数，则将栈顶元素弹出
    if (length % 2 == 1) StackPop(s);
    while (p != NULL){
        if (p->data != StackTop(s)){
            cout << "Not Centro-symmetric!";//不中心对称
            return;
        }
        else{
            StackPop(s);
            p = p->next;
        }
    }
    cout << "Centro-symmetric";//中心对称
}

int main(){
    LNode *head = TailInsert(6);
    stack *s = InitStack();
    CentroSymmetry(head, s);
    
}