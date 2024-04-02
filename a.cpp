#include <bits/stdc++.h>
#include <sodium.h>
using namespace std;

int main()
{
    if(sodium_init() < 0) {
        cerr << "Error: sodium Library Initialized Error\n";
        exit(0);
    }
    /**
     * 一、前期处理
     * 1.生成两个密钥对A与B
     * 2.A与B各自保存自己的私钥
     * 3.将PkA和PkB存入数据库中
     * 4.生成加密密钥K（Nonce生成）
     * 
     * 二、加密（假设A给B发送消息）
     * A用自己的私钥，B的公钥和K，以及明文m加密
     * 
     * 三、传输
     * 传输。。。
     * 
     * 四、解密
     * B用自己的私钥、A的公钥和K，以及密文解密
     * 
     * 五、
     * 删除加密密钥K
    */
    return 0;
}