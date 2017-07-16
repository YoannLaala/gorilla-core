#ifndef _ALLOCATOR_SHARED_HPP_
#define _ALLOCATOR_SHARED_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "../../Memory/Memory.hpp"
//#include <Windows.h>

namespace Gorilla
{
	template <class TValue>
	class AllocatorShared
	{
	protected:
		AllocatorShared		();
		AllocatorShared		(TValue** _ppArray, uint32 _iSize, uint32 _iCapacity, EMemory::Type _eMemory = EMemory::Heap);
		~AllocatorShared	();

		inline void			Resize(uint32 _iSize);
		void				Resize(uint32 _iSize, const TValue* _pValue);

		void				Expand(uint32 _iSize);
		void				Expand(uint32 _iSize, const TValue* _pValue);

		void				ExpandTo(uint32 _iIndex);
		void				ExpandTo(uint32 _iIndex, const TValue* _pValue);

		void				Reserve(uint32 uiSize, uint32 _iCapacity, bool _bInitialize = true, bool _bCopy = false);

		TValue**			New(uint32 _iCapacity, bool _bInitialize);
		void				SetBuffer(TValue** _ppArray, uint32 _uiOldSize, uint32 _uiCapacity, EMemory::Type _eMemory = EMemory::Heap);

		void				operator=	(const AllocatorShared<TValue>& _kValue);

		inline uint32		GetSize() const { return m_uiSize; }
		inline uint32		GetCapacity() const { return m_uiSize + m_uiCapacityThreshold; }
		inline bool			IsEmpty() const { return m_uiSize == 0; }
		inline void			Clear() { m_uiSize = 0; }

	protected:
		TValue**		m_ppArray;
		volatile uint32	m_uiSize;
		volatile uint32	m_uiCapacityThreshold;
		EMemory::Type	m_eMemory;
	};

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <class TValue>
	AllocatorShared<TValue>::AllocatorShared()
		: m_ppArray(NULL)
		, m_uiSize(0)
		, m_uiCapacityThreshold(1)
		, m_eMemory(EMemory::Heap)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <class TValue>
	AllocatorShared<TValue>::AllocatorShared(TValue** _ppArray, uint32 _uiSize, uint32 _uiCapacity, EMemory::Type _eMemory /*= EMemory::Heap*/)
		: m_ppArray(_ppArray)
		, m_uiSize(_uiSize)
		, m_uiCapacityThreshold(_uiCapacity)
		, m_eMemory(_eMemory)
	{
		// Nothing to do
	}

	//!	@brief		Destructor	
	//!	@date		2015-04-04
	template <class TValue>
	AllocatorShared<TValue>::~AllocatorShared()
	{
		// Allow to delete or not the pointer dependning on the type of the allocation
		if (m_eMemory == EMemory::Heap)
		{
			SAFE_DELETE(m_ppArray);
		}

		m_uiSize = 0;
		m_uiCapacityThreshold = 0;
	}

	//!	@brief		Resize	
	//!	@details	Possibly lost all previous data, when allocating new array
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::Resize(uint32 _uiSize)
	{
		Reserve(_iSize, true, false);
		m_uiSize = _uiSize;
	}

	//!	@brief		Resize	
	//!	@details	Possibly lost all previous data, when allocating new array
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::Resize(uint32 _uiSize, const TValue* _pValue)
	{
		if (Reserve(_uiSize, true, false))
		{
			// Set new default Value
			memset(m_pArray, _pValue, _uiSize * sizeof(TValue*));
		}
		m_uiSize = _uiSize;
	}

	//!	@brief		Expand	
	//!	@details	Grow the array keeping the previous data
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::Expand(uint32 _uiSize)
	{
		uint32 uiNewSize = InterlockedAddAcquire(&m_uiSize,  _uiSize);
		Reserve(uiNewSize, true, true);
		m_uiSize = uiNewSize;
	}

