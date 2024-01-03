//
// Created by Administrator on 2022/4/24.
//

#ifndef ANTISDK_BIGINT_H
#define ANTISDK_BIGINT_H


#include <iostream>
#include <algorithm>
#include <cstring>
#include <vector>
#include <string>
#include <fstream>



#define mBase_bit 12
#define mBase (1 << mBase_bit)
#define mBase_mask (mBase - 1)



class BigInt {
public:
    BigInt(int x = 0);
    // only support loading from binary or hex
    BigInt(const std::string&, int numBase = 2);
    BigInt(const BigInt&);
    void fromInt(int);
    void fromString(const std::string&, int numBase = 2);

    // only support converting to binary or hex
    void toString(std::string&, int numBase = 2) const;
    std::string toString(int numBase = 2) const;

    static BigInt random(unsigned bitSize);
    static BigInt randomOdd(unsigned bitSize);

    void setSign(bool sign) { this->sgn = sign; }
    void setOpposite() { this->sgn = !this->sgn; }
    void setPositive() { this->sgn = true; }
    void setNegtive() { this->sgn = false; }

    int digits() const {
        if (mvDigits.size() == 1 && mvDigits[0] == 0)
            return 0;
        return mvDigits.size();
    }

    int getDigit(size_t idx) const {
        if (idx < mvDigits.size())
            return mvDigits[idx];
        return -1;
    }

    bool isOdd() const {
        if (this->mvDigits.empty())
            return false;
        return (this->mvDigits[0] & 1);
    };

    /* Name: countSuffixZeros
      * Description: count the number of suffix numbers in binary
     * return value
     *				i, where this = (2^i) * d, d is and odd
     */
    int countSuffixZeros() const;

    /* Name: compare
     * Description: compare with @a
     * return value
     *				0  if equal
     *				1  if this > a
     *			   -1  if this < a
     */
    int compare(const BigInt& a) const;
    /* Name: abs_compare
     * Description: compare with @a by absolute value
     * return value
     *				0  if equal
     *				1  if abs(this) > abs(a)
     *			   -1  if abs(this) < abs(a)
     */
    int abs_compare(const BigInt& a) const;

    /* Name: abs_plus
     * Description: the sum of this and @a by absolute value
     * return value
     *				abs(this) + abs(a)
     */
    BigInt abs_plus(const BigInt& a) const;
    BigInt abs_plus(unsigned a) const;
    /* Name: abs_selfminus2
     * Description: modify this to the sum of this and @k * @a by absolute value
     * return value NONE
     */
    void abs_selfplus(const BigInt& a, unsigned k); // this + k * a
    void abs_selfplus(unsigned a);

    /* Name: abs_minus
     * Description: the difference between this and @a by absolute value
     * return value
     *				abs(abs(this) - abs(a))
     */
    BigInt abs_minus(const BigInt& a) const;

    /* Name: abs_minus2
     * Description: the minus of this and @a by absolute value
     * return value
     *				abs(this) - abs(a)
     * NOTE: please make sure abs(this) > abs(a)
     */
    BigInt abs_minus2(const BigInt& a) const;
    BigInt abs_minus2(unsigned a) const;

    /* Name: abs_selfminus2
     * Description: modify this to the minus of this and @a by absolute value
     * return value NONE
     * NOTE: please make sure abs(this) > abs(a)
     */
    void abs_selfminus2(const BigInt& a);
    void abs_selfminus2(const BigInt& a, unsigned k); // this - k * a
    void abs_selfminus2(unsigned a); // this - a

    /* Name: plus
     * Description: the sum of this and @a
     * return value
     *				this + a
     */
    BigInt plus(const BigInt& a) const;
    /* Name: plus
     * Description: the minus of this and @a
     * return value
     *				this + a
     */
    BigInt minus(const BigInt& a) const;
    /* Name: leftShift
     * Description: left shift this by @h bits
     * return value
     *				this << h
     */
    BigInt leftShift(unsigned h) const;
    /* Name: leftSelfShift
     * Description: modified this by left shift @h bits
     * return value: NONE
     */
    void leftSelfShift(unsigned h);
    /* Name: rightShift
     * Description: right shift this by @h bits
     * return value
     *				this >> h
     */
    BigInt rightShift(unsigned h) const;
    /* Name: rightSelfShift
     * Description: modified this by right shift @h bits
     * return value: NONE
     */
    void rightSelfShift(unsigned h);
    /* Name: times
     * Description: the product of this and @a
     * return value
     *				this * a
     */
    BigInt times(const BigInt& a) const;
    BigInt times(unsigned a) const;
    /* Name: divide
     * Description: the division of this and @a
     * return value
     *				this / a, and the modulate r
     */
    BigInt divide(const BigInt& a, BigInt& r) const;


    BigInt& operator =(const BigInt& a) {
        sgn = a.sgn;
        this->mvDigits.assign(a.mvDigits.begin(), a.mvDigits.end());
        //mvDigits.clear();
        //mvDigits.insert(mvDigits.begin(), a.mvDigits.begin(), a.mvDigits.end());
        return (*this);
    }
    BigInt operator +(const BigInt& a) const;
    BigInt operator -(const BigInt& a) const;
    BigInt operator <<(const int& h) const;
    BigInt operator >>(const int& h) const;
    BigInt operator *(const BigInt& a) const;
    BigInt operator /(const BigInt& a) const;
    BigInt operator %(const BigInt& a) const;
    int operator %(const int a) const;

    friend std::ostream& operator <<(std::ostream& output, const BigInt&);
private:
    bool sgn; // positive if true, negtive else false
    std::vector<int> mvDigits; // index 0 is the least number
};

const BigInt Zero(0);
const BigInt One(1);
const BigInt Two(2);




#endif //ANTISDK_BIGINT_H
