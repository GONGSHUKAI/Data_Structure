#include "config.h"

bool judge_ClientMSG(string ClientMSG){
    string filePath = "testFiles/"+ClientMSG;
    ifstream inputFile(filePath);
    string line;
    getline(inputFile, line);

    if (line == "Transaction Request") return true;//����ClientMSG.txt�ĵ�һ�У������ǽ��׻��ǲ�ѯ���ǽ����򷵻�true
    else return false;
}

int main(){
    string ClientMSG = "clientMessage1.txt";
    cout << judge_ClientMSG(ClientMSG) << endl;
}