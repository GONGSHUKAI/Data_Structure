#include<iostream>
#include"rsa.h"
using std::cout;
using std::endl;
using std::cin;


//ʾ������ std::hash�÷�������ǩ����ǩ����֤��rsaͷ�ļ����Ѱ�����Ӧ�Ŀ�

int main() {
    //std::hashʹ�÷���
    std::hash<std::string> h;
    string str0 = "2023";
    size_t n0 = h(str0);
    string str_h = std::to_string(n0);
    cout << "HASH of " << str0 << " is: " << str_h << endl << endl;

    //������Կ������ ����˽Կ ��������ǩ��
    Rsa rsaSign; //rsa��������ǩ��
    BigInt c, m;
    int publicKey = 0; //��Կ
    BigInt privateKey; //˽Կ
    BigInt signature;  //ǩ��

    rsaSign.init(0); //��ʼ��
    //���빫Կ
    cout << "Please input public key: " << endl;
    cin >> publicKey;

    rsaSign.setPu(publicKey);      //���ù�Կ��ͬʱ����˽Կ
    privateKey = rsaSign.showPr(); //��ȡ˽Կ
    cout << "generated private key: " << privateKey << endl << endl;

    cout << "use private key to verify" << endl;

    // ����ǩ������ sign��rsa��signature��message������������Ϊ��ǩ�����ַ��������ģ�
    // ʹ��rsa���˽Կ����ǩ��
    // ��ʵ��ֱ��ʹ��publicKey�������� ʵ��Ӧ�ǽ�����Ϣ
    sign(rsaSign, signature, std::to_string(publicKey));
    cout << "public key: " << std::to_string(publicKey) << endl
        << "signature:" << signature << endl << endl;


    cout << "verify public key with correspondent signature" << endl;

    // ǩ����֤CheckSig��Ҫ����
    Rsa rsaVerify; //rsa����������֤ǩ��

    rsaVerify.init(0); //��ʼ��
    cout << "known public key: "<< publicKey << "   signature: " << signature << endl;

    rsaVerify.setPu(publicKey);

    // ����ǩ����֤ʹ��ֱ��ʹ�ù�Կ�������Ľ�����֤
    bool flag = verify(rsaVerify, signature, std::to_string(publicKey));
    cout << "result of verification:" << flag << endl;

    return 0;
}

