/******************************************************************************
**	Includes
******************************************************************************/
#include "Window.hpp"
#include <Windows.h>

/******************************************************************************
**	Define
******************************************************************************/
#define WINDOW_CLASS_NAME "Gorilla"

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla 
{
	//!	@brief		Create
	//!	@date		2015-04-08
	bool bIsregistered = false;
	Window* Window::Create(const char* _szName, uint32 _uiWidth, uint32 _uiHeight, Process _pWindowProcess /*= NULL*/)
	{
		HINSTANCE hInstance = ::GetModuleHandle(NULL);

		// Register the default class for all windows
		if(!bIsregistered)
		{
			// Define the class
			WNDCLASSEX kWindowClassExt;
			kWindowClassExt.cbSize = sizeof(WNDCLASSEX);
			kWindowClassExt.cbClsExtra = 0;
			kWindowClassExt.cbWndExtra = 0;
			kWindowClassExt.style = CS_HREDRAW | CS_VREDRAW;
			kWindowClassExt.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
			kWindowClassExt.hIconSm = LoadIcon(hInstance, IDI_APPLICATION);
			kWindowClassExt.hCursor = LoadCursor(hInstance, IDC_ARROW);
			kWindowClassExt.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			kWindowClassExt.lpszMenuName = NULL;
			kWindowClassExt.hInstance = hInstance;
			kWindowClassExt.lpfnWndProc = (WNDPROC)_pWindowProcess;
			kWindowClassExt.lpszClassName = WINDOW_CLASS_NAME;

			// Register the class
			if (!::RegisterClassEx(&kWindowClassExt))
			{
				return NULL;
			}

			bIsregistered = true;
		}

		// Compute real size (including border& & title)
		const DWORD dwStyle = WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN;
		RECT kRectangle;
		ZeroMemory(&kRectangle, sizeof(RECT));
		kRectangle.right = _uiWidth;
		kRectangle.bottom = _uiHeight;
		::AdjustWindowRect(&kRectangle, dwStyle, false);

		// Create window
		_uiWidth = kRectangle.right - kRectangle.left;
		_uiHeight = kRectangle.bottom - kRectangle.top;

		HWND hWindow = ::CreateWindow(WINDOW_CLASS_NAME, _szName, dwStyle, CW_USEDEFAULT, CW_USEDEFAULT, _uiWidth, _uiHeight, NULL, NULL, hInstance, NULL);


		return reinterpret_cast<Window*>(hWindow);
	}

	//!	@brief		Destroy
	//!	@date		2015-04-08
	void Window::Destroy(Window* _pWindow)
	{
		::DestroyWindow(reinterpret_cast<HWND>(_pWindow));
	}

	//!	@brief		Show
	//!	@date		2015-04-08
	void Window::Show()
	{
		HWND hWindow = reinterpret_cast<HWND>(this);
		::ShowWindow(hWindow, SW_NORMAL);
	}

	//!	@brief		Hide
	//!	@date		2015-04-08
	void Window::Hide()
	{
		::ShowWindow(reinterpret_cast<HWND>(this), SW_HIDE);
	}

	//!	@brief		GetWidth
	//!	@date		2015-04-08
	uint32 Window::GetWidth()
	{
		RECT kRectangle;
		GetClientRect(reinterpret_cast<HWND>(this), &kRectangle);
		return kRectangle.right - kRectangle.left;
	}

	//!	@brief		GetHeight
	//!	@date		2015-04-08
	uint32 Window::GetHeight()
	{
		RECT kRectangle;
		GetClientRect(reinterpret_cast<HWND>(this), &kRectangle);
		return kRectangle.bottom - kRectangle.top;
	}

	//!	@brief		GetHeight
	//!	@date		2015-04-08
	void Window::SetTitle(const char* _szTitle)
	{
		::SetWindowTextA(reinterpret_cast<HWND>(this), _szTitle);
	}
}