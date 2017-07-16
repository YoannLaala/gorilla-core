#ifndef _DICTIONARY_HPP_
#define _DICTIONARY_HPP_

/******************************************************************************
**	Includes
*****************************************************************************/
#include <Core/Container/Node.hpp>
#include <Core/Container/Vector.hpp>
#include <Core/Container/HashMap.hpp>
#include <Core/Log/LogManager.hpp>
#include <Core/Stream/FileReader.hpp>
#include <Core/Stream/FileWriter.hpp>
#include <Core/Stream/Dictionary/DictionaryStream.hpp>

/******************************************************************************
**	Define
******************************************************************************/
#define DICTIONARY_PAGE_SIZE	65536
#define DICTIONARY_PAGE_FACTOR	1

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla
{
	class StreamReader;
	class StreamWriter;
	class DictionaryStream;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{	
	class Dictionary : public Node
	{
		friend class Node;
		friend class DictionaryStream;

	public:
		struct EFlag
		{
			enum Type : uint8
			{
				CaseInsensitive		= 1 << 0,
			};
		};

		enum class Format : uint8
		{
			Binary = 0,
			Json,
		};

		Dictionary(uint8 _eFlag = 0);
		~Dictionary();		
		void Clear();		

	public:
		template <typename T>
		bool				Read(const char* szFilePath, Dictionary::Node* _pRoot = nullptr);
		template <typename T>
		bool				Read(StreamReader* _pReader, Dictionary::Node* _pRoot = nullptr);
		template <typename T>
		bool				Write(const char* szFilePath, Dictionary::Node* _pRoot = nullptr);
		template <typename T>
		bool				Write(StreamWriter* _pWriter, Dictionary::Node* _pRoot = nullptr);

	protected:
		// Node
		Node				AddNodeInternal(Data* _pParent, const char* _szName);
		Node				GetNodeInternal(Data* _pParent, const char* _szName) const;
		Node				GetOrCreateNodeInternal(Data* _pParent, const char* _szName);
		inline void			RemoveNodeInternal(Data* _pParent, const char* _szName) { RemoveNodeInternal(_pParent, FindData(_pParent, _szName)); }
		void				RemoveNodeInternal(Data* _pData, Data* _pParent);

		// Set
		template <typename T>
		void GetBufferInternalGENERIC(Data* _pData, const T** _ppBuffer, uint32& _uiCount);

		template <typename T>
		T* AddBufferInternalGENERIC(Data* _pParent, const char*_szName, uint32 _uiCount);

		template <typename T> void	SetValueInternal(Data* _pData, T _kValue);
		inline void					SetStringInternal(Data* _pData, const char* _szValue, uint32 _uiLength);
		inline void					SetStringInternal(Data* _pData, const String& _sValue) { SetStringInternal(_pData, _sValue.GetBuffer(), _sValue.GetLength()); }

		template <typename T> void	SetBufferInternal(Data* _pData, const T* _pBuffer, uint32 _uiCount);
		template <> void			SetBufferInternal(Data* _pData, const String* _pBuffer, uint32 _uiCount);

	private:
		Data*				FindData(Data* _pParent, const char* _szName) const;
		Id					FindId(Data* _pParent, const char* _szName) const;
		inline Data*		GetDataFromId(Id _kId) const { return reinterpret_cast<Data*>(m_vPage[_kId.PageIndex] + _kId.PageOffset); }
		inline const char*	GetDataName(Data* _pData) const { const uint32 uiOffset = m_mHash.Get(_pData->Hash, (uint32)-1); return uiOffset != (uint32)-1 ? &m_vName[uiOffset] : nullptr; }

		Data*				CreateData(Data* _pParent, Data::Type _eData, const char* _szName, uint32 _uiSize);
		Data*				AllocateData(Data::Type _eData, uint32 _uiSize);
		bool				ReallocateData(Data* _pData, uint32 _uiNewSize); 

		Data*				CreateData(Data::Type _eData, uint32 _uiSize);
		
	private:
		void				AddPage(uint32 _uiCount);
		void				ResizePage(uint32 _uiCount);
		void				AllocatePage(uint32 _uiCount);

		// Flag
		inline bool			HasFlag			(EFlag::Type eFlag) const { return (m_eFlag & eFlag) != 0; }
		inline void			SetFlag			(EFlag::Type eFlag) { m_eFlag |= eFlag; }
		inline void			RemoveFlag		(EFlag::Type eFlag) { m_eFlag &= ~eFlag; }

	protected:
		Vector<uint8*>			m_vAllocation;
		Vector<uint8*>			m_vPage;
		Vector<char>			m_vName;
		HashMap<uint32, uint32>	m_mHash;
		uint8*					m_pBuffer;
		Id						m_kLastId;
		uint32					m_uiOffset;
		uint8					m_eFlag;
	};

	//!	@brief		Read
	//!	@date		2015-04-04
	template <typename T>
	bool Dictionary::Read(const char* szFilePath, Dictionary::Node* _pRoot /*= nullptr*/)
	{
		FileReader kReader(szFilePath);
		if(kReader.CanRead())
		{
			return Read<T>(&kReader, _pRoot);
		}

		return false;
	}

	//!	@brief		Read
	//!	@date		2015-04-04
	template <typename T>
	bool Dictionary::Read(StreamReader* _pReader, Dictionary::Node* _pRoot /*= nullptr*/)
	{
		T kStream;
		kStream.SetDictionary(this);
		return kStream.Read(_pReader, _pRoot ? _pRoot : this);
	}

	//!	@brief		Write
	//!	@date		2015-04-04
	template <typename T>
	bool Dictionary::Write(const char* szFilePath, Dictionary::Node* _pRoot /*= nullptr*/)
	{
		FileWriter kWriter(szFilePath);
		if(kWriter.CanWrite())
		{			
			return Write<T>(&kWriter,  _pRoot);
		}
		
		return false;
	}

	//!	@brief		Write
	//!	@date		2015-04-04
	template <typename T>
	bool Dictionary::Write(StreamWriter* _pWriter, Dictionary::Node* _pRoot /*= nullptr*/)
	{
		if(!_pRoot) _pRoot = this;
		if(!_pRoot->IsValid()) return false;

		T kStream;
		kStream.SetDictionary(this);
		return kStream.Write(_pWriter, _pRoot ? _pRoot : this);
	}

	//!	@brief		SetValueInternal
	//!	@date		2015-04-04
	template <typename T>
	void Dictionary::SetValueInternal(Data* _pData, T _kValue) 
	{
		// Check if we can save some data
		if(_pData->Format != Helper::StaticType<T>::Value)
		{
			// Check if this is the last node in dictionary
			uint32 uiNewSize = sizeof(TData<Helper::StaticType<T>::Value>);
			uint32 uiOldSize = Helper::SizeOf(_pData);			
			if(uiNewSize < uiOldSize && m_kLastId.PageOffset == m_uiOffset - uiOldSize)
			{
				m_pBuffer = reinterpret_cast<uint8*>(_pData) + uiNewSize;
				m_uiOffset = m_kLastId.PageOffset + uiNewSize;	
			}
			_pData->Format =  Helper::StaticType<T>::Value;
		}

		Helper::SetValue(_pData, _kValue);
	}

	//!	@brief		SetStringInternal
	//!	@date		2015-04-04
	void Dictionary::SetStringInternal(Data* _pData, const char* _szValue, uint32 _uiLength /*= (uint32)-1*/)
	{
		// Reallocate Data if possible
		uint32 uiNewSize = _uiLength + 1;
		if(!ReallocateData(_pData, sizeof(TData<Data::String>) + uiNewSize))
		{
			ASSERT(false, "[Dictionary] Reallocation failed");
		}

		// Copy Data
		TData<Data::String>* pTData = reinterpret_cast<TData<Data::String>*>(_pData);
		pTData->Format = Data::String;
		pTData->Length = uiNewSize;
		Container::CopyString(reinterpret_cast<char*>(_pData) + sizeof(TData<Data::String>), _szValue, uiNewSize);
	}

	//!	@brief		SetBufferInternal
	//!	@date		2015-04-04
	template <typename T>
	void Dictionary::SetBufferInternal(Data* _pData, const T* _pBuffer, uint32 _uiCount) 
	{
		// Reallocate Data if possible
		uint32 uiNewSize = sizeof(T) * _uiCount;
		if(!ReallocateData(_pData, sizeof(TData<Data::Buffer>) + uiNewSize))
		{
			ASSERT(false, "[Dictionary] Reallocation failed");
		}
	
		// Copy Data
		TData<Data::Buffer>* pTData = reinterpret_cast<TData<Data::Buffer>*>(_pData);
		pTData->Format = Data::Buffer;
		pTData->FormatChild = Helper::StaticType<T>::Value;
		pTData->Count = _uiCount;
		Container::Copy(reinterpret_cast<uint8*>(_pData) + sizeof(TData<Data::Buffer>), reinterpret_cast<const uint8*>(_pBuffer), uiNewSize);
	}

	//!	@brief		SetBufferInternal
	//!	@date		2015-04-04
	template <>
	void Dictionary::SetBufferInternal(Data* _pData, const String* _pBuffer, uint32 _uiCount) 
	{
		uint32 uiNewSize = 0;
		for(uint32 uiValue = 0; uiValue < _uiCount; ++uiValue) uiNewSize += _pBuffer[uiValue].GetLength();
		uiNewSize += _uiCount; // final character for each string
		if(!ReallocateData(_pData, sizeof(TData<Data::Buffer>) + uiNewSize))
		{
			ASSERT(false, "[Dictionary] Reallocation failed");
		}
				
		// Copy Data
		TData<Data::Buffer>* pTData = reinterpret_cast<TData<Data::Buffer>*>(_pData);
		pTData->Format = Data::Buffer;
		pTData->FormatChild = Data::String;
		pTData->Count = uiNewSize;
		char* pDestination = reinterpret_cast<char*>(pTData) + sizeof(TData<Data::Buffer>);
		for(uint32 uiValue = 0; uiValue < _uiCount; ++uiValue)
		{
			const String& sValue = _pBuffer[uiValue];
			uint32 uiCharacterCount = sValue.GetLength()+1;
			Container::CopyString(pDestination, sValue.GetBuffer(), uiCharacterCount);
			pDestination += uiCharacterCount;
		}
	}

	template <typename T>
	void Dictionary::GetBufferInternalGENERIC(Data* _pData, const T** _ppBuffer, uint32& _uiCount)
	{
		if(_pData->Format == Data::Buffer)
		{			
			*_ppBuffer = reinterpret_cast<T*>(reinterpret_cast<uint8*>(_pData) + sizeof(TData<Data::Buffer>));
			_uiCount = reinterpret_cast<TData<Data::Buffer>*>(_pData)->Count;
			return;
		}
		*_ppBuffer = nullptr; _uiCount = 0;
	}


	template <typename T>
	T* Dictionary::AddBufferInternalGENERIC(Data* _pParent, const char* _szName, uint32 _uiCount)
	{
		TData<Data::Buffer>* pTData = reinterpret_cast<TData<Data::Buffer>*>(CreateData(_pParent, Data::Buffer, _szName, sizeof(Data::Buffer) + sizeof(T) * _uiCount));
		pTData->Count = _uiCount;
		pTData->FormatChild = Helper::StaticType<T>::Value;
		return reinterpret_cast<T*>(reinterpret_cast<uint8*>(pTData) + sizeof(TData<Data::Buffer>));
	}

}

#endif