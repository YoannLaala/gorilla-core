/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Stream/Dictionary/DictionaryStream.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-08
	DictionaryStream::DictionaryStream()
		: m_pDictionary(nullptr)
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-08
	DictionaryStream::~DictionaryStream()
	{
		// Nothing to do
	}

	//!	@brief		Read
	//!	@date		2015-04-08
	bool DictionaryStream::Read(StreamReader* _pReader, Node* /*_pNode*/)
	{
		uint32 uiPageCount;
		_pReader->Read(uiPageCount);
		if(uiPageCount == 0) return true;

		// Allocate page
		m_pDictionary->AddPage(uiPageCount);
		_pReader->Read(m_pDictionary->m_uiOffset);

		// Read complete page
		uint8* pAllocation = m_pDictionary->m_vAllocation[0];
		const uint32 uiPageCompleteByte = (uiPageCount - 1) * DICTIONARY_PAGE_SIZE;
		if(uiPageCompleteByte)
		{
			_pReader->Read(pAllocation, uiPageCompleteByte);
		}
		_pReader->Read(&pAllocation[uiPageCompleteByte], m_pDictionary->m_uiOffset);

		// Read names
		uint32 uiCharacterCount;
		_pReader->Read(uiCharacterCount);
		m_pDictionary->m_vName.Resize(uiCharacterCount);
		_pReader->Read(&m_pDictionary->m_vName[0], uiCharacterCount);
		
		// Read key associated to names
		uint32 uiStringCount;
		_pReader->Read(uiStringCount);
		for(uint32 uiString = 0; uiString < uiStringCount; ++uiString)
		{
			uint32 uiKey, uiValue;
			_pReader->Read(uiKey);
			_pReader->Read(uiValue);
			m_pDictionary->m_mHash.Add(uiKey, uiValue);
		}

		return true;
	}

	//!	@brief		Write
	//!	@date		2015-04-08
	bool DictionaryStream::Write(StreamWriter* _pWriter, Node* /*_pNode*/)
	{
		const uint32 uiPageCount = m_pDictionary->m_vPage.GetSize();
		_pWriter->Write(uiPageCount);
		if(uiPageCount == 0) return true;

		_pWriter->Write(m_pDictionary->m_uiOffset);

		// Write complete page
		const uint32 uiPageCompleteCount = uiPageCount - 1;
		for(uint32 uiPage = 0; uiPage < uiPageCompleteCount; ++uiPage)
		{
			_pWriter->Write(m_pDictionary->m_vPage[uiPage], DICTIONARY_PAGE_SIZE);
		}
		_pWriter->Write(m_pDictionary->m_vPage[uiPageCompleteCount], m_pDictionary->m_uiOffset);

		// Write names
		const uint32 uiCharacterCount = m_pDictionary->m_vName.GetSize();
		_pWriter->Write(uiCharacterCount);
		_pWriter->Write(&m_pDictionary->m_vName[0], uiCharacterCount);
		
		// Write key associated to names
		const uint32 uiStringCount = m_pDictionary->m_mHash.GetSize();
		_pWriter->Write(uiStringCount);
		HashMap<uint32, uint32>::Iterator it = m_pDictionary->m_mHash.GetFirst();
		HashMap<uint32, uint32>::Iterator itEnd = m_pDictionary->m_mHash.GetLast();
		while(it != itEnd)
		{
			_pWriter->Write(it.GetKey());
			_pWriter->Write(it.GetValue());
			++it;
		}

		return true;
	}
}
