/*
 * Log_Manager.h
 *
 *  Created on: Nov 9, 2016
 *      Author: zhangyalei
 */

#ifndef LOG_MANAGER_H_
#define LOG_MANAGER_H_

#include <unordered_set>
#include <unordered_map>
#include "Bit_Buffer.h"
#include "Buffer_List.h"
#include "Object_Pool.h"
#include "Thread.h"
#include "Node_Define.h"

enum DB_MESSAGE_CMD {
	SYNC_NODE_INFO = 2,
	SYNC_SAVE_DB_DATA = 251,
};

class Log_Manager: public Thread {
	typedef Buffer_List<Mutex_Lock> Data_List;
	typedef std::unordered_set<int> Int_Set;
	typedef std::vector<int> Int_Vec;
public:
	static Log_Manager *instance(void);

	void init(const Node_Info &node_info) {
		node_info_ = node_info;
		log_node_idx_ = node_info_.node_id;
	}

	virtual void run_handler(void);
	virtual int process_list(void);

	inline void push_buffer(Byte_Buffer *buffer) {
		notify_lock_.lock();
		buffer_list_.push_back(buffer);
		notify_lock_.signal();
		notify_lock_.unlock();
	}

	void add_log_cid(int cid) { 
		log_cid_list_.push_back(cid); 
		log_connector_size_++;
	}
	int save_db_data(Bit_Buffer &buffer);

private:
	Log_Manager(void);
	virtual ~Log_Manager(void);
	Log_Manager(const Log_Manager &);
	const Log_Manager &operator=(const Log_Manager &);

private:
	static Log_Manager *instance_;

	Data_List buffer_list_;	//消息列表
	Node_Info node_info_;	//节点信息
	int log_node_idx_;		//log链接器id索引
	int log_connector_size_;//log链接器数量
	Int_Vec log_cid_list_;	//log链接器cid列表
	Int_Set log_fork_list_;	//log进程启动列表
};

#define LOG_MANAGER Log_Manager::instance()

#endif /* LOG_MANAGER_H_ */
