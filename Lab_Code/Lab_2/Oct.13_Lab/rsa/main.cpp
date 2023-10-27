#include<iostream>
#include"rsa.h"
using std::cout;
using std::endl;
using std::cin;


//示例包括 std::hash用法、数字签名、签名验证，rsa头文件中已包含相应的库

int main() {
    //std::hash使用方法
    std::hash<std::string> h;
    string str0 = "2023";
    size_t n0 = h(str0);
    string str_h = std::to_string(n0);
    cout << "HASH of " << str0 << " is: " << str_h << endl << endl;

    //给定公钥和明文 生成私钥 进行数字签名
    Rsa rsaSign; //rsa对象，用于签名
    BigInt c, m;
    int publicKey = 0; //公钥
    BigInt privateKey; //私钥
    BigInt signature;  //签名

    rsaSign.init(0); //初始化
    //输入公钥
    cout << "Please input public key: " << endl;
    cin >> publicKey;

    rsaSign.setPu(publicKey);      //设置公钥，同时生成私钥
    privateKey = rsaSign.showPr(); //获取私钥
    cout << "generated private key: " << privateKey << endl << endl;

    cout << "use private key to verify" << endl;

    // 数字签名函数 sign（rsa，signature，message）第三个函数为被签名的字符串（明文）
    // 使用rsa里的私钥进行签名
    // 此实验直接使用publicKey当做明文 实际应是交易信息
    sign(rsaSign, signature, std::to_string(publicKey));
    cout << "public key: " << std::to_string(publicKey) << endl
        << "signature:" << signature << endl << endl;


    cout << "verify public key with correspondent signature" << endl;

    // 签名验证CheckSig主要部分
    Rsa rsaVerify; //rsa对象，用于验证签名

    rsaVerify.init(0); //初始化
    cout << "known public key: "<< publicKey << "   signature: " << signature << endl;

    rsaVerify.setPu(publicKey);

    // 数字签名验证使用直接使用公钥，对明文进行验证
    bool flag = verify(rsaVerify, signature, std::to_string(publicKey));
    cout << "result of verification:" << flag << endl;

    return 0;
}

