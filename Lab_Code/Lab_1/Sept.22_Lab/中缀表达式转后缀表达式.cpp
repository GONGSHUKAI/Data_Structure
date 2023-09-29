#include <iostream>
#define OVERFLOW -2
#define UNDERFLOW -1
#define TRUE 1
#define FALSE 0

#define MAX 100//栈的大小
typedef struct char_stack{
    char data[MAX];
    int top;
}char_stack;

char_stack* InitCharStack();
void CharStackPush(char_stack *s, char value);//入栈
void Infix_To_Postfix(char InfixExpression[], char_stack *s);//中缀表达式转换为后缀表达式
int IsOperand (char s);//判断是否为操作数
int IsOperator (char s);//判断是否为操作符
void JudgePriority (char c, char_stack *s);//判断算数操作符的优先级

int main(){
    char_stack *s2 = InitCharStack();
    char InfixExpression[]="a+b*(c-d)-e/f#";//13个字符长
    Infix_To_Postfix(InfixExpression, s2);
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

int IsOperand (char s){
    if (s != '(' && s != ')' && s != '+' && s != '-' && s != '*' && s != '/' && s != '%' && s != '#'){
        return TRUE;
    }
    else return FALSE;
}

int IsOperator (char s){
    if (s == '(' || s == ')' || s == '+' || s == '-' || s == '*' || s == '/' || s == '%' || s == '#'){
        return TRUE;
    }
    else return FALSE;
}

void JudgePriority (char c, char_stack *s){
    int isp = 0;//栈内优先数
    int icp = 0;//栈外优先数
    if (c == '#') icp = 0;
    else if (c == '(') icp = 6;
    else if (c == '*' || c == '/' || c == '%') icp = 4;
    else if (c == '+' || c == '-') icp = 2;
    else icp = 1;

    if (s->data[s->top] == '#') isp = 0;
    else if (s->data[s->top] == '(') isp = 1;
    else if (s->data[s->top] == '*' || s->data[s->top] == '/' || s->data[s->top] == '%') isp = 5;
    else if (s->data[s->top] == '+' || s->data[s->top] == '-') isp = 3;
    else icp = 6;
    if (icp > isp){
        CharStackPush(s, c);
    }
    else if (icp < isp){
        if (s->data[s->top] != '('){
            std::cout << CharStackTop(s);
        }
        CharStackPop(s);
        JudgePriority(c, s);
    }
    else{
        if (s->data[s->top] == '#') return;
        else CharStackPop(s);
    }
}

void Infix_To_Postfix(char InfixExpression[], char_stack *s){
    CharStackPush(s, '#');
    int i = 0;
    while(1){
        if (IsOperand(InfixExpression[i]) == TRUE){
            std::cout << InfixExpression[i];
        }
        else if (IsOperator(InfixExpression[i]) == TRUE){
            JudgePriority(InfixExpression[i],s);
        }
        if (s->data[s->top] == '#' && InfixExpression[i] == '#') break;
        i++;
    }
}


