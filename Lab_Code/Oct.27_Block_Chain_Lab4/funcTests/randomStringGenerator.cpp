#include <iostream>
#include <random>
#include <string>
using namespace std;

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

int main(){
    cout << randomStringGenerator() << endl;
}