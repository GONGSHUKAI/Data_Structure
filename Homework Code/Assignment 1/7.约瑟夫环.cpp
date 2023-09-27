#include <iostream>
#define MAX 10000
int sequence[MAX];

typedef struct LNode{
    int data;
    struct LNode *next;
}LNode;

LNode* CreateRing(int n){
    LNode *ring = new LNode;
    ring->next = NULL;
    LNode *tail = ring;
    for (int i = 0 ; i < n ; i++){
        LNode *p = new LNode;
        p->data = i + 1;
        p->next = NULL;
        tail->next = p;
        tail = p;
    }
    tail->next = ring->next;//第n个人接到第1个人，形成循环链表
    return ring;
}

void JosephRing(int m, int n, LNode *ring){
    LNode *pos = new LNode;
    pos = ring->next;
    int member = n;
    int i = 0;
    
    if (m%n == 1){
        while (member > 0){
            sequence[i]=pos->data;
            member--;
            i++;
            pos = pos->next;
        }
    }
    else{
        while (member > 1){
            for (int i = 0 ; i < m%n-2 ; i++){//找到出列人的前面一个人
                pos = pos->next;
            }
            sequence[i]=(pos->next)->data;
            LNode *temp = pos->next;
            pos->next = temp->next;
            free(temp);
            member--;
            i++;
            pos = pos->next;
        }
        sequence[i]=(pos->next)->data;
    }

    std::cout << "出列顺序为：" << "[";
    for (int i = 0 ; i < n-1 ; i++){
        std::cout << sequence[i] << " ";
    }
    std::cout << sequence[n-1] << "]" << "\n";
}

int main() {
    //输入格式：先输入n，代表共有n个人围成约瑟夫环，再输入m，代表报数到m的人出列
        //输入样例：12 4
    //输出结果：出列顺序为：[4 8 12 5 10 3 11 7 6 9 2 1]
    int n;
    int m;
    std::cin >> n >> m;
    LNode *ring = CreateRing(n);
    JosephRing(m, n, ring);
}
