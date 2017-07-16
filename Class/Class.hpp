#ifndef _CLASS_HPP_
#define _CLASS_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Hash.hpp>
#include <Core/Stream/StreamReader.hpp>
#include <Core/Stream/StreamWriter.hpp>
/******************************************************************************
**	Define
******************************************************************************/
#define CLASS_DECLARE_VIRTUAL()																						\
virtual const Class* GetClass() const = 0;																			\

#define CLASS_DECLARE_IMPL(_section_, _type_, _flag_) 																\
class Class : public Gorilla::Class																					\
{																													\
public:																												\
	Class()																											\
	{																												\
		Gorilla::Class::New = &New;																					\
		Gorilla::Class::GetName = &GetName;																			\
		Gorilla::Class::GetId = &GetId;																				\
		Gorilla::Class::HasFlag = &HasFlag;																			\
		Gorilla::ClassManager::GetInstance()->Set(_section_, this);													\
	}																												\
																													\
	~Class()																										\
	{																												\
		/* Nothing to do */																							\
	}																												\
																													\
	inline static const Class*	GetInstance	() { return m_pInstance; }												\
	inline static void*			New			() { return new _type_(); }												\
	inline static const String&	GetName		() { static String sName(#_type_); return sName; }						\
	inline static uint32		GetId		() { static uint32 uiId = Hash::Generate(#_type_); return uiId; }		\
	inline static bool			HasFlag		(uint32 _eFlag) { return (_flag_ & _eFlag) != 0; }						\
private:																											\
	static Class* m_pInstance;																						\
};																													\
virtual const Class* GetClass() const override { return Class::GetInstance(); }

#define CLASS_DECLARE(_section_, _type_) CLASS_DECLARE_IMPL(_section_, _type_, 0)
#define CLASS_REGISTER(_type_) _type_::Class* _type_::Class::m_pInstance = new _type_::Class();

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Class
	{
	public:
		Class() { }
		~Class() { }

		void*			(*New)(void);
		const String&	(*GetName)(void);
		uint32			(*GetId)(void);
		bool			(*HasFlag)(uint32);
	};
}

#endif