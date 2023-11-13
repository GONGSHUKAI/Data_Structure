#include <iostream>

typedef struct CharacterNode{
    char character;
    struct CharacterNode *next;
}CharacterNode;


CharacterNode* CreateWord(int n){//n为单词长度
    CharacterNode *word = new CharacterNode;
    word->next = NULL;
    CharacterNode *tail = word;
    for (int i = 0 ; i < n ; i++){
        CharacterNode *p = new CharacterNode;
        std::cin >> p->character;
        p->next = NULL;
        tail->next = p;
        tail = p;
    }
    return word;
}

void PrintWord(CharacterNode *word, int n){
    CharacterNode *p = word->next;
    for (int i = 0 ; i < n ; i++){
        std::cout << p->character;
        p = p->next;
    }
}

int ListLength(CharacterNode *word){
    int length = 0;
    CharacterNode *p = word->next;
    while (p != NULL){
        length++;
        p = p->next;
    }
    return length;
}

CharacterNode* CommonSuffix(CharacterNode *str1, CharacterNode *str2){
    CharacterNode *p = str1->next;
    CharacterNode *q = str2->next;
    int str1_length = ListLength(str1);
    int str2_length = ListLength(str2);
    if (str1_length > str2_length){
        while (str1_length > str2_length){
            p = p->next;
            str1_length--;
        }
    }
    else{
        while (str2_length > str1_length){
            q = q->next;
            str2_length--;
        }
    }
    while (p != NULL && p->character != q->character){
        p = p->next;
        q = q->next;
    }
    std::cout << p->character;
    return p;
}

int main(){
    //输入格式：输入两个英文单词str1, str2，输入单词时注意每个字母空格
        //输入样例：l o a d i n g
        //输入样例：b e i n g
    //输出结果：i（即共同后缀起始位置处对应的字母）
    CharacterNode* str1 = CreateWord(7);
    CharacterNode* str2 = CreateWord(5);
    CommonSuffix(str1, str2);
}


