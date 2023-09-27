#include <iostream>

typedef struct PNode{
    double coef;
    int exp;
    struct PNode *next;
}PNode;

PNode* CreateNode(double coef, int exp, PNode *next){
    PNode* term = new PNode;
    term->coef = coef;
    term->exp = exp;
    term->next = next;
    return term;
}

void PrintPolyn(PNode *head){
    PNode *p = new PNode;
    p = head->next;
    while (p->next != head){ //循环链表遍历终止条件
        std::cout << p->coef << "x^" << p->exp << "+ ";
        p = p->next;
    }
    std::cout << p->coef << "x^" << p->exp << "\n";
}

void Derivative(PNode *P){
    PNode *cursor = P->next;//从表头结点的下一个开始遍历多项式每一项
    PNode *temp = new PNode;
    if(cursor->exp == 0){//特殊情况：如果输入为一个常数
        cursor->coef = 0;//直接令其导数为0
    }
    else{
        while (cursor != P){//循环链表遍历终止条件
            if (cursor->next->exp == 0){//如果某一项为常数，导数为0，需要把这项从链表中删除
                temp = cursor->next;
                cursor->next = cursor->next->next;
                delete temp;
            }
            cursor->coef *= cursor->exp;//求导过程1
            cursor->exp--;//求导过程2
            cursor = cursor->next;
        }
    }
}

int main(int argc, const char * argv[]) {
    //测试样例：对5x^4+ 2x^2+ 17x^1+ 5x^0求导
    //输出结果：20x^3+ 4x^1+ 17x^0
    PNode *term4 = CreateNode(5, 0, NULL);
    PNode *term3 = CreateNode(17, 1, term4);
    PNode *term2 = CreateNode(2, 2, term3);
    PNode *term1 = CreateNode(5, 4, term2);
    PNode *head = CreateNode(0, 0, term1);
    term4->next = head;
    PrintPolyn(head);
    Derivative(head);
    PrintPolyn(head);
}
