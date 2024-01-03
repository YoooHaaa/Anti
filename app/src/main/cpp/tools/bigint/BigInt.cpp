//
// Created by Administrator on 2022/4/24.
//

#include "BigInt.h"
#include <random>
#include <unordered_map>

using namespace std;

BigInt::BigInt(int x) {
    this->fromInt(x);
}

BigInt::BigInt(const std::string& str, int numBase) {
    this->fromString(str, numBase);
}

BigInt::BigInt(const BigInt& a) {
    this->sgn = a.sgn;
    this->mvDigits.assign(a.mvDigits.begin(), a.mvDigits.end());
    //mvDigits.clear();
    //mvDigits.insert(mvDigits.begin(), a.mvDigits.begin(), a.mvDigits.end());
}

void BigInt::fromInt(int x) {
    // x == 0
    if (x == 0) {
        sgn = true;
        mvDigits.clear();
        mvDigits.push_back(0);
        return;
    }

    // x != 0
    if (x > 0) sgn = true;
    else sgn = false, x = -x;

    mvDigits.clear();
    while (x > 0) {
        mvDigits.push_back(x & mBase_mask);
        x >>= mBase_bit;
    }
}

void BigInt::fromString(const string& str, int numBase) {
    if (numBase != 2 && numBase != 16) return;
    if (str.empty()) return;

    // deal with the sign
    if (str[0] == '-') {
        sgn = false;
    }
    else
        sgn = true;
    string mstr;
    if (str[0] == '-' || str[0] == '+') {
        mstr.assign(str.begin() + 1, str.end());
    }
    else
        mstr.assign(str.begin(), str.end());

    mvDigits.clear();
    int len = mstr.length();
    if (numBase == 2) { // binary
        for (int i = len - 1; i >= 0; i -= mBase_bit) {
            int digit = 0;
            for (int j = max(0, i - mBase_bit + 1); j <= i; j++) {
                digit = digit * numBase + (mstr[j] - '0');
            }
            mvDigits.push_back(digit);
        }
    }
    else // hex
    {
        for (int i = len - 1; i >= 0; i -= mBase_bit / 4) {
            int digit = 0;
            for (int j = max(0, i - mBase_bit / 4 + 1); j <= i; j++) {
                if (mstr[j] >= 'A' && mstr[j] <= 'F')
                    digit = digit * numBase + (mstr[j] - 'A') + 10;
                else
                    digit = digit * numBase + (mstr[j] - '0');
            }
            mvDigits.push_back(digit);
        }
    }

    // remove pre-zeros
    while (mvDigits.size() > 1 && mvDigits.back() == 0)
        mvDigits.pop_back();

    // remove -0
    if (mvDigits.size() == 1 && mvDigits[0] == 0)
        sgn = true;
}

BigInt BigInt::random(unsigned bitSize) {
    if (bitSize == 0) return Zero;

    random_device dev;
    mt19937 rng(dev());
    uniform_int_distribution<int> dist(0, 1);

    BigInt res;
    res.setPositive();
    res.mvDigits.resize(bitSize / mBase_bit + 1, 0);
    for (int i = 0; i < bitSize; i += mBase_bit) {
        int digit = 0;
        for (int j = 0; j < mBase_bit && i + j < bitSize; j++) {
            digit |= (dist(rng) << j);
        }
        res.mvDigits[i / mBase_bit] = digit;
    }
    res.mvDigits.back() |= (1 << ((bitSize - 1) % mBase_bit)); // make sure the greatest bit is 1
    return std::move(res);
}

BigInt BigInt::randomOdd(unsigned bitSize) {
    if (bitSize == 0) return Zero;
    BigInt a = BigInt::random(bitSize);
    a.mvDigits[0] |= 1;
    return std::move(a);
}

