#include "config.h"

block* recoverBlock(string fileName) {
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

int main() {
    string fileName = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Oct.27_Lab/funcTests/testFiles/unit_test.txt";
    block *recoveredBlock = recoverBlock(fileName);

    // Print the recovered block information
    cout << "Recovered Block Information:" << endl;
    cout << "Height: " << recoveredBlock->height << endl;
    cout << "Hash: " << recoveredBlock->hash << endl;
    cout << "Prev Hash: " << recoveredBlock->prevHash << endl;
    cout << "Merkle Root: " << recoveredBlock->merkleRoot << endl;
    cout << "Nonce: " << recoveredBlock->nonce << endl;

    // Print transaction information
    for (unsigned long long i = 0; i < 2 && recoveredBlock->transactions[i].txid!=""; i++) {
        transaction& tx = recoveredBlock->transactions[i];
        cout << "Transaction " << i << " Information:" << endl;
        cout << "TxID: " << tx.txid << endl;
        cout << "Input Count: " << tx.input_count << endl;
        cout << "Output Count: " << tx.output_count << endl;
        cout << "Is Coinbase: " << tx.is_coinbase << endl;

        // Print input information
        for (unsigned long long j = 0; j < tx.input_count; j++) {
            input& in = tx.inputs[j];
            cout << "Input " << j << " Information:" << endl;
            cout << "Pre Block: " << in.pre_block << endl;
            cout << "PrevTxID: " << in.prevTxID << endl;
            cout << "PrevTxOutIndex: " << in.prevTxOutIndex << endl;
            cout << "ScriptSig: " << in.scriptSig << endl;
        }

        // Print output information
        for (unsigned long long k = 0; k < tx.output_count; k++) {
            output& out = tx.outputs[k];
            cout << "Output " << k << " Information:" << endl;
            cout << "TxID: " << out.txid << endl;
            cout << "Index: " << out.index << endl;
            cout << "Value: " << out.value << endl;
            cout << "Script: " << out.script << endl;
        }
    }

    return 0;
}
