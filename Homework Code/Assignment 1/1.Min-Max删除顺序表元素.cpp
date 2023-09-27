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
    SqList *list = (SqList*)malloc(sizeof(SqList));
    list->data = (ElemType*)malloc(sizeof(ElemType) * LIST_INIT_SIZE);
    list->length = 0;
    list->listsize = LIST_INIT_SIZE;
    return list;
}

void MinMaxDelete(SqList *L, int min, int max){
    int index = 0;
    int elem = 0;
    while (index < L->length){
        if (L->data[index] <= min || L->data[index] >= max){
            L->data[elem]=L->data[index];
            elem++;
        }
        index++;
    }
    L->length = elem;
}

int main(int argc, const char * argv[]) {
    //测试样例：对于顺序表（1,2,3,4,5,6,7,8,9,10），删除大于5，小于10的元素
    //输出结果：1 2 3 4 5 10
    SqList *L = InitList();
    ElemType MyL[10]={1,2,3,4,5,6,7,8,9,10};
    L->length = 10;
    L->listsize = MAXLENGTH;
    for (int i = 0 ; i < L->length ; i++){
        L->data[i] = MyL[i];
    }
    MinMaxDelete(L, 5, 10);
    
    for (int i = 0 ; i < L->length ; i++){
        std::cout << L->data[i] << " ";
    }
}