void BigInt::toString(std::string& str, int numBase) const {
    if (numBase != 2 && numBase != 16) return;
    str.clear();

    int len = mvDigits.size();
    if (numBase == 2) { // binary
        for (int i = len - 1; i >= 0; i--) {
            for (int j = mBase_bit - 1; j >= 0; j--)
                str.push_back('0' + ((mvDigits[i] >> j) & 1));
        }
    }
    else // hex
    {
        for (int i = len - 1; i >= 0; i--) {
            for (int j = mBase_bit - 4; j >= 0; j -= 4) {
                int digit = ((mvDigits[i] >> j) & 15);
                if (digit >= 10) str.push_back('A' + digit - 10);
                else str.push_back('0' + digit);
            }
        }
    }

    while (str.length() > 1 && str[0] == '0')
        str.erase(str.begin());

    if (!sgn) str = "-" + str;
}

std::string BigInt::toString(int numBase) const {
    string res;
    this->toString(res, numBase);
    return std::move(res);
}

int BigInt::countSuffixZeros() const {
    int cnt = 0;
    int x = 0;
    for (int i = 0; i < mvDigits.size(); i++) {
        if (mvDigits[i] != 0) {
            x = mvDigits[i];
            break;
        }
        cnt += mBase_bit;
    }
    while (x > 0 && !(x & 1))
        x >>= 1, cnt++;
    return cnt;
}

int BigInt::compare(const BigInt& a) const {
    if (sgn && !a.sgn) return 1;
    if (!sgn && a.sgn) return -1;

    return this->abs_compare(a) * (sgn ? 1 : -1);
}

int BigInt::abs_compare(const BigInt& a) const {
    if (mvDigits.size() > a.mvDigits.size())
        return 1;
    if (mvDigits.size() < a.mvDigits.size())
        return -1;
    for (int i = mvDigits.size() - 1; i >= 0; i--) {
        if (mvDigits[i] > a.mvDigits[i])
            return 1;
        if (mvDigits[i] < a.mvDigits[i])
            return -1;
    }
    return 0;
}

BigInt BigInt::abs_plus(const BigInt& a) const {
    BigInt sum(0);
    sum.mvDigits.clear();
    sum.mvDigits.reserve(max(mvDigits.size(), a.mvDigits.size()));
    int p = 0;

    if (mvDigits.size() >= a.mvDigits.size()) {
        for (int i = 0; i < a.mvDigits.size(); i++) {
            sum.mvDigits.push_back((mvDigits[i] + a.mvDigits[i] + p) & mBase_mask);
            p = ((mvDigits[i] + a.mvDigits[i] + p) >> mBase_bit);
        }
        for (int i = a.mvDigits.size(); i < mvDigits.size(); i++) {
            sum.mvDigits.push_back((mvDigits[i] + p) & mBase_mask);
            p = ((mvDigits[i] + p) >> mBase_bit);
        }
    }
    else
    {
        for (int i = 0; i < mvDigits.size(); i++) {
            sum.mvDigits.push_back((mvDigits[i] + a.mvDigits[i] + p) & mBase_mask);
            p = ((mvDigits[i] + a.mvDigits[i] + p) >> mBase_bit);
        }
        for (int i = mvDigits.size(); i < a.mvDigits.size(); i++) {
            sum.mvDigits.push_back((a.mvDigits[i] + p) & mBase_mask);
            p = ((a.mvDigits[i] + p) >> mBase_bit);
        }
    }

    if (p > 0) {
        sum.mvDigits.push_back(p);
    }

    return std::move(sum);
}

BigInt BigInt::abs_plus(unsigned a) const {
    BigInt res(*this);
    for (int i = 0; i < mvDigits.size() && a > 0; i++) {
        res.mvDigits[i] += a;
        a = (res.mvDigits[i] >> mBase_bit);
        res.mvDigits[i] &= mBase_mask;
        /*res.mvDigits[i] += a & mBase_mask;
        a = (a >> mBase_bit) + (res.mvDigits[i] >> mBase_bit);
        res.mvDigits[i] &= mBase_mask;*/
    }
    while (a > 0) {
        res.mvDigits.push_back(a & mBase_mask);
        a >>= mBase_bit;
    }
    return std::move(res);
}

