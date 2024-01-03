

#include "Rsa.h"
#include <iostream>

using namespace std;

std::vector<int> RSA::vPrimes = vector<int>();
std::vector<int> RSA::vMillerRabinCheckList = { 2, 3, 7, 61, 24251, 24281 };
int INF = 100000;

BigInt RSA::qck(BigInt a, BigInt b, const BigInt& m) {
    BigInt res(1);
    for (; b.compare(Zero) == 1; b.rightSelfShift(1)) {
        if (b.isOdd()) res = (res * a) % m;
        a = (a * a) % m;
        //cout << "res = 0x" << res;
        //cout << "a = 0x" << a;
        //cout << "b = 0x" << b;
    }
    return std::move(res);
}

bool RSA::isPrime(const BigInt& a) {
    { // pre-check
        getPrime(0);
        int len = vPrimes.size();
        if (a.digits() <= 2) {
            int x = a.digits() == 2 ? ((a.getDigit(1) << mBase_bit) | a.getDigit(0)) :
                    ((a.digits() == 1) ? a.getDigit(0) : 0);
            for (int i = 0; i < len && vPrimes[i] * vPrimes[i] <= x; i++) {
                if (x % vPrimes[i] == 0)
                    return false;
            }
            return true;
        }

        int i = 0;
        while (i < len && vPrimes[i] < 10000) {
            if (a % vPrimes[i] == 0)
                return false;
            i++;
        }
    }

    // a = 2 ^ r * b + 1
    BigInt a_1 = a.abs_minus2(1);
    int r = a_1.countSuffixZeros();
    BigInt b = a_1.rightShift(r);

    for (int i: vMillerRabinCheckList) {
        if (!MillerRabin(a, a_1, r, b, i)) {
            return false;
        }
    }
    return true;
}

// N = 2 ^ r * M + 1
bool RSA::MillerRabin(const BigInt& N, const BigInt& N_1,
                      int r, const BigInt& M, const BigInt& A) {
    if (N.compare(One) == 0) return false; // 1 isn't a prime
    if (N.compare(Two) == 0) return true;  // 2 is a prime
    if (!N.isOdd()) return false;		   // even aren't primes

    // d = A ^ M % N
    BigInt d = qck(A, M, N);
    int c_1 = d.abs_compare(One),
            c_N_1 = d.abs_compare(N_1);
    if (c_1 == 0) return true;

    for (int i = 1; i < r && c_1 != 0 && c_N_1 != 0; i++) {
        d = (d * d) % N;

        c_1 = d.abs_compare(One);
        c_N_1 = d.abs_compare(N_1);
    }

    return c_N_1 == 0 || c_1 == 0;
}

int RSA::getPrime(size_t idx) {
    if (vPrimes.empty()) {
        bool* flag = new bool[INF];
        memset(flag, true, sizeof(bool) * INF);
        for (int i = 2; i < INF; i++) {
            if (flag[i]) vPrimes.push_back(i);
            for (int j : vPrimes) {
                if (i * j >= INF) break;
                flag[i * j] = false;
                if (i % j == 0) break;
            }
        }
        delete[] flag;
    }
    if (idx > vPrimes.size()) return 1;
    return vPrimes[idx];
}

BigInt RSA::randomPrime(unsigned bitSize) {
    BigInt x = BigInt::randomOdd(bitSize);
    while (!isPrime(x)) {
        x.abs_selfplus(2u);
    }
    return std::move(x);
}

BigInt RSA::randomPrime(const BigInt& start, unsigned bitSize) {
    BigInt x = start.plus(2);
    while (!isPrime(x)) {
        x.abs_selfplus(2u);
    }
    return std::move(x);
}

void RSA::rsa(BigInt& n, BigInt& e, BigInt& d, BigInt& p, BigInt& q, unsigned bitSize) {
    p = randomPrime((bitSize + 1) >> 1);
    q = randomPrime(p, (bitSize + 1) >> 1);
    n = p * q;
    BigInt phi_n = p.abs_minus2(1u) * q.abs_minus2(1u);

    int prime_cnt = vPrimes.size();
    int e_small = 0;
    int phi_n_mod_e;
    for (int i = prime_cnt - 1; i >= 0; i--) {
        phi_n_mod_e = phi_n % vPrimes[i];
        if (phi_n_mod_e != 0) {
            e_small = vPrimes[i];
            break;
        }
    }

    if (e_small == 0) {
        cout << "e == 0!" << endl;
        return;
    }

    // for debug
    //n = BigInt("63F7", 16);
    //e = BigInt(99991);
    //phi_n = BigInt(25272);
    //phi_n_mod_e = 25272;

    // calculate d
    int x_small, y_small;
    int gcd_e_phi_n = exgcd(e_small, phi_n_mod_e, x_small, y_small);

    if (gcd_e_phi_n != 1) {
        cout << "gcd(e, phi(n)) <> 1!" << endl;
        return;
    }

    BigInt k = phi_n / e_small;
    // y = x_small - k * y_small;
    if (y_small < 0) {
        d = BigInt(x_small) + k.times(-y_small);
    }
    else
    {
        d = BigInt(x_small) - k.times(y_small);
    }

    if (d.compare(Zero) > 0) {
        d = d % phi_n;
    }
    else
    {
        d = phi_n - (d % phi_n);
    }

    e = BigInt(e_small);
}

void RSA::init(BigInt& n, BigInt& e, BigInt& d, unsigned bitSize) {
    BigInt p, q;
    rsa(n, e, d, p, q, bitSize);
}

// calc ax + by = gcd(a, b);
int RSA::exgcd(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    int gcd = exgcd(b, a % b, x, y);
    int k = a / b;
    int tmp_x = x;
    x = y;
    y = tmp_x - k * y;
    return gcd;
}

BigInt RSA::encrypt(const BigInt& m, const BigInt& d, const BigInt& N) {
    return qck(m, d, N);
}

BigInt RSA::decrypt(const BigInt& c, const BigInt& e, const BigInt& N)
{
    return qck(c, e, N);
}

std::vector<BigInt> RSA::encrypt(const std::string& str, const BigInt& d, const BigInt& N)
{
    vector<BigInt> res;
    res.reserve(str.length());
    for (char c : str) {
        BigInt m(static_cast<int>(c));
        res.emplace_back(encrypt(m, d, N));
    }
    return std::move(res);
}

std::string RSA::decrypt(const std::vector<BigInt>& c, const BigInt& e, const BigInt& N) {
    string res;
    for (const BigInt& c_: c) {
        BigInt m = decrypt(c_, e, N);
        if (m.digits() > 1) {
            res = "Failed!";
            return res;
        }
        res.push_back(static_cast<char>(m.getDigit(0)));
    }
    return res;
}
