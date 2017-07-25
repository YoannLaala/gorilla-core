/******************************************************************************
**	Includes
******************************************************************************/
#include "FileManager.hpp"
#include <Core/Time/Time.hpp>
#include <Core/Log/LogManager.hpp>
#include <Core/Thread/Thread.hpp>
#include <Core/File/FileWatcher.hpp>
#include <Core/String/StringHelper.hpp>

#include <Windows.h>
#include <Shlobj.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	FileManager::FileManager()
	{
		// Nothing to do
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	FileManager::~FileManager()
	{
		// Nothing to do
	}

	//!	@brief		Update
	//!	@date		2015-04-04
	void FileManager::Update()
	{
		// Notify all event for the FileWatcher
		const uint32 uiFileWatcherCount = m_vFileWatcher.GetSize();
		for(uint32 uiFileWatcher = 0; uiFileWatcher < uiFileWatcherCount; ++uiFileWatcher)
		{
			FileWatcher* pFileWatcher = m_vFileWatcher[uiFileWatcher];
			if(pFileWatcher->HasNotification())
			{
				pFileWatcher->SendAllNotification();
			}
		}
	}

	//!	@brief		OpenAFile
	//!	@date		2015-05-30
	bool FileManager::IsFileExist(const char* _szFilePath)
	{
		uint32 uiFlag = ::GetFileAttributes(_szFilePath);
		return (uiFlag != EAttribute::Invalid) && !(uiFlag & EAttribute::Directory);
	}

	//!	@brief		OpenAFile
	//!	@date		2015-05-30
	File* FileManager::OpenAFile(const char* _szFilePath, EAccess::Type _eAccess /*= EAccess::Read*/)
	{
		// Prepare specific flags
		uint32 eAccess = 0, eCreateMethod = 0;
		switch(_eAccess)
		{
			case EAccess::Read:
			{
				eAccess = GENERIC_READ;
				eCreateMethod = OPEN_EXISTING;
				break;
			}

			case EAccess::Write:
			{
				eAccess = GENERIC_WRITE;
				eCreateMethod = CREATE_ALWAYS;
				if(!IsDirectoryExist(_szFilePath))
				{
					CreateADirectory(_szFilePath);
				}
				break;
			}
		}

		// Create handle
		File* pFile = (File*)::CreateFile(_szFilePath, eAccess, NULL, NULL, eCreateMethod, FILE_ATTRIBUTE_NORMAL, NULL);
		if(pFile == INVALID_HANDLE_VALUE)
		{
			LOG_INTERNAL_ERROR("[FileManager] - Failed to open file '%s'", _szFilePath);
			pFile = NULL;
		}

		return pFile;
	}

	//!	@brief		OpenAFile
	//!	@date		2015-05-30
	void FileManager::CloseAFile(File*& _pFile)
	{
		SAFE_CLOSE(_pFile);
	}

	//!	@brief		Move
	//!	@date		2015-05-30
	bool FileManager::Move(const char* _szSource, const char* _szDestination)
	{
		return ::MoveFile(_szSource, _szDestination) == TRUE;
	}

	//!	@brief		CopyAFile
	//!	@date		2015-05-30
	bool FileManager::CopyAFile(const char* _szSource, const char* _szDestination)
	{
		return ::CopyFile(_szSource, _szDestination, FALSE) == TRUE;
	}

	//!	@brief		GetRepertoryHandle
	//!	@date		2015-06-21
	bool GetDirectoryHandle(const char* _szDirectory, HANDLE& _hHandle, WIN32_FIND_DATA& _kFileData)
	{
		String sSearchPattern(_szDirectory);
		sSearchPattern.Append('*');

		// Find the first file
		_hHandle = ::FindFirstFile(sSearchPattern.GetBuffer(), &_kFileData);
		if (_hHandle == INVALID_HANDLE_VALUE)
		{
			return false;
		}

		return true;
	}

	//!	@brief		IsDirectoryExist
	//!	@date		2015-05-30
	bool FileManager::IsDirectoryExist(const char* _szFilePath)
	{
		uint32 uiFlag = ::GetFileAttributes(_szFilePath);
		return (uiFlag != EAttribute::Invalid) && (uiFlag & EAttribute::Directory);
	}

	//!	@brief		CreateDirectory
	//!	@date		2015-04-04
	void FileManager::CreateADirectory(const char* _szFilePath)
	{
		const char* szPosition = strstr(_szFilePath, REPERTORY_SEPARATOR);
		if (szPosition)
		{
			char szDirectory[MAX_PATH];
			while (szPosition)
			{
				strncpy_s(szDirectory, MAX_PATH, _szFilePath, szPosition - _szFilePath);
				::CreateDirectory(szDirectory, NULL);
				szPosition = strstr(szPosition + 1, REPERTORY_SEPARATOR);
			}
		}
	}

	//!	@brief		DeleteDirectory
	//!	@date		2015-04-04
	void FileManager::DeleteADirectory(const char* _szFilePath)
	{
		String sPathPattern(_szFilePath);
		sPathPattern.Append("*", 1);

		// Find the first file
		WIN32_FIND_DATA kFileData;
		HANDLE hFind = ::FindFirstFile(sPathPattern.GetBuffer(), &kFileData);
		if(hFind == INVALID_HANDLE_VALUE)
		{
			return;
		}

		// Iterates on each file
		String sFilePath;
		do
		{
			// Avoid Special Directory
			if (kFileData.cFileName[0] == '.')
			{
				continue;
			}

			sFilePath.Set(_szFilePath).Append(kFileData.cFileName);
			if ((kFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
			{
				DeleteADirectory(sFilePath.GetBuffer());
			}
			else
			{	
				DeleteAFile(sFilePath.GetBuffer());
			}
		} while (::FindNextFile(hFind, &kFileData));

		// Close and delete last directory
		FindClose(hFind);
		::RemoveDirectory(_szFilePath);
	}

	//!	@brief		DeleteFile
	//!	@date		2015-04-04
	void FileManager::DeleteAFile(const char* _szFilePath)
	{
		::DeleteFile(_szFilePath);
	}

	//!	@brief		GetDirectory
	//!	@date		2015-04-04
	void FileManager::GetDirectory(Directory::Type _eDirectory, String& _sDirectoryOut)
	{
		_sDirectoryOut.Resize(MAX_PATH);
		switch(_eDirectory)
		{
			case Directory::Executable:
			{
				uint32 uiLength = ::GetModuleFileName(::GetModuleHandle(NULL), &_sDirectoryOut[0], MAX_PATH);
				_sDirectoryOut.Resize(uiLength);
				uint32 uiPosition = _sDirectoryOut.FindLast(REPERTORY_SEPARATOR);
				_sDirectoryOut.Resize(uiPosition + 1);

				break;
			}

			case Directory::User:
			{
				::SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, &_sDirectoryOut[0]);
				uint32 uiLength = (uint32)strlen(&_sDirectoryOut[0]);
				_sDirectoryOut.Resize(uiLength);
				_sDirectoryOut.Append(REPERTORY_SEPARATOR);
				break;
			}

			case Directory::Temporary:
			{
				// Base Temp directory
				uint32 uiSize = GetTempPath(STRING_STACK_CAPACITY, (char*)_sDirectoryOut.GetBuffer());
				_sDirectoryOut.Resize(uiSize);

				// Unique directory
				_sDirectoryOut.Append("GorillaEngine").Append(REPERTORY_SEPARATOR);
				break;
			}
		}
		Path::Format(_sDirectoryOut);
	}

	//!	@brief		CreateTemporaryDirectory
	//!	@date		2015-04-04
	void FileManager::CreateTemporaryDirectory(String& _sDirectoryOut)
	{		
		GetDirectory(Directory::Temporary, _sDirectoryOut);

		// Unique directory
		int64 iTime = Time::GetPerformanceCounter();	
		_sDirectoryOut.Append((uint32)(iTime & 0x00000000ffffffff)).Append((uint32)(iTime & 0xffffffff00000000)).Append(REPERTORY_SEPARATOR, 1);
		CreateADirectory(_sDirectoryOut.GetBuffer());
	}

	//!	@brief		OnFileChanged
	//!	@date		2015-04-04
	void CALLBACK OnFileChanged(DWORD _uiErrorCode, DWORD _uiByteCount, LPOVERLAPPED _pOverlapped)
	{
		if(_uiByteCount != 0 && _uiErrorCode == ERROR_SUCCESS)
		{
			// Prepare information
			uint32 uiOffset = 0;
			FILE_NOTIFY_INFORMATION* pNotify = NULL;
			FileWatcher* pFileWatcher = static_cast<FileWatcher*>(_pOverlapped->hEvent);
			uint8* pBuffer = pFileWatcher->GetBuffer();

			static String sFilePath;
			{
				do
				{
					// Get the notification			
					pNotify = reinterpret_cast<FILE_NOTIFY_INFORMATION*>(&pBuffer[uiOffset]);
					uiOffset += pNotify->NextEntryOffset;				

					// Get the FilePath and call the callback
					StringHelper::ToString(pNotify->FileName, pNotify->FileNameLength / sizeof(wchar_t), sFilePath);
					Path::Format(sFilePath);
					pFileWatcher->PushNotification((FileWatcher::Notification::Type)pNotify->Action, sFilePath.GetBuffer());
				
				} while (pNotify->NextEntryOffset != 0);
			}
		}

		_pOverlapped->hEvent = NULL;
	}

	//!	@brief		FileWatcherThreadEntry
	//!	@date		2015-11-21
	void FileWatcherThreadEntry(void* _pData)
	{
		Vector<FileWatcher*>* pVecFileWatcher = static_cast<Vector<FileWatcher*>*>(_pData);
		while(pVecFileWatcher->GetSize())
		{
			const uint32 uiFileWatcherCount = pVecFileWatcher->GetSize();
			for(uint32 uiFileWatcher = 0; uiFileWatcher < uiFileWatcherCount; ++uiFileWatcher)
			{
				FileWatcher* pFileWatcher = pVecFileWatcher->Get(uiFileWatcher);

				// Read directory change after each notification
				OVERLAPPED* pOpverlapped = static_cast<OVERLAPPED*>(pFileWatcher->GetHandle());
				if(!pOpverlapped->hEvent)
				{
					pOpverlapped->hEvent = pFileWatcher;
					BOOL bResult = ReadDirectoryChangesW(pFileWatcher->GetFileHandle(), pFileWatcher->GetBuffer(), pFileWatcher->GetBufferSize(), pFileWatcher->IsRecursive(), pFileWatcher->GetFilter(), NULL, pOpverlapped, OnFileChanged);
					if(bResult == 0)
					{
						LOG_INTERNAL_ERROR("[FileManager] FileManager::ReadDirectoryChangesW failed on %s", pFileWatcher->GetDirectoryPath().GetBuffer());
					}
				}
			}

			SleepEx(INFINITE, true);
		}
	}

	//!	@brief		AddFileWatcher
	//!	@date		2015-04-04
	void FileManager::AddFileWatcher(IFileWatcher* _pWatcher, const char* _szDirectoryPath, bool _bIsRecursive /*= false*/)
	{
		// Create one FileWatcher if needed
		FileWatcher* pFileWatcher = GetFileWatcher(_szDirectoryPath);
		if(!pFileWatcher)
		{
			// Create the Handle of the directory
			uint32 eSharedMode = FILE_SHARE_READ | FILE_SHARE_WRITE | FILE_SHARE_DELETE;
			uint32 eFlag = FILE_FLAG_BACKUP_SEMANTICS | FILE_FLAG_OVERLAPPED;	// FILE_FLAG_BACKUP_SEMANTICS must be specified when creating a directory
			void* hFile = CreateFile(_szDirectoryPath, FILE_LIST_DIRECTORY, eSharedMode, NULL, OPEN_EXISTING, eFlag, NULL);

			// Handle created properly
			if (hFile != INVALID_HANDLE_VALUE)
			{
				uint32 eFilter = FILE_NOTIFY_CHANGE_LAST_WRITE | FILE_NOTIFY_CHANGE_CREATION | FILE_NOTIFY_CHANGE_SIZE | FILE_NOTIFY_CHANGE_FILE_NAME | FILE_ACTION_ADDED | FILE_ACTION_REMOVED;
				pFileWatcher = new FileWatcher(_szDirectoryPath, hFile, eFilter, _bIsRecursive);
				m_vFileWatcher.Add(pFileWatcher);

				// Create File Watcher Thread
				if(m_vFileWatcher.GetSize() == 1)
				{
					Thread::Create("FileWatcherThreadEntry", &FileWatcherThreadEntry, &m_vFileWatcher, Thread::EPriority::Low);
				}
			}
		}

		// Add the watcher
		if(pFileWatcher)
		{
			pFileWatcher->AddWatcher(_pWatcher);
		}
	}

	//!	@brief		RemoveFileWatcher
	//!	@date		2015-04-04
	void FileManager::RemoveFileWatcher(IFileWatcher* _pWatcher, const char* _szDirectoryPath)
	{
		FileWatcher* pFileWatcher = GetFileWatcher(_szDirectoryPath);
		if(pFileWatcher)
		{
			pFileWatcher->RemoveWatcher(_pWatcher);
		}
	}

	//!	@brief		GetFileWatcher
	//!	@date		2015-04-04
	FileWatcher* FileManager::GetFileWatcher(const char* _szDirectoryPath)
	{
		// Search for existing one
		const uint32 uiFileWatcherCount = m_vFileWatcher.GetSize();
		for(uint32 uiFileWatcher = 0; uiFileWatcher < uiFileWatcherCount; ++uiFileWatcher)
		{
			FileWatcher* pFileWatcher = m_vFileWatcher[uiFileWatcher];
			if(pFileWatcher->GetDirectoryPath() == _szDirectoryPath)
			{
				return pFileWatcher;
			}
		}

		return nullptr;
	}

	//!	@brief		GetAllFiles
	//!	@date		2015-04-04
	void GetAllFilesInernal(const char* _szPath, Vector<String>& _vFileOut, bool _bRecursive, const Vector<String>& _vExtension)
	{
		// Find the first file
		WIN32_FIND_DATA kFileData;
		HANDLE hDirectory;

		if (!GetDirectoryHandle(_szPath, hDirectory, kFileData)) return;

		// Iterates on each file
		String sFilePath;
		do
		{
			// Avoid Special Directory
			if (kFileData.cFileName[0] == '.' ) continue;

			if ((kFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				const uint32 uiExtensionCount = _vExtension.GetSize();
				if(uiExtensionCount == 0) _vFileOut.Add().Set(_szPath).Append(kFileData.cFileName);
				else
				{
					for(uint32 uiExtension = 0; uiExtension < uiExtensionCount; ++uiExtension)
					{
						// File contains the extension
						if(strstr(kFileData.cFileName, _vExtension[uiExtension].GetBuffer()))
						{
							_vFileOut.Add().Set(_szPath).Append(kFileData.cFileName);
						}
					}
				}
			}
			else
			{
				sFilePath.Set(_szPath).Append(kFileData.cFileName).Append(REPERTORY_SEPARATOR);			
				if (_bRecursive) GetAllFilesInernal(sFilePath.GetBuffer(), _vFileOut, _bRecursive, _vExtension);
			}
		} while (FindNextFile(hDirectory, &kFileData));
	}

	//!	@brief		GetAllFiles
	//!	@date		2015-04-04
	void FileManager::GetAllFiles(const char* _szPath, Vector<String>& _vFileOut, bool _bRecursive /*= false*/, const char* _szExtension /*= nullptr*/)
	{
		// Find each extension
		Vector<String> vExtension;
		if(_szExtension)
		{
			const char* pCursor = _szExtension;
			const char* pStart = _szExtension;
			const char* pEnd = _szExtension + strlen(_szExtension);
			while (pCursor != pEnd)
			{
				++pCursor;
				if(*pCursor == ';' || pCursor == pEnd)
				{
					vExtension.Add().Set('.').Append(pStart, (uint32)(pCursor - pStart));
					pStart = pCursor + 1;
				}
			}
		}
		
		GetAllFilesInernal(_szPath, _vFileOut, _bRecursive, vExtension);
	}

	//!	@brief		GetTree
	//!	@date		2015-04-04
	void FileManager::GetTree(Directory& _kDirectoryOut, bool _bRecursive /*= false*/)
	{
		const String& sDirectory = _kDirectoryOut.GetPath();

		// Find the first file
		WIN32_FIND_DATA kFileData;
		HANDLE hDirectory;
		if (!GetDirectoryHandle(sDirectory.GetBuffer(), hDirectory, kFileData)) return;

		// Iterates on each file
		static String sFilePath;
		do
		{
			// Avoid Special Directory
			if (kFileData.cFileName[0] == '.' ) continue;

			if ((kFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) == 0)
			{
				_kDirectoryOut.AddFile(kFileData.cFileName);
			}
			else
			{
				sFilePath.Set(sDirectory).Append(kFileData.cFileName).Append(REPERTORY_SEPARATOR);
				Directory& kDirectoryChild = _kDirectoryOut.AddDirectory(sFilePath.GetBuffer());			
				if (_bRecursive)
				{
					GetTree(kDirectoryChild, _bRecursive);
				}
			}
		} while (FindNextFile(hDirectory, &kFileData));

		// Close and delete last directory
		FindClose(hDirectory);
	}
}