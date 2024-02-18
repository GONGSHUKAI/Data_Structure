#include "Block_Chain.h"
#include <random>
#include <filesystem>
#include <map>
#include <queue>
#include <chrono>
#include <thread>

queue <string> clientMessageQueue;//���1�Ŀͻ���Ϣ����
queue <string> blockMessageQueue;//���1��������Ϣ����
map <string, transaction> transactionPool;//���1�Ľ��׳�
string folderPath1 = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server2/clientMessage";//�ͻ���Ϣ�����ļ���
string folderPath2 = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server2/blockMessage";//�ͻ���Ϣ�����ļ���
map <string, bool> processedFiles1;//�Ѵ���Ŀͻ���Ϣ�����ļ�
map <string, bool> processedFiles2;//�Ѵ����������Ϣ�����ļ�
block* serverBlock;//���1��������

string randomStringGenerator(){
    string str = "3cc8c69766e26f4ec5b4672e6224cd81c75577674f3cce8c9bb9731a2bb0bd6a";
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, str.length()-1);
    string randomString = "";
    for (int i = 0; i < 64; i++){
        randomString += str[dis(gen)];
    }
    return randomString;
}

bool isWinner(){//�����н�����Ϊ0.05
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);
    vector<int> winningNumbers = {20, 40, 60, 80, 100};
    int randomValue = dis(gen);
    return std::find(winningNumbers.begin(), winningNumbers.end(), randomValue) != winningNumbers.end();
}

queue <string> getTxtFileNames(const string& folderPath, map <string, bool>& processedFiles) {
    queue <string> fileQueue;
    vector<string> sortedFileNames; // ���ڴ洢��������ļ���
    for (const auto& entry : std::__fs::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            string fileName = entry.path().filename().string();
            if (processedFiles.find(fileName) == processedFiles.end()) {//����ļ�û�н�������(��д���������ļ�)���ͽ����ļ����ļ�����ӡ�
                sortedFileNames.push_back(fileName);
                //processedFiles[fileName] = true;
            }
        }
    }
    // ���ļ�����������
    sort(sortedFileNames.begin(), sortedFileNames.end(), [](const string& a, const string& b) {
        return a < b; // ����ASCII����������
    });

    // ���������ļ��������
    for (const auto& fileName : sortedFileNames) {
        fileQueue.push(fileName);
    }
    return fileQueue;
}

block* InitServerBlock(){
    block* head = new block;
    head->height = 0;//����߶�
    head->hash = "7c5b79677777cc627166cabbc347679b6469749c7cbb7b19617f6c3674c4c3bb";//�Զ����ͷ����hash
    head->prevHash = "";//ǰһ������Ĺ�ϣֵ
    head->merkleRoot = "229accb4c760c7c57e7c769e4afce7e434c26757472c281277ceeb36618b2cc5";//�����������н��׵�Ĭ�˶�����
    head->nonce = 114514;//������
    head->next = nullptr;
    return head;//���ؽڵ�1����������ͷ���
}

block* createBlock(block* lastBLK, int n){//�ӽ��׳���ȡ��n(>=1)�����ף����һ������newBLK��
    block *newBLK = new block;
    newBLK->prevHash = lastBLK->hash;
    newBLK->height = (lastBLK->height) + 1;
    newBLK->merkleRoot = randomStringGenerator();
    newBLK->nonce = 0;
    newBLK->next = NULL;
    newBLK->hash = randomStringGenerator();

    vector<transaction> firstNTransactions;//�������Ϊȡ������ǰn������
    auto it = transactionPool.begin();
    int count = 0;
    while (it != transactionPool.end() && count < n) {
        firstNTransactions.push_back(it->second);
        ++it;
        ++count;
    }
    for(int i = 0 ; i < n ; i++){
        newBLK->transactions[i] = firstNTransactions[i];
        transactionPool.erase(firstNTransactions[i].txid);//��ȡ����Ԫ�شӽ��׳���ɾ��
    }
    return newBLK;
}

