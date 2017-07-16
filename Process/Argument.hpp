#ifndef _ARGUMENT_HPP_
#define _ARGUMENT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "../String/String.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Argument
	{
		friend class ArgumentParser;

	public:
		Argument();
		~Argument();
	
		void Initialize(const char* _szPattern, const char* _szValue);

		inline const String&	GetPattern() const	{ return m_sPattern; }
		inline const String&	GetValue() const	{ return m_sValue; }

		const String&			Get			() const;
		void					Get			(bool& _bValueOut) const;
		void					Get			(uint32& _uiValueOut) const;
		void					Get			(int& _iValueOut) const;
		void					Get			(String& _sValueOut) const;

	protected:
		inline bool				Match		(const char* _szPattern) const { return m_sPattern == _szPattern; }
		inline void				SetPattern	(const char* _szPattern) { m_sPattern = _szPattern; }
		inline void				SetValue	(const char* _szValue) { m_sValue = _szValue; }

	private:
		String m_sPattern;
		String m_sValue;
	};

	class ArgumentExtended : public Argument
	{
		friend class ArgumentParser;

	public:
		ArgumentExtended() {}
		~ArgumentExtended() {}
	
		inline const String&	GetDescription	() const	{ return m_sDescription; }
		inline void				SetDescription	(const char* _szDescription) { m_sDescription = _szDescription; }
		inline bool				IsRequired		() const { return m_bIsrequired; }
		inline void				SetRequired		(bool _bIsRequired) { m_bIsrequired = _bIsRequired;}

	private:
		String m_sDescription;
		bool m_bIsrequired;
	};
}

#endif