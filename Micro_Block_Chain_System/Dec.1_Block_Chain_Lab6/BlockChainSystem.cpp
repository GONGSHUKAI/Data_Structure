#include "query.h"
#include "analysis.h"
#include "append.h"

int main(){
    readBlock();
    readTransactions("tx_data_part1_v2");
    while(1){
        char command;
        int detail;
        cout << "Input your command" << endl;
        cout << "------------------------------------------"<< endl;
        cout << "A. Inquiry" << endl;
        cout << "B. Data Analysis" << endl;
        cout << "C. Append Data" << endl;
        cout << "D. Quit System" << endl;
        cout << "------------------------------------------"<< endl;
        cin >> command;
        if (command == 'A'){
            cout << "1. Query all transfer-ins and transfer-outs of certain account." << endl;
            cout << "2. Query the balance of a certain account." << endl;
            cout << "3. Query the Forbes list." << endl;
            cin >> detail;
            if (detail == 1){
                cout << "------------------------------------------"<< endl;
                string accountName;
                unsigned long long timeStamp1;
                unsigned long long timeStamp2;
                int k;
                cout << "Input account name: ";
                cin >> accountName;
                cout << "Input time period (Format: Timestamp1 Timestamp2): ";
                cin >> timeStamp1 >> timeStamp2;
                cout << "Input the number of records you want to look up: ";
                cin >> k;
                cout << endl;
                getRecord(timeStamp1, timeStamp2, k, accountName);
                cout << "------------------------------------------"<< endl;
                continue;
            }
            else if (detail == 2){
                cout << "------------------------------------------"<< endl;
                string accountName;
                unsigned long long timeStamp;
                cout << "Input account name: ";
                cin >> accountName;
                cout << "Input time: ";
                cin >> timeStamp;
                cout << endl;
                printBalance(accountName, timeStamp);
                cout << "------------------------------------------"<< endl;
                continue;
            }
            else if (detail == 3){
                cout << "------------------------------------------"<< endl;
                int k;
                unsigned long long timeStamp;
                cout << "Input the number k for top k on the Forbes list: ";
                cin >> k;
                cout << "Input due time: ";
                cin >> timeStamp;
                cout << endl;
                Forbes(k, timeStamp);
                cout << "------------------------------------------"<< endl;
                continue;
            }
            else{
                cout << "------------------------------------------"<< endl;
                cout << "Invalid command! Return to menu." << endl;
                cout << "------------------------------------------"<< endl;
                continue;
            }
        }
        else if (command == 'B'){
            cout << "1. Analyze degree information of transaction graph." << endl;
            cout << "2. Detect possible ring structure in graph." << endl;
            cout << "3. Query certain account's shortest path to other accounts." << endl;
            cin >> detail;
            if (detail == 1){
                cout << "------------------------------------------"<< endl;
                int k;
                cout << "Input the number k to query accounts with top k indegrees/outdegrees: ";
                cin >> k;
                cout << endl;
                degreeAnalysis(k);
                cout << "------------------------------------------"<< endl;
                continue;
            }   
            else if (detail == 2){
                cout << "------------------------------------------"<< endl;
                topologicalSort();
                cout << "------------------------------------------"<< endl;
                continue;
            }
            else if (detail == 3){
                cout << "------------------------------------------"<< endl;
                string accountName;
                cout << "Input account name: ";
                cin >> accountName;
                cout << endl;
                Dijkstra2(accountName);
                cout << "------------------------------------------"<< endl;
                continue;
            }
            else{
                cout << "------------------------------------------"<< endl;
                cout << "Invalid command! Return to menu." << endl;
                cout << "------------------------------------------"<< endl;
                continue;
            }
        }
        else if (command == 'C'){
            cout << "------------------------------------------"<< endl;
            string fileName;
            cout << "Input the file name of new transaction dataset: ";
            cin >> fileName;
            cout << endl;
            appendFile(fileName);
            cout << "------------------------------------------"<< endl;
            continue;
        }
        else{
            cout << "------------------------------------------"<< endl;
            cout << "Exiting Block Chain System." << endl;
            cout << "------------------------------------------"<< endl;
            break;
        }
    }
}