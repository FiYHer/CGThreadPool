#pragma once

#include <Windows.h>

#include <queue>
#include <mutex>
#include <thread>

namespace cgtp
{
	typedef struct _ThreadInfo
	{
		void(*lpfn)(void*);			//线程函数
		void* pBuffer;				//线程参数
		HANDLE hEvent;				//线程退出时通知此事件
		_ThreadInfo operator=(_ThreadInfo stInfo)
		{
			lpfn = stInfo.lpfn;
			pBuffer = stInfo.pBuffer;
			hEvent = stInfo.hEvent;
			return *this;
		}
	}ThreadInfo,*PThreadInfo;

	class CGThreadPool
	{
	private:
		std::mutex m_cMuter;							//同步结构
		std::queue<ThreadInfo> m_cThreadQueue;			//线程队列
		const int m_nThreadCount;						//线程池中线程数量
		int m_nThreadRun;								//运行中的线程数量
		bool m_bExit;									//标记线程是否结束
		HANDLE m_hEvent;								//线程等待事件
	private:
		void CreateAllThread();							//创建所有线程
		void DeleteAllThread();							//删除所有线程
		void PushThreadQueueBack(ThreadInfo stInfo);	//将线程信息放入队列尾部
		ThreadInfo PopThreadQueueFront();				//将线程信息从队列头部取出
		static void _ThreadFunc(CGThreadPool* pThis);	//线程函数
	public:
		CGThreadPool(int nThreadCount = 4);
		~CGThreadPool();
	public:
		void Run_Thread_Func(void(*lpfn)(void*), void* pBuffer = 0,
			int nCount = 1, HANDLE hEvent = 0);	//放到线程池中运行
		int GetCurrentRunCount();
	};
}

