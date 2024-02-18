#ifndef append
#define append
#include "structure.h"

void appendFile(string fileName){
    cout << "Number of original accounts in the graph: " << graph.size() << endl;
    cout << endl;
    readTransactions(fileName);
    cout << "Number of accounts after appending new data: " << graph.size() << endl;
}
/*
int main(){
    readBlock();
    readTransactions("tx_data_part1_v2");
    appendFile("tx_data_part2");
}
*/
#endif