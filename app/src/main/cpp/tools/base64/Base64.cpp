//
// Created by Administrator on 2022/4/15.
//

#include "Base64.h"
#include <stdlib.h>
#include "../../base/mem//Mem.h"
#include "../../include//include.h"



UNEXPORT bool Base64::Encode(const std::string &in, std::string *out) {
    int i = 0, j = 0;
    size_t enc_len = 0;
    unsigned char a3[3];
    unsigned char a4[4];

    out->resize(EncodedLength(in));

    size_t input_len = in.size();
    std::string::const_iterator input = in.begin();

    while (input_len--) {
        a3[i++] = *(input++);
        if (i == 3) {
            a3_to_a4(a4, a3);

            for (i = 0; i < 4; i++) {
                (*out)[enc_len++] = kBase64Alphabet[a4[i]];
            }

            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++) {
            a3[j] = '\0';
        }

        a3_to_a4(a4, a3);

        for (j = 0; j < i + 1; j++) {
            (*out)[enc_len++] = kBase64Alphabet[a4[j]];
        }

        while ((i++ < 3)) {
            (*out)[enc_len++] = '=';
        }
    }

    return (enc_len == out->size());
}

UNEXPORT bool Base64::Encode(const char *input, size_t input_length, char *out, size_t out_length) {
    int i = 0, j = 0;
    char *out_begin = out;
    unsigned char a3[3];
    unsigned char a4[4];

    size_t encoded_length = EncodedLength(input_length);

    if (out_length < encoded_length) return false;

    while (input_length--) {
        a3[i++] = *input++;
        if (i == 3) {
            a3_to_a4(a4, a3);

            for (i = 0; i < 4; i++) {
                *out++ = kBase64Alphabet[a4[i]];
            }

            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 3; j++) {
            a3[j] = '\0';
        }

        a3_to_a4(a4, a3);

        for (j = 0; j < i + 1; j++) {
            *out++ = kBase64Alphabet[a4[j]];
        }

        while ((i++ < 3)) {
            *out++ = '=';
        }
    }

    return (out == (out_begin + encoded_length));
}

UNEXPORT bool Base64::Decode(const std::string &in, std::string *out) {
    int i = 0, j = 0;
    size_t dec_len = 0;
    unsigned char a3[3];
    unsigned char a4[4];

    size_t input_len = in.size();
    std::string::const_iterator input = in.begin();

    out->resize(DecodedLength(in));

    while (input_len--) {
        if (*input == '=') {
            break;
        }

        a4[i++] = *(input++);
        if (i == 4) {
            for (i = 0; i <4; i++) {
                a4[i] = b64_lookup(a4[i]);
            }

            a4_to_a3(a3,a4);

            for (i = 0; i < 3; i++) {
                (*out)[dec_len++] = a3[i];
            }

            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) {
            a4[j] = '\0';
        }

        for (j = 0; j < 4; j++) {
            a4[j] = b64_lookup(a4[j]);
        }

        a4_to_a3(a3,a4);

        for (j = 0; j < i - 1; j++) {
            (*out)[dec_len++] = a3[j];
        }
    }

    return (dec_len == out->size());
}

UNEXPORT bool Base64::Decode(const char *input, size_t input_length, char *out, size_t out_length) {
    int i = 0, j = 0;
    char *out_begin = out;
    unsigned char a3[3];
    unsigned char a4[4];

    size_t decoded_length = DecodedLength(input, input_length);

    if (out_length < decoded_length) return false;

    while (input_length--) {
        if (*input == '=') {
            break;
        }

        a4[i++] = *(input++);
        if (i == 4) {
            for (i = 0; i <4; i++) {
                a4[i] = b64_lookup(a4[i]);
            }

            a4_to_a3(a3,a4);

            for (i = 0; i < 3; i++) {
                *out++ = a3[i];
            }

            i = 0;
        }
    }

    if (i) {
        for (j = i; j < 4; j++) {
            a4[j] = '\0';
        }

        for (j = 0; j < 4; j++) {
            a4[j] = b64_lookup(a4[j]);
        }

        a4_to_a3(a3,a4);

        for (j = 0; j < i - 1; j++) {
            *out++ = a3[j];
        }
    }

    return (out == (out_begin + decoded_length));
}

UNEXPORT size_t Base64::DecodedLength(const char *in, size_t in_length) {
    int numEq = 0;

    const char *in_end = in + in_length;
    while (*--in_end == '=') ++numEq;

    return ((6 * in_length) / 8) - numEq;
}

UNEXPORT size_t Base64::DecodedLength(const std::string &in) {
    int numEq = 0;
    size_t n = in.size();

    for (std::string::const_reverse_iterator it = in.rbegin(); *it == '='; ++it) {
        ++numEq;
    }

    return ((6 * n) / 8) - numEq;
}

UNEXPORT size_t Base64::EncodedLength(size_t length) {
    return (length + 2 - ((length + 2) % 3)) / 3 * 4;
}

UNEXPORT size_t Base64::EncodedLength(const std::string &in) {
    return EncodedLength(in.length());
}

UNEXPORT void Base64::StripPadding(std::string *in) {
    while (!in->empty() && *(in->rbegin()) == '=') in->resize(in->size() - 1);
}

UNEXPORT void Base64::a3_to_a4(unsigned char * a4, unsigned char * a3) {
    a4[0] = (a3[0] & 0xfc) >> 2;
    a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
    a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
    a4[3] = (a3[2] & 0x3f);
}

UNEXPORT void Base64::a4_to_a3(unsigned char * a3, unsigned char * a4) {
    a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
    a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
    a3[2] = ((a4[2] & 0x3) << 6) + a4[3];
}

UNEXPORT unsigned char Base64::b64_lookup(unsigned char c) {
    if(c >='A' && c <='Z') return c - 'A';
    if(c >='a' && c <='z') return c - 71;
    if(c >='0' && c <='9') return c + 4;
    if(c == '+') return 62;
    if(c == '/') return 63;
    return 255;
}

UNEXPORT void Base64::test(char* data) {
    LOGE("in Base64::test");
    std::string st;
    Base64::Encode(data, &st);
    LOGI("Encode : %s -> %s", data, st.c_str());
    char* cEncode = new char[st.length() + 1];
    Mem::mem_copy(&cEncode, st.c_str());
    Base64::Decode(cEncode, &st); // "cmVhbE5hbWU="
    LOGI("Decode : %s -> %s", cEncode,  st.c_str());
}
