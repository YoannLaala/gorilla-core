#ifndef _CORE_HELPER_WINDOW_HPP_
#define _CORE_HELPER_WINDOW_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/TypeDef.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Window
	{
	public:
		typedef int64 (__stdcall* Process)(void*, uint32, uint64, int64);
		static Window* Create	(const char* _szName, uint32 _uiWidth, uint32 _uiHeight, Process _pWindowProcess = nullptr);
		static void	   Destroy	(Window* _pWindow);

		void Show();
		void Hide();

		uint32 GetWidth();
		uint32 GetHeight();

		void SetTitle(const char* _szTitle);
	};
}
#endif