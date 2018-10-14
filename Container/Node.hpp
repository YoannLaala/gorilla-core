#ifndef _NODE_HPP_
#define _NODE_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/TypeDef.hpp>
#include <Core/String/String.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla
{
	class Dictionary;
	class DictionaryStream;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{	
	class Node
	{
		friend class Dictionary;
		friend class DictionaryStream;

		/******************************************************************************
		**	Node Id
		******************************************************************************/
		struct Id
		{
			static const Id Start;
			static const Id Invalid;

			Id() : PageIndex(0), PageOffset(0) { }
			Id(uint16 uiPageIndex, uint16 uiPageOffset) : PageIndex(uiPageIndex), PageOffset(uiPageOffset) { }

			inline bool operator==(Id kId) const { return PageOffset == kId.PageOffset && PageIndex == kId.PageIndex; }
			inline bool operator!=(Id kId) const { return PageOffset != kId.PageOffset || PageIndex != kId.PageIndex; }

			uint16 PageIndex;
			uint16 PageOffset;
		};

		/******************************************************************************
		**	Node Data
		**	Dictionary limited to 256Mb with NextId on 28bits
		******************************************************************************/
	public:
		struct Data
		{
			enum Type : uint8 
			{
				Boolean = 0,
				UInteger8,
				Integer8,
				UInteger16,
				Integer16,
				UInteger32,
				Integer32,
				Float32,
				UInteger64,
				Integer64,
				Float64,
				String,
				Buffer,
				Object,
				Array,
			};

			inline Id GetNextId() const { return Id(NextPageIndex, NextPageOffset); }
			inline void SetNextId(Id kId) { NextPageIndex = kId.PageIndex; NextPageOffset = kId.PageOffset; }

			uint32 NextPageOffset	: 16;
			uint32 NextPageIndex	: 12;
			uint32 Format			: 4;
			uint32 Hash;
		};


		/******************************************************************************
		**	Templated Node Data
		******************************************************************************/
	private:
		template <Data::Type T>
		struct TData;

		template <>
		struct TData<Data::Object> : public Data
		{
			Id	FirstChildId;
			Id	LastChildId;
		};

		template <>
		struct TData<Data::Array> : public Data
		{
			Id	FirstChildId;
			Id	LastChildId;
		};

		template <>
		struct TData<Data::Boolean> : public Data
		{
			bool Value;
		};

		template <>
		struct TData<Data::UInteger8> : public Data
		{
			uint8 Value;
		};

		template <>
		struct TData<Data::UInteger16> : public Data
		{
			uint16 Value;
		};

		template <>
		struct TData<Data::UInteger32> : public Data
		{
			uint32 Value;
		};

		template <>
		struct TData<Data::UInteger64> : public Data
		{
			uint64 Value;
		};

		template <>
		struct TData<Data::Integer8> : public Data
		{
			int8 Value;
		};

		template <>
		struct TData<Data::Integer16> : public Data
		{
			int16 Value;
		};

		template <>
		struct TData<Data::Integer32> : public Data
		{
			int32 Value;
		};

		template <>
		struct TData<Data::Integer64> : public Data
		{
			int64 Value;
		};

		template <>
		struct TData<Data::Float32> : public Data
		{
			float32 Value;
		};

		template <>
		struct TData<Data::Float64> : public Data
		{
			float64 Value;
		};

		template <>
		struct TData<Data::String> : public Data
		{
			uint32 Length;
		};

		template <>
		struct TData<Data::Buffer> : public Data
		{
			uint32 Count;
			Data::Type FormatChild;
		};


		class Helper
		{
			friend class Node;
			friend class Dictionary;

			/******************************************************************************
			**	Static Data type from template
			******************************************************************************/
			template <typename T> struct StaticType;
			template <> struct StaticType<bool> { static const Data::Type Value = Data::Boolean; };
			template <> struct StaticType<uint8> { static const Data::Type Value = Data::UInteger8; };
			template <> struct StaticType<uint16> { static const Data::Type Value = Data::UInteger16; };
			template <> struct StaticType<uint32> { static const Data::Type Value = Data::UInteger32; };
			template <> struct StaticType<uint64> { static const Data::Type Value =  Data::UInteger64; };
			template <> struct StaticType<int8> { static const Data::Type Value = Data::Integer8; };
			template <> struct StaticType<int16> { static const Data::Type Value = Data::Integer16; };
			template <> struct StaticType<int32> { static const Data::Type Value = Data::Integer32; };
			template <> struct StaticType<int64> { static const Data::Type Value = Data::Integer64; };
			template <> struct StaticType<float32> { static const Data::Type Value = Data::Float32; };
			template <> struct StaticType<float64> { static const Data::Type Value = Data::Float64; };
			template <> struct StaticType<const char*> { static const Data::Type Value = Data::String; };

			/******************************************************************************
			**	GetValue
			******************************************************************************/
			template <typename T> inline static T	GetValue(Data* /*_pData*/) { static_assert(false); } // Type not supported
			template <> inline static bool			GetValue(Data* _pData) { return reinterpret_cast<TData<Data::Boolean>*>(_pData)->Value; };
			template <> inline static uint8			GetValue(Data* _pData) { return reinterpret_cast<TData<Data::UInteger8>*>(_pData)->Value; };
			template <> inline static uint16		GetValue(Data* _pData) { return reinterpret_cast<TData<Data::UInteger16>*>(_pData)->Value;};
			template <> inline static uint32		GetValue(Data* _pData) { return reinterpret_cast<TData<Data::UInteger32>*>(_pData)->Value; };
			template <> inline static uint64		GetValue(Data* _pData) { return reinterpret_cast<TData<Data::UInteger64>*>(_pData)->Value; };
			template <> inline static int8			GetValue(Data* _pData) { return reinterpret_cast<TData<Data::Integer8>*>(_pData)->Value; };
			template <> inline static int16			GetValue(Data* _pData) { return reinterpret_cast<TData<Data::Integer16>*>(_pData)->Value;};
			template <> inline static int32			GetValue(Data* _pData) { return reinterpret_cast<TData<Data::Integer32>*>(_pData)->Value; };
			template <> inline static int64			GetValue(Data* _pData) { return reinterpret_cast<TData<Data::Integer64>*>(_pData)->Value; };
			template <> inline static float32		GetValue(Data* _pData) { return reinterpret_cast<TData<Data::Float32>*>(_pData)->Value; };
			template <> inline static float64		GetValue(Data* _pData) { return reinterpret_cast<TData<Data::Float64>*>(_pData)->Value;; };
			template <> inline static const char*	GetValue<const char*>(Data* _pData) { return _pData->Format == Data::String ? (const char*)_pData + sizeof(TData<Data::String>) : ""; }

			/******************************************************************************
			**	SetValue
			******************************************************************************/
			template <typename T> inline static void SetValue(Data* /*_pData*/, T /*_kValue*/) { static_assert(false); } // Type not supported
			template <> inline static void			SetValue(Data* _pData, bool _bValue) { reinterpret_cast<TData<Data::Boolean>*>(_pData)->Value = _bValue; };
			template <> inline static void			SetValue(Data* _pData, uint8 _uiValue) { reinterpret_cast<TData<Data::UInteger8>*>(_pData)->Value = _uiValue; };
			template <> inline static void			SetValue(Data* _pData, uint16 _uiValue) { reinterpret_cast<TData<Data::UInteger16>*>(_pData)->Value = _uiValue; };
			template <> inline static void			SetValue(Data* _pData, uint32 _uiValue) { reinterpret_cast<TData<Data::UInteger32>*>(_pData)->Value = _uiValue; };
			template <> inline static void			SetValue(Data* _pData, uint64 _uiValue) { reinterpret_cast<TData<Data::UInteger64>*>(_pData)->Value = _uiValue; };
			template <> inline static void			SetValue(Data* _pData, int8 _iValue) { reinterpret_cast<TData<Data::Integer8>*>(_pData)->Value = _iValue; };
			template <> inline static void			SetValue(Data* _pData, int16 _iValue) { reinterpret_cast<TData<Data::Integer16>*>(_pData)->Value = _iValue; };
			template <> inline static void			SetValue(Data* _pData, int32 _iValue) { reinterpret_cast<TData<Data::Integer32>*>(_pData)->Value = _iValue; };
			template <> inline static void			SetValue(Data* _pData, int64 _iValue) { reinterpret_cast<TData<Data::Integer64>*>(_pData)->Value = _iValue; };
			template <> inline static void			SetValue(Data* _pData, float32 _fValue) { reinterpret_cast<TData<Data::Float32>*>(_pData)->Value = _fValue; };
			template <> inline static void			SetValue(Data* _pData, float64 _fValue) { reinterpret_cast<TData<Data::Float64>*>(_pData)->Value = _fValue; };
			template <> inline static void			SetValue(Data* /*_pData*/, const char* /*_szValue*/) {  };

			/******************************************************************************
			**	Compute the size of the current Data
			******************************************************************************/
			inline static uint32 SizeOf(Data* _pData) 
			{ 
				switch(_pData->Format)
				{
					case Data::Object: return sizeof(TData<Data::Object>);
					case Data::Array: return sizeof(TData<Data::Array>);
					case Data::Boolean: return sizeof(TData<Data::Boolean>);
					case Data::UInteger8: return sizeof(TData<Data::UInteger8>);
					case Data::Integer8: return sizeof(TData<Data::Integer8>);
					case Data::UInteger16: return sizeof(TData<Data::UInteger16>);
					case Data::Integer16: return  sizeof(TData<Data::Integer16>);
					case Data::UInteger32: return sizeof(TData<Data::UInteger32>);
					case Data::Integer32: return sizeof(TData<Data::Integer32>);
					case Data::Float32: return sizeof(TData<Data::Float32>);
					case Data::UInteger64: return sizeof(TData<Data::UInteger64>);
					case Data::Integer64: return sizeof(TData<Data::Integer64>);
					case Data::Float64: return sizeof(TData<Data::Float64>);
					case Data::String: return sizeof(TData<Data::String>) + reinterpret_cast<TData<Data::String>*>(_pData)->Length;
					case Data::Buffer:
					{
						uint32 uiByteCount = 0;
						TData<Data::Buffer>* pTData = reinterpret_cast<TData<Data::Buffer>*>(_pData);
						switch(pTData->FormatChild)
						{
							case Data::UInteger8: uiByteCount = pTData->Count; break;
							case Data::Integer8: uiByteCount = pTData->Count; break;
							case Data::UInteger16: uiByteCount = pTData->Count * sizeof(uint16); break;
							case Data::Integer16: uiByteCount = pTData->Count * sizeof(int16); break;
							case Data::UInteger32: uiByteCount = pTData->Count * sizeof(uint32); break;
							case Data::Integer32: uiByteCount = pTData->Count * sizeof(int32); break;
							case Data::Float32: uiByteCount = pTData->Count * sizeof(float32); break;
							case Data::UInteger64: uiByteCount = pTData->Count * sizeof(uint64); break;
							case Data::Integer64: uiByteCount = pTData->Count * sizeof(int64); break;
							case Data::Float64: uiByteCount = pTData->Count * sizeof(float64); break;
							case Data::String: uiByteCount = pTData->Count; break;
						}
						return sizeof(TData<Data::Buffer>) + uiByteCount;
					}
				}

				return 0; 
			}
		};

	private:
		Node(Dictionary* _pDictionary, Data* _pData) : m_pDictionary(_pDictionary), m_pData(_pData) { }

	public:
		Node() : m_pDictionary(nullptr), m_pData(nullptr) { }
		~Node() { }

		inline bool							IsValid() const { return m_pData != nullptr; }

		inline Data::Type					GetType() const { return (Data::Type)m_pData->Format; }
		inline Data::Type					GetTypeExt() const { return (Data::Type)reinterpret_cast<TData<Data::Buffer>*>(m_pData)->FormatChild; }
		const char*							GetName() const;
		inline uint32						GetId() const { return m_pData->Hash; }
		void								Next();
		Node								GetNext() const;
		Node								GetFirstChild() const;
		Node								GetLastChild() const;
		
		Node								Get(const char* _szName) const;
		Node								Add(const char* _szName = nullptr);
		void								Remove(const char* _szName);

		// Type check
			// Explicit
		inline bool							IsBool() const { return m_pData->Format == Data::Boolean; }
		inline bool							IsUInt8() const { return m_pData->Format == Data::UInteger8; }
		inline bool							IsUInt16() const { return m_pData->Format == Data::UInteger16; }
		inline bool							IsUInt32() const { return m_pData->Format == Data::UInteger32; }
		inline bool							IsUInt64() const { return m_pData->Format == Data::UInteger64; }
		inline bool							IsInt8() const { return m_pData->Format == Data::Integer8; }
		inline bool							IsInt16() const { return m_pData->Format == Data::Integer16; }
		inline bool							IsInt32() const { return m_pData->Format == Data::Integer32; }
		inline bool							IsInt64() const { return m_pData->Format == Data::Integer64; }
		inline bool							IsFloat32() const { return m_pData->Format == Data::Float32; }
		inline bool							IsFloat64() const { return m_pData->Format == Data::Float64; }
		inline bool							IsString() const { return m_pData->Format == Data::String; }
		inline bool							IsBuffer() const { return m_pData->Format == Data::Buffer; }
			
			// Implicit
		template <typename T> inline bool	Is() const { static_assert(false); return false; }	// Type not supported
		template <> inline bool				Is<bool>() const { return IsBool(); }
		template <> inline bool				Is<uint8>() const { return IsUInt8(); }
		template <> inline bool				Is<uint16>() const { return IsUInt16(); }
		template <> inline bool				Is<uint32>() const { return IsUInt32(); }
		template <> inline bool				Is<uint64>() const { return IsUInt64(); }
		template <> inline bool				Is<int8>() const { return IsInt8(); }
		template <> inline bool				Is<int16>() const { return IsInt16(); }
		template <> inline bool				Is<int32>() const { return IsInt32(); }
		template <> inline bool				Is<int64>() const { return IsInt64(); }
		template <> inline bool				Is<float32>() const { return IsFloat32(); }
		template <> inline bool				Is<float64>() const { return IsFloat64(); }
		template <> inline bool				Is<const char*>() const { return IsString(); }
		template <> inline bool				Is<String>() const { return IsString(); }

		// GetValue
			// Explicit
		inline bool							GetBool() const  { return Helper::GetValue<bool>(m_pData); }
		inline uint8						GetUInt8() const { return Helper::GetValue<uint8>(m_pData); }
		inline uint16						GetUInt16() const { return Helper::GetValue<uint16>(m_pData); }
		inline uint32						GetUInt32() const { return Helper::GetValue<uint32>(m_pData); }
		inline uint64						GetUInt64() const { return Helper::GetValue<uint64>(m_pData); }
		inline int8							GetInt8() const  { return Helper::GetValue<int8>(m_pData); }
		inline int16						GetInt16() const { return Helper::GetValue<int16>(m_pData); }
		inline int32						GetInt32() const { return Helper::GetValue<int32>(m_pData); }
		inline int64						GetInt64() const { return Helper::GetValue<int64>(m_pData); }
		inline float32						GetFloat32() const { return Helper::GetValue<float32>(m_pData); }
		inline float64						GetFloat64() const { return Helper::GetValue<float64>(m_pData); }
		inline const char*					GetString() const { return Helper::GetValue<const char*>(m_pData); }

			// Implicit
		template <typename T> inline T		GetValue() const { static_assert(false); return T(); }	// Type not supported
		template <> inline bool				GetValue() const { return GetBool(); }
		template <> inline uint8			GetValue() const { return GetUInt8(); }
		template <> inline uint16			GetValue() const { return GetUInt16(); }
		template <> inline uint32			GetValue() const { return GetUInt32(); }
		template <> inline uint64			GetValue() const { return GetUInt64(); }
		template <> inline int8				GetValue() const { return GetInt8(); }
		template <> inline int16			GetValue() const { return GetInt16(); }
		template <> inline int32			GetValue() const { return GetInt32(); }
		template <> inline int64			GetValue() const { return GetInt64(); }
		template <> inline float32			GetValue() const { return GetFloat32(); }
		template <> inline float64			GetValue() const { return GetFloat64(); }
		template <> inline const char*		GetValue() const { return GetString(); }
		template <typename T> inline void	GetValue(T& _kValue) const { _kValue = GetValue<T>(); }
		template <typename T> bool			GetValue(const char* _szName, T& _kValue) const;

		// SetValue
			// Explicit
		void								SetBool(bool _bValue);
		void								SetUInt8(uint8 _uiValue);
		void								SetUInt16(uint16 _uiValue);
		void								SetUInt32(uint32 _uiValue);
		void								SetUInt64(uint64 _uiValue);
		void								SetInt8(int8 _iValue);
		void								SetInt16(int16 _iValue);
		void								SetInt32(int32 _iValue);
		void								SetInt64(int64 _iValue);
		void								SetFloat32(float32 _fValue);
		void								SetFloat64(float64 _fValue);
		void								SetString(const char* _szValue);
		void								SetString(const String& _sValue);

			// Implicit
		inline void							SetValue(bool _bValue) { SetBool(_bValue); }
		inline void							SetValue(uint8 _uiValue) { SetUInt8(_uiValue); }
		inline void							SetValue(uint16 _uiValue) { SetUInt16(_uiValue); }
		inline void							SetValue(uint32 _uiValue) { SetUInt32(_uiValue); }
		inline void							SetValue(uint64 _uiValue) { SetUInt64(_uiValue); }
		inline void							SetValue(int8 _iValue) { SetInt8(_iValue); }
		inline void							SetValue(int16 _iValue) { SetInt16(_iValue); }
		inline void							SetValue(int32 _iValue) { SetInt32(_iValue); }
		inline void							SetValue(int64 _iValue) { SetInt64(_iValue); }
		inline void							SetValue(float32 _fValue) { SetFloat32(_fValue); }
		inline void							SetValue(float64 _fValue) { SetFloat64(_fValue); }
		inline void							SetValue(const char* _szValue) { SetString(_szValue); }
		inline void							SetValue(const String& _sValue) { SetString(_sValue); }
		template <typename T> inline void	SetValue(const char* _szName, T _kValue) { *this[_szName] = _kValue; }

		// GetBuffer
			// Explicit
		void 								GetBufferBool(const bool** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferUInt8(const uint8** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferUInt16(const uint16** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferUInt32(const uint32** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferUInt64(const uint64** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferInt8(const int8** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferInt16(const int16** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferInt32(const int32** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferInt64(const int64** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferFloat32(const float32** _ppBuffer, uint32& _uiCount) const;
		void 								GetBufferFloat64(const float64** _ppBuffer, uint32& _uiCount) const;
		void								GetBufferString(Vector<String>& _vValue) const;

		template <typename T> inline void	GetValue(const T** _ppBuffer, uint32& _uiCount) const { static_assert(false); }	// Type not supported
		template <> inline void				GetValue(const bool** _ppBuffer, uint32& _uiCount) const { return GetBufferBool(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const uint8** _ppBuffer, uint32& _uiCount) const { return GetBufferUInt8(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const uint16** _ppBuffer, uint32& _uiCount) const { return GetBufferUInt16(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const uint32** _ppBuffer, uint32& _uiCount) const { return GetBufferUInt32(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const uint64** _ppBuffer, uint32& _uiCount) const { return GetBufferUInt64(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const int8** _ppBuffer, uint32& _uiCount) const { return GetBufferInt8(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const int16** _ppBuffer, uint32& _uiCount) const { return GetBufferInt16(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const int32** _ppBuffer, uint32& _uiCount) const { return GetBufferInt32(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const int64** _ppBuffer, uint32& _uiCount) const { return GetBufferInt64(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const float32** _ppBuffer, uint32& _uiCount) const { return GetBufferFloat32(_ppBuffer, _uiCount); }
		template <> inline void				GetValue(const float64** _ppBuffer, uint32& _uiCount) const { return GetBufferFloat64(_ppBuffer, _uiCount); }
		template <typename T> inline void	GetValue(Vector<T>& _vValue) const;
		template <> inline void				GetValue(Vector<String>& _vValue) const { return GetBufferString(_vValue); }
		template <typename T> bool			GetValue(const char* _szName, Vector<T>& _vValue) const;

		// SetBuffer
			// Explicit
		void								SetBufferBool(const bool* _pBuffer, uint32 _uiCount);
		void								SetBufferUInt8(const uint8* _pBuffer, uint32 _uiCount);
		void								SetBufferUInt16(const uint16* _pBuffer, uint32 _uiCount);
		void								SetBufferUInt32(const uint32* _pBuffer, uint32 _uiCount);
		void								SetBufferUInt64(const uint64* _pBuffer, uint32 _uiCount);
		void								SetBufferInt8(const int8* _pBuffer, uint32 _uiCount);
		void								SetBufferInt16(const int16* _pBuffer, uint32 _uiCount);
		void								SetBufferInt32(const int32* _pBuffer, uint32 _uiCount);
		void								SetBufferInt64(const int64* _pBuffer, uint32 _uiCount);
		void								SetBufferFloat32(const float32* _pBuffer, uint32 _uiCount);
		void								SetBufferFloat64(const float64* _pBuffer, uint32 _uiCount);
		void								SetBufferString(const Vector<String>& _vValue);

			// Implicit
		inline void							SetValue(const Vector<bool>& _vValue) { SetBufferBool(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<uint8>& _vValue) { SetBufferUInt8(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<uint16>& _vValue) { SetBufferUInt16(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<uint32>& _vValue) { SetBufferUInt32(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<uint64>& _vValue) { SetBufferUInt64(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<int8>& _vValue) { SetBufferInt8(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<int16>& _vValue) { SetBufferInt16(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<int32>& _vValue) { SetBufferInt32(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<int64>& _vValue) { SetBufferInt64(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<float32>& _vValue) { SetBufferFloat32(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<float64>& _vValue) { SetBufferFloat64(&_vValue[0], _vValue.GetSize()); }
		inline void							SetValue(const Vector<String>& _vValue) { SetBufferString(_vValue); }
		template <typename T> inline void	SetValue(const char* _szName, const Vector<T>& _vValue) { *this[_szName] = _vValue; }
		
		// operator
		Node								operator[](const char* _szName) const;
		template <typename T> inline void	operator=(T _kValue) { SetValue(_kValue); }
		inline void							operator=(const String& _sValue) { SetValue(_sValue); }
		inline Node&						operator++() { Next(); return *this; }
		inline Node							operator++(int /*iValue*/) { return GetNext(); }
											operator bool() const { return GetBool(); }
											operator uint8() const { return GetUInt8(); }
											operator uint16() const { return GetUInt16(); }
											operator uint32() const { return GetUInt32(); }
											operator uint64() const { return GetUInt64(); }
											operator int8() const { return GetInt8(); }
											operator int16() const { return GetInt16(); }
											operator int32() const { return GetInt32(); }
											operator int64() const { return GetInt64(); }
											operator float32() const { return GetFloat32(); }
											operator float64() const { return GetFloat64(); }
											operator const char*() const { return GetString(); }
		template <typename T>				operator Vector<T>() const { Vector<T> vValue; GetValue(vValue); return vValue; }

	private:
		Dictionary*	m_pDictionary;
		Data*		m_pData;
	};

	//!	@brief		GetValue
	//!	@date		2015-04-04
	template <typename T> 
	bool Node::GetValue(const char* _szName, T& _kValue) const
	{
		Node kNode = Get(_szName);
		if(kNode.IsValid())
		{
			_kValue = kNode.GetValue<T>();
			return true;
		}

		return false;
	}

	//!	@brief		GetValue
	//!	@date		2015-04-04
	template <typename T> 
	bool Node::GetValue(const char* _szName, Vector<T>& _vValue) const
	{
		Node kNode = Get(_szName);
		if(kNode.IsValid())
		{
			kNode.GetValue<T>(_vValue);
			return true;
		}

		return false;
	}

	//!	@brief		GetBuffer
	//!	@date		2015-04-04
	template <typename T> 
	void Node::GetValue(Vector<T>& _vValue) const
	{
		_vValue.Clear();

		// Retrieve pointer
		const T* pBuffer; uint32 uiCount;
		GetValue(&pBuffer, uiCount);

		// Copy to Vector
		if(uiCount)
		{
			uint32 uiByteCount = sizeof(T) * uiCount;
			_vValue.Resize(uiCount);
			memcpy_s(&_vValue[0], uiByteCount, pBuffer, uiByteCount);
		}
	}
}

#endif