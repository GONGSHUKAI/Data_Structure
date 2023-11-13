#include <iostream>

void CommonStr(char *str, char *sub, int &loc){
//求串str中出现的第一个最长重复子串sub及其位置loc
//KMP，时间复杂度为O(n^2)
    int next[50];
    int i,j;
    int len = 0;
    int max = 0;
    for(int k = 0; k < strlen(str); ++k){
        //这里为了找最长重复字串，必须以str每个字符为起点开始模式匹配
        //从串str的第k个字符开始，后面的部分作为KMP的主串，开始模式匹配
        i = k;
        j = k - 1;
        next[k] = k - 1;//赋next[0]初值
        while(i < strlen(str)){
            if(j == k - 1 || str[i] == str[j]){//模式匹配
                ++i;
                ++j;
                next[i] = j;
                if(str[i] == str[j]) len = j - k + 1;//如果模式匹配成功len+1
                else len = j - k;//否则保持len不变
            }
            else j = next[j];//模式匹配失败，模式串向右移
            if(len > max){//不断更新重复字串的max
                loc = k;    //记录出现第一个最长重复子串的位置
                max = len;  //记录最长重复子串的长度
            }
        }
    }
    //把最长重复字串储存在sub[]中
    for(i = loc, j = 0; i < max + loc; ++i, ++j) sub[j] = str[i];
    sub[j] = '\0';
    //打印最长重复字串的起始位置loc和最长重复字串sub
    std::cout << "最长重复字串起始位置：" << loc << std::endl;
    for (int i = 0 ; i < strlen(sub) ; i++){
        std::cout << sub[i];
    }
    std::cout << std::endl;
}

int main(){
    char str[]="abcdsuckmysuckmydick";
    char sub[10];
    int loc;
    CommonStr(str,sub,loc);
}
