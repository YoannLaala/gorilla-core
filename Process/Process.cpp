/******************************************************************************
**	Includes
******************************************************************************/
#include "Process.hpp"
#include <Core/Log/LogManager.hpp>
#include <Core/String/String16.hpp>
#include <Core/String/StringHelper.hpp>
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-05-15
	Process::Process()
	{	
		// Noyhing to do
	}

	//!	@brief		Constructor
	//!	@date		2015-05-15
	Process::Process(const char* _szExecutablePath, const char* _szArgument /*= nullptr*/)
	{
		Initialize(_szExecutablePath, _szArgument);
	}

	//!	@brief		Destructor
	//!	@date		2015-05-15
	Process::~Process()
	{
		// Nothing to do
	}

	//!	@brief		Initialize
	//!	@date		2015-05-15
	void Process::Initialize(const char* _szExecutablePath, const char* _szArgument /*= nullptr*/)
	{
		m_kPath = _szExecutablePath;
		SetArgument(_szArgument);
	}

	//!	@brief		SetArgument
	//!	@date		2015-05-15
	void Process::SetArgument(const char* _szArgument)
	{
		m_sCommandLine.Set(m_kPath.GetFull());
		if(_szArgument)
		{
			m_sCommandLine.Append(" ").Append(_szArgument);
		}
		m_sCommandLine.Replace("\\", "/");
	}

	//!	@brief		Execute
	//!	@date		2015-05-15
	bool Process::Execute(String* _pResultOut /*= nullptr*/)
	{
		PROCESS_INFORMATION kPocessInformation;

		// Set up the security attributes struct	
		SECURITY_ATTRIBUTES kSecurityAttribute;
		ZeroMemory(&kSecurityAttribute, sizeof(SECURITY_ATTRIBUTES));
		kSecurityAttribute.nLength= sizeof(SECURITY_ATTRIBUTES);
		kSecurityAttribute.lpSecurityDescriptor = NULL;
		kSecurityAttribute.bInheritHandle = TRUE;

		// Init compile process
		STARTUPINFO kStartupInfo;
		ZeroMemory(&kStartupInfo, sizeof(STARTUPINFO));
		kStartupInfo.cb = sizeof(STARTUPINFO);
		kStartupInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
		kStartupInfo.wShowWindow = SW_HIDE;

		// Create Handle to read std & err output
		HANDLE hOutputRead, hErrorRead;
		CreatePipe(&hOutputRead, &kStartupInfo.hStdOutput, &kSecurityAttribute, 0);
		CreatePipe(&hErrorRead, &kStartupInfo.hStdError, &kSecurityAttribute, 0);

		//Create the process
		bool bSuccess = CreateProcessA(
				nullptr								,//__in_opt     LPCTSTR lpApplicationName,
				(LPSTR)m_sCommandLine.GetBuffer(),	//__inout_opt  LPWSTR lpCommandLine,
				NULL,								//__in_opt     LPSECURITY_ATTRIBUTES lpProcessAttributes,
				NULL,								//__in_opt     LPSECURITY_ATTRIBUTES lpThreadAttributes,
				TRUE,								//__in         BOOL bInheritHandles,
				0,									//__in         DWORD dwCreationFlags,
				NULL,								//__in_opt     LPVOID lpEnvironment,
				NULL,								//__in_opt     LPCTSTR lpCurrentDirectory,
				&kStartupInfo,						//__in         LPSTARTUPINFO lpStartupInfo,
				&kPocessInformation					//__out        LPPROCESS_INFORMATION lpProcessInformation
			) != 0;

		// Successfully created the process.  Wait for it to finish.
		if(bSuccess)
		{
			WaitForSingleObject(kPocessInformation.hProcess, INFINITE);

			// Get the ExitCode
			DWORD dwExitCode;
			GetExitCodeProcess(kPocessInformation.hProcess, &dwExitCode);
			bSuccess = (dwExitCode == 0);
		}

		// Handle LogError
		if(!bSuccess && _pResultOut)
		{
			ReadHandle(hOutputRead, *_pResultOut);
			ReadHandle(hErrorRead, *_pResultOut);
		}

		// Close all pipe
		SAFE_CLOSE(kStartupInfo.hStdInput);
		SAFE_CLOSE(kStartupInfo.hStdOutput);
		SAFE_CLOSE(kPocessInformation.hProcess);
		SAFE_CLOSE(kPocessInformation.hThread);
		SAFE_CLOSE(hOutputRead);
		SAFE_CLOSE(hErrorRead);

		return bSuccess;
	}

	//!	@brief		ReadHandle
	//!	@date		2015-05-15
	void Process::ReadHandle(HANDLE hValue, String& _sValueOut) const
	{
		DWORD dwByteReadAvailable;
		PeekNamedPipe(hValue, NULL, NULL, NULL, &dwByteReadAvailable, NULL);
		if(dwByteReadAvailable)
		{
			char szBuffer[MAX_PATH];
			uint32 uiMaxSizeToRead = sizeof(szBuffer)-1;
			DWORD dwByteReadCount;
			do
			{
				ReadFile(hValue, szBuffer, uiMaxSizeToRead, &dwByteReadCount, NULL);
				
				szBuffer[dwByteReadCount] = '\0';
				_sValueOut += szBuffer;
			}
			while(dwByteReadCount == uiMaxSizeToRead);
		}
	}
}