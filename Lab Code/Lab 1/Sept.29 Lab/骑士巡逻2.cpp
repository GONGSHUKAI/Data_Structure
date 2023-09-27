#include <iostream>
#include <stack>
#include <set>

#define N 8
using namespace std;
int step[8][2]={{2,1},{1,2},{-1,2},{-2,1},{-2,-1},{-1,-2},{1,-2},{2,-1}};
int depth = 0;
int sum = 0;
int flag = 0;

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

void DFS(int ChessBoard[N][N], int x, int y, int n){//x,y是骑士的位置
    if (depth >= n * n){ //搜索深度达到n*n说明已经搜到答案
        sum++;
        PrintChessboard(ChessBoard, n, sum);
        flag = 1;
        return;
    }
    for (int i = 0 ; i < 8 ; i++){
        int new_x = x + step[i][0];
        int new_y = y + step[i][1];
        if (ValidIndex(new_x, new_y, n) == true && ChessBoard[new_x][new_y] == 0){
            ChessBoard[new_x][new_y] = ++depth;
            DFS(ChessBoard, new_x, new_y, n);
            depth--;
            ChessBoard[new_x][new_y] = 0;
        }
    }
}

int main(){
    int ChessBoard[N][N];
    int n;//棋盘大小
    int x,y;//骑士的起始位置
    memset(ChessBoard, 0, sizeof(ChessBoard));
    
    cout << "请输入棋盘的边长n：";
    cin >> n;
    cout << "请输入骑士在棋盘上的起始位置x,y：";
    cin >> x >> y;
    ChessBoard[x][y] = ++depth;
    DFS(ChessBoard, x, y, n);
    if (flag == 0) cout << "不存在骑士巡逻路线" << endl;
}
