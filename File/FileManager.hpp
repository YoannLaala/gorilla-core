#ifndef _FILE_MANAGER_HPP_
#define _FILE_MANAGER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/String/Path.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define REPERTORY_SEPARATOR "\\"

/******************************************************************************
**	Forward Declaration
******************************************************************************/
namespace Gorilla
{
	class File;
	class FileWatcher;
	class IFileWatcher;
}

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class FileManager
	{
	public:

		struct EAccess
		{
			enum Type : uint8
			{
				Read = 0,
				Write,

				Count
			};
		};

		struct EAttribute
		{
			enum Type
			{
				Invalid		= (uint32)-1,
				Directory	= 0x00000010,

				Count
			};
		};

		class Directory
		{
			friend class FileManager;
		public:
			enum Type : uint8
			{
				Executable = 0,
				User,
				Temporary,
			};

		public:
			Directory() { }
			Directory(const char* _szPath) : m_sPath(_szPath) { }
			~Directory() { }

			inline const String& GetPath() const { return m_sPath; }

			inline const Vector<String>& GetVecFile() const {return m_vFile; }
			inline uint32 GetFileCount() const { return m_vFile.GetSize(); }
			inline const String& GetFile(uint32 _uiIndex) const { return m_vFile[_uiIndex]; }

			inline const Vector<Directory>& GetVecDirectory() const {return m_vDirectory; }
			inline uint32 GetDirectoryCount() const { return m_vDirectory.GetSize(); }
			inline const Directory& GetDirectory(uint32 _uiIndex) const { return m_vDirectory[_uiIndex]; }

			inline void AddFile(const char* _szFileName) { m_vFile.Add(_szFileName); }

			inline Directory& AddDirectory(const char* _szPath)
			{
				Directory& kDirectory = m_vDirectory.Add();
				kDirectory.m_sPath = _szPath;
				return kDirectory;
			}

		private:
			Vector<String> m_vFile;
			Vector<Directory> m_vDirectory;
			String m_sPath;
		};

		FileManager();
		~FileManager();

		void				Update();

		static bool			IsFileExist			(const char* _szFilePath);
		static File*		OpenAFile			(const char* _szFilePath, EAccess::Type _eAccess = EAccess::Read);
		static void			CloseAFile			(File*& _pFile);
		
		static bool			Move				(const char* _szSource, const char* _szDestination);
		static bool			CopyAFile			(const char* _szSource, const char* _szDestination);
		static bool			IsDirectoryExist	(const char* _szFilePath);
		static void			CreateADirectory	(const char* _szFilePath);
		static void			DeleteADirectory	(const char* _szFilePath);
		static void			DeleteAFile			(const char* _szFilePath);

		static void			GetDirectory		(Directory::Type _eDirectory, String& _sDirectoryOut);
		static void			CreateTemporaryDirectory(String& _sDirectoryOut);

		static void			GetAllFiles(const char* _szPath, Vector<String>& _vFileOut, bool _bRecursive = false, const char* _szExtension = nullptr);
		static void			GetTree(Directory& _kDirectoryOut, bool _bRecursive = false);

		void				AddFileWatcher(IFileWatcher* _pWatcher, const char* _szDirectoryPath, bool _bIsRecursive = false);
		void				RemoveFileWatcher(IFileWatcher* _pWatcher, const char* _szDirectoryPath);

	private:
		FileWatcher*		GetFileWatcher(const char* _szDirectoryPath);

	private:
		Vector<FileWatcher*> m_vFileWatcher;
	};
}
#endif