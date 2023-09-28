#include <iostream>

using namespace std;
typedef struct DblStack{
    int top[2];//两个栈顶指针
    int *data;//栈
    int length;//数组长度
}DblStack;

DblStack* InitDblStack(int n){
    DblStack *S = new DblStack;
    S->data = new int(n);
    //数组下标：0,1,...,n-1
    S->length = n;
    S->top[0] = -1;//0号栈初始栈顶位于-1
    S->top[1] = S->length;//1号栈初始栈顶位于n
    return S;
}

bool IsEmpty(DblStack *S, int i){
    //判断i号栈栈空与否，i表示双号栈中的第i个(i=0,1)
    if (i == 0){
        if (S->top[i] == -1) return true;
        else return false;
    }
    else{
        if (S->top[i] == S->length) return true;
        else return false;
    }
}

bool IsFull(DblStack *S){
    //判断整个数组是否已满
    if (S->top[0] + 1 == S->top[1]) return true;
    else return false;
}

void DblStackPush(DblStack *S, int value, int i){
    //向i号栈入栈元素value
    if (i == 0){
        if (IsFull(S)) return;
        else S->data[++S->top[i]] = value;
    }
    else{
        if (IsFull(S)) return;
        else S->data[--S->top[i]] = value;
    }
}

void DblStackPop(DblStack *S, int i){
    //弹出i号栈元素
    if (i == 0){
        if (IsEmpty(S, i)) return;
        else S->top[i]--;
    }
    else{
        if (IsEmpty(S, i)) return;
        else S->top[i]++;
    }
}

void PrintStack(DblStack *S){
    for (int i = 0 ; i < S->length ; i++){
        cout << S->data[i] << " ";
    }
}

int main(){
    DblStack *s = InitDblStack(10);
    DblStackPush(s,114,0);
    DblStackPush(s,514,0);
    DblStackPush(s,1919,1);
    DblStackPush(s,810,1);
    PrintStack(s);
}