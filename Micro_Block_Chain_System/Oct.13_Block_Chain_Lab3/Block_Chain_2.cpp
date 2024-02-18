#include <iostream>
#include <stack>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cmath>
#include "rsa.h"
#include "Infix_Calculation.h"
#define yes 100
#define no -100
#define NotUsed 200
#define Used -200
#define TransactionExist 999

#define MAXTRANS 100//一个块内最高有100条交易信息
#define MAXINPUT 100//一条交易信息中最高有100个输入
#define MAXOUTPUT 100//一条交易信息中最高有100个输出
#define MAXSIZE 200

typedef struct output{
    std::string txid;//该output所属的交易
    unsigned long long index;//该output在所属交易中的索引值
    unsigned long long value;//该output的价值(数据已乘10^8,避免浮点误差)
    std::string script;//脚本

    int IsUse = NotUsed;
}output;

typedef struct input{
    unsigned long long pre_block;//该input所引用的output所在区块的高度
    std::string prevTxID;//该input所引用的output所在交易的txID
    unsigned long long prevTxOutIndex;//该input所引用的output位于所在交易output集合中的索引
    std::string scriptSig;//脚本和签名
}input;

typedef struct transaction{
    std::string txid;//交易的编号，具有唯一性
    unsigned long long input_count;//inputs的数量
    unsigned long long output_count;//outputs的数量
    input inputs[MAXINPUT];//一组input的集合，表示当前交易的输入所用到的输出
    output outputs[MAXOUTPUT];//一组output的集合，表示当前交易的输出
    int is_coinbase;//1为coinbase交易，0为非coinbase交易

    int valid = yes;
}transaction;

