//
// Created by Administrator on 2022/4/24.
//
#include"../bigint/BigInt.h"

#ifndef ANTISDK_RSA_H
#define ANTISDK_RSA_H




class RSA {
public:

    static BigInt qck(BigInt a, BigInt b, const BigInt& m);

    static bool isPrime(const BigInt& a);

    static bool MillerRabin(const BigInt&, const BigInt&,
                            int, const BigInt&, const BigInt&);

    static int getPrime(size_t);

    static BigInt randomPrime(unsigned bitSize = 512);
    static BigInt randomPrime(const BigInt&, unsigned bitSize = 512);


    static void rsa(BigInt&, BigInt&, BigInt&, BigInt&, BigInt&, unsigned bitSize = 768);

    //产生一对公钥密钥十分耗时，1024位要几十分钟
    //初始化 产生公钥私钥，参数一 = N， 参数二 = 公钥，参数三 = 私钥， 参数四 = 加密长度
    static void init(BigInt&, BigInt&, BigInt&, unsigned bitSize = 768);

    static int exgcd(int a, int b, int& x, int& y);

    static BigInt encrypt(const BigInt& m, const BigInt& d, const BigInt& N); //公钥加密很快 适合放在客户端
    static BigInt decrypt(const BigInt& c, const BigInt& e, const BigInt& N); //私钥解密很慢 适合放在服务端

    static std::vector<BigInt> encrypt(const std::string& str, const BigInt& d, const BigInt& N); //参数一 = 加密的明文， 参数二 = 公钥， 参数三 = N
    static std::string decrypt(const std::vector<BigInt>& h, const BigInt& e, const BigInt& N); //参数一 = 加密的密文， 参数二 = 私钥， 参数三 = N

private:
    static std::vector<int> vPrimes;
    static std::vector<int> vMillerRabinCheckList;
};

#endif //ANTISDK_RSA_H
