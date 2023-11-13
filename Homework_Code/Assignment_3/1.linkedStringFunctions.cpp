#include <iostream>
#define MAXSIZE 1000
typedef struct LinkString{
    char c;
    struct LinkString *next;
}LinkString;

LinkString* StrAssign(char *str){
    LinkString *Mystring = new LinkString;
    Mystring->c = '\0';
    Mystring->next = NULL;
    LinkString *p = Mystring;
    for (int i = 0 ; i < strlen(str) ; i++){
        LinkString *node = new LinkString;
        node->c = str[i];
        node->next = NULL;
        p->next = node;
        p = node;
    }
    return Mystring;
}

void StrCopy(LinkString *s, LinkString *t){//把串t拷贝给串s
    LinkString *p = t->next;
    LinkString *q = s;
    while (p != NULL){
        LinkString *node = new LinkString;
        node->c = p->c;
        node->next = NULL;
        q->next = node;
        p = p->next;
        q = q->next;
    }
}

int StrCompare(LinkString *s, LinkString *t){//比较串s和串t
    LinkString *p = t->next;
    LinkString *q = s->next;
    while (p != NULL && q != NULL){
        if (p->c != q->c) return 0;//两串不相等
        p = p->next;
        q = q->next;
    }
    return 1;//两串相等
}

int StrLength(LinkString *s){//返回串的长度
    int len = 0;
    LinkString *p = s->next;
    while (p != NULL){
        len++;
        p = p->next;
    }
    return len;
}

void Concat(LinkString *s, LinkString *t){//把串t接到串s之后
    LinkString *p = s;
    LinkString *q = t;
    while (p->next != NULL){
        p = p->next;
    }//找到串s的末尾位置
    p->next = q->next;//把串t接到串s之后
    delete q;//删除串t的头结点
}

void SubString(LinkString *sub, LinkString *text, int pos, int len){
    //将串text中第pos位置起长度为len的子串存到串sub中
    LinkString *p = sub;
    LinkString *q = text;
    for (int i = 0 ; i <= pos ; i++){
        q = q->next;
    }//找到串text第pos位置
    for (int i = 0 ; i < len ; i++){
        LinkString *node = new LinkString;
        node->c = q->c;
        node->next = NULL;
        p->next = node;
        p = node;
        q = q->next;
    }
}

void PrintLinkString(LinkString *s){
    LinkString *p = s->next;
    while (p != NULL){
        std::cout << p->c;
        p = p->next;
    }
    std::cout << std::endl;
}



int main(){
    char str1[100]="abcdefghij";
    char str2[100]="xyzwu";
    char str3[100]="abc";
    LinkString *test = StrAssign(str1);
    LinkString *test2 = StrAssign(str2);
    LinkString *sub = StrAssign(str3);
    std::cout << StrCompare(test, test2) << std::endl;
    std::cout << StrLength(test) << std::endl;
    StrCopy(test, test2);
    PrintLinkString(test);
    Concat(test, test2);
    PrintLinkString(test);
    SubString(sub, test, 3, 3);
    PrintLinkString(sub);
}
