#ifndef _CORE_FILEWATCHER_HPP_
#define _CORE_FILEWATCHER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Thread/Mutex.hpp>

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla
{
	class FileManager;
	class IFileWatcher;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class FileWatcher
	{
		friend class FileManager;

	private:
		FileWatcher(const char* _szDirectoryPath, void* _hFile, uint32 _eFilter, bool _bIsRecursive);
		~FileWatcher();

	public:
		struct Notification
		{
			enum Type
			{
				Added = 1,
				Removed,
				Modified,
				Renamed,
			};

			Type	Action;
			String  FilePath; 
		};

		void PushNotification(Notification::Type _eType, const char* _szFilePath);

		// Information Used by the FileWatcherThread
		inline const String& GetDirectoryPath() const { return m_sDirectoryPath; }
		inline void* GetFileHandle() { return m_hFile; }
		inline uint32 GetFilter() { return m_eFilter; }
		inline uint8* GetBuffer() { return m_aBuffer; }
		inline uint32 GetBufferSize() const { return sizeof(m_aBuffer); }
		inline bool IsRecursive() const { return m_bIsRecursive; }
		inline void* GetHandle() { return m_pHandle; }

	private:
		inline void AddWatcher(IFileWatcher* _pFileWatcher) { m_vWatcher.Add(_pFileWatcher); }
		inline void RemoveWatcher(IFileWatcher* _pFileWatcher) { m_vWatcher.Remove(_pFileWatcher); }
		void		SendAllNotification();

		inline bool HasNotification() const { return m_vNotification.GetSize() > 0; }
		
	private:
		uint8					m_aBuffer[1024];
		Vector<IFileWatcher*>	m_vWatcher;
		Vector<Notification>	m_vNotification;
		String					m_sDirectoryPath;
		Mutex					m_kMutex;
		void*					m_pHandle;
		void*					m_hFile;
		uint32					m_eFilter;
		bool					m_bIsRecursive;
	};

	class IFileWatcher
	{
		friend class FileWatcher;

	private:
		virtual void OnFileChanged(FileWatcher::Notification::Type _eType, const char* _szDirectoryPath, const char* _szRelativePath) = 0;
	};
}
#endif