void BigInt::abs_selfplus(const BigInt& a, unsigned k) {
    int p = 0;

    if (mvDigits.size() >= a.mvDigits.size()) {
        for (int i = 0; i < a.mvDigits.size(); i++) {
            mvDigits[i] = ((mvDigits[i] + k * a.mvDigits[i] + p) & mBase_mask);
            p = ((mvDigits[i] + k * a.mvDigits[i] + p) >> mBase_bit);
        }
        for (int i = a.mvDigits.size(); i < mvDigits.size(); i++) {
            mvDigits[i] = ((mvDigits[i] + p) & mBase_mask);
            p = ((mvDigits[i] + p) >> mBase_bit);
        }
    }
    else
    {
        for (int i = 0; i < mvDigits.size(); i++) {
            mvDigits[i] = ((mvDigits[i] + k * a.mvDigits[i] + p) & mBase_mask);
            p = ((mvDigits[i] + k * a.mvDigits[i] + p) >> mBase_bit);
        }
        for (int i = mvDigits.size(); i < a.mvDigits.size(); i++) {
            mvDigits.push_back((k * a.mvDigits[i] + p) & mBase_mask);
            p = ((k * a.mvDigits[i] + p) >> mBase_bit);
        }
    }
    while (p > 0) {
        mvDigits.push_back(p & mBase_mask);
        p >>= mBase_bit;
    }
}

void BigInt::abs_selfplus(unsigned a) {
    int len = mvDigits.size();
    for (int i = 0; i < len && a > 0; i++) {
        mvDigits[i] += a;
        a = (mvDigits[i] >> mBase_bit);
        mvDigits[i] &= mBase_mask;
    }
    while (a > 0) {
        mvDigits.push_back(a & mBase_mask);
        a >>= mBase_bit;
    }
}

BigInt BigInt::abs_minus(const BigInt& a) const {
    int c = this->abs_compare(a);

    if (c == 1) { // this > a
        return std::move(this->abs_minus2(a));
    }
    else
    if (c == -1) { // this < a
        return std::move(a.abs_minus2(*this));
    }

    // this == a
    return Zero;
}

BigInt BigInt::abs_minus2(const BigInt& a) const {
    BigInt diff(0);
    diff.mvDigits.clear();
    diff.mvDigits.reserve(mvDigits.size());
    int p = 0;

    for (int i = 0; i < a.mvDigits.size(); i++) {
        int x = mvDigits[i] - a.mvDigits[i] - p;
        if (x < 0) x += mBase, p = 1;
        else p = 0;
        diff.mvDigits.push_back(x);
    }

    for (int i = a.mvDigits.size(); i < mvDigits.size(); i++) {
        int x = mvDigits[i] - p;
        if (x < 0) x += mBase, p = 1;
        else p = 0;
        diff.mvDigits.push_back(x);
    }

    // remoe pre-zeros
    while (diff.mvDigits.size() > 1 && diff.mvDigits.back() == 0)
        diff.mvDigits.pop_back();

    return std::move(diff);
}

BigInt BigInt::abs_minus2(unsigned a) const {
    BigInt res(*this);
    res.abs_selfminus2(a);
    return std::move(res);
}

void BigInt::abs_selfminus2(const BigInt& a) {
    int p = 0;

    for (int i = 0; i < a.mvDigits.size(); i++) {
        mvDigits[i] -= a.mvDigits[i] + p;
        if (mvDigits[i] < 0) mvDigits[i] += mBase, p = 1;
        else p = 0;
    }

    for (int i = a.mvDigits.size(); i < mvDigits.size(); i++) {
        mvDigits[i] -= p;
        if (mvDigits[i] < 0) mvDigits[i] += mBase, p = 1;
        else break;
    }

    // remoe pre-zeros
    while (mvDigits.size() > 1 && mvDigits.back() == 0)
        mvDigits.pop_back();
}

