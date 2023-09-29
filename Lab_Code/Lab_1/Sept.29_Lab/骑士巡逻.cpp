#include <iostream>
#define N 8
#define MAXSIZE 1000

using namespace std;
//骑士的8个移动方向用step[][]记录
//第一个分量表示行位移，第二个分量表示列位移
int step[8][2]={{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};
int depth = 0;//搜索深度，最大为n*n(棋盘格大小)
int sum = 0;//记录骑士巡逻的方法数（回溯法）
int sum2 = 0;//记录骑士巡逻的方法数（非递归法）
int flag = 0;//判断骑士巡逻路线是否存在（回溯法）
int flag2 = 0;//判断骑士巡逻路线是否存在（非递归法）

typedef struct coord{
    int row;
    int col;
}coord;//存放骑士的位置(row, col)

typedef struct stack{
    coord data[MAXSIZE];
    int top;
}stack;//存放骑士位置的栈

stack* InitStack();//初始化栈
bool StackEmpty(stack *s);//判断栈空
coord StackTop(stack *s);//判断栈满
void StackPush(stack *s, coord value);//入栈骑士的位置(row, col)
void StackPop(stack *s);//弹出栈顶元素
//打印栈中储存的骑士巡逻坐标，n为棋盘格大小，sum为骑士巡逻的方法数
void PrintStack(stack *s, int ChessBoard[N][N], int n, int sum);

coord* Position(int row, int col);//输入骑士所在行列，返回骑士所在坐标
bool ValidIndex(int i, int j,int n);//判断[i][j]是否为一个合法的位置
//打印整个棋盘，n为棋盘格大小，sum为骑士巡逻的方法数
void PrintChessboard(int ChessBoard[N][N], int n, int sum);
//递归回溯法寻找骑士巡逻路线
void KnightPatrol_Recursion(int ChessBoard[N][N], int row, int col, int n);
//依托栈的非递归法寻找骑士巡逻路线
void KnightPatrol_Stack(int ChessBoard[N][N], int rol, int col, int n, stack *patrol);

int main(){
    int ChessBoard[N][N];
    int n;//棋盘大小
    int row, col;//骑士的起始位置
    memset(ChessBoard, 0, sizeof(ChessBoard));//将棋盘置0
    
    cout << "请输入棋盘的边长n：";
    cin >> n;
    cout << "请输入骑士在棋盘上的起始位置row, col：";
    cin >> row >> col;
    
    cout << "骑士巡逻方法1:递归回溯法：" << endl;
    ChessBoard[row][col] = ++depth;
    KnightPatrol_Recursion(ChessBoard, row, col, n);
    if (flag == 0) cout << "不存在骑士巡逻路线" << endl;
    
    
    cout << "骑士巡逻方法2:依托栈实现的非递归法：" << endl;
    stack *patrol = InitStack();//创建一个存放骑士坐标的栈
    coord *init = Position(row, col);//骑士的初始位置
    StackPush(patrol, *init);
    KnightPatrol_Stack(ChessBoard, row, col, n, patrol);
    if (flag2 == 0) cout << "不存在骑士巡逻路线" << endl;
}

stack* InitStack(){
    stack *s = new stack;
    s->top = -1;//栈顶指针（即数组下标）赋初值-1
    return s;
}

bool StackEmpty(stack *s){//如果栈空返回1，否则返回0
    if (s->top == -1) return true;
    else return false;
}

coord StackTop(stack *s){//返回栈顶元素
    if (s->top > -1 && s->top < MAXSIZE) return s->data[s->top];
    else{
        coord wrongpos;
        wrongpos.row = -1;
        wrongpos.col = -1;
        return wrongpos;
    }
}

void StackPush(stack *s, coord value){//压入栈
    if (s->top == MAXSIZE - 1) return;
    else s->data[++s->top] = value;
}

void StackPop(stack *s){//弹出栈顶元素
    if (s->top == -1) return;
    else{
        s->top--;
    }
}

coord* Position(int row, int col){
    coord *pos = new coord;
    pos->row = row;
    pos->col = col;
    return pos;
}

bool ValidIndex(int i, int j,int n){//n是棋盘的大小
    if (i >= 0 && j >= 0 && i < n && j < n) return true;
    else return false;
}

void PrintChessboard(int ChessBoard[N][N], int n, int sum){
    cout << "骑士巡逻路线" << sum << endl;
    for (int i = 0 ; i < n ; i++){
        for (int j = 0 ; j < n ; j++){
            cout << ChessBoard[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void PrintStack(stack *s, int ChessBoard[N][N], int n, int sum){
    for (int i = 0 ; i <= s->top ; i++){
        ChessBoard[s->data[i].row][s->data[i].col] = i+1;
    }
    PrintChessboard(ChessBoard, n, sum);
}

void KnightPatrol_Recursion(int ChessBoard[N][N], int row, int col, int n){
    //递归回溯法解决骑士巡逻问题
    //row, col是骑士的行列
    if (depth >= n * n){ //搜索深度达到n*n说明已经搜到答案
        sum++;//骑士巡逻方法数+1
        PrintChessboard(ChessBoard, n, sum);
        flag = 1;//骑士巡逻路线存在
        return;//回溯
    }
    for (int i = 0 ; i < 8 ; i++){//骑士分别向8个方向移动
        int new_row = row + step[i][0];
        int new_col = col + step[i][1];
        //如果移动方向合法，且该方向之前没有走过
        if (ValidIndex(new_row, new_col, n) == true && ChessBoard[new_row][new_col] == 0){
            ChessBoard[new_row][new_col] = ++depth;
            KnightPatrol_Recursion(ChessBoard, new_row, new_col, n);//从新位置开始搜索
            //到这里仍然没搜到，则说明这条路走不通，需回溯
            depth--;//搜索深度-1
            ChessBoard[new_row][new_col] = 0;//搜索过的位置置零
        }
    }
}

void KnightPatrol_Stack(int ChessBoard[N][N], int row, int col, int n, stack *patrol){
    //非递归法解决骑士巡逻问题
    //rol, col是骑士的行列
    if (depth == n * n){ //搜索深度达到n*n说明已经搜到答案
        sum2++;//骑士巡逻方法数+1
        PrintStack(patrol, ChessBoard, n, sum2);
        flag2 = 1;//骑士巡逻路线存在
        return;//回溯
    }
    for (int i = 0 ; i < 8 ; i++){//骑士分别向8个方向移动
        int new_row = row + step[i][0];
        int new_col = col + step[i][1];
        //如果移动方向合法，且该方向之前没有走过
        if (ValidIndex(new_row, new_col, n) == true && ChessBoard[new_row][new_col] == 0){
            ChessBoard[new_row][new_col] = ++depth;
            coord *nextpos = Position(new_row, new_col);//记录新位置
            StackPush(patrol, *nextpos);//将新位置入栈
            KnightPatrol_Stack(ChessBoard, new_row, new_col, n, patrol);//从新位置开始搜索
            //到这里仍然没搜到，则说明这条路走不通，需回溯
            ChessBoard[new_row][new_col] = 0;//搜索过的位置置零
            depth--;//搜索深度-1
            StackPop(patrol);//将走不通的位置弹出栈
        }
    }
}

