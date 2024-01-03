//
// Created by Administrator on 2022/4/22.
//
#include <string>


#ifndef ANTISDK_MD5_H
#define ANTISDK_MD5_H

#define MD5_LONG unsigned long

// 分组大小
#define MD5_CBLOCK	64

// 分块个数
#define MD5_LBLOCK	(MD5_CBLOCK/4)

// 摘要长度（字节）
#define MD5_DIGEST_LENGTH 16

#define MD32_REG_T long
// 小端序
#define DATA_ORDER_IS_LITTLE_ENDIAN

// 四个初始化常量
#define INIT_DATA_A (unsigned long)0x67452301L
#define INIT_DATA_B (unsigned long)0xefcdab89L
#define INIT_DATA_C (unsigned long)0x98badcfeL
#define INIT_DATA_D (unsigned long)0x10325476L

// 循环左移以及取低64位
#define ROTATE(a,n)     (((a)<<(n))|(((a)&0xffffffff)>>(32-(n))))

// 大小端序互转
#define HOST_c2l(c,l)	(l =(((unsigned long)(*((c)++)))    ),		\
			 l|=(((unsigned long)(*((c)++)))<< 8),		\
			 l|=(((unsigned long)(*((c)++)))<<16),		\
			 l|=(((unsigned long)(*((c)++)))<<24)		)

#define HOST_l2c(l,c)	(*((c)++)=(unsigned char)(((l)    )&0xff),	\
			 *((c)++)=(unsigned char)(((l)>> 8)&0xff),	\
			 *((c)++)=(unsigned char)(((l)>>16)&0xff),	\
			 *((c)++)=(unsigned char)(((l)>>24)&0xff),	\
			 l)

// 更新链接变量值
#define	HASH_MAKE_STRING(c,s)	do {	\
	unsigned long ll;		\
	ll=(c)->A; (void)HOST_l2c(ll,(s));	\
	ll=(c)->B; (void)HOST_l2c(ll,(s));	\
	ll=(c)->C; (void)HOST_l2c(ll,(s));	\
	ll=(c)->D; (void)HOST_l2c(ll,(s));	\
	} while (0)

// 四个初始化非线性函数，或者叫逻辑函数
#define	F(b,c,d)	((((c) ^ (d)) & (b)) ^ (d))
#define	G(b,c,d)	((((b) ^ (c)) & (d)) ^ (c))
#define	H(b,c,d)	((b) ^ (c) ^ (d))
#define	I(b,c,d)	(((~(d)) | (b)) ^ (c))

// F函数，每隔16步/轮 换下一个
#define R0(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+F((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R1(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+G((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b;};

#define R2(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+H((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

#define R3(a,b,c,d,k,s,t) { \
	a+=((k)+(t)+I((b),(c),(d))); \
	a=ROTATE(a,s); \
	a+=b; };

typedef struct MD5state_st1{
    MD5_LONG A,B,C,D; // ABCD
    MD5_LONG Nl,Nh; // 数据的bit数计数器(对2^64取余)，Nh存储高32位，Nl存储低32位。这种设计是服务于32位处理器，MD5的设计就是为了服务于32位处理器的。
    MD5_LONG data[MD5_LBLOCK];//数据缓冲区
    unsigned int num;
}MD5_CTX; // 存放MD5算法相关信息的结构体定义



class MD5{
public:
	static unsigned char cleanse_ctr;

	static void test(char *dataBuffer);

	// 初始化链接变量/幻数
    static int MD5_Init(MD5_CTX *c);

	// md5 一个分组中的全部运算
    static void md5_block_data_order(MD5_CTX *c, const void *data_, unsigned int num);

	// 传入需要哈希的明文,支持多次调用
    static int MD5_Update(MD5_CTX *c, const void *data_, size_t len);

	// 得出最终结果
    static int MD5_Final(unsigned char *md, MD5_CTX *c);

	//清除加载的各种算法，包括对称算法、摘要算法以及 PBE 算法，并清除这些算法相关的哈希表的内容。
    void OPENSSL_cleanse(void *ptr, size_t len);
};

#endif //ANTISDK_MD5_H
