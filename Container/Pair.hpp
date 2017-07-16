#ifndef _PAIR_HPP_
#define _PAIR_HPP_

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	template <class TKey, class TValue>
	class Pair
	{
	public:
		Pair();
		Pair(const TKey& _kKey, const TValue& _kValue);
		~Pair();

		void operator=(const Pair& kNode);

		inline const TKey&		GetKey() const { return m_kKey; }
		inline const TValue&	GetValue() const { return m_kValue; }

	protected:
		TKey m_kKey;
		TValue m_kValue;
	};

	/******************************************************************************
	**	Class Definition
	******************************************************************************/

	//!	@brief		Constructor	
	//!	@date		2015-09-19
	template <class TKey, class TValue>
	Pair<TKey, TValue>::Pair()
	{
		// Nothing to do
	}

	//!	@brief		Constructor	
	//!	@date		2015-09-19
	template <class TKey, class TValue>
	Pair<TKey, TValue>::Pair(const TKey& _kKey, const TValue& _kValue)
		: m_kKey(_kKey)
		, m_kValue(_kValue)
	{
		// Nothing to do
	}

	//!	@brief		Destructor	
	//!	@date		2015-09-19
	template <class TKey, class TValue>
	Pair<TKey, TValue>::~Pair()
	{
		// Nothing to do
	}

	//!	@brief		operator=	
	//!	@date		2015-09-19
	template <class TKey, class TValue>
	void Pair<TKey, TValue>::operator=(const Pair<TKey, TValue>& kPair)
	{
		m_kKey = kPair.m_kKey;
		m_kValue = kPair.m_kValue;
	}
}

#endif