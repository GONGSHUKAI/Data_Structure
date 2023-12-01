#ifndef Block_Chain
#define Block_Chain
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#define yes 100
#define no -100
#define NotUsed 200
#define Used -200
#define TransactionExist 999

#define MAXTRANS 100//一个块内最高有100条交易信息
#define MAXINPUT 100//一条交易信息中最高有100个输入
#define MAXOUTPUT 100//一条交易信息中最高有100个输出
using namespace std;

typedef struct output{
    string txid;//该output所属的交易
    unsigned long long index;//该output在所属交易中的索引值
    unsigned long long value;//该output的价值(数据已乘10^8,避免浮点误差)
    string script;//脚本

    int IsUse = NotUsed;
}output;

typedef struct input{
    unsigned long long pre_block;//该input所引用的output所在区块的高度
    string prevTxID;//该input所引用的output所在交易的txID
    unsigned long long prevTxOutIndex;//该input所引用的output位于所在交易output集合中的索引
    string scriptSig;//脚本和签名
}input;

typedef struct transaction{
    string txid;//交易的编号，具有唯一性
    unsigned long long input_count;//inputs的数量
    unsigned long long output_count;//outputs的数量
    input inputs[MAXINPUT];//一组input的集合，表示当前交易的输入所用到的输出
    output outputs[MAXOUTPUT];//一组output的集合，表示当前交易的输出
    int is_coinbase;//1为coinbase交易，0为非coinbase交易

    int valid = yes;
}transaction;

typedef struct block{
    unsigned long long height;//当前块的高度，一条链上每个区块的Height均不相同
    string hash;//本区块的哈希值
    string prevHash;//前一个区块的哈希值
    string merkleRoot;//本区块中所有交易的默克尔树根
    unsigned long long nonce;//神秘数
    transaction transactions[MAXTRANS];//一组transaction的集合
    struct block *next;
}block;

block* FileToBlock();
void FileToTransaction(block *currentBlock);
void FileToInput(block *currentBlock);
void FileToOutput(block *currentBlock);
int BlocksLength(block *firstblock);
void BlockInfo(int height, block *firstblock);
int TransactionInfo(string txid, block *firstblock, block *endblock);
block* InitBlockChain();
void CheckValidTransaction(block *firstblock);
bool IOCheck(block *firstblock, block *currentblock, transaction t);
transaction* FindPrevOutput(string My_txid, block *firstblock, block *endblock);

block* FileToBlock() {
    block *firstBlock = new block;
    ifstream file("demo/blocks.csv"); //打开CSV文件
    if (!file) {
        cout << "无法打开文件" << endl;
        return nullptr;
    }
    string line;
    getline(file, line); // 读取第一行标题（忽略）
    /*getline 函数返回一个布尔值，表示是否成功读取一行内容。
    如果读取成功，则返回true；
    如果已到达文件末尾或发生错误，则返回false。*/
    block *currentBlock = nullptr;
    block *prevBlock = nullptr;
    while (getline(file, line)) {
        stringstream ss(line);//创建一个stringstream 对象，字符串ss作为初始输入。
        string cell;
        int column = 0;
        block *newBlock = new block();// 创建新的 block

        while (getline(ss, cell, ',')){
        //使用逗号作为分隔符，从ss中提取每个字段的内容，并将其存储到字符串变量cell中
            switch (column) {
                case 0:
                    newBlock->height = stoull(cell);//stoi函数将字符串cell转换为unsigned long long
                    break;
                case 1:
                    newBlock->hash = cell;
                    break;
                case 2:
                    newBlock->prevHash = cell;
                    break;
                case 3:
                    newBlock->merkleRoot = cell;
                    break;
                case 4:
                    newBlock->nonce = stoull(cell);
                    break;
                default://当没有匹配到任何 case 标签时执行的代码块。
                    break;
            }
            column++;
        }
        // 如果是第一个 block，则将其设置为第一个 block
        if (prevBlock == nullptr) {
            firstBlock = newBlock;
            currentBlock = newBlock;
        } else {
            prevBlock->next = newBlock; // 将前一个 block 的 next 指针指向新的 block
            newBlock->next = nullptr;
            currentBlock = newBlock;
        }
        prevBlock = currentBlock;
    }
    file.close(); // 关闭文件
    return firstBlock;
}