void sendBlockMessage(int blockMessageNumber, block* newBLK){
    //��newBLK�����͡�����һ���������ڵ�ġ�������Ϣ���С���
    string fileName = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server1/blockMessage/blockMessage" + to_string(blockMessageNumber) + ".txt";
    ofstream outputfile;
    outputfile.open(fileName);
    if (outputfile.is_open()){
        outputfile << newBLK->height << "\n";
        outputfile << newBLK->hash << "\n";
        outputfile << newBLK->prevHash << "\n";
        outputfile << newBLK->merkleRoot << "\n";
        outputfile << newBLK->nonce << "\n";
        int i = 0;
        int j = 0;
        int k = 0;
        outputfile << "\n";
        while (newBLK->transactions[i].txid != ""){
            outputfile << "transaction" << i << "info"<< "\n";
            outputfile << (newBLK->transactions[i]).txid << "\n";
            outputfile << (newBLK->transactions[i]).input_count << "\n";
            outputfile << (newBLK->transactions[i]).output_count << "\n";
            outputfile << (newBLK->transactions[i]).is_coinbase << "\n";
            while (newBLK->transactions[i].inputs[j].scriptSig != ""){
                outputfile << "\n";
                outputfile << "input" << j << "info"<< "\n";
                outputfile << newBLK->transactions[i].inputs[j].pre_block << "\n";
                outputfile << newBLK->transactions[i].inputs[j].prevTxID << "\n";
                outputfile << newBLK->transactions[i].inputs[j].prevTxOutIndex << "\n";
                outputfile << newBLK->transactions[i].inputs[j].scriptSig << "\n";
                j++;
            }
            while (newBLK->transactions[i].outputs[k].script != ""){
                outputfile << "\n";
                outputfile << "output" << k << "info"<< "\n";
                outputfile << newBLK->transactions[i].outputs[k].txid << "\n";
                outputfile << newBLK->transactions[i].outputs[k].index << "\n";
                outputfile << newBLK->transactions[i].outputs[k].value << "\n";
                outputfile << newBLK->transactions[i].outputs[k].script << "\n";
                k++;
            }
            i++;
        }
        outputfile.close();
    }
    else{
        cout << "Unable to open file";
    }
}

int judge_ClientMSG(string filePath){
    ifstream inputFile(filePath);
    string line;
    getline(inputFile, line);

    if (line == "Transaction Request") return 1;//����ClientMSG.txt�ĵ�һ�У������ǽ��׻��ǲ�ѯ���ǽ����򷵻�true
    else if (line == "") return 2;//�ļ�Ϊ�գ�����false
    else return 3;
}

void inquiryServerBlock(block *serverBlock, string filePath){
    ifstream inputFile(filePath);
    string category;
    string content;
    getline(inputFile, category);// inquiry category: height/hash/txid
    getline(inputFile, content);// inquiry content: heightNumber/hash/txid
    if (category == "height"){
        cout << "Client's inquiry height: " << content << endl;//"�û���ѯ������߶�Ϊ��
        int heightNumber = stoi(content);
        BlockInfo(heightNumber, serverBlock);
    }
    else if (category == "hash"){
        cout << "Client's inquiry hash: " << content << endl;
        //Not yet developed :)
    }
    else if (category == "txid"){
        cout << "Client's inquiry txid: " << content << endl;
        TransactionInfo(content, serverBlock, nullptr);
    }
    else cout << "Wrong inquiry category!" << endl;
}

bool find_tsc_in_tscPool(transaction tsc){
    auto it = transactionPool.find(tsc.txid);
    if (it != transactionPool.end()) return true;//�ҵ���
    else return false;//û�ҵ�
}

bool judgeConflictBlock(block *myBlock, block *serverBlock){
    block *temp = serverBlock;
    while (temp != NULL){
        if (temp->prevHash == myBlock->prevHash) return true;//�ҵ���
        else temp = temp->next;
    }
    return false;//û�ҵ�
}

