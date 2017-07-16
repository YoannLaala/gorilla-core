#ifndef _MEMORY_ALLOCATION_HPP_
#define _MEMORY_ALLOCATION_HPP_

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class MemoryAllocation
	{		
	public:
		MemoryAllocation();
		~MemoryAllocation();
		void Initialize(const size_t _uiDomain, const size_t _uiSize);

		inline size_t GetDomain	() const { return m_uiDomain; }
		inline size_t GetSize	() const { return m_uiSize; }

	private:
		size_t	m_uiDomain;
		size_t	m_uiSize;
	};
}

#endif