/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Container/Dictionary.hpp>
#include <Core/Log/LogManager.hpp>
#include <string>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla 
{
	//!	@brief		Cosntructor
	//!	@date		2015-04-04
	Dictionary::Dictionary(uint8 _eFlag /*= 0 */)
		: Node(this, nullptr)
		, m_pBuffer(nullptr)
		, m_uiOffset(DICTIONARY_PAGE_SIZE)
		, m_eFlag(_eFlag)
	{
		 // Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	Dictionary::~Dictionary()
	{
		const uint32 uiAllocationCount = m_vAllocation.GetSize();
		for(uint32 uiAllocation = 0; uiAllocation < uiAllocationCount; ++uiAllocation)
		{
			delete m_vAllocation[uiAllocation];
		}
		m_vAllocation.Clear();
	}

	//!	@brief		AddNodeInternal
	//!	@date		2015-04-04
	Node Dictionary::AddNodeInternal(Data* _pParent, const char* _szName) 
	{ 
		TData<Data::Object>* pTData = reinterpret_cast<TData<Data::Object>*>(CreateData(_pParent, Data::Object, _szName, sizeof(TData<Data::Object>))); 
		pTData->FirstChildId = Id::Invalid; 
		pTData->LastChildId = Id::Invalid; 
		return Node(this, pTData); 
	}

	//-----------------------------------------------------------------------------
	//! @brief		GetNodeInternal
	//! @author		YLA 
	//-----------------------------------------------------------------------------
	Node Dictionary::GetNodeInternal(Node::Data* _pParent, const char* _szName) const
	{ 
		Node::Data* pData = FindData(_pParent, _szName);
		return Node(const_cast<Dictionary*>(this), pData);
	}

	//-----------------------------------------------------------------------------
	//! @brief		GetOrCreateNodeInternal
	//! @author		YLA 
	//-----------------------------------------------------------------------------
	Node Dictionary::GetOrCreateNodeInternal(Node::Data* _pParent, const char* _szName)
	{ 
		Node::Data* pData = _szName ? FindData(_pParent, _szName) : nullptr;
		if(!pData) return AddNodeInternal(_pParent, _szName);
		return Node(const_cast<Dictionary*>(this), pData);
	}

	//-----------------------------------------------------------------------------
	//! @brief		RemoveNodeInternal
	//! @author		YLA 
	//-----------------------------------------------------------------------------
	void Dictionary::RemoveNodeInternal(Data* _pData, Data* _pParent)
	{
		TData<Data::Object>* pDataParent = reinterpret_cast<TData<Data::Object>*>(_pParent);
		Data* pPreviousData = _pParent;
		Id kId = pDataParent->FirstChildId;
		Id kPreviousId = Id::Invalid;
		while(kId != Id::Invalid)
		{
			// Remove Link
			Data* pCurrentData = GetDataFromId(kId);
			if(pCurrentData == _pData)
			{
				// Handle the remove of the first one
				if(pPreviousData == pDataParent)
				{
					pDataParent->FirstChildId = _pData->GetNextId();
					// It may also be the last one
					if(pDataParent->FirstChildId == Id::Invalid)
					{
						pDataParent->LastChildId = Id::Invalid;
					}
				}
				else
				{
					// Handle the remove of the last one
					if(_pData->GetNextId() == Id::Invalid)
					{
						pDataParent->LastChildId = kPreviousId;
					}
					pPreviousData->SetNextId(_pData->GetNextId());
				}
				
				return;
			}		

			pPreviousData = pCurrentData;
			kPreviousId = kId;
			kId = pCurrentData->GetNextId();
		}
	}

	//-----------------------------------------------------------------------------
	//! @brief		FindData
	//! @author		YLA 
	//-----------------------------------------------------------------------------
	Node::Data* Dictionary::FindData(Node::Data* _pParent, const char* _szName) const
	{	
		Id kId = FindId(_pParent, _szName);
		if(kId != Id::Invalid)
		{
			return GetDataFromId(kId);
		}
		
		return nullptr;
	}

	//-----------------------------------------------------------------------------
	//! @brief		FindId
	//! @author		YLA 
	//-----------------------------------------------------------------------------
	Dictionary::Id Dictionary::FindId(Node::Data* _pParent, const char* _szName) const
	{	
		TData<Data::Object>* pDataParent = reinterpret_cast<TData<Data::Object>*>(_pParent);
		if(pDataParent && pDataParent->FirstChildId != Id::Invalid)
		{
			Id kId = pDataParent->FirstChildId;
			uint32 uiHash = Hash::Generate(_szName);
			while(kId != Id::Invalid)
			{
				Data* pData = GetDataFromId(kId);
				if(pData->Hash == uiHash)
				{
					return kId;
				}		

				kId = pData->GetNextId();
			}
		}

		return Id::Invalid;
	}

	//-----------------------------------------------------------------------------
	//! @brief		CreateData
	//! @author		YLA 
	//-----------------------------------------------------------------------------
	Node::Data* Dictionary::CreateData(Data* _pParent, Data::Type _eData, const char* _szName, uint32 _uiSize)
	{
		Data* pData = AllocateData(_eData, _uiSize);

		// Update links
		if(!_pParent) _pParent = m_pData;	// Root		
		TData<Data::Object>* pTDataParent = reinterpret_cast<TData<Data::Object>*>(_pParent);				
		if(pTDataParent->FirstChildId == Id::Invalid)
		{
			pTDataParent->FirstChildId = m_kLastId;
		}
		else
		{
			Data* pDataChild = GetDataFromId(pTDataParent->LastChildId);
			pDataChild->SetNextId(m_kLastId);
		}
		pTDataParent->LastChildId = m_kLastId;
		pData->SetNextId(Id::Invalid);

		// Handle Name
		if(_szName)
		{
			pData->Hash = Hash::Generate(_szName);
			uint32 uiStringOffset = m_mHash.Get(pData->Hash, (uint32)-1);
			if(uiStringOffset == (uint32)-1)
			{
				// Check Name length
				uint32 uiNameLength = static_cast<uint32>(strlen(_szName));

				// Copy the name
				uiStringOffset = m_vName.GetSize();
				const uint32 uiNewSize = uiStringOffset + uiNameLength;
				if(m_vName.GetCapacity() <= uiNewSize) m_vName.Reserve(uiNewSize << 1);

				uint32 uiByteCount = uiNameLength + 1;
				m_vName.Expand(uiByteCount);
				char* pString = &m_vName[uiStringOffset];
				strcpy_s(pString, uiByteCount, _szName);

				// Add the Hash
				m_mHash.Add(pData->Hash, uiStringOffset);
			}
		}
		else
		{
			pTDataParent->Format = Data::Array;
		}

		return pData;
	}

	//!	@brief		AllocateData
	//!	@date		2015-04-04
	Node::Data* Dictionary::AllocateData(Node::Data::Type _eData, uint32 _uiSize)
	{
		m_kLastId.PageOffset = static_cast<uint16>(m_uiOffset);

		// Create new pool if necessary
		m_uiOffset += _uiSize;
		if(m_uiOffset >= DICTIONARY_PAGE_SIZE)
		{
			uint32 uiPageToAdd = (_uiSize / DICTIONARY_PAGE_SIZE) + 1;
			AddPage(uiPageToAdd);
			m_uiOffset += _uiSize;
		}
		
		// Fill the new member
		Data* pData = reinterpret_cast<Data*>(m_pBuffer);
		pData->Format = _eData;
		pData->Hash = (uint32)-1;
		m_pBuffer += _uiSize;

		return pData;
	}

	//!	@brief		ReallocateData
	//!	@date		2015-04-04
	bool Dictionary::ReallocateData(Data* _pData, uint32 _uiNewSize) 
	{
		uint32 uiOldSize = Helper::SizeOf(_pData);
		if(_uiNewSize <= uiOldSize)
		{
			return true;
		}
		else
		{
			// Check if there is enough space in current page (and if this is the last node)
			uint32 uiByteLeft = DICTIONARY_PAGE_SIZE - _uiNewSize;
			if(uiByteLeft >= _uiNewSize && m_kLastId.PageOffset == m_uiOffset - uiOldSize)
			{
				m_pBuffer = reinterpret_cast<uint8*>(_pData) + _uiNewSize;
				m_uiOffset = m_kLastId.PageOffset + _uiNewSize;
				return true;
			}
		}

		return false;
	}

	//!	@brief		AddPage
	//!	@date		2015-04-04
	void Dictionary::AddPage(uint32 _uiCount) 
	{ 	
		const uint32 uiOldSize = m_vPage.GetSize();
		const uint32 uiNewSize = uiOldSize + _uiCount;
		ASSERT(uiNewSize < (1 << 12), "[Dictionary] Total size limited to 256Mb");

		// Reset offset
		m_kLastId.PageIndex = static_cast<uint16>(uiOldSize);
		m_kLastId.PageOffset = 0;
		m_uiOffset = 0;

		// Allocate Memory if needed
		if(m_vPage.GetCapacity() < uiNewSize)
		{
			uint32 uiPageToAllocate = (_uiCount > 1) ? uiNewSize - uiOldSize : DICTIONARY_PAGE_FACTOR;
			AllocatePage(uiPageToAllocate);
		}
		m_vPage.Resize(uiNewSize);
		m_pBuffer = m_vPage[uiOldSize];

		// Create Root Node on the first page
		// We do it in the AddPage to avoid a page allocation directly in constructor
		if(!m_pData)
		{
			TData<Data::Object>* pData = reinterpret_cast<TData<Data::Object>*>(AllocateData(Data::Object, sizeof(TData<Data::Object>)));
			pData->FirstChildId = Id::Invalid;
			pData->LastChildId = Id::Invalid;
			pData->SetNextId(Id::Invalid);
			m_pData = pData;
			m_kLastId.PageOffset = sizeof(TData<Data::Object>);
			m_uiOffset = sizeof(TData<Data::Object>);
		}
	}

	//!	@brief		AllocatePage
	//!	@date		2015-04-04
	void Dictionary::AllocatePage(uint32 _uiCount) 
	{ 	
		// Allocate Memory;
		uint8* pAllocation = new uint8[_uiCount * DICTIONARY_PAGE_SIZE];
		m_vAllocation.Add(pAllocation);

		// Construct page
		const uint32 uiOldSize = m_vPage.GetSize();
		const uint32 uiNewSize = uiOldSize + _uiCount;

		uint8* pPage = pAllocation;
		m_vPage.Reserve(uiNewSize);
		for(uint32 uiPage = uiOldSize; uiPage < uiNewSize; ++uiPage)
		{
			m_vPage[uiPage] = pPage;
			pPage += DICTIONARY_PAGE_SIZE;
		}
	}

	//!	@brief		Clear
	//!	@date		2015-04-04
	void Dictionary::Clear() 
	{ 	
		m_vPage.Clear();
		m_vName.Clear();
		m_mHash.Clear();
		
		// Reset Root & Offsets
		m_pData = nullptr;
		m_kLastId = Id::Start;
		m_uiOffset = m_vPage.GetSize() == 0 ? DICTIONARY_PAGE_SIZE : 0;
	}
}
