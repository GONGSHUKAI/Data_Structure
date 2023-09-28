#include <iostream>
using namespace std;

#define MAXQSIZE 8//队列的最大长度
typedef struct SqQueue{
    char *base;  //队列的存储数组
    int front;  //队头指针   
    int rear;   //队尾指针
}SqQueue;  

SqQueue* InitQueue () {
    SqQueue *Q = new SqQueue;
    Q->base = new char[MAXQSIZE];
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

void HeadEnQueue (SqQueue *Q, char value){//头部入队列（相当于在头部插入元素）
    if (QueueFull(Q)) return;//队满，无法入列
    else{
        Q->front--;
        Q->front = (MAXQSIZE + Q->front) % MAXQSIZE;//防止下标为负越界
        Q->base[Q->front] = value;
    }
}

void HeadDeQueue (SqQueue *Q){//出队列（相当于在头部删除元素）
    if (QueueEmpty(Q)) return;//队空，无法出列
    else{
        Q->front = (Q->front + 1) % MAXQSIZE;
    }
}

void TailEnQueue (SqQueue *Q, char value){
    if (QueueFull(Q)) return;//队满，无法入列
    else{
        Q->base[Q->rear] = value;
        Q->rear = (Q->rear + 1) % MAXQSIZE;
    }
}

void TailDeQueue (SqQueue *Q){
    if (QueueEmpty(Q)) return;//队空，无法出列
    else{
        Q->rear--;
        Q->rear = (MAXQSIZE + Q->rear) % MAXQSIZE;
    }
}

void PrintQueue(SqQueue *Q){
    int p = Q->front;
    while (p != Q->rear){
        cout << Q->base[p] << " ";
        p = (p + 1) % MAXQSIZE;
    }
}

void TrainArrange(SqQueue *Q){
    int n;
    cin >> n;
    char input[n];
    for (int i = 0 ; i < n ; i++){
        cin >> input[i];
    }
    for (int i = 0 ; i < n ; i++){
        //如果扫到P，因为P要排在最前面，可以直接输出
        //具体操作符：头部入队列"E"，再出队列"D"
        if (input[i] == 'P') cout << "E" << " " << "D" << " ";
        //如果扫到S，因为S要排中间，所以从头部入队列储存起来
        else if (input[i] == 'S'){
            cout << "E" << " ";
            HeadEnQueue(Q, input[i]);
        }
        //如果扫到H，因为H要排最后，所以从尾部入队列储存起来
        else{
            cout << "A" << " ";
            TailEnQueue(Q, input[i]);
        }
    }
    //此时双端队列中车厢的排布方式必然是H...H...S...S
    //接下来将双端队列中的车厢一个个输出出来
    while (!QueueEmpty(Q)){
        if (Q->base[Q->front] == 'S') cout << "D" << " ";
        if (Q->base[Q->front] == 'H') cout << "D" << " ";
        Q->front = (Q->front + 1) % MAXQSIZE;
    }
}

int main(){
    SqQueue *Q = InitQueue();
    TrainArrange(Q);


}
