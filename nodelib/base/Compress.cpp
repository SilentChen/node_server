/*
 *	Compress.cpp
 *
 *  Created on: Dec 28, 2016
 *      Author: zhangyalei
 */

#include <stdio.h>
#include "Base_Function.h"
#include "Log.h"
#include "Compress.h"

uLong comp_bound(uLong src_len) {
    return compressBound(src_len);
}

bool comp(char* dest, uLongf* dest_len, const char* src, uLong src_len) {
    int res = compress((Bytef*)dest, (uLongf*)dest_len, (const Bytef*)src, src_len);
    if(res != Z_OK) {
    	LOG_ERROR("compress failure:%d", res);
        return false;
    }
    return true;
}

bool decomp(char* dest, uLongf* dest_len, const char* src, uLong src_len) {
    int res = uncompress((Bytef*)dest, (uLongf*)dest_len, (const Bytef*)src, src_len);
    if(res != Z_OK) {
    	LOG_ERROR("uncompress failure:%d", res);
        return false;
    }
    return true;
}

int file_comp(const char* file_path) {
	FILE* file = NULL;
	//�Զ�дģʽ���ļ������ļ�ͷ����ʼ��д�������ģʽ��д�ļ�����ͷ��ʼд��������ļ����ļ�����δ���ǲ��ֱ���
	file = fopen(file_path, "rb+");
	if (file == NULL) {
		LOG_ERROR("open file error, rb+ mode, file_path:%s", file_path);
		return -1;
	}
	//��ȡ�ļ�����
	fseek(file, 0, SEEK_END);
	size_t src_len = ftell(file);
	rewind(file);
	//��ȡ�ļ�����
	char* src = new char[src_len + 1];
	src[src_len] = '\0';
	for (size_t i = 0; i < src_len;) {
		i += fread(&src[i], 1, src_len - i, file);
		if (ferror(file)) {
			fclose(file);
			delete[] src;
			return -1;
		}
	}
	fclose(file);

	//��ȡѹ����ĳ���
	uLong dest_len = comp_bound(src_len);
	char* dest = new char[dest_len + 1];
	dest[dest_len] = '\0';
	//ѹ������
	if(!comp(dest, &dest_len, src, src_len)) {
		LOG_ERROR("compress file:%s fail", file_path);
		delete[] src;
		delete[] dest;
		return -1;
	}

	//�Զ�дģʽ���ļ������ļ�ͷ����ʼ��д����ģʽд�ļ�������ļ�
	file = fopen(file_path, "wb+");
	if (file == NULL) {
		LOG_ERROR("open file error, wb+ mode, file_path:%s", file_path);
		delete[] src;
		delete[] dest;
		return -1;
	}
	//��ѹ���������д���ļ�
	fwrite(dest, 1, dest_len, file);
	fclose(file);

	delete[] src;
	delete[] dest;
	return 0;
}

int file_decomp(const char* file_path) {
	FILE* file = NULL;
	//�Զ�дģʽ���ļ������ļ�ͷ����ʼ��д�������ģʽ��д�ļ�����ͷ��ʼд��������ļ����ļ�����δ���ǲ��ֱ���
	file = fopen(file_path, "rb+");
	if (file == NULL) {
		LOG_ERROR("open file error, rb+ mode, file_path:%s", file_path);
		return -1;
	}
	//��ȡ�ļ�����
	fseek(file, 0, SEEK_END);
	size_t src_len = ftell(file);
	rewind(file);
	//��ȡ�ļ�����
	char* src = new char[src_len + 1];
	src[src_len] = '\0';
	for (size_t i = 0; i < src_len;) {
		i += fread(&src[i], 1, src_len - i, file);
		if (ferror(file)) {
			fclose(file);
			delete[] src;
			return -1;
		}
	}
	fclose(file);

	//��ʼ����ѹ��buf����Ϊһ��������
	uLong dest_len = 65536*100;
	char* dest = new char[dest_len + 1];
	dest[dest_len] = '\0';
	//��ѹ����
	if(!decomp(dest, &dest_len, src, src_len)) {
		LOG_ERROR("decompress file:%s fail", file_path);
		delete[] src;
		delete[] dest;
		return -1;
	}

	//�Զ�дģʽ���ļ������ļ�ͷ����ʼ��д����ģʽд�ļ�������ļ�
	file = fopen(file_path, "wb+");
	if (file == NULL) {
		LOG_ERROR("open file error, wb+ mode, file_path:%s", file_path);
		delete[] src;
		delete[] dest;
		return -1;
	}
	//����ѹ�������д���ļ�
	fwrite(dest, 1, dest_len, file);
	fclose(file);

	delete[] src;
	delete[] dest;
	return 0;
}

int folder_comp(const char* folder_path) {
	return read_folder(folder_path, file_comp);
}

int folder_decomp(const char* folder_path) {
	return read_folder(folder_path, file_decomp);
}