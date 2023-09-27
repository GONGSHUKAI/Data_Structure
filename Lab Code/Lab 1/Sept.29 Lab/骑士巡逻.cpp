#include <iostream>
#include <stack>
#include <set>

#define N 8

using namespace std;

bool ValidIndex(int i, int j,int n){//n是棋盘的大小
    if (i >= 0 && j >= 0 && i < n && j < n) return true;
    else return false;
}

void PrintChessboard(int ChessBoard[N][N], int n){
    for (int i = 0 ; i < n ; i++){
        for (int j = 0 ; j < n ; j++){
            cout << ChessBoard[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

void DFS(int ChessBoard[N][N], int x, int y, int n, int depth){//x,y是骑士的位置
    if (depth == n * n){ //搜索深度达到n*n说明已经搜到答案
        PrintChessboard(ChessBoard, n);
        return;
    }
    if (ValidIndex(x, y, n) == false || ChessBoard[x][y] != 0) return;//下标越界或已访问过该下标则回溯
    else{
        ChessBoard[x][y] = ++depth;
        //PrintChessboard(ChessBoard, n);
        DFS(ChessBoard, x-2, y-1, n, depth);
        DFS(ChessBoard, x-1, y-2, n, depth);
        DFS(ChessBoard, x+1, y-2, n, depth);
        DFS(ChessBoard, x+2, y-1, n, depth);
        DFS(ChessBoard, x+2, y+1, n, depth);
        DFS(ChessBoard, x+1, y+2, n, depth);
        DFS(ChessBoard, x-1, y+2, n, depth);
        DFS(ChessBoard, x-2, y+1, n, depth);
        //到这儿还没搜到说明此路不通，需要回溯
        ChessBoard[x][y] = 0;//先将走不通的路置0
        return;//然后回溯
    }
}

int main(){
    int ChessBoard[N][N];
    int depth = 0;
    int n;//棋盘大小
    int x,y;//骑士的起始位置
    memset(ChessBoard, 0, sizeof(ChessBoard));
    
    cout << "请输入棋盘的边长n：";
    cin >> n;
    cout << "请输入骑士在棋盘上的起始位置x,y：";
    cin >> x >> y;
    
    DFS(ChessBoard, x, y, n, depth);
}