void BigInt::abs_selfminus2(const BigInt& a, unsigned k) {
    int p = 0;

    for (int i = 0; i < a.mvDigits.size(); i++) {
        mvDigits[i] -= a.mvDigits[i] * k + p;
        if (mvDigits[i] < 0) {
            p = ((-mvDigits[i] - 1) >> mBase_bit) + 1;
            mvDigits[i] += (p << mBase_bit);
        }
        else p = 0;
    }

    for (int i = a.mvDigits.size(); i < mvDigits.size(); i++) {
        mvDigits[i] -= p;
        if (mvDigits[i] < 0) {
            p = ((-mvDigits[i] - 1) >> mBase_bit) + 1;
            mvDigits[i] += (p << mBase_bit);
        }
        else break;
    }

    // remoe pre-zeros
    while (mvDigits.size() > 1 && mvDigits.back() == 0)
        mvDigits.pop_back();
}

void BigInt::abs_selfminus2(unsigned a) {
    int len = mvDigits.size();
    for (int i = 0; i < len && a > 0; i++) {
        mvDigits[i] -= (a & mBase_mask);
        a >>= mBase_bit;
        if (mvDigits[i] < 0) {
            mvDigits[i] += mBase;
            a++;
        }
    }
}

BigInt BigInt::plus(const BigInt& a) const {
    // the same sign
    if ((sgn && a.sgn) || (!sgn && !a.sgn)) {
        BigInt res = this->abs_plus(a);
        res.setSign(sgn);
        return std::move(res);
    }
    else // different signs
    {
        BigInt res = this->abs_minus(a);
        int c = this->abs_compare(a);
        if (sgn && !a.sgn) { // this > 0, a < 0
            if (c >= 0) res.setPositive();
            else res.setNegtive();
        }
        else // this < 0, a > 0
        {
            if (c <= 0) res.setPositive();
            else res.setNegtive();
        }
        return std::move(res);
    }
    return BigInt();
}

BigInt BigInt::minus(const BigInt& a) const {
    // different signs
    if (sgn && !a.sgn) { // this >= 0 > a
        return std::move(this->abs_plus(a));
    }
    else
    if (!sgn && a.sgn) { // a >= 0 > this
        BigInt res = this->abs_plus(a);
        res.setNegtive();
        return std::move(res);
    }
    else // the same sign
    {
        int c = this->abs_compare(a);
        switch (c) {
            case 1: // |this| > |a|
            {
                BigInt res = this->abs_minus2(a);
                res.setSign(sgn);
                return std::move(res);
            }
            case -1: // |a| > |this|
            {
                BigInt res = a.abs_minus2(*this);
                res.setSign(!sgn);
                return std::move(res);
            }
            default:
                return BigInt();
        }
    }

    return BigInt();
}

BigInt BigInt::leftShift(unsigned h) const {
    BigInt res;
    res.setSign(sgn);
    res.mvDigits = vector<int>(h / mBase_bit, 0);

    if (h % mBase_bit == 0) {
        res.mvDigits.insert(res.mvDigits.end(), mvDigits.begin(), mvDigits.end());
        return std::move(res);
    }

    const int h1 = h % mBase_bit;
    const int h2 = mBase_bit - h1;
    int p = 0;
    for (int i = 0; i < mvDigits.size(); i++) {
        int x = ((mvDigits[i] << h1) & mBase_mask) | p;
        p = mvDigits[i] >> h2;
        res.mvDigits.push_back(x);
    }
    if (p > 0)
        res.mvDigits.push_back(p);

    return std::move(res);
}

void BigInt::leftSelfShift(unsigned h) {
    mvDigits.insert(mvDigits.begin(), h / mBase_bit, 0);

    const int h1 = h % mBase_bit;
    const int h2 = mBase_bit - h1;
    if (h1 == 0) return;
    int p = 0;
    for (int i = 0; i < mvDigits.size(); i++) {
        int x = ((mvDigits[i] << h1) & mBase_mask) | p;
        p = mvDigits[i] >> h2;
        mvDigits[i] = x;
    }

    if (p > 0) mvDigits.push_back(p);
}

