//#ifndef _QUEUE_HPP_
//#define _QUEUE_HPP_
//
///******************************************************************************
//**	Includes
//******************************************************************************/
//#include "Allocator.hpp"
//
///******************************************************************************
//**	Defines
//******************************************************************************/
//#define QUEUE_BLOCK_ALLOCATION		4
//
///******************************************************************************
//**	Class Declaration
//******************************************************************************/
//namespace Gorilla
//{
//	template <class TValue, class TUnit = unsigned int>
//	class Queue : public Allocator<TValue, TUnit>
//	{
//	public:
//		Queue	();
//		Queue	(TUnit _iSize);
//		~Queue	();
//
//		TValue&			Push();
//		TValue&			Push(const TValue& _kValue);
//		TValue&			Pop();
//
//		// Exposed from Allocator
//		using Allocator::GetSize;
//		using Allocator::GetCapacity;
//		using Allocator::IsEmpty;
//		using Allocator::Clear;
//	};
//
//	//!	@brief		Constructor
//	//!	@date		2015-04-04
//	template <class TValue, class TUnit>
//	Queue<TValue, TUnit>::Queue()
//	{
//		// Nothing to do
//	}
//
//	//!	@brief		Constructor
//	//!	@details	Specify a size at creation
//	//!	@date		2015-04-04
//	template <class TValue, class TUnit>
//	Queue<TValue, TUnit>::Queue(TUnit _iSize)
//	{
//		Resize(_iSize);
//	}
//
//	//!	@brief		Destructor	
//	//!	@date		2015-04-04
//	template <class TValue, class TUnit>
//	Queue<TValue, TUnit>::~Queue()
//	{
//		// Nothing to do
//	}
//
//	//!	@brief		Push	
//	//!	@details	Reserve memory if needed
//	//!	@return		Last element created
//	//!	@date		2015-04-04
//	template <class TValue, class TUnit>
//	TValue& Queue<TValue, TUnit>::Push()
//	{
//		const TUnit iOldSize = m_iSize;
//		const TUnit iNewSize = iOldSize + 1;
//		if (iNewSize > m_iCapacity)
//		{
//			Reserve(iNewSize * QUEUE_BLOCK_ALLOCATION, true, true);
//		}
//		++m_iSize;
//
//		return m_pArray[iOldSize];
//	}
//
//	//!	@brief		Push	
//	//!	@details	Specify a value to copy
//	//!	@params		_kValue the value to add to the vector
//	//!	@return		Element added
//	//!	@date		2015-04-04
//	template <class TValue, class TUnit>
//	TValue& Queue<TValue, TUnit>::Push(const TValue& _kValue)
//	{
//		TValue& kValue = Push();
//		kValue = _kValue;
//
//		return kValue;
//	}
//
//	//!	@brief		Pop	
//	//!	@return		Remove last element added
//	//!	@date		2015-11-29
//	template <class TValue, class TUnit>
//	TValue& Queue<TValue, TUnit>::Pop()
//	{
//		return m_pArray[--m_iSize];
//	}
//}
//
//#undef QUEUE_BLOCK_ALLOCATION
//
//#endif