#include <iostream>
#define MAXLENGTH 10000
#define LIST_INIT_SIZE 1000

typedef int ElemType;
typedef struct SqList{
    ElemType *data;
    int length;
    int listsize;
}SqList;

typedef struct LNode{
    ElemType data;
    struct LNode *next;
}LNode;

SqList* InitList(){
    SqList *list = (SqList*)malloc(sizeof(SqList));
    list->data = (ElemType*)malloc(sizeof(ElemType) * LIST_INIT_SIZE);
    list->length = 0;
    list->listsize = LIST_INIT_SIZE;
    return list;
}

SqList* ReverseSqList(SqList *L){
    int start = 0;
    int end = L->length - 1;
    ElemType temp;
    while (start < end){
        std::swap(L->data[start], L->data[end]);
        start++;
        end--;
    }
    return L;
}

LNode* CreateList_Head(int n){
    LNode *L = new LNode;
    L->next = NULL;
    for (int i = n; i > 0 ; i--){
        LNode *p = new LNode;
        p->data = i;
        p->next = L->next;
        L->next = p;
    }
    return L;
}

void PrintLinkList(LNode *L){
    LNode *p = new LNode;
    p = L->next;
    while (p != NULL){
        std::cout << p->data << " ";
        p = p->next;
    }
}

LNode* ReverseLinkList(LNode *L){
    LNode *p = new LNode;
    LNode *q = new LNode;
    p = L->next;// p记住原链表第一个结点
    L->next = NULL;//断开原链表头结点与第一个结点
    while (p != NULL){
        q = p->next; //q记住p的下一个结点
        p->next = L->next;
        L->next = p;//p头插法插入原表头
        p = q;//p移动到下一个结点，准备把下一个结点插入
    }
    return L;
}


int main(int argc, const char * argv[]) {
    //倒置顺序表的测试样例：将顺序表（4,2,7,3,0）倒置
    //输出结果：
    SqList *L = InitList();
    ElemType MyL[5]={4,2,7,3,0};
    L->length = 5;
    L->listsize = MAXLENGTH;
    for (int i = 0 ; i < L->length ; i++){
        L->data[i] = MyL[i];
    }
    
    ReverseSqList(L);
    for (int i = 0 ; i < L->length ; i++){
        std::cout << L->data[i] << " ";
    }
    std::cout << std::endl;

    //倒置链表的测试样例：将链表（1,2,3,4,5,6,7,8,9,10）倒置
    //输出结果：10 9 8 7 6 5 4 3 2 1
    int n = 10;
    LNode *L2 = CreateList_Head(n);
    LNode *L3 = ReverseLinkList(L2);
    PrintLinkList(L3);
    
    
    
    
}
