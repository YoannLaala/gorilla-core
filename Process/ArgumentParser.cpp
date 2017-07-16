/******************************************************************************
**	Includes
******************************************************************************/
#include "ArgumentParser.hpp"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-05
	ArgumentParser::ArgumentParser(const char* _szTitle, const char*_szDescription, const char* _szVersion)
		: m_sTitle(_szTitle)
		, m_sDescription(_szDescription)
		, m_sVersion(_szVersion)
	{
		Add("--help", "Display all the option available for the process", false);
	}

	//!	@brief		Destructor
	//!	@date		2015-04-05
	ArgumentParser::~ArgumentParser()
	{
		// Nothing to do
	}

	//!	@brief		Add
	//!	@details	Add en Argument composed with pattern / description / require
	//!	@date		2015-04-05
	void ArgumentParser::Add(const char* _szPattern, const char* _szDescription /*= nullptr*/, bool _bIsRequired /*= true*/)
	{
		ArgumentExtended& kArgument = m_vArgument.Add();
		kArgument.Initialize(_szPattern, nullptr);
		kArgument.SetDescription(_szDescription);
		kArgument.SetRequired(_bIsRequired);
	}

	//!	@brief		Parse
	//!	@details	Parse application input arguments
	//!	@date		2015-04-05

	bool ArgumentParser::Parse(int argc, const char** argv, String* pUsageOut /*= nullptr*/)
	{
		// Iterates on each Argument 
		const uint32 uiArgumentCount = m_vArgument.GetSize();
		for(uint32 uiArgument = 1; uiArgument < uiArgumentCount; ++uiArgument)
		{
			// Find the current argument in those given by the application
			bool bArgumentFound = false;
			ArgumentExtended& kArgument = m_vArgument[uiArgument];
			for(int iarg = 1; iarg < argc; ++iarg)
			{
				if(kArgument.Match(argv[iarg]))
				{
					// Set the value for this argument
					kArgument.SetValue(argv[++iarg]);
					bArgumentFound = true;
					break;
				}
			}

			// Argument not found - If it is required, the parsing has failed
			if(!bArgumentFound && kArgument.IsRequired())
			{
				// Generate automatically the usage if the parsing failed
				if(pUsageOut)
				{
					GenerateUsage(*pUsageOut);
					pUsageOut->Set("\n\nUsage error: Required argument '").Append(kArgument.GetPattern()).Append("' not found\n");
				}

				return false;
			}
		}

		return true;
	}

	//!	@brief		GenerateUsage
	//!	@details	Generate a string to display to give more information on the usage of the application
	//!	@date		2015-04-05
	void ArgumentParser::GenerateUsage(String& _sUsageOut) const
	{
		// Title
		_sUsageOut.Append("/******************************************************************************\n");
		_sUsageOut.Append("**	").Append(m_sTitle);
		_sUsageOut.Append("\n**	").Append(m_sDescription);
		_sUsageOut.Append("\n**	Version:").Append(m_sVersion);
		_sUsageOut.Append("\n******************************************************************************/\n");
		
		// Options
		const uint32 uiArgumentCount = m_vArgument.GetSize();
		for(uint32 uiArgument = 0; uiArgument < uiArgumentCount; ++uiArgument)
		{
			const ArgumentExtended& kArgument = m_vArgument[uiArgument];
			_sUsageOut.Append(kArgument.GetPattern()).Append("	").Append(kArgument.GetDescription());
			if(kArgument.IsRequired())
			{
				_sUsageOut.Append(" [REQUIRED]");
			}
			_sUsageOut.Append("\n", 1);
		}
	}
}