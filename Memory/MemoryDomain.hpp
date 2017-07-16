#ifndef _MEMORY_DOMAIN_HPP_
#define _MEMORY_DOMAIN_HPP_

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class MemoryDomain
	{
	public:
		MemoryDomain();
		~MemoryDomain();

		inline bool			IsInitialized	() const { return m_szName != 0; }
		inline const char*	GetName			() const { return m_szName; }
		inline void			SetName			(const char* _szName) { m_szName = _szName; }
		inline size_t		GetSize			() const { return m_uiSize; }
		inline void			Add				(size_t _uiSize) { m_uiSize += _uiSize; }
		inline void			Remove			(size_t _uiSize) { m_uiSize -= _uiSize; }

	private:
		const char* m_szName;
		size_t m_uiSize;
	};
}
#endif