//
// Created by Administrator on 2023/6/9.
//

#include "CRC.h"
#include <cstdio>
#include <cerrno>

UNEXPORT quint16
CRC::calculate_crc16(quint16 CRC_INIT, quint16 CRC_POLY, quint16 CRC_RESULT_XOR, bool input_invert,
                     bool ouput_invert, const unsigned char *inputData, int inputDataLen) {
    quint8 data_item = 0;
    //商
    quint32 ans = 0;
    while (inputDataLen--){
        data_item = *(inputData++);
        //输入值反转 (最高位变最低位，最低位变最高位, 1100 0100 反转 0010 0011)
        if(input_invert){
            quint8 temp_char = data_item;
            data_item=0;
            for(int i=0;i<8;++i){
                if(temp_char&0x01)
                    data_item|=0x01<<(7-i);
                temp_char>>=1;
            }
        }
        //---多项式除法----------------------
        //---模2除法结果与异或相同
        //---模2除法可以用在CRC冗余校验上

        //多项式除法的余数R(x)=CRC，存储在CRC_INIT,作为下一次计算的被除数
        //data_item扩展成16位，异或初始值,
        CRC_INIT ^= (data_item << 8);

        //--当前输入字节的每一位执行模2除法
        for (int i = 0; i < 8; i++){
            //--从最高的16位开始模2除法
            if (CRC_INIT & 0x8000){//--第一位是1,商是1,执行摸2相减
                CRC_INIT = (CRC_INIT << 1) ^ CRC_POLY;
                ans += 1;
            }
            else{//--第一位是0,商是0,继续移位,不执行摸2相减
                CRC_INIT = CRC_INIT << 1;
                ans += 0;
            }
            ans = ans<<1;
        }
        ans<<1;
    }

    //输出值(16位)反转 (最高位变最低位，最低位变最高位, 1010 0010 0011 1001 反转 1001 1100 0100 0101 )
    if(ouput_invert){
        quint16 temp_short = CRC_INIT;
        CRC_INIT=0;
        for(int i=0;i<16;++i){
            if(temp_short&0x01)
                CRC_INIT|=0x01<<(15-i);
            temp_short>>=1;
        }
    }
    return (CRC_INIT^CRC_RESULT_XOR);
}

UNEXPORT int CRC::calcCRC(const unsigned char *inputData, int inputDataLen) {
    return calculate_crc16(0xFFFF, 0x8005, 0x0000, true, true, inputData, inputDataLen);
}

