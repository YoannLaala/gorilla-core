//#ifndef _TASK_MANAGER_HPP_
//#define _TASK_MANAGER_HPP_
//
///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "../Math.hpp"
//#include "../Container/Vector.hpp"
//#include <mutex>
//
///******************************************************************************
//**	Forward Declarations
//******************************************************************************/
//namespace std
//{
//	class thread;
//}
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace Common
//{
//	/******************************************************************************
//	**	Base Task
//	******************************************************************************/
//	class Task
//	{
//	public:
//		Task	();
//		~Task	();
//
//		virtual void Execute() = 0;
//
//		bool IsReady	() const { return m_bReady; }
//		void Start		() { m_bReady = true; }
//
//	private:
//		bool m_bReady;
//	};
//
//	/******************************************************************************
//	**	TaskList handeling the execution of taks on a specific thread
//	******************************************************************************/
//	class TaskList
//	{
//	public:
//		TaskList	();
//		~TaskList	();
//
//		void Execute();
//
//		// Add a new Task
//		template <class T>
//		T* Add(bool _bReady = false)
//		{
//			Task* pTask = new T();
//			pTask->SetReady(_bReady);
//
//			m_kMutex.lock();
//			m_vecTask.Add(pTask);
//			m_kMutex.unlock();
//
//			return (T*)pTask;
//		}
//
//		bool IsRunning	() const { return m_bRunning; }
//		void Run		() { m_bRunning = true; }
//		void Stop		();
//
//	private:
//		bool			m_bRunning;
//		Vector<Task*>	m_vecTask;
//		std::mutex		m_kMutex;
//	};
//
//	/******************************************************************************
//	**	TaskManager feeding properly TaskList
//	******************************************************************************/
//	class TaskManager
//	{
//	public:
//		TaskManager		();
//		~TaskManager	();
//		
//		template <class T>
//		T* AddTask(bool _bReady = false)
//		{
//			T* pTask = m_vecTaskList[m_uiCursorThread++]->Add<T>(_bReady);
//
//			// Reset Thread Cursor
//			if(m_uiCursorThread >= m_vecThread.GetSize())
//			{
//				m_uiCursorThread = 0;
//			}
//
//			return pTask;
//		}
//
//		void Join	();
//
//	private:
//		uint32					m_uiCursorThread;
//		Vector<std::thread*>	m_vecThread;
//		Vector<TaskList*>		m_vecTaskList;
//	};
//}
//
//#endif