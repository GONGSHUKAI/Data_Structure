#include "config.h"

transaction recover_tsc(string fileName){
    transaction tx;//根据ClientMSG(即txt文件名)复原一个transaction tx
    
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
    // Test recover_tsc function
    string ClientMSG = "clientMessage1.txt";
    transaction tx = recover_tsc(ClientMSG);

    // Output transaction information
    cout << "Transaction ID: " << tx.txid << endl;
    cout << "Input count: " << tx.input_count << endl;
    cout << "Output count: " << tx.output_count << endl;
    cout << "Is coinbase: " << tx.is_coinbase << endl;

    // Output inputs
    for (int i = 0; i < tx.input_count; i++) {
        input in = tx.inputs[i];
        cout << "Input " << i << " - Previous block: " << in.pre_block << endl;
        cout << "Input " << i << " - Previous transaction ID: " << in.prevTxID << endl;
        cout << "Input " << i << " - Previous transaction output index: " << in.prevTxOutIndex << endl;
        cout << "Input " << i << " - Script signature: " << in.scriptSig << endl;
    }

    // Output outputs
    for (int i = 0; i < tx.output_count; i++) {
        output out = tx.outputs[i];
        cout << "Output " << i << " - Transaction ID: " << out.txid << endl;
        cout << "Output " << i << " - Index: " << out.index << endl;
        cout << "Output " << i << " - Value: " << out.value << endl;
        cout << "Output " << i << " - Script: " << out.script << endl;
    }

    return 0;
}