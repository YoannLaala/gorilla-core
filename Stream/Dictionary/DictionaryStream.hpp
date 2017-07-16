#ifndef _DICTIONARY_STREAM_HPP_
#define _DICTIONARY_STREAM_HPP_

#include <Core/Container/Dictionary.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{	
	class StreamReader;
	class StreamWriter;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{	
	class DictionaryStream
	{
		friend class Dictionary;

	public:
		DictionaryStream();
		~DictionaryStream();

		virtual bool Read(StreamReader* _pReader, Node* _pNode);
		virtual bool Write(StreamWriter* _pWriter, Node* _pNode);

	protected:
		template <typename T>
		inline T* AddBuffer(Node* _pParent, const char* _szName, uint32 _uiCount) { return m_pDictionary->AddBufferInternalGENERIC<T>(_pParent->m_pData, _szName, _uiCount); }

	private:
		inline void SetDictionary(Dictionary* _pDictionary) { m_pDictionary = _pDictionary; }

	private:
		Dictionary* m_pDictionary;
	};
}

#endif