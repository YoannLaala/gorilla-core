/******************************************************************************
**	Includes
******************************************************************************/
#include "FileWatcher.hpp"
#include <Core/String/String.hpp>
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-04
	FileWatcher::FileWatcher(const char* _szDirectoryPath, void* _hFile, uint32 _eFilter, bool _bIsRecursive)
		: m_sDirectoryPath(_szDirectoryPath)
		, m_hFile(_hFile)
		, m_eFilter(_eFilter)
		, m_bIsRecursive(_bIsRecursive)
	{
		// Create OVERLAPPED structure to handle asynchronous notification
		OVERLAPPED* pOpverlapped = new OVERLAPPED();
		memset(pOpverlapped, 0, sizeof(OVERLAPPED));
		m_pHandle = pOpverlapped;
	}

	//!	@brief		Destructor
	//!	@date		2015-04-04
	FileWatcher::~FileWatcher()
	{
		SAFE_DELETE(m_pHandle);
	}

	//!	@brief		PushNotification
	//!	@date		2015-04-04
	void FileWatcher::PushNotification(Notification::Type _eType, const char* _szFilePath)
	{
		bool bIsPresent = false;

		// Check if the notification is already present
		const uint32 uiNotificationCount = m_vNotification.GetSize();
		for(uint32 uiNotification = 0; uiNotification < uiNotificationCount; ++uiNotification)
		{
			Notification& kNotification = m_vNotification[uiNotification];
			if(kNotification.Action == _eType && kNotification.FilePath == _szFilePath)
			{
				bIsPresent = true;
				break;
			}
		}

		// Push the notificationonly if not alreadu present
		if(!bIsPresent)
		{
			m_kMutex.Lock();
			Notification& kNotification = m_vNotification.Add();
			kNotification.Action = _eType;
			kNotification.FilePath = _szFilePath;
			m_kMutex.Unlock();
		}
	}

	//!	@brief		SendAllNotification
	//!	@date		2015-04-04
	void FileWatcher::SendAllNotification()
	{
		// Check if the notification is already present
		m_kMutex.Lock();
		const uint32 uiNotificationCount = m_vNotification.GetSize();
		for(uint32 uiNotification = 0; uiNotification < uiNotificationCount; ++uiNotification)
		{
			Notification& kNotification = m_vNotification[uiNotification];
					
			const uint32 uiWatcherCount = m_vWatcher.GetSize();
			for(uint32 uiWatcher = 0; uiWatcher < uiWatcherCount; ++uiWatcher)
			{
				IFileWatcher* pFileWatcher = m_vWatcher[uiWatcher];
				pFileWatcher->OnFileChanged(kNotification.Action, m_sDirectoryPath.GetBuffer(), kNotification.FilePath.GetBuffer());
			}
		}
		m_vNotification.Clear();
		m_kMutex.Unlock();
	}
}
