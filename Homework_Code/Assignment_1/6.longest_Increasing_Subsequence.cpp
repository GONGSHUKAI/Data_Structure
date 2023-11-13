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

void PrintSqList(SqList *L, int start, int end){
    for (int i = start ; i <= end ; i++){
        std::cout << L->data[i] << " ";
    }
}

void MaxAscendingSubsequence(SqList *L){
    int start = 0;
    int end = 0;
    int final_start = 0;
    int final_end = 0;
    int MaxLength = 1;
    for (int i = 0 ; i < L->length ; i++){
        if (L->data[i]<L->data[i + 1]){
            end++;
        }
        else{
            if (MaxLength < end - start + 1){
                MaxLength = end - start + 1;
                final_start = start;
                final_end = end;
            }
            start = i + 1;
            end = i + 1;
        }
    }
    std::cout << "最长连续递增子序列为：";
    PrintSqList(L, final_start, final_end);
}

int main(){
    //测试格式：
        //先输入n，作为该顺序表的长度
        //再输入n个顺序表的元素，注意用空格分开
            //输入样例：1 9 2 5 7 3 4 6 8 0
    //输出结果：最长连续递增子序列为：3 4 6 8
    SqList *L = InitList();
    int n;
    std::cout << "输入顺序表的长度：";
    std::cin >> n;
    std::cout << "输入顺序表的元素：";
    CreateSqList(L, n);
    MaxAscendingSubsequence(L);
}