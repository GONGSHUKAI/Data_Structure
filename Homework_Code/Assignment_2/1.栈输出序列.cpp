#include <iostream>
#define N 20
#define MAXSIZE 100

using namespace std;

typedef struct stack{
    int data[MAXSIZE];
    int top;
}stack;

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


bool ValidOutputSequence(int input[N],int n, stack *MyInput){
    //i记录出栈序列下标
    int i = 0;
    //设出栈序列第一个元素为x，则先入栈1,2,...,x
    for (int j = 1 ; j <= input[i] ; j++){
        StackPush(MyInput, j);
    }
    //flag是标识变量，current记录出栈元素
    int flag = StackTop(MyInput);
    int current = StackTop(MyInput);
    //将第一个元素x出栈
    StackPop(MyInput);
    i++;
    while (i < n){
        //如果当前出栈元素input[i]比此前的栈顶元素大，那么将current+1,...,input[i]全部入栈
        if (input[i] > flag){
            for (int j = current + 1 ; j <= input[i] ; j++){
                StackPush(MyInput, j);
            }
            flag = StackTop(MyInput);
            current = StackTop(MyInput);
            StackPop(MyInput);
            i++;
        }
        //如果当前出栈元素input[i]比此前的栈顶元素小，那么现在应该出栈
        else{
            //如果出栈元素不是input[i]，说明不可能以输入的出栈序列的方式出栈！
            if (StackTop(MyInput) != input[i]){
                cout << "Invalid" << endl;
                return false;
            }
            //否则将当前栈顶元素出栈，flag记住出栈的元素
            else{
                flag = StackTop(MyInput);
                StackPop(MyInput);
                i++;
            }
        }
    }
    cout << "Valid" << endl;
    return true;
}

int main(){
    int n;
    int input[N];
    stack *MyInput = InitStack();
    cin >> n;
    for (int i = 0 ; i < n ; i++){
        cin >> input[i];
    }
    ValidOutputSequence(input, n, MyInput);
}
