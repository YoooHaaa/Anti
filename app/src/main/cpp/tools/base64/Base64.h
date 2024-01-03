//
// Created by Administrator on 2022/4/15.
//

#include <string>

#ifndef ANTITOOLS_BASE64_H
#define ANTITOOLS_BASE64_H

const char kBase64Alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                               "abcdefghijklmnopqrstuvwxyz"
                               "0123456789+/";

class Base64 {
public:
    static void test(char* data);

    static bool Encode(const std::string &in, std::string *out);

    static bool Encode(const char *input, size_t input_length, char *out, size_t out_length);

    static bool Decode(const std::string &in, std::string *out);

    static bool Decode(const char *input, size_t input_length, char *out, size_t out_length);

    static size_t DecodedLength(const char *in, size_t in_length);

    static size_t DecodedLength(const std::string &in);

    static size_t EncodedLength(size_t length);

    static size_t EncodedLength(const std::string &in);

    static void StripPadding(std::string *in);

private:
    static inline void a3_to_a4(unsigned char * a4, unsigned char * a3);

    static inline void a4_to_a3(unsigned char * a3, unsigned char * a4);

    static inline unsigned char b64_lookup(unsigned char c);
};


#endif //ANTITOOLS_BASE64_H