// todo: deal with negtive flooring
BigInt BigInt::rightShift(unsigned h) const {
    if (h > mvDigits.size()* mBase_bit)
        return BigInt();

    BigInt res;
    res.mvDigits.assign(mvDigits.begin() + h / mBase_bit, mvDigits.end());
    res.setSign(sgn);

    const int h1 = h % mBase_bit;
    const int h2 = mBase_bit - h1;
    if (h1 == 0) return std::move(res);

    int len = res.mvDigits.size();
    for (int i = 0; i < len; i++) {
        res.mvDigits[i] >>= h1;
        if (i < len - 1) {
            res.mvDigits[i] |= ((res.mvDigits[i + 1] & ((1 << h1) - 1)) << h2);
        }
    }

    // remoe pre-zeros
    while (res.mvDigits.size() > 1 && res.mvDigits.back() == 0)
        res.mvDigits.pop_back();

    return std::move(res);
}

void BigInt::rightSelfShift(unsigned h) {
    if (h > mvDigits.size()* mBase_bit) {
        mvDigits.clear();
        mvDigits.push_back(0);
        sgn = true;
        return;
    }

    mvDigits.erase(mvDigits.begin(), mvDigits.begin() + h / mBase_bit);

    const int h1 = h % mBase_bit;
    const int h2 = mBase_bit - h1;
    if (h1 == 0) return;

    int len = mvDigits.size();
    for (int i = 0; i < len; i++) {
        mvDigits[i] >>= h1;
        if (i < len - 1) {
            mvDigits[i] |= ((mvDigits[i + 1] & ((1 << h1) - 1)) << h2);
        }
    }

    // remoe pre-zeros
    while (mvDigits.size() > 1 && mvDigits.back() == 0)
        mvDigits.pop_back();
}

BigInt BigInt::times(const BigInt& a) const
{
    if (this->compare(Zero) == 0 ||
        a.compare(Zero) == 0)
        return BigInt();
    BigInt res;
    res.setSign((sgn && a.sgn) || (!sgn && !a.sgn));
    int len = mvDigits.size() + a.mvDigits.size();
    res.mvDigits.assign(len, 0);
    for (int i = 0; i < mvDigits.size(); i++) {
        for (int j = 0; j < a.mvDigits.size(); j++)
            res.mvDigits[i + j] += mvDigits[i] * a.mvDigits[j];
    }
    // carry
    int p = 0;
    for (int i = 0; i < len; i++) {
        res.mvDigits[i] = res.mvDigits[i] + p;
        p = res.mvDigits[i] >> mBase_bit;
        res.mvDigits[i] &= mBase_mask;
    }

    // remove pre-zeros
    while (res.mvDigits.size() > 1 && res.mvDigits.back() == 0)
        res.mvDigits.pop_back();

    return std::move(res);
}

BigInt BigInt::times(unsigned a) const {
    if (a == 0) return Zero;
    BigInt res;
    res.mvDigits.clear();
    int p = 0;
    for (int i = 0; i < mvDigits.size(); i++) {
        res.mvDigits.push_back((mvDigits[i] * a + p) & mBase_mask);
        p = ((mvDigits[i] * a + p) >> mBase_bit);
    }

    while (p > 0) {
        res.mvDigits.push_back(p & mBase_mask);
        p >>= mBase_bit;
    }

    return std::move(res);
}

