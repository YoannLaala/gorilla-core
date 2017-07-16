#ifndef _ARRAY_HPP_
#define _ARRAY_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Container/Container.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define ARRAY_DEFAULT_SIZE		64

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	template <typename TYPE, typename CAPACITY = ARRAY_DEFAULT_SIZE>
	class Array
	{
		Array	();
		~Array	();

		TYPE&					Add() { return m_pArray[m_uiSize++]; }
		TYPE&					Add(const TYPE& _kValue) { return Add() = _kValue; }
		void					Remove(const TYPE& _kValue);
		void					RemoveIndex(uint32 _uiIndex);

		void					operator=	(const Array<TYPE, CAPACITY>& _kArray);
		inline TYPE&			operator[]	(uint32 _iIndex) { return m_aStack[_iIndex]; }
		inline const TYPE&		operator[]	(uint32 _iIndex) const { return m_aStack[_iIndex]; }

		inline uint32			GetSize() const { return m_uiSize; }
		inline uint32			GetCapacity() const { return CAPACITY; }
		inline bool				IsEmpty() const { return m_uiSize == 0; }
		inline void				Clear() { m_uiSize = 0; }

		uint32					Find(const TYPE& _kValue) const;

	private:
		TYPE	m_aStack[CAPACITY];
		uint32	m_uiSize;
	};

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <typename TYPE, typename CAPACITY>
	Array<TYPE, CAPACITY>::Array()
		: m_uiSize(0)
	{
		// Nothing to do
	}

	//!	@brief		Destructor	
	//!	@date		2015-04-04
	template <typename TYPE, typename CAPACITY>
	Array<TYPE, CAPACITY>::~Array()
	{
		// Nothing to do
	}

	//!	@brief		Remove	
	//!	@date		2016-04-30
	template <typename TYPE, typename CAPACITY>
	void Array<TYPE, CAPACITY>::Remove(const TYPE& _kValue)
	{
		uint32 uiIndex = Find(_kValue);
		if(uiIndex != (uint32)-1)
		{
			RemoveIndex(uiIndex);
		}
	}

	//!	@brief		RemoveIndex	
	//!	@date		2016-04-30
	template <typename TYPE, typename CAPACITY>
	void Array<TYPE, CAPACITY>::RemoveIndex(uint32 _uiIndex)
	{
		if(_uiIndex < m_kAllocator.GetSize())
		{
			uint32 uiLastIndex = --m_uiSize;
			if(_uiIndex != uiLastIndex)
			{
				m_aStack[_uiIndex] = m_aStack[uiLastIndex];
			}
		}
	}

	//!	@brief		operator=
	//!	@details	Handle operator of other class that container (Force to copy data, not array pointer)
	//!	@date		2015-04-04
	template <typename TYPE, typename CAPACITY>
	void Array<TYPE, CAPACITY>::operator= (const Array<TYPE, CAPACITY>& _kValue)
	{
		m_uiSize = _kValue.GetSize();
		if(m_uiSize)
		{
			Container::Copy<TYPE>(m_aStack, _kValue.m_aStack, m_uiSize);
		}
	}

	//!	@brief		Find	
	//!	@date		2015-04-04
	template <typename TYPE, typename CAPACITY>
	uint32 Array<TYPE, CAPACITY>::Find(const TYPE& _kValue) const
	{
		for (uint32 uiElement = 0; uiElement < m_uiSize; ++uiElement)
		{
			if (m_pArray[uiElement] == _kValue)
			{
				return uiElement;
			}
		}

		return (uint32)-1;
	}
}

#endif