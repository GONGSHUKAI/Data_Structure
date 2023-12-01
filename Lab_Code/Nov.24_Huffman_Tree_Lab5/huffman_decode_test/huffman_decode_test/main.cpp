#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <sstream>
#define error -1
#define MAXSIZE 200
#define MaxValue 9999
using namespace std;
unordered_map <char, string> codeTable;
int idx = 0;
double WPL_value = 0;

typedef struct HuffmanNode{
    char data;
    double weight;
    int parent, lchild, rchild;
}HuffmanNode;

typedef struct HuffmanTree{
    HuffmanNode elem[MAXSIZE];//Array that keeps Huffman Nodes
    int currentNumber;//Curren node numbers
}HuffmanTree;

typedef struct HNode {
    char character;
    HNode* left;
    HNode* right;
    HNode(char c) : character(c), left(nullptr), right(nullptr) {}
}HNode;

string readText(string fileName){//Tree-encoding mode (-e)
    string filePath = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Nov.24_Lab/"+ fileName + ".txt";
    ifstream inputFile(filePath);
    string line;
    getline(inputFile, line);
    return line;
}

unordered_map <char, int> getWeight(string text){
    unordered_map <char, int> weight;
    for (char c : text){
        if (weight.count(c) > 0){//If c already exists in unordered map
            weight[c]++;//weight++
        }
        else weight[c] = 1;//Add c to unordered map
    }
    return weight;
}

void HuffmanCodeTable(HuffmanTree HT, int curNode, const string &curCode){
    //Instruction: The initial call of the function should be as follows
    //HuffmanCodeTable(HT, root, "");
    //The DFS search process starts from the root of the static linked list HT
    if (curNode == -1) return; //null node
    if (HT.elem[curNode].lchild == -1 && HT.elem[curNode].rchild == -1){//Reach to the leaf nodes
        char c = HT.elem[curNode].data;//Record leaf node character
        codeTable[c] = curCode;//Store the character into codeTable
    }
    HuffmanCodeTable(HT, HT.elem[curNode].lchild, curCode + "0");
    //Recurse to the lchild, append '0' to curCode
    HuffmanCodeTable(HT, HT.elem[curNode].rchild, curCode + "1");
    //Recurse to the rchild, append '1' to curCode
}

HuffmanTree CreateHuffmanTree (unordered_map <char, int> weight, int n) {
    HuffmanTree HT;
    for (int i = 0 ; i < MAXSIZE ; i++){
        HT.elem[i].data = '\0';
        HT.elem[i].parent = -2;
        HT.elem[i].lchild = HT.elem[i].rchild = -2;
    }//Initialize elem[MAXSIZE]
    int p1, p2, min1, min2;
    int idx = 0;
    for (int i = 0; i < 2 * n-1; i++){ //初始化静态链表，n个结点的静态链表长度至多为2n-1
        HT.elem[i].parent = -1;
        HT.elem[i].lchild = HT.elem[i].rchild = -1;
    }
    for (const auto &bit : weight){
        HT.elem[idx].data = bit.first;
        HT.elem[idx].weight = bit.second;
        idx++;
    }
    for (int i = n; i < 2 * n - 1; i++) { //循环创建新的结点
        min1 = min2 = MaxValue;
        for (int j = 0; j < i; j++) {//在已有结点中找权重最小和次小的结点
            if (HT.elem[j].parent == -1)//树根结点
                if (HT.elem[j].weight < min1){
                    p2 = p1;
                    min2 = min1;
                    p1 = j;
                    min1 = HT.elem[j].weight;
                }
                else if (HT.elem[j].weight < min2){
                    p2 = j;
                    min2 = HT.elem[j].weight;
                }
                else{}
        }//找权重最小和次小的结点结束
        HT.elem[i].lchild = p1;
        HT.elem[i].rchild = p2;
        HT.elem[i].weight = HT.elem[p1].weight + HT.elem[p2].weight;
        HT.elem[p1].parent = HT.elem[p2].parent = i;
    } //创建新结点结束
    HT.currentNumber = 2 * n - 1;
    return HT;
}

