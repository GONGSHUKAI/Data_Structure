#include <iostream>
#define MAXLENGTH 10000
#define LIST_INIT_SIZE 1000

typedef int ElemType;
typedef struct SqList{
    ElemType *data;
    int length;
    int listsize;
}SqList;

SqList* InitList(){
    SqList *list = new SqList;
    list->data = new ElemType[LIST_INIT_SIZE];
    list->length = 0;
    list->listsize = LIST_INIT_SIZE;
    return list;
}

void CreateSqList(SqList *L, int length){
    L->length = length;
    for (int i = 0 ; i < L->length ; i++){
        std::cin >> L->data[i];
    }
}

void PrintSqList(SqList *L){
    for (int i = 0 ; i < L->length ; i++){
        std::cout << L->data[i] << " ";
    }
}

void ReverseSqList(SqList *L, int start, int end){
    //start，end分别为下标，[start,end]表示倒置范围
    ElemType temp;
    while (start < end){
        std::swap(L->data[start], L->data[end]);
        start++;
        end--;
    }
}

void MoveSqList(SqList *L, int m){
    ReverseSqList(L, 0, L->length - m - 1);
    ReverseSqList(L, L->length - m, L->length - 1);
    ReverseSqList(L, 0, L->length - 1);
}


int main(){
    //测试样例：输入一个长度为8的顺序表：1 2 5 7 3 4 6 8，注意用空格分开
    //输出结果：4 6 8 1 2 5 7 3
    SqList *L = InitList();
    CreateSqList(L, 8);
    MoveSqList(L, 3);
    PrintSqList(L);
}