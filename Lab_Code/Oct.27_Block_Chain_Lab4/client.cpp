#include "Block_Chain.h"
#include <random>

int getRandom(int start, int end){
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(start, end);
    return dis(gen);
}

void sendTransaction(int fileNumber, int server, block *firstblock, block *endblock){
    cout << "Visit Server " << server << endl;
    string fileName = "block_chain_server" + to_string(server) + "/clientMessage/clientMessage" + to_string(fileNumber) + ".txt";
    ofstream outputfile;
    outputfile.open(fileName);

    if (outputfile.is_open()){
        string client_txid;
        cout << "Input txid: ";
        cin >> client_txid;

        block *p = firstblock;
        int find = 0;//找到交易信息则find = 1, 否则find = 0
        int i = 0;
        int j = 0;
        int k = 0;
        while (p != endblock){
            while (p->transactions[i].txid != ""){
                if (p->transactions[i].txid == client_txid){
                    outputfile << "Transaction Request" << "\n";
                    outputfile << "transaction" << i << "info"<< "\n";
                    outputfile << p->height << "\n";
                    outputfile << p->transactions[i].txid << "\n";
                    outputfile << p->transactions[i].input_count << "\n";
                    outputfile << p->transactions[i].output_count << "\n";
                    outputfile << p->transactions[i].is_coinbase << "\n";
                    while (p->transactions[i].inputs[j].scriptSig != ""){
                        outputfile << "\n";
                        outputfile << "input" << j << "info"<< "\n";
                        outputfile << p->transactions[i].inputs[j].pre_block << "\n";
                        outputfile << p->transactions[i].inputs[j].prevTxID << "\n";
                        outputfile << p->transactions[i].inputs[j].prevTxOutIndex << "\n";
                        outputfile << p->transactions[i].inputs[j].scriptSig << "\n";
                        j++;
                    }
                    while (p->transactions[i].outputs[k].script != ""){
                        outputfile << "\n";
                        outputfile << "output" << k << "info"<< "\n";
                        outputfile << p->transactions[i].outputs[k].txid << "\n";
                        outputfile << p->transactions[i].outputs[k].index << "\n";
                        outputfile << p->transactions[i].outputs[k].value << "\n";
                        outputfile << p->transactions[i].outputs[k].script << "\n";
                        k++;
                    }                
                    find = 1;//找到这条交易记录
                    break;
                }
                i++;
            }
            if (find == 1) break;
            else{
                i = 0;
                p = p->next;
            }
        }
        if (find == 1){
            cout << "Message Successfully Sent!" << endl;
            outputfile.close();
        }
        else{
            cout << "Transaction Not Found!" << endl;
        }
    }
    else{
        cout << "Unable to open file";
    }
}

void sendInquiry(int fileNumber, int server, int category, string content){
    cout << "Visit Server " << server << endl;
    string fileName = "block_chain_server" + to_string(server) + "/clientMessage/clientMessage" + to_string(fileNumber) + ".txt";
    ofstream outputfile;
    outputfile.open(fileName);
    if (category == 1){//根据height查询
        outputfile << "height" << "\n";
        outputfile << content << "\n";
        cout << "Message Successfully Sent!" << endl;
        outputfile.close();
    }
    else if (category == 2){//根据hash查询
        outputfile << "hash" << "\n";
        outputfile << content << "\n";
        cout << "Message Successfully Sent!" << endl;
        outputfile.close();
    }
    else{//根据txid查询
        outputfile << "txid" << "\n";
        outputfile << content << "\n";
        cout << "Message Successfully Sent!" << endl;
        outputfile.close();
    }
}

int main(){
    int request = 0;
    int request2 = 0;
    int fileNumber1 = 1;
    int fileNumber2 = 1;
    block* firstBlock = InitBlockChain();
    while(1){
        cout << "Input your request: 1 Transaction; 2 Inquiry" << endl;
        cin >> request;
        if (request == 1){
            int server = getRandom(1, 2);
            if (server == 1){
                sendTransaction(fileNumber1, 1, firstBlock, nullptr);
                fileNumber1++;
            }
            else{
                sendTransaction(fileNumber2, 2, firstBlock, nullptr);
                fileNumber2++;
            }
        }
        else if (request == 2){
            cout << "Inquiry Type: 1 Search by height; 2 Search by hash; 3 Search by txid" << endl;
            cin >> request2;
            if (request2 == 1){//按照height查询
                cout << "Input server: " << endl;
                int server;
                cin >> server;
                string height;
                cout << "Input height: " << endl;
                cin >> height;
                if (server == 1){
                    sendInquiry(fileNumber1, server, 1, height);
                    fileNumber1++;
                }
                else{
                    sendInquiry(fileNumber2, server, 1, height);
                    fileNumber2++;
                }
            }
            else if (request2 == 2){//按照hash查询
                cout << "Input server: " << endl;
                int server;
                cin >> server;
                string hash;
                cout << "Input hash: " << endl;
                cin >> hash;

                if (server == 1){
                    sendInquiry(fileNumber1, server, 2, hash);
                    fileNumber1++;
                }
                else{
                    sendInquiry(fileNumber2, server, 2, hash);
                    fileNumber2++;
                }
            }
            else{//按照txid查询
                cout << "Input server: " << endl;
                int server;
                cin >> server;
                string txid;
                cout << "Input txid: " << endl;
                cin >> txid;
                
                if (server == 1){
                    sendInquiry(fileNumber1, server, 3, txid);
                    fileNumber1++;
                }
                else{
                    sendInquiry(fileNumber2, server, 3, txid);
                    fileNumber2++;
                }
            }
        }
        else{
            cout << "Invalid Request! Please input again!" << endl;
        }
    }
}