void WPL(HuffmanTree HT, int curNode, int depth){
    if (curNode == -1) return; //null node
    if (HT.elem[curNode].lchild == -1 && HT.elem[curNode].rchild == -1){//Reach to the leaf nodes
        WPL_value += HT.elem[curNode].weight * depth;
    }
    WPL(HT, HT.elem[curNode].lchild, depth+1);
    //Recurse to the lchild, depth++
    WPL(HT, HT.elem[curNode].rchild, depth+1);
    //Recurse to the rchild, depth++
}

void printHuffmanTree(HuffmanTree HT){
    for (int i = 0 ; i < HT.currentNumber ; i++){
        cout << HT.elem[i].data << " " << HT.elem[i].parent << " " << HT.elem[i].lchild << " " << HT.elem[i].rchild << endl;
    }
}

void printCodeTable(){
    for (const auto &pair : codeTable){
        cout << pair.first << ":" << pair.second << endl;
    }
}

void codeTableToFile(string code_file){//Tree-encoding mode (-e)
    string fileName = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Nov.24_Lab/"+ code_file + ".txt";
    ofstream outputfile;
    outputfile.open(fileName);
    if (outputfile.is_open()){
        outputfile << "char" << "\t" << "code" << endl;
        for (const auto &pair : codeTable){
            outputfile << pair.first << "\t" << pair.second << endl;
        }
    }
    else{
        cout << "Unable to open file";
    }
}

void HuffmanTreeToFile(HuffmanTree HT, string model_file){//Tree-building mode (-c)
    string MyfileName = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Nov.24_Lab/" + model_file + ".txt";
    ofstream outputfile;
    outputfile.open(MyfileName);
    if (outputfile.is_open()){
        outputfile << "char" << "\t" << "parent" << "\t" << "lchild" << "\t" << "rchild" << endl;
        for (int i = 0 ; i < HT.currentNumber ; i++){
            outputfile << HT.elem[i].data << "\t" << HT.elem[i].parent << "\t" << HT.elem[i].lchild << "\t" << HT.elem[i].rchild << endl;
        }
    }
    else{
        cout << "Unable to open file";
    }
}

unordered_map <char, int> fileToWeight(string input_file){//Tree-building mode (-c)
    string filePath = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Nov.24_Lab/" + input_file + ".txt";
    ifstream file(filePath);
    string line;
    unordered_map <char, int> fileWeight;
    while (getline(file, line)){
        stringstream ss(line);//
        string cell;
        int column = 0;
        char temp;
        while (getline(ss, cell, ':')){
            switch (column) {
                case 0:
                    temp = cell[0];
                    fileWeight[temp] = 0;
                    break;
                case 1:
                    fileWeight[temp] = stoi(cell);
                    break;
                default://当没有匹配到任何 case 标签时执行的代码块。
                    break;
            }
            column++;
        }
    }
    file.close();
    return fileWeight;
}

void Encode(string text, string output_file){//Tree-encoding mode (-e)
    string encodeResult = "";
    for (const char c : text){
        encodeResult = encodeResult + codeTable[c];
    }
    string fileName = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Nov.24_Lab/" + output_file + ".txt";
    ofstream outputfile;
    outputfile.open(fileName);
    if (outputfile.is_open()){
        outputfile << encodeResult << endl;
    }
    else{
        cout << "Unable to open file";
    }
}

HNode* DecodeTree(unordered_map<char, string>& dict) {//Convert codeTable to a Decoding Tree
    HNode* root = new HNode('\0');//先创建一棵空Huffman树
    for (const auto &pair : dict) {//遍历哈希表dict中的每一个键值对
        char character = pair.first;//提取出被编码字母
        string code = pair.second;//提取出Huffman编码

        HNode* currentNode = root;
        for (int i = 0 ; i < code.size() ; i++){//遍历Huffman编码
            if (code[i] == '0') {
                if (!currentNode->left) {//如果左子树为空，那么创建左子树
                    currentNode->left = new HNode('\0');
                }
                currentNode = currentNode->left;//从左子树开始遍历
            }
            else if (code[i] == '1') {//如果右子树为空，那么创建右子树
                if (!currentNode->right) {
                    currentNode->right = new HNode('\0');
                }
                currentNode = currentNode->right;//从右子树开始遍历
            }
        }
        currentNode->character = character;
    }
    return root;
}

