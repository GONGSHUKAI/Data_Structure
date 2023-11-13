#include <iostream>
using namespace std;
#define MAXQSIZE 100//队列的最大长度
typedef struct SqQueue{
    int *base;  //队列的存储数组
    int front;  //队头指针   
    int rear;   //队尾指针
}SqQueue;  

SqQueue* InitQueue () {
    SqQueue *Q = new SqQueue;
    Q->base = new int[MAXQSIZE];
    Q->front = Q->rear = 0;
    return Q;
}

//循环队列在front和near之间还空出了一个空间，从而区分队满和队空的差别
//这个base[MAXQSIZE]的数组实际上用到的空间为MAXQSIZE-1

bool QueueEmpty (SqQueue *Q) {
    return Q->rear == Q->front;//队空标志
}

bool QueueFull (SqQueue *Q) {
    return (Q->rear + 1) % MAXQSIZE == Q->front;//队满标志
}

void EnQueue (SqQueue *Q, int value){
    if (QueueFull(Q)) return;//队满，无法入列
    else{
        Q->base[Q->rear] = value;
        Q->rear = (Q->rear + 1) % MAXQSIZE;
    }
}

void DeQueue (SqQueue *Q){
    if (QueueEmpty(Q)) return;//队空，无法出列
    else{
        Q->front = (Q->front + 1) % MAXQSIZE;
    }
}

void MyEnqueue(SqQueue *Q, int value){
    int ave = (Q->base[Q->front] + Q->base[Q->rear-1]) / 2;
    if (QueueFull(Q)) return;
    else{
        if (value < ave){
            if ((Q->front - 1) % MAXQSIZE < 0){
                Q->front = MAXQSIZE + (Q->front - 1) % MAXQSIZE;
                Q->base[Q->front] = value;
            }
            else{
                Q->front = (Q->front - 1) % MAXQSIZE;
                Q->base[Q->front] = value;
            }
        }
        else{
            Q->base[Q->rear] = value;
            Q->rear = (Q->rear + 1) % MAXQSIZE;
        }
    }
}

void PrintQueue(SqQueue *Q){
    int p = Q->front;
    while (p != Q->rear){
        cout << Q->base[p] << " ";
        p = (p + 1) % MAXQSIZE;
    }
}



int main(){
    SqQueue *Q = InitQueue();
    MyEnqueue(Q, 1);
    MyEnqueue(Q, 2);
    MyEnqueue(Q, 0);
    PrintQueue(Q);
}