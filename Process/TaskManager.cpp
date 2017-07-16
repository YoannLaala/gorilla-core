///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "TaskManager.hpp"
//#include <thread>
//
///******************************************************************************
//**	Class Definition
//******************************************************************************/
//namespace Common
//{
//	/******************************************************************************
//	**	Base Task
//	******************************************************************************/
//	Task::Task()
//	{
//		m_bReady = true;
//	}
//
//	Task::~Task()
//	{
//	}
//
//	/******************************************************************************
//	**	TaskList handeling the execution of taks on a specific thread
//	******************************************************************************/
//	TaskList::TaskList()
//	{
//		m_bRunning = false;
//	}
//
//	void TaskList::Execute()
//	{
//		m_kMutex.lock();
//		bool bClearTask = true;
//		for(uint32 uiTask = 0; uiTask < m_vecTask.GetSize(); ++uiTask)
//		{
//			// Execute a task when ready
//			Task* pTask = m_vecTask[uiTask];
//			if(pTask)
//			{
//				if(!pTask->IsReady())
//				{
//					bClearTask = false;
//					continue;
//				}
//
//				pTask->Execute();
//				SAFE_DELETE(pTask);
//			}
//		}
//		// Clear VecTask only when all task are done
//		if(bClearTask)
//		{
//			m_vecTask.Clear();
//		}
//		m_kMutex.unlock();
//	}
//
//	void TaskList::Stop()
//	{
//		m_bRunning = false;
//		Execute();	// Finalize the last Task
//	}
//
//	TaskList::~TaskList()
//	{
//		// Clean All Task
//		for(uint32 uiTask = 0; uiTask < m_vecTask.GetSize(); ++uiTask)
//		{
//			Task* pTask = m_vecTask[uiTask];
//			SAFE_DELETE(pTask)
//		}
//		m_vecTask.Clear();
//	}
//
//	/******************************************************************************
//	**	TaskManager feeding properly TaskList
//	******************************************************************************/
//	void RunTaskList(TaskList* _pTaskList)
//	{
//		// Process all Task available
//		while(_pTaskList->IsRunning())
//		{
//			_pTaskList->Execute();
//
//			// Avoid overload on the CPU
//			std::this_thread::sleep_for(std::chrono::nanoseconds(1));
//		}
//	}
//
//	TaskManager::TaskManager()
//	{
//		m_uiCursorThread = 0;
//		m_vecThread.Resize(std::thread::hardware_concurrency());
//		m_vecTaskList.Resize(m_vecThread.GetSize());
//		for(uint32 uiThread = 0; uiThread < m_vecThread.GetSize(); ++uiThread)
//		{
//			// Create a TaskManager and start it
//			TaskList* pTaskList = new TaskList();
//			pTaskList->Run();
//			m_vecTaskList[uiThread] = pTaskList;
//
//			// Affect a Task Manager per thread
//			m_vecThread[uiThread] = new std::thread(RunTaskList, pTaskList);
//		}
//	}
//
//	void TaskManager::Join()
//	{
//		for(uint32 uiThread = 0; uiThread < m_vecThread.GetSize(); ++uiThread)
//		{
//			m_vecTaskList[uiThread]->Stop();
//			m_vecThread[uiThread]->join();
//		}
//	}
//
//	TaskManager::~TaskManager()
//	{
//		// Clean TaskManager & Thread
//		for(uint32 uiThread = 0; uiThread < m_vecThread.GetSize(); ++uiThread)
//		{
//			TaskList* pTaskList = m_vecTaskList[uiThread];
//			pTaskList->Stop();
//
//			std::thread* pThread = m_vecThread[uiThread];
//			pThread->join();
//
//			SAFE_DELETE(pTaskList)
//			SAFE_DELETE(pThread)
//		}
//		m_vecTaskList.Clear();
//		m_vecThread.Clear();		
//	}
//}