void FileToTransaction(block *currentBlock){
    ifstream file("demo/transactions.csv");
    if (!file) {
        cout << "无法打开文件" << endl;
        return;
    }
    string line;
    getline(file, line);
    int transIndex = 0;

    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        int column = 0;
        int transHeight = 0;
        transaction currentTrans;
        currentTrans.valid = yes;
        while (getline(ss, cell, ',')) {//扫描一行的数据，以逗号为分隔符
            switch (column) {
                case 0:
                    transHeight = stoull(cell);
                    break;
                case 1:
                    currentTrans.txid = cell;
                    break;
                case 2:
                    currentTrans.is_coinbase = stoi(cell);
                    break;
                case 3:
                    currentTrans.input_count = stoull(cell);
                    break;
                case 4:
                    currentTrans.output_count = stoull(cell);
                    break;
                default:
                    break;
            }
            column++;
        }
        if (transHeight == currentBlock->height){
            currentBlock->transactions[transIndex] = currentTrans;
            transIndex++;
        }
        else{
            while (currentBlock->height != transHeight){
                currentBlock = currentBlock->next;
            }
            transIndex = 0;
            currentBlock->transactions[transIndex] = currentTrans;
            transIndex++;
        }
    }
    file.close();
}

void FileToInput(block *currentBlock){
    ifstream file("demo/inputs.csv");
    if (!file) {
        cout << "无法打开文件" << endl;
        return;
    }
    string line;
    getline(file, line);
    int transIndex = 0;
    int inputsIndex = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        int column = 0;
        int inputHeight = 0;
        string input_txid;
        input currentInput;
        while (getline(ss, cell, ',')) {//扫描一行的数据，以逗号为分隔符
            switch (column) {
                case 0:
                    inputHeight = stoull(cell);
                    break;
                case 1:
                    input_txid = cell;
                    break;
                case 2:
                    currentInput.pre_block = stoi(cell);
                    break;
                case 3:
                    currentInput.prevTxID = cell;
                    break;
                case 4:
                    currentInput.prevTxOutIndex = stoull(cell);
                    break;
                case 5:
                    currentInput.scriptSig = cell;
                default:
                    break;
            }
            column++;
        }
        if (inputHeight == currentBlock->height && input_txid == currentBlock->transactions[transIndex].txid){
            currentBlock->transactions[transIndex].inputs[inputsIndex] = currentInput;
            inputsIndex++;
        }
        else{
            while (currentBlock->height != inputHeight){
                currentBlock = currentBlock->next;
            }
            while (currentBlock->transactions[transIndex].txid != input_txid && currentBlock->transactions[transIndex].txid != ""){
                transIndex++;
            }
            inputsIndex = 0;
            currentBlock->transactions[transIndex].inputs[inputsIndex] = currentInput;
            inputsIndex++;
        }
    }
    file.close();
}

void FileToOutput(block *currentBlock){
    ifstream file("demo/outputs.csv");
    if (!file) {
        cout << "无法打开文件" << endl;
        return;
    }
    string line;
    getline(file, line);
    int transIndex = 0;
    int outputsIndex = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string cell;
        int column = 0;
        int outputHeight = 0;
        string output_txid;
        output currentOutput;
        currentOutput.IsUse = NotUsed;
        while (getline(ss, cell, ',')) {//扫描一行的数据，以逗号为分隔符
            switch (column) {
                case 0:
                    outputHeight = stoull(cell);
                    break;
                case 1:
                    currentOutput.txid = cell;
                    break;
                case 2:
                    currentOutput.index = stoi(cell);
                    break;
                case 3:
                    currentOutput.value = stoull(cell);
                    break;
                case 4:
                    currentOutput.script = cell;
                    break;
                default:
                    break;
            }
            column++;
        }
        if (outputHeight == currentBlock->height && currentOutput.txid == currentBlock->transactions[transIndex].txid){
            currentBlock->transactions[transIndex].outputs[outputsIndex] = currentOutput;
            outputsIndex++;
        }
        else{
            while (currentBlock->height != outputHeight){
                currentBlock = currentBlock->next;
            }
            while (currentBlock->transactions[transIndex].txid != currentOutput.txid && currentBlock->transactions[transIndex].txid != ""){
                transIndex++;
            }
            outputsIndex = 0;
            currentBlock->transactions[transIndex].outputs[outputsIndex] = currentOutput;
            outputsIndex++;
        }
    }
    file.close();
}