block* recoverBlock(string fileName) {//����blockMSG.text�ļ���ԭһ��block recoverBLK
    block *recoverBLK = new block;
    ifstream inputFile(fileName);

    if (inputFile.is_open()) {
        string line;
        unsigned long long transactionIndex = 0;
        unsigned long long inputIndex = 0;
        unsigned long long outputIndex = 0;

        // Read block information
        getline(inputFile, line);
        istringstream blockHeight(line);
        blockHeight >> recoverBLK->height;

        getline(inputFile, line);
        istringstream blockHash(line);
        blockHash >> recoverBLK->hash;

        getline(inputFile, line);
        istringstream blockPrevHash(line);
        blockPrevHash >> recoverBLK->prevHash;

        getline(inputFile, line);
        istringstream blockMerkleRoot(line);
        blockMerkleRoot >> recoverBLK->merkleRoot;

        getline(inputFile, line);
        istringstream blockNonce(line);
        blockNonce >> recoverBLK->nonce;

        recoverBLK->next = nullptr;
        // Skip empty line
        getline(inputFile, line);

        while (getline(inputFile, line)) {
            if (line.find("transaction") != string::npos) {
                // Read transaction information
                getline(inputFile, line); // Skip transaction info line
                transaction& tx = recoverBLK->transactions[transactionIndex];
                istringstream txid(line);
                txid >> tx.txid;

                getline(inputFile, line);
                istringstream inputCount(line);
                inputCount >> tx.input_count;

                getline(inputFile, line);
                istringstream outputCount(line);
                outputCount >> tx.output_count;

                getline(inputFile, line);
                istringstream isCoinbase(line);
                isCoinbase >> tx.is_coinbase;

                // Read inputs
                for (int i = 0; i < tx.input_count; i++) {
                    getline(inputFile, line); // Skip empty line
                    getline(inputFile, line); // Skip input info line
                    input& in = tx.inputs[inputIndex];
                    
                    getline(inputFile, line);
                    istringstream pre_block(line);
                    pre_block >> in.pre_block;
                    
                    getline(inputFile, line);
                    istringstream prevTxID(line);
                    prevTxID >> in.prevTxID;

                    getline(inputFile, line);
                    istringstream prevTxOutIndex(line);
                    prevTxOutIndex >> in.prevTxOutIndex;

                    getline(inputFile, line);
                    istringstream scriptSig(line);
                    ostringstream scriptSigStream;
                    scriptSigStream << scriptSig.rdbuf();  // �� scriptSig �е������ַ�д�뵽 ostringstream ��
                    in.scriptSig = scriptSigStream.str();  // �� ostringstream �е����ݸ�ֵ�� in.scriptSig
                    inputIndex++;
                }
                // Read outputs
                for (int i = 0; i < tx.output_count; i++) {
                    getline(inputFile, line);// Skip empty line

                    getline(inputFile, line);// Skip output info line
                    output& out = tx.outputs[outputIndex];

                    getline(inputFile, line);
                    istringstream txid(line);
                    txid >> out.txid;

                    getline(inputFile, line);
                    istringstream index(line);
                    index >> out.index;

                    getline(inputFile, line);
                    istringstream value(line);
                    value >> out.value;

                    getline(inputFile, line);
                    istringstream script(line);
                    ostringstream scriptStream;
                    scriptStream << script.rdbuf();  // �� script �е������ַ�д�뵽 ostringstream ��
                    out.script = scriptStream.str();  // �� ostringstream �е����ݸ�ֵ�� out.script
                    outputIndex++;
                }

                // Reset input and output index for the next transaction
                inputIndex = 0;
                outputIndex = 0;

                // Increment transaction index
                transactionIndex++;
            }
        }
        inputFile.close();
    }
    return recoverBLK;
}

