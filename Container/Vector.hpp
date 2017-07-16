#ifndef _VECTOR_HPP_
#define _VECTOR_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Container/Container.hpp>
#include <type_traits>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	template <typename TYPE>
	class Vector
	{
	public:
		Vector	();
		Vector	(uint32 _uiSize);
		Vector  (const Vector<TYPE>& _vValue);
		Vector  (const Vector<TYPE>&& _vValue);
		~Vector	();

		TYPE&					Add();
		TYPE&					Add(const TYPE& _kValue);
		void					Add(const Vector<TYPE>& _vValue);

		void					Insert(uint32 _uiIndex, const TYPE& _kValue);

		void					Remove(const TYPE& _kValue);
		void					RemoveIndex(uint32 _uiIndex);

		inline void				Resize(uint32 _uiSize);
		void					Resize(uint32 _uiSize, const TYPE& _kValue);

		void					Expand(uint32 _uiSize);
		void					Expand(uint32 _uiSize, const TYPE& _kValue);

		void					ExpandTo(uint32 _iIndex);
		void					ExpandTo(uint32 _iIndex, const TYPE& _kValue);

		bool					Reserve(uint32 _uiCapacity, bool _bCopy = true);

		inline void				operator=	(const Vector<TYPE>& _vValue);
		inline void				operator=	(const Vector<TYPE>&& _vValue);
		inline TYPE&			operator[]	(uint32 _uiIndex) { return m_pArray[_uiIndex]; }
		inline const TYPE&		operator[]	(uint32 _uiIndex) const { return m_pArray[_uiIndex]; }

		inline TYPE&			Get			(uint32 _uiIndex) { return m_pArray[_uiIndex]; }
		inline const TYPE&		Get			(uint32 _uiIndex) const { return m_pArray[_uiIndex]; }

		inline uint32			GetSize() const { return m_uiSize; }
		inline uint32			GetCapacity() const { return m_uiCapacity; }
		inline bool				IsEmpty() const { return m_uiSize == 0; }
		inline void				Clear() { m_uiSize = 0; }

		uint32					Find(const TYPE& _kValue) const;
		void					Sort();

		void					SetBuffer(TYPE* _pArray, uint32 _uiSize, uint32 _uiCapacity, bool _bHeap);

	protected:
		TYPE*			m_pArray;
		uint32			m_uiCapacity;
		uint32			m_uiSize:31;	// Limit to 31bit to allow heap/stack
		uint32			m_bHeap:1;
	};

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <typename TYPE>
	Vector<TYPE>::Vector()
		: m_pArray(nullptr)
		, m_uiSize(0)
		, m_uiCapacity(0)
		, m_bHeap(true)
	{
		// Nothing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <typename TYPE>
	Vector<TYPE>::Vector(uint32 _uiSize)
		: Vector()
	{
		Resize(_uiSize);
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <typename TYPE>
	Vector<TYPE>::Vector(const Vector<TYPE>& _vValue)
		: Vector()
	{
		operator=(_vValue);
	}

	//!	@brief		Constructor
	//!	@date		2015-04-04
	template <typename TYPE>
	Vector<TYPE>::Vector(const Vector<TYPE>&& _vValue)
		: Vector()
	{
		operator=(std::move(_vValue));
	}

	//!	@brief		Destructor	
	//!	@date		2015-04-04
	template <typename TYPE>
	Vector<TYPE>::~Vector()
	{	
		SetBuffer(nullptr, 0, 0, true);
	}

	//!	@brief		Add	
	//!	@details	Reserve memory if needed
	//!	@return		Last element created
	//!	@date		2015-04-04
	template <typename TYPE>
	TYPE& Vector<TYPE>::Add()
	{
		const uint32 uiOldSize = m_uiSize;
		const uint32 uiNewSize = m_uiSize + 1;
		if (uiNewSize > m_uiCapacity)
		{
			Reserve(uiNewSize << 1, true);
		}
		++m_uiSize;

		return m_pArray[uiOldSize];
	}

	//!	@brief		Add	
	//!	@details	Specify a value to copy
	//!	@params		_kValue the value to add to the vector
	//!	@return		Element added
	//!	@date		2015-04-04
	template <typename TYPE>
	TYPE& Vector<TYPE>::Add(const TYPE& _kValue)
	{
		TYPE& kValue = Add();
		kValue = _kValue;

		return kValue;
	}

	//!	@brief		Add	
	//!	@details	Add a complete Vector to the current one
	//!	@params		_vValue the vector to copy
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::Add(const Vector<TYPE>& _vValue)
	{
		const uint32 uiOtherSize = _vValue.GetSize();
		const uint32 uiNewSize = m_uiSize + uiOtherSize;
		if (uiNewSize > m_uiCapacity)
		{
			Reserve(uiNewSize << 1, true);
		}

		// Set all the value
		for (uint32 uiElement = 0; uiElement < uiOtherSize; ++uiElement)
		{
			m_pArray[uiElement] = _vValue[uiElement];
		}
		m_uiSize = uiNewSize;
	}

	//!	@brief		Insert	
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::Insert(uint32 _uiIndex, const TYPE& _kValue)
	{
		// Last Element
		if(m_uiSize <= _uiIndex) ExpandTo(_uiIndex);
		else
		{
			Add();

			uint32 uiByteToMoveCount = (m_uiSize - 1) * sizeof(TYPE);
			if(uiByteToMoveCount)
			{
				memmove_s(&m_pArray[_uiIndex+1], uiByteToMoveCount, &m_pArray[_uiIndex], uiByteToMoveCount);
			}
		}
		m_pArray[_uiIndex] = _kValue;
	}

	//!	@brief		Remove	
	//!	@date		2016-04-30
	template <typename TYPE>
	void Vector<TYPE>::Remove(const TYPE& _kValue)
	{
		uint32 uiIndex = Find(_kValue);
		if(uiIndex != (uint32)-1)
		{
			RemoveIndex(uiIndex);
		}
	}

	//!	@brief		RemoveIndex	
	//!	@date		2016-04-30
	template <typename TYPE>
	void Vector<TYPE>::RemoveIndex(uint32 _uiIndex)
	{
		if(_uiIndex < m_uiSize)
		{
			uint32 uiLastIndex = --m_uiSize;
			if(_uiIndex != uiLastIndex)
			{
				m_pArray[_uiIndex] = m_pArray[uiLastIndex];
			}
		}
	}

	//!	@brief		Resize	
	//!	@details	Possibly lost all previous data, when allocating new array
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::Resize(uint32 _uiSize)
	{
		Reserve(_uiSize, false);
		m_uiSize = _uiSize;
	}

	//!	@brief		Resize	
	//!	@details	Possibly lost all previous data, when allocating new array
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::Resize(uint32 _uiSize, const TYPE& _kValue)
	{
		if (Reserve(_uiSize, false))
		{
			// Set new default Value
			for (uint32 iElement = 0; iElement < _uiSize; ++iElement)
			{
				m_pArray[iElement] = _kValue;
			}
		}
		m_uiSize = _uiSize;
	}

	//!	@brief		Expand	
	//!	@details	Grow the array keeping the previous data
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::Expand(uint32 _uiSize)
	{
		uint32 uiNewSize = m_uiSize + _uiSize;
		if (uiNewSize > m_uiCapacity) Reserve(uiNewSize << 1, true);
		m_uiSize = uiNewSize;
	}

	//!	@brief		Expand	
	//!	@details	Grow the array keeping the previous data
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::Expand(uint32 _uiSize, const TYPE& _kValue)
	{
		uint32 iOldSize = m_uiSize;
		uint32 iNewSize = m_uiSize + _uiSize;
		
		bool bGrowed = false;
		if (uiNewSize > m_uiCapacity) bGrowed = Reserve(iNewSize << 1, true, true);
		
		if (bGrowed)
		{
			// Set default Value
			for (uint32 iElement = iOldSize; iElement < _uiSize; ++iElement)
			{
				m_pArray[iElement] = _kValue;
			}
		}
	}

	//!	@brief		ExpandTo	
	//!	@details	Expand the array to allow acces to the index
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::ExpandTo(uint32 _iIndex)
	{
		if(m_uiSize <= _iIndex)
		{
			Reserve(++_iIndex, true);
			m_uiSize = _iIndex;
		}
	}

	//!	@brief		Expand	
	//!	@details	Expand the array to allow acces to the index
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::ExpandTo(uint32 _iIndex, const TYPE& _kValue)
	{
		// Set default Value
		if(m_uiSize <= _iIndex)
		{
			uint32 uiOldSize = m_uiSize;
			Reserve(++_iIndex, true);
			for (uint32 iElement = uiOldSize; iElement < _iIndex; ++iElement)
			{
				m_pArray[iElement] = _kValue;
			}
			m_uiSize = _iIndex;
		}
	}

	//!	@brief		Reserve	
	//!	@details	Allocate memory only if needed. VECTOR_BLOCK_ALLOCATION allow to reduce 
	//!				the number of allocation
	//!	@params		_uiSize the size needed, maybe less than we already allocated
	//!	@params		_bCopyOldData copy the old data or not
	//!	@date		2015-04-04
	template <typename TYPE>
	bool Vector<TYPE>::Reserve(uint32 _uiCapacity, bool _bCopy /*= true*/)
	{
		if (_uiCapacity > m_uiCapacity)
		{
			// Allocate new Array
			TYPE* pNewArray = new TYPE[_uiCapacity];

			// Copy old Element
			if (_bCopy && m_pArray != nullptr)
			{
				Container::Copy<TYPE>(pNewArray, m_pArray, m_uiSize);
			}

			SetBuffer(pNewArray, m_uiSize, _uiCapacity, true);
			return true;
		}

		return false;
	}

	//!	@brief		operator=
	//!	@details	Handle operator of other class that container (Force to copy data, not array pointer)
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::operator= (const Vector<TYPE>& _vValue)
	{
		m_uiSize = _vValue.GetSize();
		if(m_uiSize)
		{
			Reserve(_vValue.GetSize(), false);
			Container::Copy<TYPE>(m_pArray, _vValue.m_pArray, m_uiSize);
		}
	}

	//!	@brief		operator=
	//!	@details	Handle operator of other class that container (Force to copy data, not array pointer)
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::operator= (const Vector<TYPE>&& _vValue)
	{
		Clear();

		m_uiSize = _vValue.GetSize();
		if(m_uiSize)
		{
			m_uiCapacity = _vValue.GetCapacity();
			m_pArray = _vValue.m_pArray;
			m_bHeap = _vValue.m_bHeap;
			const_cast<Vector<TYPE>*>(&_vValue)->m_pArray = nullptr;
		}
	}

	//!	@brief		Find	
	//!	@date		2015-04-04
	template <typename TYPE>
	uint32 Vector<TYPE>::Find(const TYPE& _kValue) const
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

	//!	@brief		Compare
	//!	@details	qsort compare implementation between 2 elements
	//!	@date		2015-04-04
	template <typename TYPE>
	int Compare(const void* _pValueA, const void* _pValueB)
	{
		if (*(TYPE*)_pValueA < *(TYPE*)_pValueB)
		{
			return -1;
		}

		if (*(TYPE*)_pValueA > *(TYPE*)_pValueB)
		{
			return 1;
		}

		return 0;
	}

	//!	@brief		Sort	
	//!	@details	Based on qsort: elements need to implements "<" & ">" operators 
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::Sort()
	{
		std::qsort(m_pArray, m_uiSize, sizeof(TYPE), &Compare<TYPE>);
	}

	//!	@brief		Add	
	//!	@details	Specify a value to copy
	//!	@params		_kValue the value to add to the vector
	//!	@return		Element added
	//!	@date		2015-04-04
	template <typename TYPE>
	void Vector<TYPE>::SetBuffer(TYPE* _pArray, uint32 _uiSize, uint32 _uiCapacity, bool _bHeap)
	{
		// Delete heap array
		if(m_pArray && m_bHeap)
		{
			SAFE_DELETE_ARRAY(m_pArray);
		}

		m_pArray = _pArray;
		m_uiSize = _uiSize;
		m_uiCapacity = _uiCapacity;
		m_bHeap = _bHeap;
	}
}

#endif