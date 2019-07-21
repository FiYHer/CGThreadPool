#include "CGThreadPool.h"

cgtp::CGThreadPool::CGThreadPool(int nThreadCount):m_nThreadCount(nThreadCount)
{
	m_nThreadRun = 0;
	m_bExit = true;
	m_hEvent = CreateEventA(0, false, false, 0);
	CreateAllThread();
}

cgtp::CGThreadPool::~CGThreadPool()
{
	DeleteAllThread();
}

void cgtp::CGThreadPool::CreateAllThread()
{
	m_bExit = false;
	for (int nIndex = 0; nIndex < m_nThreadCount; nIndex++)
	{
		std::thread cThread(_ThreadFunc,this);
		cThread.detach();
	}
}

void cgtp::CGThreadPool::DeleteAllThread()
{
	m_bExit = true;
	for (int nIndex = 0; nIndex < m_nThreadCount; nIndex++)
		SetEvent(m_hEvent);
}

void cgtp::CGThreadPool::PushThreadQueueBack(ThreadInfo stInfo)
{
	m_cMuter.lock();
	m_cThreadQueue.emplace(stInfo);
	m_cMuter.unlock();
}

cgtp::ThreadInfo cgtp::CGThreadPool::PopThreadQueueFront()
{
	m_cMuter.lock();
	ThreadInfo stInfo = m_cThreadQueue.front();
	m_cThreadQueue.pop();
	m_cMuter.unlock();
	return stInfo;
}

void cgtp::CGThreadPool::_ThreadFunc(CGThreadPool* pThis)
{
	ThreadInfo stInfo;
	while (1)
	{
		WaitForSingleObject(pThis->m_hEvent, INFINITE);
		if (pThis->m_bExit)
			return;
		//如果线程队列里面有等待线程
		while (!pThis->m_cThreadQueue.empty())
		{
			stInfo = pThis->PopThreadQueueFront();
			InterlockedIncrement((long*)&pThis->m_nThreadRun);
			if (stInfo.lpfn)
				stInfo.lpfn(stInfo.pBuffer);
			if (stInfo.hEvent)
				SetEvent(stInfo.hEvent);
			InterlockedDecrement((long*)&pThis->m_nThreadRun);
		}
	}
}

void cgtp::CGThreadPool::Run_Thread_Func(
	void(*lpfn)(void*), void* pBuffer, int nCount, HANDLE hEvent)
{
	ThreadInfo stInfo;
	stInfo.hEvent = hEvent;
	stInfo.lpfn = lpfn;
	stInfo.pBuffer = pBuffer;
	for (int nIndex = 0; nIndex < nCount; nIndex++)
	{
		PushThreadQueueBack(stInfo);
		SetEvent(m_hEvent);
	}
}

int cgtp::CGThreadPool::GetCurrentRunCount()
{
	return m_nThreadRun;
}
