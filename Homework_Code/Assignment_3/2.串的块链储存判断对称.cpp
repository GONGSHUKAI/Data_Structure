#include <iostream>
#include <stack>
#include <cstring>
#define CHUNKSIZE 5
using namespace std;
typedef struct Chunk{
    char ch[CHUNKSIZE];//每个节点含有CHUNKSIZE个字符
    struct Chunk *next;
}Chunk;

typedef struct{
    Chunk *head, *tail;
    int curlen;
}ChunkString;

stack <char> temp;//存放字符的栈

void InitString(ChunkString *str){
    str->head = new Chunk;
    str->tail = new Chunk;
    str->head->next = nullptr;
    str->tail = str->head;
    str->curlen = 0;
    string str_input;
    Chunk *p = new Chunk;
    cout << "输入字符串: ";
    cin >> str_input;
    int i = 0;
    int j = CHUNKSIZE - 1;
    while(str_input[i] != '\0'){
        if (j == CHUNKSIZE - 1){
            j = 0;
            Chunk *q = new Chunk;
            q->ch[j] = str_input[i];
            q->next = nullptr;
            i++;
            str->curlen++;
            str->tail->next = q;
            str->tail = q;
            p = q;
        }
        else{
            p->ch[++j] = str_input[i];
            i++;
            str->curlen++;
        }
    }
}

void PrintChunkString(ChunkString *str){
    int i = 0;
    int j = 0;
    Chunk *p = str->head->next;
    while (i < str->curlen){
        if (j == CHUNKSIZE){
            p = p->next;
            j = 0;
        }
        else{
            cout << p->ch[j];
            j++;
            i++;
        }
    }
}

bool IsSymmetric_ChunkString(ChunkString *str){
    int idx = 0;
    Chunk *p = str->head->next;
    char top;
    for (int i = 0 ; i < str->curlen ; i++){
        if (idx == CHUNKSIZE){
            p = p->next;
            idx = 0;
        }
        if (i < str->curlen / 2){
            temp.push(p->ch[idx]);
            idx++;
        }
        else if (i > (str->curlen - 1) / 2){
            top = temp.top();
            temp.pop();
            if (top != p->ch[idx]) return false;
            idx++;
        }
    }
    return true;
}

int main(){
    ChunkString str;
    InitString(&str);
    PrintChunkString(&str);
    cout << IsSymmetric_ChunkString(&str) << endl;
}

