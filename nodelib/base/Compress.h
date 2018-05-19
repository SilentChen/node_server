/*
 *	Compress.h
 *
 *  Created on: Dec 28, 2016
 *      Author: zhangyalei
 */

#ifndef COMPRESS_H_
#define COMPRESS_H_

#include <zlib.h>

//��ȡѹ����buf���ȣ���ѹ��ǰ����
uLong comp_bound(uLong src_len);
//zlibѹ���㷨
bool comp(char* dest, uLongf* dest_len, const char* src, uLong src_len);
bool decomp(char* dest, uLongf* dest_len, const char* src, uLong src_len);

//�ļ�ѹ����ѹ
int file_comp(const char* file_path);
int file_decomp(const char* file_path);

//�ļ���ѹ����ѹ
int folder_comp(const char* folder_path);
int folder_decomp(const char* folder_path);

#endif // COMPRESS_H_

