#ifndef _DICTIONARY_STREAM_JSON_HPP_
#define _DICTIONARY_STREAM_JSON_HPP_

#include <Core/Stream/Dictionary/DictionaryStream.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{	
	class DictionaryStreamJson : public DictionaryStream
	{
	public:
		virtual bool Read(StreamReader* _pReader, Node* _pNode) override;
		virtual bool Write(StreamWriter* _pWriter, Node* _pNode) override;

	private:
		void ReadNode(Node _kParent, const char* _szName, void* _pValue);
		void WriteNode(Node _kNode, void* pWriter);
	};
}

#endif