transaction recover_tsc(string fileName){
    transaction tx;//����ClientMSG.txt��ԭһ��transaction tx
    
    ifstream inputFile(fileName);

    if (inputFile.is_open()) {
        string line;
        unsigned long long inputIndex = 0;
        unsigned long long outputIndex = 0;

        getline(inputFile, line);// Skip transaction request line

        // Read transaction information
        getline(inputFile, line); // Skip transaction info line
        getline(inputFile, line); // Skip transaction height

        getline(inputFile, line);
        istringstream txid(line);
        txid >> tx.txid;

        getline(inputFile, line);
        istringstream inputCount(line);
        inputCount >> tx.input_count;

        getline(inputFile, line);
        istringstream outputCount(line);
        outputCount >> tx.output_count;

        getline(inputFile, line);
        istringstream isCoinbase(line);
        isCoinbase >> tx.is_coinbase;

        // Read inputs
        for (int i = 0; i < tx.input_count; i++) {
            getline(inputFile, line); // Skip empty line
            getline(inputFile, line); // Skip input info line
            input& in = tx.inputs[inputIndex];
            
            getline(inputFile, line);
            istringstream pre_block(line);
            pre_block >> in.pre_block;
            
            getline(inputFile, line);
            istringstream prevTxID(line);
            prevTxID >> in.prevTxID;

            getline(inputFile, line);
            istringstream prevTxOutIndex(line);
            prevTxOutIndex >> in.prevTxOutIndex;

            getline(inputFile, line);
            istringstream scriptSig(line);
            ostringstream scriptSigStream;
            scriptSigStream << scriptSig.rdbuf();  // �� scriptSig �е������ַ�д�뵽 ostringstream ��
            in.scriptSig = scriptSigStream.str();  // �� ostringstream �е����ݸ�ֵ�� in.scriptSig
            inputIndex++;
        }
        // Read outputs
        for (int i = 0; i < tx.output_count; i++) {
            getline(inputFile, line);// Skip empty line
            getline(inputFile, line);// Skip output info line
            output& out = tx.outputs[outputIndex];

            getline(inputFile, line);
            istringstream txid(line);
            txid >> out.txid;

            getline(inputFile, line);
            istringstream index(line);
            index >> out.index;

            getline(inputFile, line);
            istringstream value(line);
            value >> out.value;

            getline(inputFile, line);
            istringstream script(line);
            ostringstream scriptStream;
            scriptStream << script.rdbuf();  // �� script �е������ַ�д�뵽 ostringstream ��
            out.script = scriptStream.str();  // �� ostringstream �е����ݸ�ֵ�� out.script
            outputIndex++;
        }
        inputFile.close();
    }
    return tx;
}