	//!	@brief		Expand	
	//!	@details	Grow the array keeping the previous data
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::Expand(uint32 _uiSize, const TValue* _pValue)
	{
		uint32 uiNewSize = InterlockedAddAcquire(&m_iSize,  _uiSize);
		uint32 uiOldSize = uiNewSize - _uiSize;
		
		if (Reserve(uiNewSize, true, true))
		{
			// Set default Value
			memset(&m_ppArray[uiOldSize], _pValue, (_uiSize - iOldSize) * sizeof(TValue*));
		}
	}

	//!	@brief		ExpandTo	
	//!	@details	Expand the array to allow acces to the index
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::ExpandTo(uint32 _uiIndex)
	{
		if (Reserve(++_iIndex, true, true))
		{
			m_iSize = _iIndex;
		}
	}

	//!	@brief		Expand	
	//!	@details	Expand the array to allow acces to the index
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::ExpandTo(uint32 _uiIndex, const TValue* _pValue)
	{
		uint32 uiOldSize = InterlockedAddAcquire(&m_iSize, 0);

		if (Reserve(++_iIndex, true, true))
		{
			// Set default Value
			memset(&m_ppArray[uiOldSize], _pValue, (_iIndex - uiOldSize) * sizeof(TValue*));
			m_iSize = _iIndex;
		}
	}

	//!	@brief		New	
	//!	@details	Allocate an array of specific capacity
	//!	@date		2015-04-04
	template <class TValue>
	TValue** AllocatorShared<TValue>::New(uint32 _uiCapacity, bool _bInitialize)
	{
		TValue** ppNewArray = new TValue*[_uiCapacity];

		// Initialize elements
		if (_bInitialize)
		{
			memset(ppNewArray, 0, _uiCapacity * sizeof(TValue*));
		}

		return ppNewArray;
	}

	//!	@brief		Reserve	
	//!	@details	Allocate memory only if needed. VECTOR_BLOCK_ALLOCATION allow to reduce 
	//!				the number of allocation
	//!	@params		_iSize the size needed, maybe less than we already allocated
	//!	@params		_bCopyOldData copy the old data or not
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::Reserve(uint32 uiSize, uint32 _uiCapacity, bool _bInitialize /*= true*/, bool _bCopy /*= false*/)
	{
		// Allocate new Array
		TValue** ppNewArray = New(_uiCapacity, _bInitialize);

		// Copy old Element
		if (_bCopy && m_ppArray != NULL)
		{
			memcpy_s(ppNewArray, _uiCapacity * sizeof(TValue*), m_ppArray, (uiSize) * sizeof(TValue*));
		}

		// Allow to delete or not the pointer dependning on the type of the allocation
		SetBuffer(ppNewArray, uiSize, _uiCapacity, EMemory::Heap);
	}

	//!	@brief		SetPointer	
	//!	@details	Allow multiple Vector Allocation and then just affect the pointer
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::SetBuffer(TValue** _ppArray, uint32 _uiSize, uint32 _uiCapacity, EMemory::Type _eMemory /*= EMemory::Heap*/)
	{
		// Allow to delete or not the pointer dependning on the type of the allocation
		if (m_eMemory == EMemory::Heap)
		{
			SAFE_DELETE(m_ppArray);
		}
		m_eMemory = _eMemory;

		// Affect the new one
		m_ppArray = _ppArray;		
		InterlockedAddRelease((volatile LONG*)&m_uiCapacityThreshold, _uiCapacity - _uiSize);
	}

	//!	@brief		operator=
	//!	@details	Handle operator of other class that container (Force to copy data, not array pointer)
	//!	@date		2015-04-04
	template <class TValue>
	void AllocatorShared<TValue>::operator= (const AllocatorShared<TValue>& _kValue)
	{
		m_uiSize = _kValue.GetSize();
		Reserve(_kValue.GetSize(), true, false);
		memcpy_s(m_ppArray, m_uiSize * sizeof(TValue*), &_kValue[0], m_uiSize * sizeof(TValue*));
	}
}

#endif