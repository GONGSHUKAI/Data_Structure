#include <iostream>
#include <cmath>
//#define OVERFLOW -2
//#define UNDERFLOW -1
#define TRUE 1
#define FALSE 0
#define MAX 100//栈的大小
#define MAXLENGTH 100//中缀表达式的大小

typedef struct char_stack{
    char data[MAX];
    int top;
}char_stack;

typedef struct stack{
    double data[MAX];
    int top;
}Mystack;

int i = 0;

char_stack* InitCharStack();
void CharStackPush(char_stack *s, char value);//入栈
void CharStackPop(char_stack *s);//出栈
void CharStackPrint(char_stack *s);//打印当前栈内所有元素
int IsOperand (char s);//判断是否为操作数
int IsOperator (char s);//判断是否为操作符
void JudgePriority (char c,Mystack *OPND, char_stack *OPTR);//判断算数操作符的优先级
int Digitizer(char str[]);

Mystack* InitStack();
void DestroyStack(Mystack *s);
int StackEmpty(Mystack *s);
double StackTop(Mystack *s);
void StackPush(Mystack *s, double value);
void StackPop(Mystack *s);
int StackSize(Mystack *s);
void StackPrint(char_stack *s);//打印当前栈内所有元素
double char_to_double(char s);
double Calculate (double x, double y, char s);

double InfixCalculation(char InfixExpression[], Mystack *OPND, char_stack *OPTR);

int main(){
    Mystack *OPND = InitStack();
    char_stack *OPTR = InitCharStack();
    
    char InfixExpression[MAXLENGTH];
    gets(InfixExpression);
    InfixCalculation(InfixExpression, OPND, OPTR);
    std::cout << std::endl;
    double a = InfixCalculation(InfixExpression, OPND, OPTR);
    std::cout << a << std::endl;
}

Mystack* InitStack(){
    Mystack *s = new Mystack;
    s->top = -1;//栈顶指针（即数组下标）赋初值-1
    return s;
}

//栈底下标是0，栈顶下标最多到MAX-1
void DestroyStack(Mystack *s){
    delete s;
}

int StackEmpty(Mystack *s){//如果栈空返回1，否则返回0
    if (s->top == -1) return TRUE;
    else return FALSE;
}

double StackTop(Mystack *s){//返回栈顶元素
    if (s->top > -1 && s->top < MAX) return s->data[s->top];
    else return FALSE;
}

void StackPush(Mystack *s, double value){//压入栈
    if (s->top == MAX - 1) return;
    else s->data[++s->top] = value;
}

void StackPop(Mystack *s){//弹出栈顶元素
    if (s->top == -1) return;
    else{
        s->top--;
    }
}

int StackSize(Mystack *s){//返回栈的大小
    return s->top++;
}

void StackPrint(Mystack *s){//打印当前栈内所有元素
    for (int i = 0 ; i <= s->top ; i++){
        std::cout << s->data[i] << " ";
    }
    std::cout << std::endl;
}

double char_to_double(char s){
    if (s == '0') return 0;
    else if (s == '1') return 1;
    else if (s == '2') return 2;
    else if (s == '3') return 3;
    else if (s == '4') return 4;
    else if (s == '5') return 5;
    else if (s == '6') return 6;
    else if (s == '7') return 7;
    else if (s == '8') return 8;
    else return 9;
}

double Calculate (double x, double y, char s){
    if (s == '+') return x+y;
    else if (s == '-') return x-y;
    else if (s == '*') return x*y;
    else if (s == '/') return x/y;
    else return std::pow(x,y);
}

char_stack* InitCharStack(){
    char_stack *s = new char_stack;
    s->top = -1;//栈顶指针（即数组下标）赋初值-1
    return s;
}

char CharStackTop (char_stack *s){
    if (s->top > -1 && s->top < MAX) return s->data[s->top];
    else return FALSE;
}

void CharStackPush(char_stack *s, char value){//压入栈
    if (s->top == MAX - 1) return;
    else s->data[++s->top] = value;
}

void CharStackPop(char_stack *s){
    if (s->top == -1) return;
    else{
        s->top--;
    }
}

void CharStackPrint(char_stack *s){//打印当前栈内所有元素
    for (int i = 0 ; i <= s->top ; i++){
        std::cout << s->data[i] << " ";
    }
    std::cout << std::endl;
}

int IsOperand (char s){
    if (s != '(' && s != ')' && s != '+' && s != '-' && s != '*' && s != '/' && s != '%' && s != '^' && s != '='){
        return TRUE;
    }
    else return FALSE;
}

