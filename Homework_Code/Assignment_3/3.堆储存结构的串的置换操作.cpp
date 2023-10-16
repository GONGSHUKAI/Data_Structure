#include <iostream>
#include <cstring>

typedef struct {
    char* ch;
    int length;
}HString;

void Replace(HString *S, char *T, char *V) {
    int tLen = strlen(T);
    int vLen = strlen(V);
    int sLen = S->length;
    
    // 遍历堆中的串 S
    for (int i = 0; i < sLen; ) {
        // 检查当前字符与待替换的串 T 是否匹配
        if (strncmp(S->ch + i, T, tLen) == 0) {
            // 匹配成功，将替换串 V 复制到结果串 U 中
            char* U = new char[sLen - tLen + vLen + 1];
            strncpy(U, S->ch, i);
            strncpy(U + i, V, vLen);
            strcpy(U + i + vLen, S->ch + i + tLen);
            // 更新堆中的串 S
            delete[] S->ch;
            S->ch = U;
            S->length = sLen - tLen + vLen;
            sLen = S->length;
            i += vLen;
            delete[] U;
        } 
        else {
            // 当前字符与待替换的串 T 不匹配，继续遍历下一个字符
            i++;
        }
    }
}

int main() {
    HString *S = new HString;
    S->length = 100;
    S->ch = new char[S->length + 1];

    char char1[] = "Hello, World, Fuck you!";
    char char2[] = "World";
    char char3[] = "Sir Gong";
    strcpy(S->ch, char1);

    Replace(S, char2, char3);

    std::cout << S->ch << std::endl;
}