void BlockInfo(int height, block *firstblock){
    //输入区块高度，输出该区块内容
    block *p = firstblock;
    while (p->height != height && p != nullptr){
        p = p->next;
    }//找到height值对应的block
    if (p == nullptr){
        cout << "Block Not Found!" << endl;
        return;
    }
    else{
        cout << "Block Height: " << p->height << endl;
        cout << "Block Hash: " << p->hash << endl;
        cout << "Block prevHash: " << p->prevHash << endl;
        cout << "Block merkleRoot: " << p->merkleRoot << endl;
        cout << "Block nonce: " << p->nonce << endl;
        int i = 0;
        cout << endl;
        while (p->transactions[i].txid != ""){
            cout << "Block Transaction " << i << " txid: " << (p->transactions[i]).txid << endl;
            cout << "Block Transaction " << i << " input count: " << (p->transactions[i]).input_count << endl;
            cout << "Block Transaction " << i << " output count: " << (p->transactions[i]).output_count << endl;
            cout << "Block Transaction " << i << " Coinbase: " << (p->transactions[i]).is_coinbase << endl;
            TransactionInfo(p->transactions[i].txid, firstblock, nullptr);
            cout << endl;
            i++;
        }
        cout << endl;
    }
}

int BlocksLength(block *firstblock){
    block *p = firstblock;
    int len = 0;
    while (p != nullptr){
        len++;
        p = p->next;
    }
    return len;
}