int IsOperator (char s){
    if (s == '(' || s == ')' || s == '+' || s == '-' || s == '*' || s == '/' || s == '%' || s == '^' ||s == '='){
        return TRUE;
    }
    else return FALSE;
}

int Digitizer(char str[]){
    double x = 0;//要返回的数字
    int sign = 0;//标识变量，用于将字符串转换为十位数、百位数
    int negative_sign = 0;//标识变量，用于将字符串转换为负数
    while (IsOperand(str[i]) == TRUE){
        if (sign != 1){
            x = char_to_double(str[i]);
            sign = 1;
            //把这个字符转换为数字后，置sign = 1
            //如果这是一位数，则str[i+1]必然不是数字
            //如果这是多位数，则str[i+1]仍然是数，由下面的语句将各个位数拼接成多位数
        }
        else{
            x = x * 10 + char_to_double(str[i]);
        }
        i++;
    }
    i--;
    return x;
}

void JudgePriority (char c,Mystack *OPND, char_stack *OPTR){
    //s1是操作数栈
    //s2是操作符栈
    int isp = 0;//栈内优先数
    int icp = 0;//栈外优先数
    if (c == '=') icp = 0;
    else if (c == '(') icp = 8;
    else if (c == '^') icp = 6;
    else if (c == '*' || c == '/' || c == '%') icp = 4;
    else if (c == '+' || c == '-') icp = 2;
    else icp = 1;

    if (OPTR->data[OPTR->top] == '=') isp = 0;
    else if (OPTR->data[OPTR->top] == '(') isp = 1;
    else if (OPTR->data[OPTR->top] == '^') isp = 7;
    else if (OPTR->data[OPTR->top] == '*' || OPTR->data[OPTR->top] == '/' || OPTR->data[OPTR->top] == '%') isp = 5;
    else if (OPTR->data[OPTR->top] == '+' || OPTR->data[OPTR->top] == '-') isp = 3;
    else isp = 8;

    if (icp > isp){
        //如果icp(c) > isp(OPTR)，则c进OPTR栈
        //从中缀表达式中取下一字符送入c
        CharStackPush(OPTR, c);
        std::cout << "当前操作符栈为：";
        CharStackPrint(OPTR);
        std::cout << "当前操作数栈为：";
        StackPrint(OPND);
        std::cout << std::endl;
    }
    else if (icp < isp){
        //如果icp(c) < isp(OPTR)
        //OPND栈退出a2和a1，从OPTR栈退出θ，形成运算指令(a1)θ(a2)
        //结果压入OPND栈
        double a2 = StackTop(OPND);
        StackPop(OPND);
        double a1 = StackTop(OPND);
        StackPop(OPND);
        char this_OPTR = CharStackTop(OPTR);
        double ans = Calculate(a1, a2 ,this_OPTR);
        std::cout << "当前执行操作：" << a1 << this_OPTR << a2 << '=' << ans << std::endl;
        StackPush(OPND,ans);//运算结果压入OPND栈
        CharStackPop(OPTR);//运算符θ退出OPTR栈
        std::cout << "当前操作符栈为：";
        CharStackPrint(OPTR);
        std::cout << "当前操作数栈为：";
        StackPrint(OPND);
        std::cout << std::endl;

        JudgePriority(c, OPND, OPTR);
    }
    else{
        //果icp(c) = isp(OPTR)，有两种情况：
        //如果c = '=' 则整个运算结束
        //如果c = ')' 则需要从OPTR中弹出'('从而对消括号
        if (OPTR->data[OPTR->top] == '=') return;
        else{
            CharStackPop(OPTR);
            std::cout << "当前操作符栈为：";
            CharStackPrint(OPTR);
            std::cout << "当前操作数栈为：";
            StackPrint(OPND);
            std::cout << std::endl;
        }
    }
}

double InfixCalculation(char InfixExpression[], Mystack *OPND, char_stack *OPTR){
    CharStackPush(OPTR, '=');
    while(1){
        if (IsOperand(InfixExpression[i]) == TRUE){
            double num = Digitizer(InfixExpression);
            //double num = char_to_double(InfixExpression[i]);
            StackPush(OPND, num);
            std::cout << "当前操作符栈为：";
            CharStackPrint(OPTR);
            std::cout << "当前操作数栈为：";
            StackPrint(OPND);
            std::cout << std::endl;
        }
        else if (IsOperator(InfixExpression[i]) == TRUE){
            JudgePriority(InfixExpression[i],OPND,OPTR);
        }
        if (OPTR->data[OPTR->top] == '=' && InfixExpression[i] == '=') break;
        i++;
    }
    return OPND->data[OPND->top];
}
