//
// Created by Administrator on 2022/4/26.
//
#include <string>

#ifndef ANTISDK_SIMPLERSA_H
#define ANTISDK_SIMPLERSA_H


using namespace std;

class SimpleRsa
{
public:
    string plaintext = "";
    string ciphertext = "";
    __uint64_t p;			//素数p   __uint64_t => unsigned __int64
    __uint64_t q;			//素数q
    __uint64_t n;			//n=p*q
    __uint64_t b;			//a对于φ(n)的模反元素
    __uint64_t a = 65537;

    SimpleRsa(int nn, int ab) {
        n = nn;
        b = a = ab;
    }

    string Encrypt(string plaintext);

    string Decrypt(string ciphertext);

    //产生私钥和公钥
    void generateKey();

private:
    //三位素数 从这里面取
    int prime[90] = { 401, 409, 419, 421, 431, 433, 439,
                      443, 449, 457, 461, 463, 467, 479, 487, 491, 499,
                      503, 509, 521, 523, 541, 547, 557, 563, 569, 571,
                      577, 587, 593, 599, 601, 607, 613, 617, 619, 631,
                      641, 643, 647, 653, 659, 661, 673, 677, 683, 691,
                      701, 709, 719, 727, 733, 739, 743, 751, 757, 761,
                      769, 773, 787, 797, 809, 811, 821, 823, 827, 829,
                      839, 853, 857, 859, 863, 877, 881, 883, 887, 907,
                      911, 919, 929, 937, 941, 947, 953, 967, 971, 977,
                      983, 991, 997 };

    //模乘运算，返回值 x=a*b mod n
    __uint64_t MulMod(__uint64_t a, __uint64_t b, __uint64_t n);
    //模幂运算，返回值 x=base^pow mod n
    __uint64_t PowMod(__uint64_t &base, __uint64_t &pow, __uint64_t &n);
    //返回d=gcd(a,b);和对应于等式ax+by=d中的x,y
    long long extend_gcd(long long a, long long b, long long &x, long long &y);
    //ax = 1(mod n) 求X
    long long mod_reverse(long long a, long long n);
};

#endif //ANTISDK_SIMPLERSA_H