typedef struct block{
    unsigned long long height;//当前块的高度，一条链上每个区块的Height均不相同
    std::string hash;//本区块的哈希值
    std::string prevHash;//前一个区块的哈希值
    std::string merkleRoot;//本区块中所有交易的默克尔树根
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
int TransactionInfo(std::string txid, block *firstblock, block *endblock);
block* InitBlockChain();
void CheckValidTransaction(block *firstblock);
bool IOCheck(block *firstblock, block *currentblock, transaction t);
transaction* FindPrevOutput(std::string My_txid, block *firstblock, block *endblock);
bool CheckIOScript(std::string scriptSig, std::string script);

std::stack <std::string> SmartContract;
std::vector <std::string> keywords;
int flag = 1;

block* FileToBlock() {
    block *firstBlock = new block;
    std::ifstream file("demo/blocks.csv"); //打开CSV文件
    if (!file) {
        std::cout << "无法打开文件" << std::endl;
        return nullptr;
    }
    std::string line;
    std::getline(file, line); // 读取第一行标题（忽略）
    /*std::getline 函数返回一个布尔值，表示是否成功读取一行内容。
    如果读取成功，则返回true；
    如果已到达文件末尾或发生错误，则返回false。*/
    block *currentBlock = nullptr;
    block *prevBlock = nullptr;
    while (std::getline(file, line)) {
        std::stringstream ss(line);//创建一个std::std::stringstream 对象，字符串ss作为初始输入。
        std::string cell;
        int column = 0;
        block *newBlock = new block();// 创建新的 block

        while (std::getline(ss, cell, ',')){
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
    std::ifstream file("demo/transactions.csv");
    if (!file) {
        std::cout << "无法打开文件" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    int transIndex = 0;

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int column = 0;
        int transHeight = 0;
        transaction currentTrans;
        currentTrans.valid = yes;
        while (std::getline(ss, cell, ',')) {//扫描一行的数据，以逗号为分隔符
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
    std::ifstream file("demo/inputs.csv");
    if (!file) {
        std::cout << "无法打开文件" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    int transIndex = 0;
    int inputsIndex = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int column = 0;
        int inputHeight = 0;
        std::string input_txid;
        input currentInput;
        while (std::getline(ss, cell, ',')) {//扫描一行的数据，以逗号为分隔符
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
    std::ifstream file("demo/outputs.csv");
    if (!file) {
        std::cout << "无法打开文件" << std::endl;
        return;
    }
    std::string line;
    std::getline(file, line);
    int transIndex = 0;
    int outputsIndex = 0;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string cell;
        int column = 0;
        int outputHeight = 0;
        std::string output_txid;
        output currentOutput;
        currentOutput.IsUse = NotUsed;
        while (std::getline(ss, cell, ',')) {//扫描一行的数据，以逗号为分隔符
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
    while (height != p->height && p != nullptr){
        p = p->next;
    }//找到height值对应的block
    std::cout << "Block Height: " << p->height << std::endl;
    std::cout << "Block Hash: " << p->hash << std::endl;
    std::cout << "Block prevHash: " << p->prevHash << std::endl;
    std::cout << "Block merkleRoot: " << p->merkleRoot << std::endl;
    std::cout << "Block nonce: " << p->nonce << std::endl;
    int i = 0;
    std::cout << std::endl;
    while (p->transactions[i].txid != ""){
        std::cout << "Block Transaction " << i << " txid: " << (p->transactions[i]).txid << std::endl;
        std::cout << "Block Transaction " << i << " input count: " << (p->transactions[i]).input_count << std::endl;
        std::cout << "Block Transaction " << i << " output count: " << (p->transactions[i]).output_count << std::endl;
        std::cout << "Block Transaction " << i << " Coinbase: " << (p->transactions[i]).is_coinbase << std::endl;
        TransactionInfo(p->transactions[i].txid, firstblock, nullptr);
        std::cout << std::endl;
        i++;
    }
    std::cout << std::endl;
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

int TransactionInfo(std::string My_txid, block *firstblock, block *endblock){
    block *p = firstblock;
    int find = 0;//找到交易信息则find = 1, 否则find = 0
    int i = 0;
    int j = 0;
    int k = 0;
    while (p != endblock){
        while (p->transactions[i].txid != ""){
            if (p->transactions[i].txid == My_txid){
                std::cout << "Transaction "<< i <<" in Block: " << p->height << std::endl;
                std::cout << "Transaction "<< i <<" input count: " << (p->transactions[i]).input_count << std::endl;
                std::cout << "Transaction "<< i <<" output count: " << (p->transactions[i]).output_count << std::endl;
                std::cout << "Transaction "<< i <<" Coinbase: " << (p->transactions[i]).is_coinbase << std::endl;
                while (p->transactions[i].inputs[j].scriptSig != ""){
                    std::cout << "Transaction "<< i <<" input " << j << " pre_block: " << (p->transactions[i]).inputs[j].pre_block << std::endl;
                    std::cout << "Transaction "<< i <<" input " << j << " prevTxID: " << (p->transactions[i]).inputs[j].prevTxID << std::endl;
                    std::cout << "Transaction "<< i <<" input " << j << " prevTxOutIndex: " << (p->transactions[i]).inputs[j].prevTxOutIndex << std::endl;
                    std::cout << "Transaction "<< i <<" input " << j << " scriptSig: " << (p->transactions[i]).inputs[j].scriptSig << std::endl;
                    j++;
                }
                while (p->transactions[i].outputs[k].script != ""){
                    std::cout << "Transaction "<< i << " output " << k << " txid: " << (p->transactions[i]).outputs[k].txid << std::endl;
                    std::cout << "Transaction "<< i << " output " << k << " index: " << (p->transactions[i]).outputs[k].index << std::endl;
                    std::cout << "Transaction "<< i << " output " << k << " value: " << (p->transactions[i]).outputs[k].value << std::endl;
                    std::cout << "Transaction "<< i << " output " << k << " script: " << (p->transactions[i]).outputs[k].script << std::endl;
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
        std::cout << "Transaction Not Found!" << std::endl;
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

    std::cout << "不合法交易数: " << invalid << std::endl;
    std::cout << "合法交易数: " << valid << std::endl;
}

bool IOCheck(block *firstblock, block *currentblock, transaction t){
    int i = 0;
    while (t.inputs[i].scriptSig != ""){
        transaction *PrevOutput = FindPrevOutput(t.inputs[i].prevTxID, firstblock, currentblock);
        if (PrevOutput != nullptr){
            if (PrevOutput->outputs[t.inputs[i].prevTxOutIndex].IsUse == NotUsed &&
                PrevOutput->valid == yes &&
                CheckIOScript(t.inputs[i].scriptSig, PrevOutput->outputs[t.inputs[i].prevTxOutIndex].script) == true){
                int sum_in = 0;
                int sum_out = 0;
                for (int j = 0 ; j < t.input_count ; j++) sum_in += PrevOutput->outputs[t.inputs[j].prevTxOutIndex].value;
                for (int j = 0 ; j < t.output_count ; j++) sum_out += t.outputs[j].value;
                if (sum_in >= sum_out){
                    PrevOutput->outputs[t.inputs[i].prevTxOutIndex].IsUse = Used;
                    return true;
                }
                else return false;
            }
        }
        i++;
    }
    return false;
}

transaction* FindPrevOutput(std::string My_txid, block *firstblock, block *endblock){
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

void OP_HASH160(std::string script){
    //弹出栈顶的数，调用std::hash计算，并将结果压入栈中
    std::string calc = SmartContract.top();//取栈顶元素
    SmartContract.pop();
    std::hash <std::string> h;//调用哈希函数
    SmartContract.push(std::to_string(h(calc)));//计算calc的哈希值，并压入栈
    /*观察到OP_HASH160后面总会再跟一个验证串(例如: OP_HASH160 17730432883776450158)
    把这个串也顺便压进栈内，从而便于后续计算(OP_EQUALVERIFY)*/
    //先找到OP_HASH160后面空格的位置
    std::string substr = "OP_HASH160";
    size_t pos = script.find(substr);
    size_t startPos = pos + substr.length();//OP_HASH160后第一个空格的位置
    std::string substr2 = "OP_EQUALVERIFY";
    size_t endPos = script.find(substr2, startPos);
    std::string VerifyStr = script.substr(startPos + 1, endPos - startPos - 2);
    SmartContract.push(VerifyStr);
}

void OP_BEGIN_CALC(std::string scriptSig, std::string script){
    //先找到OP_BEGIN_CALC后面第一个空格的位置
    std::string substr = "OP_BEGIN_CALC";
    unsigned long long startPos = script.find(substr);
    startPos += substr.length();
    //再找到OP_END_CALC的位置
    unsigned long long endPos = script.find("OP_END_CALC", startPos);
    std::string infix_expression = script.substr(startPos + 1,endPos - startPos - 2);
    //str.substr(a,b) 返回从a开始, 长度为b的子串
    int idx = 0;
    char converted_infix[MAXSIZE];
    //把script的子串infix_expression转换为Infix_Calculation.cpp能读取的形式
    for (int i = 0 ; i < infix_expression.size() ; i++){
        if (infix_expression[i] != ' '){
            converted_infix[idx] = infix_expression[i];
            idx++;
        }
    }
    converted_infix[idx] = '=';
    Mystack *OPND = InitStack();
    char_stack *OPTR = InitCharStack();
    int ans = InfixCalculation(converted_infix, OPND, OPTR);
    SmartContract.push(std::to_string(ans));//把中缀表达式运算结果压入栈中
}

void OP_DUP(){//复制（非弹出）栈顶元素的值，并将复制的结果压入栈中
    std::string sig = SmartContract.top();//先将之前压入的input的scriptSig取出
    std::string signature;
    std::string public_key;
    //将scriptSig分解为signature和public key两部分
    unsigned long long pos = sig.find(' ');
    if (pos != std::string::npos){
    //如果没有返回一个无效的位置，即sig中存在空格(例如: 22308B989987AEE01DED1BAC4C84497F 20221009)
        signature = sig.substr(0,pos);//22308B989987AEE01DED1BAC4C84497F是signature
        public_key = sig.substr(pos + 1, sig.length());//20221009是public key
        SmartContract.push(signature);//将signature压入栈中
        SmartContract.push(public_key);//将public_key压入栈中
        SmartContract.push(public_key);//将public_key再压入栈中(相当于复制)
    }
    else{
    //如果返回一个无效的位置(scriptSig中根本没有空格，例如2023)
        SmartContract.push(sig);
        SmartContract.push(sig);//DUP指令将sig复制了一遍再压进栈
    }
}

void OP_EQUALVERIFY(){//弹出栈顶两个操作数，判断它们是否相等
    std::string top1 = SmartContract.top();
    SmartContract.pop();
    std::string top2 = SmartContract.top();
    SmartContract.pop();
    if (top1 == top2) flag = 1;
    else flag = 0;
}

void OP_CHECKSIG(){
    std::string public_key = SmartContract.top();
    SmartContract.pop();
    std::string signature = SmartContract.top();
    SmartContract.pop();
    BigInt MySignature(signature);

    Rsa rsaSign; //rsa对象，用于签名
    Rsa rsaVerify; //rsa对象，用于验证签名
    rsaVerify.init(0); //初始化
    rsaSign.setPu(std::stoi(public_key));
    flag = verify(rsaVerify, MySignature, public_key);
}

void Execution(std::string keyword, std::string script, std::string scriptSig){
    if (keyword == "OP_HASH160"){
        OP_HASH160(script);
    } 
    else if (keyword == "OP_BEGIN_CALC"){
        OP_BEGIN_CALC(scriptSig, script);
    } 
    else if (keyword == "OP_DUP"){
        OP_DUP();
    } 
    else if (keyword == "OP_EQUALVERIFY"){
        OP_EQUALVERIFY();
    } 
    else if (keyword == "OP_CHECKSIG"){
        OP_CHECKSIG();
    } 
    else{
    }
}

bool CheckIOScript(std::string scriptSig, std::string script){
    keywords.push_back("OP_HASH160");
    keywords.push_back("OP_BEGIN_CALC");
    keywords.push_back("OP_DUP");
    keywords.push_back("OP_EQUALVERIFY");
    keywords.push_back("OP_CHECKSIG");
    SmartContract.push(scriptSig);
    size_t pos = 0;
    flag = 1;
    while (pos < script.length()) {
        std::string keyword;
        size_t spacePos = script.find(' ', pos);
        if (spacePos != std::string::npos) {
            keyword = script.substr(pos, spacePos - pos);
        } 
        else {
            keyword = script.substr(pos);
        }
        Execution(keyword, script, scriptSig);

        if (keyword == "OP_BEGIN_CALC") {
            size_t endCalcPos = script.find("OP_END_CALC", pos);
            if (endCalcPos != std::string::npos) {
                pos = endCalcPos + sizeof("OP_END_CALC") - 1;
            } 
            else {
                std::cout << "OP_BEGIN_CALC found, but OP_END_CALC is missing" << std::endl;
                break;
            }
        } 
        else {
            pos = spacePos != std::string::npos ? spacePos + 1 : script.length();
        }
    }
    if (flag == 1) return true;
    else return false;
}

void CheckIOScript_Test(){
    std::string scriptSig = "2023";
    std::string script = "OP_DUP OP_BEGIN_CALC ( 100 * 10 + 24 ) / ( 2 + 2 ) + 1767 OP_END_CALC OP_EQUALVERIFY OP_HASH160 9252305976066785989 OP_EQUALVERIFY";
    std::cout << CheckIOScript(scriptSig, script) << std::endl;
}



int main(){
    /*clock_t startTime = clock();//计时开始
    block* firstBlock = InitBlockChain();
    clock_t endTime = clock();//计时结束
    std::cout << "Time spent reading data: " <<(double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << std::endl;
    
    std::cout << "Block count: " << BlocksLength(firstBlock) << std::endl;
    CheckValidTransaction(firstBlock);
    std::cout << std::endl;
    int height;
    std::cout << "Please input block's height: " << std::endl;
    std::cin >> height;
    std::cout << std::endl;
    BlockInfo(height, firstBlock);

    std::string txid;
    std::cout << "Please input transaction's txid: " << std::endl;
    std::cin >> txid;
    std::cout << std::endl;
    TransactionInfo(txid, firstBlock, nullptr);

    return 0;*/
    CheckIOScript_Test();
    
}