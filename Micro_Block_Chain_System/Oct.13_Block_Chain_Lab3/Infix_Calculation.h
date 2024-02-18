#ifndef Infix_Calculation_H
#define Infix_Calculation_H
#include <iostream>
#include <cmath>
//#define OVERFLOW -2
//#define UNDERFLOW -1
#define TRUE 1
#define FALSE 0
#define MAX 100//ջ�Ĵ�С
#define MAXLENGTH 100//��׺���ʽ�Ĵ�С

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
void CharStackPush(char_stack *s, char value);//��ջ
void CharStackPop(char_stack *s);//��ջ
void CharStackPrint(char_stack *s);//��ӡ��ǰջ������Ԫ��
int IsOperand (char s);//�ж��Ƿ�Ϊ������
int IsOperator (char s);//�ж��Ƿ�Ϊ������
void JudgePriority (char c,Mystack *OPND, char_stack *OPTR);//�ж����������������ȼ�
int Digitizer(char str[]);

Mystack* InitStack();
void DestroyStack(Mystack *s);
int StackEmpty(Mystack *s);
double StackTop(Mystack *s);
void StackPush(Mystack *s, double value);
void StackPop(Mystack *s);
int StackSize(Mystack *s);
void StackPrint(char_stack *s);//��ӡ��ǰջ������Ԫ��
double char_to_double(char s);
double Calculate (double x, double y, char s);

int InfixCalculation(char InfixExpression[], Mystack *OPND, char_stack *OPTR);

Mystack* InitStack(){
    Mystack *s = new Mystack;
    s->top = -1;//ջ��ָ�루�������±꣩����ֵ-1
    return s;
}

//ջ���±���0��ջ���±���ൽMAX-1
void DestroyStack(Mystack *s){
    delete s;
}

int StackEmpty(Mystack *s){//���ջ�շ���1�����򷵻�0
    if (s->top == -1) return TRUE;
    else return FALSE;
}

double StackTop(Mystack *s){//����ջ��Ԫ��
    if (s->top > -1 && s->top < MAX) return s->data[s->top];
    else return FALSE;
}

void StackPush(Mystack *s, double value){//ѹ��ջ
    if (s->top == MAX - 1) return;
    else s->data[++s->top] = value;
}

void StackPop(Mystack *s){//����ջ��Ԫ��
    if (s->top == -1) return;
    else{
        s->top--;
    }
}

int StackSize(Mystack *s){//����ջ�Ĵ�С
    return s->top++;
}

void StackPrint(Mystack *s){//��ӡ��ǰջ������Ԫ��
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
    s->top = -1;//ջ��ָ�루�������±꣩����ֵ-1
    return s;
}

char CharStackTop (char_stack *s){
    if (s->top > -1 && s->top < MAX) return s->data[s->top];
    else return FALSE;
}

void CharStackPush(char_stack *s, char value){//ѹ��ջ
    if (s->top == MAX - 1) return;
    else s->data[++s->top] = value;
}

void CharStackPop(char_stack *s){
    if (s->top == -1) return;
    else{
        s->top--;
    }
}

void CharStackPrint(char_stack *s){//��ӡ��ǰջ������Ԫ��
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
    double x = 0;//Ҫ���ص�����
    int sign = 0;//��ʶ���������ڽ��ַ���ת��Ϊʮλ������λ��
    int negative_sign = 0;//��ʶ���������ڽ��ַ���ת��Ϊ����
    while (IsOperand(str[i]) == TRUE){
        if (sign != 1){
            x = char_to_double(str[i]);
            sign = 1;
            //������ַ�ת��Ϊ���ֺ���sign = 1
            //�������һλ������str[i+1]��Ȼ��������
            //������Ƕ�λ������str[i+1]��Ȼ���������������佫����λ��ƴ�ӳɶ�λ��
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
    //s1�ǲ�����ջ
    //s2�ǲ�����ջ
    int isp = 0;//ջ��������
    int icp = 0;//ջ��������
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
        //���icp(c) > isp(OPTR)����c��OPTRջ
        //����׺���ʽ��ȡ��һ�ַ�����c
        CharStackPush(OPTR, c);
    }
    else if (icp < isp){
        //���icp(c) < isp(OPTR)
        //OPNDջ�˳�a2��a1����OPTRջ�˳��ȣ��γ�����ָ��(a1)��(a2)
        //���ѹ��OPNDջ
        double a2 = StackTop(OPND);
        StackPop(OPND);
        double a1 = StackTop(OPND);
        StackPop(OPND);
        char this_OPTR = CharStackTop(OPTR);
        double ans = Calculate(a1, a2 ,this_OPTR);
        StackPush(OPND,ans);//������ѹ��OPNDջ
        CharStackPop(OPTR);//��������˳�OPTRջ
        JudgePriority(c, OPND, OPTR);
    }
    else{
        //��icp(c) = isp(OPTR)�������������
        //���c = '=' �������������
        //���c = ')' ����Ҫ��OPTR�е���'('�Ӷ���������
        if (OPTR->data[OPTR->top] == '=') return;
        else{
            CharStackPop(OPTR);
        }
    }
}

int InfixCalculation(char InfixExpression[], Mystack *OPND, char_stack *OPTR){
    CharStackPush(OPTR, '=');
    while(1){
        if (IsOperand(InfixExpression[i]) == TRUE){
            double num = Digitizer(InfixExpression);
            //double num = char_to_double(InfixExpression[i]);
            StackPush(OPND, num);
        }
        else if (IsOperator(InfixExpression[i]) == TRUE){
            JudgePriority(InfixExpression[i],OPND,OPTR);
        }
        if (OPTR->data[OPTR->top] == '=' && InfixExpression[i] == '=') break;
        i++;
    }
    return OPND->data[OPND->top];
}

#endif