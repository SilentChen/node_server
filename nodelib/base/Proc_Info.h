/*
 * Proc_Info.h
 *
 *  Created on: Nov 28,2016
 *      Author: zhangyalei
 */

#ifndef PROC_INFO_H_
#define PROC_INFO_H_

#include <map>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define GET_NAME(v)		(#v)
#define DEBUG_PROC_INFO		0

typedef std::map<std::string, std::string>	proc_info_t;
typedef std::map<int, proc_info_t>	proc_cpuinfo_t;

typedef struct {
	uint64_t VmSize;//���������ַ�ռ�Ĵ�С
	uint64_t VmRSS;	//ʵ��ʹ�������ڴ棨���������ռ�õ��ڴ棩
	uint64_t VmStk;	//�������û�̬��ջ�Ĵ�С
	uint64_t VmExe;	//������ӵ�еĿ�ִ�������ڴ�Ĵ�С,�����,����������ʹ�õĿ�
	uint64_t VmData;//�������ݶεĴ�С
} proc_pid_mem_t;

typedef struct {
	char cpu[8];
	uint32_t user;		//�û�̬CPUʱ��
	uint32_t nice;		//������CPUʱ��
	uint32_t system;	//����̬CPUʱ��
	uint32_t idle;		//��IO�ȴ����������ȴ�CPUʱ��
	uint32_t iowait;	//IO�ȴ�CPUʱ��
	uint32_t irq;		//Ӳ�ж�CPUʱ��
	uint32_t softirq;	//���ж�CPUʱ��
} proc_stat_cpu_t;

typedef struct {
	uint32_t size;		//total program size (same as VmSize in /proc/[pid]/status)
	uint32_t resident;	//resident set size (same as VmRSS in /proc/[pid]/status)
	uint32_t share;		//shared pages (from shared mappings)
	uint32_t text;		//text (code)
	uint32_t lib;		//library (unused in Linux 2.6)
	uint32_t data;		//data + stack
	uint32_t dt;		//dirty pages (unused in Linux 2.6)
} proc_pid_statm_t;

typedef struct {
	int32_t pid;		//prosses ID
	char comm[260];		//filename of the executable
	char state;			//one character in "RSDZTW":Read, Sleep, Disk sleep, Zombie, Traced or sTopped, W is paging.
	int32_t ppid;		//parent prosses ID
	int32_t pgrp;		//group ID
	int32_t session;	//session ID
	int32_t tty_nr;
	int32_t tpgid;
	uint32_t flags;
	uint32_t minflt;
	uint32_t cminflt;
	uint32_t majflt;
	uint32_t cmajflt;
	uint32_t utime;		//�û�̬CPUʱ��
	uint32_t stime;		//����̬CPUʱ��
	int32_t cutime;		//���߳��û�̬CPUʱ��
	int32_t cstime;		//���̺߳���̬CPUʱ��
	int32_t priority;
	int32_t nice;
	int32_t num_threads;
	int32_t itrealvalue;
	uint32_t starttime;
	uint32_t vsize;		//�����ڴ�
	int32_t rss;		//�����ڴ�
	uint32_t rlim;		//�����ڴ�����
	uint32_t startcode;
	uint32_t endcode;
	uint32_t startstack;
	uint32_t kstkesp;
	uint32_t kstkeip;
	uint32_t signal;
	uint32_t blocked;
	uint32_t sigignore;
	uint32_t sigcatch;
	uint32_t wchan;
	uint32_t nswap;
	uint32_t cnswap;
	int32_t exit_signal;
	int32_t processor;
	uint32_t rt_priority;
	uint32_t policy;
	uint64_t delayacct_blkio_ticks;
} proc_pid_stat_t;

void str_ltrim(char*str, char ch = ' ');
void str_rtrim(char*str, char ch = ' ');
void str_trim(char* str, char ch = ' ');

void get_proc_cpuinfo(proc_cpuinfo_t* info);
void get_proc_pid_status(uint32_t pid, proc_info_t* info);
void get_proc_stat_cpu(proc_stat_cpu_t* info);
void get_proc_pid_statm(uint32_t pid, proc_pid_statm_t* info);
void get_proc_pid_stat(uint32_t pid, proc_pid_stat_t* info);

uint32_t get_cpu_processor_num(void);
uint32_t get_total_cpu_used(void);
uint32_t get_pid_cpu_used(uint32_t pid);
float get_cpu_used_percent(uint32_t pid);

uint32_t get_total_mem_size(void);
proc_pid_mem_t get_pid_mem_used(uint32_t pid);
float get_mem_used_percent(uint32_t pid);

void ps(void);

#endif //PROC_INFO_H_