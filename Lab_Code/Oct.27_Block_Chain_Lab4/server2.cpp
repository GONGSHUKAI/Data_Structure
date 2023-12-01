#include "Block_Chain.h"
#include <random>
#include <filesystem>
#include <map>
#include <queue>
#include <chrono>
#include <thread>

queue <string> clientMessageQueue;//结点1的客户消息队列
queue <string> blockMessageQueue;//结点1的区块消息队列
map <string, transaction> transactionPool;//结点1的交易池
string folderPath1 = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server2/clientMessage";//客户消息队列文件夹
string folderPath2 = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server2/blockMessage";//客户消息队列文件夹
map <string, bool> processedFiles1;//已处理的客户消息队列文件
map <string, bool> processedFiles2;//已处理的区块消息队列文件
block* serverBlock;//结点1的区块链

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

bool isWinner(){//假设中奖概率为0.05
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(1, 100);
    vector<int> winningNumbers = {20, 40, 60, 80, 100};
    int randomValue = dis(gen);
    return std::find(winningNumbers.begin(), winningNumbers.end(), randomValue) != winningNumbers.end();
}

queue <string> getTxtFileNames(const string& folderPath, map <string, bool>& processedFiles) {
    queue <string> fileQueue;
    vector<string> sortedFileNames; // 用于存储已排序的文件名
    for (const auto& entry : std::__fs::filesystem::directory_iterator(folderPath)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            string fileName = entry.path().filename().string();
            if (processedFiles.find(fileName) == processedFiles.end()) {//如果文件没有进过队列(是写进来的新文件)，就将该文件的文件名入队。
                sortedFileNames.push_back(fileName);
                //processedFiles[fileName] = true;
            }
        }
    }
    // 对文件名进行排序
    sort(sortedFileNames.begin(), sortedFileNames.end(), [](const string& a, const string& b) {
        return a < b; // 按照ASCII码升序排序
    });

    // 将排序后的文件名入队列
    for (const auto& fileName : sortedFileNames) {
        fileQueue.push(fileName);
    }
    return fileQueue;
}

block* InitServerBlock(){
    block* head = new block;
    head->height = 0;//区块高度
    head->hash = "7c5b79677777cc627166cabbc347679b6469749c7cbb7b19617f6c3674c4c3bb";//自定义的头结点的hash
    head->prevHash = "";//前一个区块的哈希值
    head->merkleRoot = "229accb4c760c7c57e7c769e4afce7e434c26757472c281277ceeb36618b2cc5";//本区块中所有交易的默克尔树根
    head->nonce = 114514;//神秘数
    head->next = nullptr;
    return head;//返回节点1的区块链的头结点
}

block* createBlock(block* lastBLK, int n){//从交易池中取出n(>=1)个交易，组成一个区块newBLK。
    block *newBLK = new block;
    newBLK->prevHash = lastBLK->hash;
    newBLK->height = (lastBLK->height) + 1;
    newBLK->merkleRoot = randomStringGenerator();
    newBLK->nonce = 0;
    newBLK->next = NULL;
    newBLK->hash = randomStringGenerator();

    vector<transaction> firstNTransactions;//这里就认为取出池中前n个交易
    auto it = transactionPool.begin();
    int count = 0;
    while (it != transactionPool.end() && count < n) {
        firstNTransactions.push_back(it->second);
        ++it;
        ++count;
    }
    for(int i = 0 ; i < n ; i++){
        newBLK->transactions[i] = firstNTransactions[i];
        transactionPool.erase(firstNTransactions[i].txid);//将取出的元素从交易池中删除
    }
    return newBLK;
}

void sendBlockMessage(int blockMessageNumber, block* newBLK){
    //将newBLK“发送”给另一个区块链节点的“区块消息队列”。
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

    if (line == "Transaction Request") return 1;//考察ClientMSG.txt的第一行，看他是交易还是查询，是交易则返回true
    else if (line == "") return 2;//文件为空，返回false
    else return 3;
}

void inquiryServerBlock(block *serverBlock, string filePath){
    ifstream inputFile(filePath);
    string category;
    string content;
    getline(inputFile, category);// inquiry category: height/hash/txid
    getline(inputFile, content);// inquiry content: heightNumber/hash/txid
    if (category == "height"){
        cout << "Client's inquiry height: " << content << endl;//"用户查询的区块高度为：
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
    if (it != transactionPool.end()) return true;//找到了
    else return false;//没找到
}

bool judgeConflictBlock(block *myBlock, block *serverBlock){
    block *temp = serverBlock;
    while (temp != NULL){
        if (temp->prevHash == myBlock->prevHash) return true;//找到了
        else temp = temp->next;
    }
    return false;//没找到
}

block* recoverBlock(string fileName) {//根据blockMSG.text文件复原一个block recoverBLK
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
                    scriptSigStream << scriptSig.rdbuf();  // 将 scriptSig 中的所有字符写入到 ostringstream 中
                    in.scriptSig = scriptSigStream.str();  // 将 ostringstream 中的内容赋值给 in.scriptSig
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
                    scriptStream << script.rdbuf();  // 将 script 中的所有字符写入到 ostringstream 中
                    out.script = scriptStream.str();  // 将 ostringstream 中的内容赋值给 out.script
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
    transaction tx;//根据ClientMSG.txt复原一个transaction tx
    
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
            scriptSigStream << scriptSig.rdbuf();  // 将 scriptSig 中的所有字符写入到 ostringstream 中
            in.scriptSig = scriptSigStream.str();  // 将 ostringstream 中的内容赋值给 in.scriptSig
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
            scriptStream << script.rdbuf();  // 将 script 中的所有字符写入到 ostringstream 中
            out.script = scriptStream.str();  // 将 ostringstream 中的内容赋值给 out.script
            outputIndex++;
        }
        inputFile.close();
    }
    return tx;
}


