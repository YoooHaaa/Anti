//
// Created by Administrator on 2022/4/26.
//



#include "SimpleRsa.h"
#include <string>
#include <ctime>
#include "../../include/include.h"
using namespace std;



string SimpleRsa::Encrypt(string plaintext) {
    __uint64_t num;
    __uint64_t N;
    char cnum[7];
    string s;
    for (int i = 0; i < plaintext.size(); i += 2)
    {
        //把2个char用ASCII表转化在一起构成一个6位数字
        num = plaintext[i] * 1000 + plaintext[i + 1];
        N = PowMod(num, a, n);
        sprintf(cnum, "%d", N);
        s = cnum;
        while (s.size() < 6)		//若不够6位则在前面补0
        {
            s = "0" + s;
        }
        ciphertext += s;		//把6位数字存入密文
    }
    return ciphertext;
}

string SimpleRsa::Decrypt(string ciphertext) {
    __uint64_t num;
    int aa;
    __uint64_t N;
    char cnum[7];
    string s ="";
    for (int i = 0; i < ciphertext.size(); i += 6)
    {
        //把秘文中每6位拿出来做解密，得到一个6位数，前3位为一个char，后3位为一个char
        strcpy(cnum, ciphertext.substr(i, 6).c_str());
        sscanf(cnum, "%d", &aa);
        N = aa;
        num = PowMod(N, b, n);
        plaintext += char(num / 1000);
        plaintext += char(num % 1000);
    }
    return plaintext;
}

//产生私钥和公钥
void SimpleRsa::generateKey() {
    //私钥：(n,a)
    a = 65537;
    srand(time(0));
    p = prime[rand() % 90];
    q = prime[rand() % 90];
    n = p * q;
    b = mod_reverse(a, (p - 1)*(q - 1));
    LOGI("公钥：%d %d", n, a);
    LOGI("私钥：%d %d", n, b);
}

//模乘运算，返回值 x=a*b mod n
inline __uint64_t SimpleRsa::MulMod(__uint64_t a, __uint64_t b, __uint64_t n)
{
    return a * b % n;
}

//模幂运算，返回值 x=base^pow mod n
__uint64_t SimpleRsa::PowMod(__uint64_t &base, __uint64_t &pow, __uint64_t &n)
{
    __uint64_t    a = base, b = pow, c = 1;
    while (b)
    {
        while (!(b & 1))
        {
            b >>= 1;            //a=a * a % n;    //函数看起来可以处理64位的整数，但由于这里a*a在a>=2^32时已经造成了溢出，因此实际处理范围没有64位
            a = MulMod(a, a, n);
        }        b--;        //c=a * c % n;        //这里也会溢出，若把64位整数拆为两个32位整数不知是否可以解决这个问题。
        c = MulMod(a, c, n);
    }    return c;
}

//返回d=gcd(a,b);和对应于等式ax+by=d中的x,y
long long SimpleRsa::extend_gcd(long long a, long long b, long long &x, long long &y) {
    if (a == 0 && b == 0) return -1;//无最大公约数
    if(b==0) {
        x=1;y=0;return a;
    }
    long long d=extend_gcd(b,a%b,y,x);
    y-=a/b*x;
    return d;
}

//ax = 1(mod n) 求X
long long SimpleRsa::mod_reverse(long long a, long long n) {
    long long x, y;
    long long d = extend_gcd(a, n, x, y);
    if (d == 1)
        return (x%n + n) % n;
    else return -1;
}
