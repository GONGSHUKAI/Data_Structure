#ifndef structure
#define structure
#include <iostream>
#include <string>
#include <stack>
#include <unordered_map>
#include <queue>
#include <list>
#include <fstream>
#include <sstream>
#include <ctime>
#include <algorithm>
using namespace std;

//define blocks, transactions, graph

//define transactions
typedef struct tx{
    unsigned long long tx_id;
    unsigned long long blockID;
    double money;//transaction amount
    unsigned long long timeStamp;
}tx;

typedef struct blockInfo{
    string hash;
    unsigned long long timeStamp;
    unordered_map <unsigned long long, tx> transactions;
    //int: tx_id
    //tx: Information of the transaction under tx_id
}blockInfo;

//define blocks
unordered_map <unsigned long long, blockInfo> blocks;
//key(unsigned long long): blockID
//value(blockInfo): Information of the block under blockID


//Every vertex of the graph represents an account
//Every directed edge between two vertexs represents the transaction between two accounts

//define edge
typedef struct edge{
    double totalAmount;//total transaction amounts
    list <tx> txs;//detailed transactions records
    //the sum-up of amount in detailed transactions records should be totalAmount
}edge;

typedef struct vertex{
    string accountName;
    unordered_map <string, edge> outEdges;//transfer out money
    //string: account name
    unordered_map <string, edge> inEdges;//receive money
    //string: account name
}vertex;

//define graph
unordered_map <string, vertex> graph;
//key: account name
//value: vertex corresponding to account name

void makeEdge(tx Mytx, string from, string to);
void makeVertex(string Account);

void readBlock(){
    ifstream file("dataset.1/block_part1.csv");
    if (!file) {
        cout << "Unable to open file!" << endl;
        return;
    }
    int count = 0;
    string line;
    getline(file, line); //read column title
    clock_t startTime = clock();//count time
    while (getline(file, line)){
        stringstream ss(line);
        string cell;
        int column = 0;
        unsigned long long blockID = 0;
        string hash = "";
        unsigned long long timeStamp = 0;

        while (getline(ss, cell, ',')){
            switch (column) {
                case 0:
                    blockID = stoull(cell);//stoull:string to unsigned long long
                    break;
                case 1:
                    hash = cell;
                    break;
                case 2:
                    timeStamp = stoull(cell);
                    break;
                default:
                    break;
            }
            column++;
        }
        blockInfo unit;
        unit.hash = hash;
        unit.timeStamp = timeStamp;
        blocks[blockID] = unit;
        count++;
    }
    file.close();
    cout << "Number of Blocks: "<< count << endl;
    clock_t endTime = clock();
    cout << "Block Reading Runtime: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    cout << endl;
}

void readTransactions(string fileName){
    fileName = "dataset.1/"+ fileName +".csv";
    ifstream file(fileName);
    if (!file) {
        cout << "Unable to open file!" << endl;
        return;
    }
    int count = 0;
    string line;
    getline(file, line); //read column title
    clock_t startTime = clock();//count time
    while (getline(file, line)){
        stringstream ss(line);
        string cell;
        int column = 0;
        unsigned long long txid = 0;
        unsigned long long blockID = 0;
        string from = "";
        double money = 0;
        string to = "";

        while (getline(ss, cell, ',')){
            switch (column) {
                case 0:
                    txid = stoull(cell);//stoull:string to unsigned long long
                    break;
                case 1:
                    blockID = stoull(cell);
                    break;
                case 2:
                    from = cell;
                    if (cell.find('\r') != string::npos){
                        from = cell.erase(cell.find('\r'));
                    }
                    break;
                case 3:
                    money = stod(cell);
                    break;
                case 4:
                    to = cell;
                    if (cell.find('\r') != string::npos){
                        to = cell.erase(cell.find('\r'));
                    }
                    break;
                default:
                    break;
            }
            column++;
        }
        //Formulate a transaction
        tx myTx;
        myTx.tx_id = txid;
        myTx.blockID = blockID;
        myTx.money = money;
        myTx.timeStamp = blocks[blockID].timeStamp;
        //Storage the transaction to corresponding block
        blocks[blockID].transactions[txid] = myTx;

        //make a vertex for "from" account
        makeVertex(from);//if account already exists then skip
        makeVertex(to);//if account already exists then skip
        makeEdge(myTx, from, to);
        count++;
    }
    file.close();
    cout << "Number of Transactions: "<< count << endl;
    clock_t endTime = clock();
    cout << "Transaction Reading Runtime: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    cout << endl;
}

void makeVertex(string Account){
    if (graph.find(Account) == graph.end()){//The vertex is not in the graph
        vertex V;
        V.accountName = Account;
        graph[Account] = V;
    }
}

void makeEdge(tx Mytx, string from, string to){
    //Modify/Adds OutEdges & InEdges of vertex "from" & "to"
    //Check if an edge from "from" to "to" already exists
    if (graph[from].outEdges[to].totalAmount != 0){
        //edge exists
        graph[from].outEdges[to].txs.push_back(Mytx);//adds the new transaction to the list
        graph[from].outEdges[to].totalAmount += Mytx.money;
    }
    else{
        //need to generate an edge
        edge E;
        E.totalAmount = Mytx.money;
        E.txs.push_back(Mytx);
        //Modify OutEdges of "from" and InEdges of "to"
        graph[from].outEdges[to] = E;//adds outEdge of "from"
        graph[to].inEdges[from] = E;//adds inEdge of "to"
    }
}

//Other micro structures
//1.getRecord
typedef struct nameAndRecord{
    string account;//sender or receiver
    tx tsc;//transaction
}nameAndRecord;

bool compareByAmount(const nameAndRecord &r1, const nameAndRecord &r2){
    return r1.tsc.money > r2.tsc.money;
}

//2.Forbes
typedef struct worthOfMan{
    string account;
    double balance;
}worthOfMan;

bool forbesDescendComp(const worthOfMan &man1, const worthOfMan &man2){
    return man1.balance > man2.balance;
}

//3.Degree Analysis
bool DescendComp(const pair <string, int> &pair1, const pair <string, int> &pair2){
    return pair1.second > pair2.second;
}




#endif
