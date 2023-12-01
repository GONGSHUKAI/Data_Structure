#include "config.h"

bool judge_ClientMSG(string ClientMSG){
    string filePath = "testFiles/"+ClientMSG;
    ifstream inputFile(filePath);
    string line;
    getline(inputFile, line);

    if (line == "Transaction Request") return true;//考察ClientMSG.txt的第一行，看他是交易还是查询，是交易则返回true
    else return false;
}

int main(){
    string ClientMSG = "clientMessage1.txt";
    cout << judge_ClientMSG(ClientMSG) << endl;
}