int main(){
    block *serverBlock = InitServerBlock();
    block *tail = serverBlock;
    int blockMessageNumber = 1;//计数，统计一共发送过几次区块消息，从而给区块消息文件命名
    while(1){
        clientMessageQueue = getTxtFileNames(folderPath1, processedFiles1);
        blockMessageQueue = getTxtFileNames(folderPath2, processedFiles2);
        if (isWinner()){//中奖几率在0.01-0.1之间
            /*从“交易池”中取出n (>=1)个交易，组成一个区块newBLK。
            newBLK的prevHash等于本节点区块链表最后一个区块lastBLK的hash值。
            newBLK的hash值可以采用一个随机函数来生成；
            height值为lastBLK的height+1； merkleRoot和nonce都为空。
            newBLK中的交易集合由上述n个交易构成。
            将newBLK插入本节点的区块链表末尾。
            将newBLK以某个格式（比如JSON）组成字符串“发送”给另一个区块链节点的“区块消息队列”。
            */
            if (transactionPool.empty()){
                cout << "No transaction in the transaction pool!" << endl;
                this_thread::sleep_for(chrono::seconds(5));//隔一会儿再执行下一趟循环，避免server过载
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
        else{//没有中奖
            if (!blockMessageQueue.empty()){ //“区块消息队列”不为空
                //从“区块消息队列”头部取出一个消息（内容为区块）
                //判断该区块是否与已有区块冲突（即是否存在一个区块，与新区块的preHash相同。）
                string firstBlockMSG = blockMessageQueue.front();//从“区块消息队列”头部取出一个消息（内容为区块）
                blockMessageQueue.pop();
                processedFiles2[firstBlockMSG] = true;//将此区块消息标记为被处理过
                string filePath_BMSG = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server2/blockMessage/"+firstBlockMSG;
                block *firstBLK = recoverBlock(filePath_BMSG);//将该消息恢复成一个区块firstBLK

                if (judgeConflictBlock(firstBLK, serverBlock)){//"冲突"则丢弃该区块
                    cout << "Conflict! The block has been discarded!" << endl;
                    delete firstBLK;
                }
                else{
                //将该区块插入到本节点区块链表末尾；
                    firstBLK->next = tail->next;
                    tail->next = firstBLK;
                    tail = firstBLK;
                    cout << "The block has been inserted to the chain!" << endl;
                //从“交易池”中删除该区块中包含的交易；
                    int eraseNum = 0;
                    while (firstBLK->transactions[eraseNum].txid != ""){
                        transactionPool.erase(firstBLK->transactions[eraseNum].txid);
                        eraseNum++;
                    }
                    cout << "Correspondent transactions deleted in the transaction pool!" << endl;
                }
            }//区块消息队列不为空
            else{//“区块消息队列”为空，从“客户消息队列”头部取出一个消息MSG；
                if (!clientMessageQueue.empty()){
                    string firstClientMSG = clientMessageQueue.front();//从“客户消息队列”头部取出一个消息MSG；
                    string filePath_CMSG = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/block_chain_server2/clientMessage/"+firstClientMSG;
                    //如果filePath_CMSG内容为空，执行下一趟循环
                    if (judge_ClientMSG(filePath_CMSG) == 2){
                        this_thread::sleep_for(chrono::seconds(5));//隔一会儿再执行下一趟循环，避免server过载
                        continue;
                    }
                    clientMessageQueue.pop();
                    processedFiles1[firstClientMSG] = true;//将此客户消息标记为被处理过
                    if (judge_ClientMSG(filePath_CMSG) == 1){//MSG 是交易
                        cout << "Client's transaction request!" << endl;
                        transaction tsc = recover_tsc(filePath_CMSG);
                        if (!find_tsc_in_tscPool(tsc)){//“交易池”不包含该交易，将该交易加入“交易池”；
                            cout << "transaction added to the transaction pool!" << endl;
                            transactionPool.insert(pair<string, transaction>(tsc.txid, tsc));//加入交易池
                        }
                        else{//丢弃该交易
                            cout << "The transaction has been discarded!" << endl;
                        }
                    }
                    else if (judge_ClientMSG(filePath_CMSG) == 3){//MSG 是查询
                        //在本节点维护的区块链表中执行查询；
                        //将查询结果输出在屏幕上；
                        cout << "Client's inquiry request!" << endl;
                        inquiryServerBlock(serverBlock, filePath_CMSG);
                    }
                    else{}
                }
            }//区块消息队列为空，客户消息队列不为空
        }//中奖or没有中奖
        this_thread::sleep_for(chrono::seconds(5));//隔一会儿再执行下一趟循环，避免server过载
    }//while(1)
}

