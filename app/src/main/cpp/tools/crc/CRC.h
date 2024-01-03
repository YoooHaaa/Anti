//
// Created by Administrator on 2023/6/9.
//

#ifndef ANTISDK_CRC_H
#define ANTISDK_CRC_H


#include "../../include/include.h"
#include <string>
using namespace std;


typedef signed char qint8;         /* 8 bit signed */
typedef unsigned char quint8;      /* 8 bit unsigned */
typedef short qint16;              /* 16 bit signed */
typedef unsigned short quint16;    /* 16 bit unsigned */
typedef int qint32;                /* 32 bit signed */
typedef unsigned int quint32;      /* 32 bit unsigned */

class CRC{
public:
    static int calcCRC(const unsigned  char *inputData, int inputDataLen);
private:
    /**
     * \brief calculate_crc16   通用的16位CRC校验算法
     * \param CRC_INIT          CRC16算法的初始值
     * \param CRC_POLY          特征多项式
     * \param CRC_RESULT_XOR    结果异或值
     * \param input_invert      输入值是否反转
     * \param ouput_invert      输出值是否反转
     * \param inputData         输入数据的起始地址
     * \param inputDataLen      输入数据的长度
     * \return                  16位CRC校验结果
     */
    static quint16 calculate_crc16(quint16 CRC_INIT,quint16 CRC_POLY,quint16 CRC_RESULT_XOR,bool input_invert,bool ouput_invert,const unsigned char *inputData, int inputDataLen);

};

#endif //ANTISDK_CRC_H