int main(){
    block *serverBlock = InitServerBlock();
    block *tail = serverBlock;
    int blockMessageNumber = 1;//������ͳ��һ�����͹�����������Ϣ���Ӷ���������Ϣ�ļ�����
    while(1){
        clientMessageQueue = getTxtFileNames(folderPath1, processedFiles1);
        blockMessageQueue = getTxtFileNames(folderPath2, processedFiles2);
        if (isWinner()){//�н�������0.01-0.1֮��
            /*�ӡ����׳ء���ȡ��n (>=1)�����ף����һ������newBLK��
            newBLK��prevHash���ڱ��ڵ������������һ������lastBLK��hashֵ��
            newBLK��hashֵ���Բ���һ��������������ɣ�
            heightֵΪlastBLK��height+1�� merkleRoot��nonce��Ϊ�ա�
            newBLK�еĽ��׼���������n�����׹��ɡ�
            ��newBLK���뱾�ڵ����������ĩβ��
            ��newBLK��ĳ����ʽ������JSON������ַ��������͡�����һ���������ڵ�ġ�������Ϣ���С���
            */
            if (transactionPool.empty()){
                cout << "No transaction in the transaction pool!" << endl;
                this_thread::sleep_for(chrono::seconds(5));//��һ�����ִ����һ��ѭ��������server����
                continue;
            }
            else{
                block* newBLK = createBlock(tail, 1);
                tail->next = newBLK;
                tail = newBLK;
                cout << "Jackpot! A new block has been inserted to the chain!" << endl;
                sendBlockMessage(blockMessageNumber, newBLK);
            }
        }
        else{//û���н�
            if (!blockMessageQueue.empty()){ //��������Ϣ���С���Ϊ��
                //�ӡ�������Ϣ���С�ͷ��ȡ��һ����Ϣ������Ϊ���飩
                //�жϸ������Ƿ������������ͻ�����Ƿ����һ�����飬���������preHash��ͬ����
                string firstBlockMSG = blockMessageQueue.front();//�ӡ�������Ϣ���С�ͷ��ȡ��һ����Ϣ������Ϊ���飩
                blockMessageQueue.pop();
                processedFiles2[firstBlockMSG] = true;//����������Ϣ���Ϊ�������
                string filePath_BMSG = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server2/blockMessage/"+firstBlockMSG;
                block *firstBLK = recoverBlock(filePath_BMSG);//������Ϣ�ָ���һ������firstBLK

                if (judgeConflictBlock(firstBLK, serverBlock)){//"��ͻ"����������
                    cout << "Conflict! The block has been discarded!" << endl;
                    delete firstBLK;
                }
                else{
                //����������뵽���ڵ���������ĩβ��
                    firstBLK->next = tail->next;
                    tail->next = firstBLK;
                    tail = firstBLK;
                    cout << "The block has been inserted to the chain!" << endl;
                //�ӡ����׳ء���ɾ���������а����Ľ��ף�
                    int eraseNum = 0;
                    while (firstBLK->transactions[eraseNum].txid != ""){
                        transactionPool.erase(firstBLK->transactions[eraseNum].txid);
                        eraseNum++;
                    }
                    cout << "Correspondent transactions deleted in the transaction pool!" << endl;
                }
            }//������Ϣ���в�Ϊ��
            else{//��������Ϣ���С�Ϊ�գ��ӡ��ͻ���Ϣ���С�ͷ��ȡ��һ����ϢMSG��
                if (!clientMessageQueue.empty()){
                    string firstClientMSG = clientMessageQueue.front();//�ӡ��ͻ���Ϣ���С�ͷ��ȡ��һ����ϢMSG��
                    string filePath_CMSG = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server2/clientMessage/"+firstClientMSG;
                    //���filePath_CMSG����Ϊ�գ�ִ����һ��ѭ��
                    if (judge_ClientMSG(filePath_CMSG) == 2){
                        this_thread::sleep_for(chrono::seconds(5));//��һ�����ִ����һ��ѭ��������server����
                        continue;
                    }
                    clientMessageQueue.pop();
                    processedFiles1[firstClientMSG] = true;//���˿ͻ���Ϣ���Ϊ�������
                    if (judge_ClientMSG(filePath_CMSG) == 1){//MSG �ǽ���
                        cout << "Client's transaction request!" << endl;
                        transaction tsc = recover_tsc(filePath_CMSG);
                        if (!find_tsc_in_tscPool(tsc)){//�����׳ء��������ý��ף����ý��׼��롰���׳ء���
                            cout << "transaction added to the transaction pool!" << endl;
                            transactionPool.insert(pair<string, transaction>(tsc.txid, tsc));//���뽻�׳�
                        }
                        else{//�����ý���
                            cout << "The transaction has been discarded!" << endl;
                        }
                    }
                    else if (judge_ClientMSG(filePath_CMSG) == 3){//MSG �ǲ�ѯ
                        //�ڱ��ڵ�ά��������������ִ�в�ѯ��
                        //����ѯ����������Ļ�ϣ�
                        cout << "Client's inquiry request!" << endl;
                        inquiryServerBlock(serverBlock, filePath_CMSG);
                    }
                    else{}
                }
            }//������Ϣ����Ϊ�գ��ͻ���Ϣ���в�Ϊ��
        }//�н�orû���н�
        this_thread::sleep_for(chrono::seconds(5));//��һ�����ִ����һ��ѭ��������server����
    }//while(1)
}

