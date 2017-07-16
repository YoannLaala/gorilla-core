#ifndef _QUEUE_SHARED_HPP_
#define _QUEUE_SHARED_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "AllocatorShared.hpp"

/******************************************************************************
**	Defines
******************************************************************************/
#define QUEUE_SHARED_BLOCK_ALLOCATION		4

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	template <class TValue>
	class QueueShared : public AllocatorShared<TValue>
	{
	public:
		QueueShared	();
		QueueShared	(uint32 _uiSize);
		~QueueShared();

		void			Push(TValue* _pValue);
		TValue*			Pop();

		// Exposed from Allocator
		using AllocatorShared::GetSize;
		using AllocatorShared::GetCapacity;
		using AllocatorShared::IsEmpty;
		using AllocatorShared::Clear;
	};

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <class TValue>
	QueueShared<TValue>::QueueShared()
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@details	Specify a size at creation
	//!	@date		2015-04-04
	template <class TValue>
	QueueShared<TValue>::QueueShared(uint32 _uiSize)
	{
		Resize(_iSize);
	}

	//!	@brief		Destructor	
	//!	@date		2015-04-04
	template <class TValue>
	QueueShared<TValue>::~QueueShared()
	{
		// Nothing to do
	}

	//!	@brief		Push	
	//!	@details	Reserve memory if needed
	//!	@return		Last element created
	//!	@date		2015-04-04
	template <class TValue>
	void QueueShared<TValue>::Push(TValue* _pValue)
	{
		uint32 uiNewSize = InterlockedIncrementNoFence(&m_uiSize);
		
		while(!InterlockedAddAcquire((volatile LONG*)&m_uiCapacityThreshold, 0)) { }
		if (InterlockedDecrementAcquire(&m_uiCapacityThreshold) == 0)
		{
			uint32 uiOldSize = uiNewSize - 1;
			uint32 uiNewCapacity = uiNewSize * QUEUE_SHARED_BLOCK_ALLOCATION;
			AllocatorShared<TValue>::Reserve(uiOldSize, uiNewCapacity, false, true);
		}

		m_ppArray[--uiNewSize] = _pValue;
	}

	//!	@brief		Pop	
	//!	@details	Reserve memory if needed
	//!	@return		Last element created
	//!	@date		2015-04-04
	template <class TValue>
	TValue* QueueShared<TValue>::Pop()
	{
		while(!InterlockedAddAcquire((volatile LONG*)&m_uiCapacityThreshold, 0)) { }
		InterlockedIncrement(&m_uiCapacityThreshold);
		uint32 uiLastIndex = InterlockedDecrement(&m_uiSize);
		
		return m_ppArray[uiLastIndex];
	}
}

#undef QUEUE_SHARED_BLOCK_ALLOCATION

#endif