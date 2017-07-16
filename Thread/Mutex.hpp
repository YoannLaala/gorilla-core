#ifndef _CORE_MUTEX_HPP_
#define _CORE_MUTEX_HPP_

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Mutex
	{
	public:
		Mutex	();
		~Mutex	();

		void Lock		();
		void Unlock		();

	public:
		void* m_pHandle;
	};
}

#endif