/*
 * Aes.h
 *
 *  Created on: Dec 27, 2016
 *      Author: zhangyalei
 */

#ifndef AES_H_
#define AES_H_

#include <stdint.h>

//aes128λ�����㷨(ecbģʽ)
//ECBģʽ���������뱾ģʽ��Electronic codebook��
//ECB����򵥵Ŀ��������ģʽ������ǰ���ݼ��ܿ��С����AESΪ128λ���ֳ����ɿ飬֮��ÿ��ʹ����ͬ����Կ�������ܣ�����ͬ��
void aes128_ecb_encrypt(uint8_t* input, const uint8_t* key, uint8_t *output);
void aes128_ecb_decrypt(uint8_t* input, const uint8_t* key, uint8_t *output);

//aes128λ�����㷨(cbcģʽ)
//CBCģʽ������������ӣ�Cipher-block chaining��
//CBCģʽ����ÿ�������ܵ�������ڼ���ǰ������ǰһ���������������Ȼ�����ü��������ܡ���һ�����Ŀ���һ���г�ʼ�����������ݿ����
void aes128_cbc_encrypt(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);
void aes128_cbc_decrypt(uint8_t* output, uint8_t* input, uint32_t length, const uint8_t* key, const uint8_t* iv);

//�ļ����ܽ���
int file_encrypt(const char* file_path);
int file_decrypt(const char* file_path);

//�ļ��м��ܽ���
int folder_encrypt(const char* folder_path);
int folder_decrypt(const char* folder_path);

void phex(uint8_t* str);
void test_encrypt_ecb(void);
void test_decrypt_ecb(void);
void test_encrypt_ecb_verbose(void);
void test_encrypt_cbc(void);
void test_decrypt_cbc(void);

#endif //AES_H_
