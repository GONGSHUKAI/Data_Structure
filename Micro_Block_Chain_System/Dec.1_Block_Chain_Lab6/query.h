#ifndef query
#define query
#include "structure.h"

/*
Function1: 
(1)Find all transactions record of a given account
(2)Print the number of records
(3)Print k transactions with the largest amount of money.
*/
void getRecord(unsigned long long timeStamp1, unsigned long long timeStamp2, int k, string account){
    unsigned long long allOutRecords = 0;
    unsigned long long allInRecords = 0;

    vector <nameAndRecord> outRecords;
    vector <nameAndRecord> inRecords;
    
    for (const auto &pair : graph[account].outEdges){
        string toAccount = pair.first;
        list <tx> txOut = pair.second.txs;
        for (const auto &elem : txOut){
            if (elem.timeStamp <= timeStamp2 && elem.timeStamp >= timeStamp1){
                nameAndRecord thisRecord;
                thisRecord.account = toAccount;
                thisRecord.tsc = elem;
                outRecords.push_back(thisRecord);
                allOutRecords++;
            }
        }
    }
    sort(outRecords.begin(),outRecords.end(),compareByAmount);
    cout << "Number of all transfer-out records: " << allOutRecords << endl;
    for (int i = 0 ; i < k && i < outRecords.size(); i++){
        cout << "Receiver: " << outRecords[i].account << endl;
        cout << "Transfer-out Records: " << endl;
        cout << "txid: " << outRecords[i].tsc.tx_id << ", " << "blockID: " << outRecords[i].tsc.blockID << ", " << "amount: " << outRecords[i].tsc.money << ", " << "timeStamp: " << outRecords[i].tsc.timeStamp << endl;
    }
    cout << endl;

    for (const auto &pair : graph[account].inEdges){
        string fromAccount = pair.first;
        list <tx> txIn = pair.second.txs;
        for (const auto &elem : txIn){
            if (elem.timeStamp <= timeStamp2 && elem.timeStamp >= timeStamp1){
                nameAndRecord thisRecord;
                thisRecord.account = fromAccount;
                thisRecord.tsc = elem;
                inRecords.push_back(thisRecord);
                allInRecords++;
            }
        }
    }
    sort(inRecords.begin(),inRecords.end(),compareByAmount);
    cout << "Number of all transfer-in records: " << allInRecords << endl;
    for (int i = 0 ; i < k && i < inRecords.size(); i++){
        cout << "Sender: " << inRecords[i].account << endl;
        cout << "Transfer-in Records: " << endl;
        cout << "txid: " << inRecords[i].tsc.tx_id << ", " << "blockID: " << inRecords[i].tsc.blockID << ", " << "amount: " << inRecords[i].tsc.money << ", " << "timeStamp: " << inRecords[i].tsc.timeStamp << endl;
    }
    cout << endl;
}
/*
Function2:Find the balance of certain account at certain time
*/

double getBalance(string account, unsigned long long timeStamp){
    double balance = 0;
    unordered_map <string, edge> income = graph[account].inEdges;
    unordered_map <string, edge> expense = graph[account].outEdges;
    for (const auto &pair : income){
        list <tx> incomeRecord = pair.second.txs;
        for (const auto &elem : incomeRecord){
            if (elem.timeStamp <= timeStamp){
                balance += elem.money;
            }
        }
    }
    for (const auto &pair : expense){
        list <tx> expenseRecord = pair.second.txs;
        for (const auto &elem : expenseRecord){
            if (elem.timeStamp <= timeStamp){
                balance -= elem.money;
            }
        }
    }
    return balance;
}

void printBalance(string account, unsigned long long timeStamp){
    cout << "Account Name: " << account << endl;
    cout << "Current Balance: " << getBalance(account, timeStamp) << endl;
}

/*
Function3: Prints the Forbes rank of the k richest people in the block chain network
*/

void Forbes(int k, unsigned long long timeStamp){
    //Output the k richest people in the network
    vector <worthOfMan> forbes;
    for (const auto &pair : graph){
        worthOfMan worth;
        worth.account = pair.first;
        worth.balance = getBalance(pair.first, timeStamp);
        forbes.push_back(worth);
    }
    sort(forbes.begin(), forbes.end(), forbesDescendComp);
    for (int i = 0 ; i < k ; i++){
        cout << "The " << i + 1 <<"th richest account: " << forbes[i].account << " Balance: "<< forbes[i].balance << endl;
    }
    cout << endl;
}
/*
int main(){
    readBlock();
    readTransactions("tx_data_part1_v2");
    //getRecord(1284753029, 1358886914, 5, "1EHew3p43C2YUFX4LX3dav5RMqvvzVMP5o");
    printBalance("1dice8EMZmqKvrGE4Qc9bUFf9PX3xaYDp", 1358886914);
    Forbes(5, 1358886914);
    return 0;
}
*/
#endif