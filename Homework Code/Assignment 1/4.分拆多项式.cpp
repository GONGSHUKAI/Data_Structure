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

void Decompose(PNode *Poly){
    PNode *odd = new PNode;//奇次项头指针
    PNode *odd_tail;//奇次项尾指针
    PNode *p = Poly;//原多项式头指针
    PNode *q = Poly->next;
    odd->next = NULL;
    odd_tail = odd;
    while (q != Poly){//循环链表遍历条件
        if (q->exp % 2 == 0){//偶次项保留在原多项式Poly中
            p = q;
            q = q->next;
        }
        else{//奇次项尾插入odd这个多项式中
            p->next = q->next; //先从Poly中删除这个节点
            //接下来把q这个新结点插入odd这个链表里
            q->next = NULL;//赋新结点的指针域为NULL
            odd_tail->next = q;//让上一个尾巴指向新结点（尾插）
            odd_tail = q;//新结点成为了新尾巴
            q = p->next;//更新q的位置，继续遍历原多项式Poly
        }
    }
    odd_tail->next = odd;//让奇次项多项式形成一个循环链表
    std::cout << "输出奇次项多项式：";
    PrintPolyn(odd);
    std::cout << "输出偶次项多项式：";
    PrintPolyn(Poly);
}

int main(int argc, const char * argv[]) {
    //测试样例：对5x^4+ 2x^2+ 17x^1+ 5x^0进行奇偶多项式分拆
    //输出结果：
        //输出奇次项多项式：17x^1
        //输出偶次项多项式：5x^4+ 2x^2+ 5x^0
        
    PNode *term4 = CreateNode(5, 0, NULL);
    PNode *term3 = CreateNode(17, 1, term4);
    PNode *term2 = CreateNode(2, 2, term3);
    PNode *term1 = CreateNode(5, 4, term2);
    PNode *Poly = CreateNode(0, 0, term1);
    term4->next = Poly;
    PrintPolyn(Poly);
    Decompose(Poly);
    
}
