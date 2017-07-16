#ifndef _CORE_SINGLETON_HPP_
#define _CORE_SINGLETON_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "Memory/Memory.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	template <class T>
	class Singleton
	{
	public:
		static inline void	Create() { m_pInstance = new T(); }
		static inline void	Destroy() { SAFE_DELETE(m_pInstance); m_pInstance = NULL; }
		static inline T*	GetInstance() { return m_pInstance; }
		static inline bool	IsCreated() { return m_pInstance != NULL; }
		static inline void	SetInstance(T* _pInstance) { m_pInstance = _pInstance; }

	protected:
		Singleton(){}
		~Singleton(){}

	protected:
		static T* m_pInstance;
	};

	template <class T>
	T* Singleton<T>::m_pInstance = 0;
}

#endif