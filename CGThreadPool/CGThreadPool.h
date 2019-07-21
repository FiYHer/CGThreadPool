#pragma once

#include <Windows.h>

#include <queue>
#include <mutex>
#include <thread>

namespace cgtp
{
	typedef struct _ThreadInfo
	{
		void(*lpfn)(void*);			//�̺߳���
		void* pBuffer;				//�̲߳���
		HANDLE hEvent;				//�߳��˳�ʱ֪ͨ���¼�
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
		std::mutex m_cMuter;							//ͬ���ṹ
		std::queue<ThreadInfo> m_cThreadQueue;			//�̶߳���
		const int m_nThreadCount;						//�̳߳����߳�����
		int m_nThreadRun;								//�����е��߳�����
		bool m_bExit;									//����߳��Ƿ����
		HANDLE m_hEvent;								//�̵߳ȴ��¼�
	private:
		void CreateAllThread();							//���������߳�
		void DeleteAllThread();							//ɾ�������߳�
		void PushThreadQueueBack(ThreadInfo stInfo);	//���߳���Ϣ�������β��
		ThreadInfo PopThreadQueueFront();				//���߳���Ϣ�Ӷ���ͷ��ȡ��
		static void _ThreadFunc(CGThreadPool* pThis);	//�̺߳���
	public:
		CGThreadPool(int nThreadCount = 4);
		~CGThreadPool();
	public:
		void Run_Thread_Func(void(*lpfn)(void*), void* pBuffer = 0,
			int nCount = 1, HANDLE hEvent = 0);	//�ŵ��̳߳�������
		int GetCurrentRunCount();
	};
}

