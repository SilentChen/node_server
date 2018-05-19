/*
 * Thread.h
 *
 *  Created on: Dec 16,2015
 *      Author: zhangyalei
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>
#include "Thread_Lock.h"

class Thread {
public:
	Thread(void) : tid(0) { }
	virtual ~Thread(void) { }

	static void *create_func(void *arg) {
		Thread *self = (Thread *)arg;
		pthread_cleanup_push(cleanup_func, arg);
		self->run_handler();
		pthread_cleanup_pop(1);
		return self;
	}

	static void cleanup_func(void *arg) {
		Thread *self = (Thread *)arg;
		self->exit_handler();
		return ;
	}

	virtual void run_handler(void) { }

	virtual void exit_handler(void) { }

	int thr_create(void) {
		return ::pthread_create(&tid, NULL, create_func, this);
	}

	int thr_cancel(void) {
		return ::pthread_cancel(tid);
	}

	int thr_join(void) {
		return ::pthread_join(tid, NULL);
	}

	int thr_cancel_join(void) {
		int ret;
		if ((ret = thr_cancel()) == 0) {
			ret = thr_join();
		} else {
			return -1;
		}
		return ret;
	}

	void thr_exit(void *rval_ptr) {
		::pthread_exit(rval_ptr);
	}

	pthread_t thread_id() {
		return tid;
	}

protected:
	Notify_Lock notify_lock_;		//条件变量通知锁

private:
	pthread_t tid;
};

#endif /* THREAD_H_ */