//BigInt BigInt::divide(const BigInt& a, BigInt& r) const {
//	if (a.compare(Zero) == 0) return BigInt();
//
//	// store a*2^0, a*2^1...
//	vector<BigInt> aExps;
//	aExps.emplace_back(a);
//	for (int i = 1; i < mBase_bit; i++) {
//		aExps.emplace_back(aExps.back().leftShift(1));
//	}
//
//	int len = mvDigits.size();
//	r.mvDigits.clear();
//	r.setPositive();
//	BigInt d;
//	d.mvDigits.clear();
//	d.mvDigits.insert(d.mvDigits.begin(), len, 0);
//	for (int i = len - 1; i >= 0; i--) {
//		if (r.compare(Zero) == 0) r.mvDigits[0] = mvDigits[i];
//			else r.mvDigits.insert(r.mvDigits.begin(), 1, mvDigits[i]);
//		int c = r.abs_compare(a);
//		if (c >= 0) {
//			int digit = 0;
//			for (int j = mBase_bit - 1; j >= 0; j--) {
//				if (r.abs_compare(aExps[j]) >= 0) {
//					//r = r.abs_minus2(aExps[j]);
//					r.abs_selfminus2(aExps[j]);
//					digit |= (1 << j);
//				}
//			}
//			d.mvDigits[i] = digit;
//		}
//	}
//
//	// remoe pre-zeros
//	while (d.mvDigits.size() > 1 && d.mvDigits.back() == 0)
//		d.mvDigits.pop_back();
//
//	if ((sgn && a.sgn) || (!sgn && !a.sgn)) {
//		d.setPositive();
//	}
//	else
//	{
//		d.setNegtive();
//	}
//
//	return std::move(d);
//}

BigInt BigInt::divide(const BigInt& a, BigInt& r) const {
    const int h1 = this->digits();
    const int h2 = a.digits();
    if (h2 == 0) return Zero; // a == 0
    if (h1 < h2) { // this < a
        r = *this;
        r.setPositive();
        return Zero;
    }

    BigInt d;
    d.mvDigits.resize(h1, 0);
    r.mvDigits.assign(mvDigits.begin() + (h1 - h2 + 1), mvDigits.end());
    for (int i = h1 - h2; i >= 0; i--) {
        r.mvDigits.insert(r.mvDigits.begin(), 1, mvDigits[i]);
        while (r.compare(a) >= 0) {
            int s = r.mvDigits.back(), t;
            if (s < a.mvDigits.back()) {
                s = ((s << mBase_bit) | r.mvDigits[r.mvDigits.size() - 2]);
            }
            t = s / (a.mvDigits.back() + 1);
            if (t == 0) t = 1;
            // r = r - a.times(t);
            r.abs_selfminus2(a, static_cast<unsigned>(t));
            d.mvDigits[i] += t;
        }
    }

    // remoe pre-zeros
    while (d.mvDigits.size() > 1 && d.mvDigits.back() == 0)
        d.mvDigits.pop_back();

    if ((sgn && a.sgn) || (!sgn && !a.sgn)) {
        d.setPositive();
    }
    else
    {
        d.setNegtive();
    }

    return std::move(d);
}

BigInt BigInt::operator+(const BigInt& a) const {
    return std::move(this->plus(a));
}

BigInt BigInt::operator-(const BigInt& a) const {
    return std::move(this->minus(a));
}

BigInt BigInt::operator<<(const int& h) const
{
    return std::move(leftShift(h));
}

BigInt BigInt::operator>>(const int& h) const
{
    return std::move(rightShift(h));
}

BigInt BigInt::operator*(const BigInt& a) const {
    return std::move(this->times(a));
}

BigInt BigInt::operator/(const BigInt& a) const {
    BigInt r;
    return std::move(this->divide(a, r));
}

BigInt BigInt::operator%(const BigInt& a) const {
    BigInt r;
    this->divide(a, r);
    return std::move(r);
}

int BigInt::operator%(const int a) const {
    int p = 0;
    for (int i = mvDigits.size() - 1; i >= 0; i--) {
        p = ((p << mBase_bit) | mvDigits[i]) % a;
    }
    return p;
}

std::ostream& operator<<(std::ostream& output, const BigInt& a) {
    std::string str;
    a.toString(str, 16);
    output << str << endl;
    return output;
}



