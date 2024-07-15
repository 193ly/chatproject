#ifndef MYAES_H
#define MYAES_H

#define AES_BITS 128
#define MSG_LEN 128
#define AES_GROUP_BLOCK 16

#include "openssl/aes.h"
class MyAES
{
public:
    MyAES();
    //len������ĳ���
    static int aes_encrypt(const char* in,  char* out, int len/*����ĳ���*/, char* key);
    //len������ĳ���
    static int aes_decrypt(const char* in,  char* out,int len/*����ĳ���*/, char* key);
    void ase_key_init();
    char key[AES_BLOCK_SIZE];
};
#endif // MYAES_H