string Decode(HNode* root, string encoded_Line) {
    string decoded_Line;
    HNode* p = root;
    for (char bit : encoded_Line) {
        if (bit == '0'){
            p = p->left;
        }
        else if (bit == '1'){
            p = p->right;
        }
        if (p->left == nullptr && p->right == nullptr) {
            decoded_Line += p->character;
            p = root;
        }
    }
    return decoded_Line;
}

void outputDecode(string input_file, string code_file, string output_file){
    string filePath = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Nov.24_Lab/";
    
    string encodeResultPath = filePath + input_file + ".txt";
    string codeTablePath = filePath + code_file + ".txt";
    //read codeTable first
    ifstream file(codeTablePath);
    string line;
    getline(file, line);//Omit the column title

    unordered_map <char, string> myCodeTable;
    while (getline(file, line)){
        stringstream ss(line);
        string cell;
        int column = 0;
        char temp;
        while (getline(ss, cell, '\t')){
            switch (column) {
                case 0:
                    temp = cell[0];
                    myCodeTable[temp] = "";
                    break;
                case 1:
                    myCodeTable[temp] = cell;
                    break;
                default:
                    break;
            }
            column++;
        }
    }
    file.close();

    ifstream file2(encodeResultPath);
    string encoded_Line;
    getline(file2, encoded_Line);//Get Encoded Result

    HNode *root = DecodeTree(myCodeTable);
    string decode_Line = Decode(root, encoded_Line);
    string MyfileName = "/Users/gongshukai/Desktop/SCHOOL WORK/SOPHOMORE SEM1/DATA STRUCTURE  & ALGORITHM /SLIDES & HOMEWORK & LAB/LAB/Nov.24_Lab/" + output_file + ".txt";
    ofstream outputfile;
    outputfile.open(MyfileName);
    if (outputfile.is_open()){
        outputfile << decode_Line << endl;
    }
    else{
        cout << "Unable to open file";
    }
}

int main(int argc, char *argv[]) {
    
    string mode, input_file, model_file, code_file, output_file;
    /*if (argc < 4) return error;  // 命令行参数个数不够，退出
    mode = argv[1];
    input_file = argv[2];
    if (mode == "-c"){
        model_file = argv[3];
    }
    if (mode == "-e" || mode == "-d"){
        if (argc < 5) return error;  // 命令行参数个数不够，退出
        code_file = argv[3];
        output_file = argv[4];
    }

    if (strcmp(argv[1],"-c") == 0){ //执行建树任务
        unordered_map <char, int> MyWeight = fileToWeight("MyWeight");
        HuffmanTree HT = CreateHuffmanTree(MyWeight, MyWeight.size());
        HuffmanTreeToFile(HT,"modelFile");
        printCodeTable();
        cout << "WPL = "<< WPL_value << endl;
    }
    else if (strcmp(argv[1],"-e") == 0){ //执行编码任务
        string text = readText(input_file);
        unordered_map <char, int> Weight = getWeight(text);
        HuffmanTree HT = CreateHuffmanTree(Weight, Weight.size());
        codeTableToFile(code_file);
        cout << "Successfully Export Code Table!" << endl;
        printCodeTable();
        Encode(output_file);
        cout << "Successfully Encode the text!" << endl;
    }
    else if (strcmp(argv[1],"-d") == 0) { //执行解码任务
        outputDecode(input_file, code_file, output_file);
        cout << "Successfully Decode the text!" << endl;
    }
    else{
        cout << "Undefined Command!" << endl;
        return error;
    }
    return 0;
    */
    input_file = "Text";
    code_file = "codeTable";
    output_file = "Encode";
    string text = readText(input_file);
    unordered_map <char, int> Weight = getWeight(text);
    HuffmanTree HT = CreateHuffmanTree(Weight, Weight.size());
    HuffmanCodeTable(HT, HT.currentNumber - 1, "");
    codeTableToFile(code_file);
    cout << "Successfully Export Code Table!" << endl;
    printCodeTable();
    Encode(text, output_file);
    cout << "Successfully Encode the text!" << endl;
}