int TransactionInfo(string My_txid, block *firstblock, block *endblock){
    block *p = firstblock;
    int find = 0;//找到交易信息则find = 1, 否则find = 0
    int i = 0;
    int j = 0;
    int k = 0;
    while (p != endblock){
        while (p->transactions[i].txid != ""){
            if (p->transactions[i].txid == My_txid){
                cout << "Transaction "<< i <<" in Block: " << p->height << endl;
                cout << "Transaction "<< i <<" input count: " << (p->transactions[i]).input_count << endl;
                cout << "Transaction "<< i <<" output count: " << (p->transactions[i]).output_count << endl;
                cout << "Transaction "<< i <<" Coinbase: " << (p->transactions[i]).is_coinbase << endl;
                while (p->transactions[i].inputs[j].scriptSig != ""){
                    cout << "Transaction "<< i <<" input " << j << " pre_block: " << (p->transactions[i]).inputs[j].pre_block << endl;
                    cout << "Transaction "<< i <<" input " << j << " prevTxID: " << (p->transactions[i]).inputs[j].prevTxID << endl;
                    cout << "Transaction "<< i <<" input " << j << " prevTxOutIndex: " << (p->transactions[i]).inputs[j].prevTxOutIndex << endl;
                    cout << "Transaction "<< i <<" input " << j << " scriptSig: " << (p->transactions[i]).inputs[j].scriptSig << endl;
                    j++;
                }
                while (p->transactions[i].outputs[k].script != ""){
                    cout << "Transaction "<< i << " output " << k << " txid: " << (p->transactions[i]).outputs[k].txid << endl;
                    cout << "Transaction "<< i << " output " << k << " index: " << (p->transactions[i]).outputs[k].index << endl;
                    cout << "Transaction "<< i << " output " << k << " value: " << (p->transactions[i]).outputs[k].value << endl;
                    cout << "Transaction "<< i << " output " << k << " script: " << (p->transactions[i]).outputs[k].script << endl;
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
    if (find == 1) return find;
    else{
        cout << "Transaction Not Found!" << endl;
        return find;
    }
}

block* InitBlockChain(){
    block* firstBlock = FileToBlock();
    FileToTransaction(firstBlock);
    FileToInput(firstBlock);
    FileToOutput(firstBlock);
    return firstBlock;
}

void CheckValidTransaction(block *firstblock){
    block *p = firstblock;
    int i = 0;

    int valid = 0;
    int invalid = 0;
    //规则1:每个input所使用的output能够找到。
    //规则2:每个input所使用的output没有被之前的交易用过。   
    //规则3:该交易所有input所引用的output的价值（value）之和大于等于该交易所有output的价值（value）之和。

    //a、有一类特殊交易，其is_coinbase字段为true，该类交易的特点是没有input，只有output。该类交易是合法的，其中的output可能被后续的transaction所引用。
    //b、每一个output只能被使用一次，即便还有剩余的value没有被使用。
    //c、如果某个交易是非法的，那么引用了该交易作为input的交易也同样是非法的（非法交易不会被包括在区块内，如果使用了则相当于违反了规则a）。
    while (p != nullptr){
        while (p->transactions[i].txid != ""){
            //规则1：其is_coinbase字段为true，该类交易的特点是没有input，只有output。该类交易是合法的，其中的output可能被后续的transaction所引用。
            if (p->transactions[i].is_coinbase == 1){
                valid++;
                i++;
                continue;
            }
            //规则2:如果每个input所使用的output找不到，交易不合法；
            //规则3:每个input所使用的output被之前的交易用过，交易不合法
            //规则4:如果引用的output来自不合法的交易，交易不合法
            //规则5:所引用的output的value之和<该所有output的value之和，交易不合法
            else if (IOCheck(firstblock, p, p->transactions[i]) == false){
                p->transactions[i].valid = no;
                invalid++;
                i++;
                continue;
            }
            else{//
                valid++;
                i++;
                continue;
            }
        }
        i = 0;
        p = p->next;
    }

    cout << "不合法交易数: " << invalid << endl;
    cout << "合法交易数: " << valid << endl;
}

bool IOCheck(block *firstblock, block *currentblock, transaction t){
    int i = 0;
    int IOCheck_flag = 0;
    while (t.inputs[i].scriptSig != ""){
        IOCheck_flag = 0;
        transaction *PrevOutput = FindPrevOutput(t.inputs[i].prevTxID, firstblock, currentblock);
        if (PrevOutput != nullptr){
            if (PrevOutput->outputs[t.inputs[i].prevTxOutIndex].IsUse == NotUsed &&
                PrevOutput->valid == yes){
                int sum_in = 0;
                int sum_out = 0;
                for (int j = 0 ; j < t.input_count ; j++) sum_in += PrevOutput->outputs[t.inputs[j].prevTxOutIndex].value;
                for (int j = 0 ; j < t.output_count ; j++) sum_out += t.outputs[j].value;
                if (sum_in >= sum_out){
                    PrevOutput->outputs[t.inputs[i].prevTxOutIndex].IsUse = Used;
                    IOCheck_flag = 1;
                }
                else IOCheck_flag = 0;
            }
        }
        i++;
    }
    if (IOCheck_flag == 1) return true;
    else return false;
}

transaction* FindPrevOutput(string My_txid, block *firstblock, block *endblock){
    //根据PrevTxid找到当前input所用的output的Txid，再根据当前prevTxOutIndex找到所用的output
    block *p = firstblock;
    int i = 0;
    while (p != endblock){
        while (p->transactions[i].txid != ""){
            if (p->transactions[i].txid == My_txid) return &p->transactions[i];      
            else i++;
        }
        i = 0;
        p = p->next;
    }
    return nullptr;
}

/*int main(){
    clock_t startTime = clock();//计时开始
    block* firstBlock = InitBlockChain();
    clock_t endTime = clock();//计时结束
    cout << "Time spent reading data: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    
    cout << "Block count: " << BlocksLength(firstBlock) << endl;
    CheckValidTransaction(firstBlock);
    cout << endl;
    int height;
    cout << "Please input block's height: " << endl;
    cin >> height;
    cout << endl;
    BlockInfo(height, firstBlock);

    string txid;
    cout << "Please input transaction's txid: " << endl;
    cin >> txid;
    cout << endl;
    TransactionInfo(txid, firstBlock, nullptr);

    return 0;
}*/
#endif