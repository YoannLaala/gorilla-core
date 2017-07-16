/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Container/Node.hpp>
#include <Core/Container/Dictionary.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla 
{
	const Node::Id Node::Id::Start = Node::Id(0, 0);
	const Node::Id Node::Id::Invalid = Node::Id(0x0FFF, 0xFFFF);

	//!	@brief		GetName
	//!	@date		2015-04-04
	const char*	Node::GetName() const 
	{ 
		return m_pDictionary->GetDataName(m_pData); 
	}

	//!	@brief		Next
	//!	@date		2015-04-04
	void Node::Next() 
	{ 
		m_pData = m_pData->GetNextId() != Id::Invalid ? m_pDictionary->GetDataFromId(m_pData->GetNextId()) : nullptr; 
	}

	//!	@brief		GetNext
	//!	@date		2015-04-04
	Node Node::GetNext() const 
	{ 
		return Node(m_pDictionary, m_pData->GetNextId() != Id::Invalid ? m_pDictionary->GetDataFromId(m_pData->GetNextId()) : nullptr); 
	}

	//!	@brief		GetFirstChild
	//!	@date		2015-04-04
	Node Node::GetFirstChild() const 
	{ 
		Id kId = reinterpret_cast<TData<Data::Object>*>(m_pData)->FirstChildId; 
		return Node(m_pDictionary, kId != Id::Invalid ? m_pDictionary->GetDataFromId(kId) : nullptr); 
	}

	//!	@brief		GetLastChild
	//!	@date		2015-04-04
	Node Node::GetLastChild() const 
	{ 
		Id kId = reinterpret_cast<TData<Data::Object>*>(m_pData)->LastChildId; 
		return Node(m_pDictionary, kId != Id::Invalid ? m_pDictionary->GetDataFromId(kId) : nullptr); 
	}

	//!	@brief		Get
	//!	@date		2015-04-04
	Node Node::Get(const char* _szName) const
	{
		return Node(m_pDictionary, m_pDictionary->FindData(m_pData, _szName));
	}

	//!	@brief		Add
	//!	@date		2015-04-04
	Node Node::Add(const char* _szName /*= nullptr*/)
	{
		return m_pDictionary->AddNodeInternal(m_pData, _szName);
	}

	//!	@brief		Remove
	//!	@date		2015-04-04
	void Node::Remove(const char* _szName) 
	{ 
		m_pDictionary->RemoveNodeInternal(m_pData, _szName);  
	}

	//!	@brief		SetValue
	//!	@date		2015-04-04
	void Node::SetBool(bool _bValue)												{ m_pDictionary->SetValueInternal(m_pData, _bValue); }
	void Node::SetUInt8(uint8 _uiValue)												{ m_pDictionary->SetValueInternal(m_pData, _uiValue); }
	void Node::SetUInt16(uint16 _uiValue)											{ m_pDictionary->SetValueInternal(m_pData, _uiValue); }
	void Node::SetUInt32(uint32 _uiValue)											{ m_pDictionary->SetValueInternal(m_pData, _uiValue); }
	void Node::SetUInt64(uint64 _uiValue)											{ m_pDictionary->SetValueInternal(m_pData, _uiValue); }
	void Node::SetInt8(int8 _iValue)												{ m_pDictionary->SetValueInternal(m_pData, _iValue); }
	void Node::SetInt16(int16 _iValue)												{ m_pDictionary->SetValueInternal(m_pData, _iValue); }
	void Node::SetInt32(int32 _iValue)												{ m_pDictionary->SetValueInternal(m_pData, _iValue); }
	void Node::SetInt64(int64 _iValue)												{ m_pDictionary->SetValueInternal(m_pData, _iValue); }
	void Node::SetFloat32(float32 _fValue)											{ m_pDictionary->SetValueInternal(m_pData, _fValue); }
	void Node::SetFloat64(float64 _fValue)											{ m_pDictionary->SetValueInternal(m_pData, _fValue); }
	void Node::SetString(const char* _szValue)										{ m_pDictionary->SetStringInternal(m_pData, _szValue, (uint32)strlen(_szValue)); }
	void Node::SetString(const String& _sValue)										{ m_pDictionary->SetStringInternal(m_pData, _sValue); }

	//!	@brief		GetBuffer
	//!	@date		2015-04-04
	void Node::GetBufferBool(const bool** _ppBuffer, uint32& _uiCount) const		{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferUInt8(const uint8** _ppBuffer, uint32& _uiCount) const		{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferUInt16(const uint16** _ppBuffer, uint32& _uiCount) const	{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferUInt32(const uint32** _ppBuffer, uint32& _uiCount) const	{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferUInt64(const uint64** _ppBuffer, uint32& _uiCount) const	{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferInt8(const int8** _ppBuffer, uint32& _uiCount) const		{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferInt16(const int16** _ppBuffer, uint32& _uiCount) const		{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferInt32(const int32** _ppBuffer, uint32& _uiCount) const		{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferInt64(const int64** _ppBuffer, uint32& _uiCount) const		{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferFloat32(const float32** _ppBuffer, uint32& _uiCount) const	{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferFloat64(const float64** _ppBuffer, uint32& _uiCount) const	{ m_pDictionary->GetBufferInternalGENERIC(m_pData, _ppBuffer, _uiCount); }
	void Node::GetBufferString(Vector<String>& _vValue) const						
	{ 
		const uint8* pBuffer; uint32 uiLength;
		m_pDictionary->GetBufferInternalGENERIC(m_pData, &pBuffer, uiLength); 

		_vValue.Clear();
		if(uiLength)
		{
			uint32 uiOffset = 0;
			while(uiOffset < uiLength)
			{
				String& sValue = _vValue.Add();
				sValue = reinterpret_cast<const char*>(&pBuffer[uiOffset]);
				uiOffset += sValue.GetLength() + 1;
			}
		}
	}

	//!	@brief		SetBuffer
	//!	@date		2015-04-04
	void Node::SetBufferBool(const bool* _pBuffer, uint32 _uiCount)					{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferUInt8(const uint8* _pBuffer, uint32 _uiCount)				{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferUInt16(const uint16* _pBuffer, uint32 _uiCount)				{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferUInt32(const uint32* _pBuffer, uint32 _uiCount)				{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferUInt64(const uint64* _pBuffer, uint32 _uiCount)				{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferInt8(const int8* _pBuffer, uint32 _uiCount)					{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferInt16(const int16* _pBuffer, uint32 _uiCount)				{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferInt32(const int32* _pBuffer, uint32 _uiCount)				{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferInt64(const int64* _pBuffer, uint32 _uiCount)				{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferFloat32(const float32* _pBuffer, uint32 _uiCount)			{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferFloat64(const float64* _pBuffer, uint32 _uiCount)			{ m_pDictionary->SetBufferInternal(m_pData, _pBuffer, _uiCount); }
	void Node::SetBufferString(const Vector<String>& _vValue)						{ m_pDictionary->SetBufferInternal(m_pData, &_vValue[0], _vValue.GetSize()); }

	// operator
	Node Node::operator[](const char* _szName) const { return m_pDictionary->GetOrCreateNodeInternal(m_pData, _szName); }
}
