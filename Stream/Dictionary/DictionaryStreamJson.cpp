/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Stream/Dictionary/DictionaryStreamJson.hpp>
#include <Core/ThirdParty/rapidjson/rapidjson.h>
#include <Core/ThirdParty/rapidjson/document.h>
#include <Core/ThirdParty/rapidjson/prettywriter.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		RapidjsonStreamToStreamWriter
	//!	@date		2015-04-04
	class RapidjsonStreamToStreamWriter
	{
	public:
		RapidjsonStreamToStreamWriter(StreamWriter* _pWriter) : m_pWriter(_pWriter) { }
		~RapidjsonStreamToStreamWriter() { }

		inline void Put(char cValue) { m_pWriter->Write(cValue); }
		inline void Flush() {}

	private:
		StreamWriter* m_pWriter;
	};

	//!	@brief		Read
	//!	@date		2015-04-04
	bool DictionaryStreamJson::Read(StreamReader* _pReader, Node* _pNode)
	{
		// Read file
		uint32 uiFileLength = _pReader->GetSize();
		char* pBuffer = new char[uiFileLength + 1];
		_pReader->Read(pBuffer, uiFileLength);
		pBuffer[uiFileLength] = 0;

		// Parse Buffer et build a rapidjson document
		rapidjson::StringStream sText(pBuffer);
		rapidjson::Document kDocumentJson;
		kDocumentJson.ParseStream<rapidjson::ParseFlag::kParseDefaultFlags>(sText);
		delete pBuffer;

		// Check for errors
		if (kDocumentJson.HasParseError())
		{
			return false;
		}
		
		// Iterates on root
		rapidjson::Value::MemberIterator it = kDocumentJson.MemberBegin();
		rapidjson::Value::MemberIterator itEnd = kDocumentJson.MemberEnd();
		while (it != itEnd)
		{
			ReadNode(*_pNode, it->name.GetString(), &it->value);
			++it;
		}

		return true;
	}

	//!	@brief		ReadNode
	//!	@date		2015-04-04
	void DictionaryStreamJson::ReadNode(Node _kParent, const char* _szName, void* _pValue) 
	{
		rapidjson::Value* pValueJson = reinterpret_cast<rapidjson::Value*>(_pValue);
		switch (pValueJson->GetType())
		{
			case rapidjson::kArrayType:
			{
				// Handle Buffer
				const uint32 uiChildCount = pValueJson->Size();
				if(uiChildCount && pValueJson->Begin()->GetType() != rapidjson::kObjectType)
				{
					switch(pValueJson->Begin()->GetType())
					{
						case rapidjson::kTrueType:
						case rapidjson::kFalseType:
						{
							bool* pBuffer = AddBuffer<bool>(&_kParent, _szName, uiChildCount);
							for(uint32 uiValue = 0; uiValue < uiChildCount; ++uiValue) 
							{
								*pBuffer++ = (*pValueJson)[uiValue].GetBool();
							}
							break;
						}

						case rapidjson::kNumberType:
						{
							uint64* pBuffer = AddBuffer<uint64>(&_kParent, _szName, uiChildCount);
							for(uint32 uiValue = 0; uiValue < uiChildCount; ++uiValue)
							{
								*pBuffer++ = (*pValueJson)[uiValue].GetUint64();
							}
							break;
						}

						case rapidjson::kStringType:
						{
							static Vector<String> vValue; vValue.Clear();
							for(uint32 uiValue = 0; uiValue < uiChildCount; ++uiValue) vValue.Add((*pValueJson)[uiValue].GetString());
							_kParent[_szName] = vValue;

							break;
						}
					}					

					return;
				}

				// Basic Array of element
				Node kChild = _kParent[_szName];
				for(uint32 uiChild = 0; uiChild < uiChildCount; ++uiChild)
				{
					ReadNode(kChild, nullptr, &(*pValueJson)[uiChild]);
				}
	
				break;
			}
			
			case rapidjson::kObjectType:
			{
				Node kChild = _kParent[_szName];

				// Iterates all children
				rapidjson::Value::MemberIterator it = pValueJson->MemberBegin();
				rapidjson::Value::MemberIterator itEnd = pValueJson->MemberEnd();
				while(it != itEnd)
				{	
					ReadNode(kChild, it->name.GetString(), &it->value);
					++it;
				}

				break;
			}
			
			case rapidjson::kTrueType:
			case rapidjson::kFalseType:
			{
				_kParent[_szName] = pValueJson->GetBool();
				break;
			}

			case rapidjson::kNumberType:
			{
				// We can't see the difference between float32/float64
				if(pValueJson->IsDouble())
				{
					_kParent[_szName] = (float64)pValueJson->GetDouble();
				}
				else
				{
					// We can't see the difference between int8/int16/int32
					_kParent[_szName] =  (int64)pValueJson->GetInt64();
				}
				break;
			}

			case rapidjson::kStringType:
			{
				_kParent[_szName] = pValueJson->GetString();
				break;
			}
		}
	}

	//!	@brief		Write
	//!	@date		2015-04-04
	bool DictionaryStreamJson::Write(StreamWriter* _pWriter, Node* _pRoot)
	{
		RapidjsonStreamToStreamWriter kSreamWriter(_pWriter);
		rapidjson::Writer<RapidjsonStreamToStreamWriter> kWriterJson(kSreamWriter);

		// Write Tree
		WriteNode(*_pRoot, &kWriterJson);
		return true;
	}

	//!	@brief		WriteNode
	//!	@date		2015-04-04
	void DictionaryStreamJson::WriteNode(Node _kNode, void* pWriter)
	{		
		// Write proper json depending on node type
		const char* szName = _kNode.GetName();
		rapidjson::Writer<RapidjsonStreamToStreamWriter>* pWriterJson = static_cast<rapidjson::Writer<RapidjsonStreamToStreamWriter>*>(pWriter);
		switch(_kNode.GetType())
		{
			case Node::Data::Object:
			{
				// Object in Array has no name
				if(szName) pWriterJson->String(szName);
				pWriterJson->StartObject();
				Node kChild = _kNode.GetFirstChild();
				while(kChild.IsValid())
				{
					WriteNode(kChild, pWriter);
					++kChild;
				}
				pWriterJson->EndObject();
				break;
			}

			case Node::Data::Array:
			{
				// Object in Array has no name
				if(szName) pWriterJson->String(szName);
				pWriterJson->StartArray();
				Node kChild = _kNode.GetFirstChild();
				while(kChild.IsValid())
				{
					WriteNode(kChild, pWriter);
					++kChild;
				}
				pWriterJson->EndArray();
				break;
			}

			case Node::Data::Boolean:
			{
				pWriterJson->String(szName);
				pWriterJson->Bool(_kNode.GetBool());
				break;
			}

			case Node::Data::Integer8:
			{
				pWriterJson->String(szName);
				pWriterJson->Int(_kNode.GetInt8());
				break;
			}

			case Node::Data::UInteger8:
			{
				pWriterJson->String(szName);
				pWriterJson->Uint(_kNode.GetUInt8());
				break;
			}

			case Node::Data::Integer16:
			{
				pWriterJson->String(szName);
				pWriterJson->Int(_kNode.GetInt16());
				break;
			}

			case Node::Data::UInteger16:
			{
				pWriterJson->String(szName);
				pWriterJson->Uint(_kNode.GetUInt16());
				break;
			}

			case Node::Data::Integer32:
			{
				pWriterJson->String(szName);
				pWriterJson->Int(_kNode.GetInt32());
				break;
			}

			case Node::Data::UInteger32:
			{
				pWriterJson->String(szName);
				pWriterJson->Uint(_kNode.GetUInt32());
				break;
			}

			case Node::Data::Integer64:
			{
				pWriterJson->String(szName);
				pWriterJson->Int64(_kNode.GetInt64());
				break;
			}

			case Node::Data::UInteger64:
			{
				pWriterJson->String(szName);
				pWriterJson->Uint64(_kNode.GetUInt64());
				break;
			}

			case Node::Data::Float32:
			{
				pWriterJson->String(szName);
				pWriterJson->Double(_kNode.GetFloat32());
				break;
			}

			case Node::Data::Float64:
			{
				pWriterJson->String(szName);
				pWriterJson->Double(_kNode.GetFloat64());
				break;
			}

			case Node::Data::String:
			{
				pWriterJson->String(szName);
				pWriterJson->String(_kNode.GetString());
				break;
			}

			case Node::Data::Buffer:
			{
				pWriterJson->String(szName);
				pWriterJson->StartArray();

				switch(_kNode.GetTypeExt())
				{
					case Node::Data::Boolean:
					{
						const bool* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Bool(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::UInteger8:
					{
						const uint8* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Uint(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::UInteger16:
					{
						const uint16* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Uint(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::UInteger32:
					{
						const uint32* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Uint(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::UInteger64:
					{
						const uint64* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Uint64(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::Integer8:
					{
						const int8* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Int(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::Integer16:
					{
						const int16* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Int(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::Integer32:
					{
						const int32* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Int(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::Integer64:
					{
						const int64* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Int64(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::Float32:
					{
						const float32* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Double(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::Float64:
					{
						const float64* pBuffer; uint32 uiCount;
						_kNode.GetValue(&pBuffer, uiCount);;
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->Double(pBuffer[uiValue]);
						}
						break;
					}

					case Node::Data::String:
					{
						static Vector<String> vValue;
						_kNode.GetValue(vValue);
						const uint32 uiCount = vValue.GetSize();
						for(uint32 uiValue = 0; uiValue < uiCount; ++uiValue)
						{
							pWriterJson->String(vValue[uiValue].GetBuffer());
						}
						break;
					}
				}
				pWriterJson->EndArray();
				break;
			}
		}
	}
}
