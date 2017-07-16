/******************************************************************************
**	Includes
******************************************************************************/
#include "Input.hpp"
#include <Core/Log/LogManager.hpp>
#include <Core/Thread/Thread.hpp>
#include <windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor 
	//!	@date		2015-11-08
	Input::Input()
	{
		memset(m_aKeyboardState, Mouse::EKeyState::Up, sizeof(m_aKeyboardState));
		memset(&m_kMouse, 0, sizeof(Mouse::Data));
		SetWindowsHook(WH_KEYBOARD, Input::KeyboardCallback);
		SetWindowsHook(WH_MOUSE, Input::MouseCallback);
	}

	//!	@brief		Destructor 
	//!	@date		2015-11-08
	Input::~Input()
	{
		// Nothing to do
	}

	//!	@brief		Update
	//!	@date		2015-11-08
	void Input::Update()
	{
		//memset(m_kMouse.Button, Mouse::EKeyState::None, sizeof(m_kMouse.Button));
		m_kMouse.Relative.Set(0.0f, 0.0f);
		m_kMouse.Wheel = 0.0f;
	}

	//!	@brief		MouseCallback 
	//!	@date		2015-11-08
	int64 Input::MouseCallback(int32 /*_iCode*/, uint64 _uiParam, int64 _iParam)
	{
		Input::GetInstance()->UpdateMouse(_uiParam, _iParam);
		return 0;
	}

	//!	@brief		UpdateMouse 
	//!	@date		2015-11-08
	void Input::UpdateMouse(uint64 _eInput, int64 _iParam)
	{
		MSLLHOOKSTRUCT* pData = reinterpret_cast<MSLLHOOKSTRUCT*>(_iParam);
		
		Mouse::Type eButton = Mouse::Unknown;
		IInputListener::EEvent::Type eEvent = IInputListener::EEvent::Unknown;
		switch(_eInput)
		{
			case WM_MOUSEMOVE:
			{
				RECT kWindowRect, kClientRect;
				HWND hWindow = GetActiveWindow();	
				GetWindowRect(hWindow, &kWindowRect);
				GetClientRect(hWindow, &kClientRect);
				
				float32 fBorderSize = (kWindowRect.right - kWindowRect.left - kClientRect.right) * 0.5f;
				float32 fTitleSize = kWindowRect.bottom - kWindowRect.top - kClientRect.bottom - fBorderSize;
				Math::Vector4 vScreen((float32)kWindowRect.left + fBorderSize, (float32)kWindowRect.top + fTitleSize, (float32)kClientRect.right, (float32)kClientRect.bottom);

				eEvent = IInputListener::EEvent::Move;
				m_kMouse.Relative = m_kMouse.Position;
				m_kMouse.Position.SetX(Math::Clamp<float32>(pData->pt.x - vScreen.GetX(), 0.0f, vScreen.GetZ()));
				m_kMouse.Position.SetY(Math::Clamp<float32>(pData->pt.y - vScreen.GetY(), 0.0f, vScreen.GetW()));
				m_kMouse.Relative -= m_kMouse.Position;
				break;
			}

			case WM_MOUSEWHEEL:
			{
				eEvent = IInputListener::EEvent::Wheel;
				m_kMouse.Wheel = (float32)(((int32)pData->mouseData) >> 16);
				break;
			}

			case WM_LBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_LBUTTONDBLCLK:
			case WM_RBUTTONDOWN:
			case WM_RBUTTONUP:
			case WM_RBUTTONDBLCLK:
			case WM_MBUTTONDOWN:
			case WM_MBUTTONUP:
			case WM_MBUTTONDBLCLK:
			{
				eEvent = IInputListener::EEvent::Button;
				Mouse::EKeyState::Type eState = (Mouse::EKeyState::Type)(Mouse::EKeyState::Down + (_eInput - WM_LBUTTONDOWN) % 3);
				eButton = (Mouse::Type)((_eInput - WM_LBUTTONDOWN) / 3);
				m_kMouse.Button[eButton] = eState;
				break;
			}

			/*case WM_XBUTTONDOWN:
			{
				uint8 eButton = (uint8)(pData->mouseData >> 16);
				m_kMouse.Button[eButton] = Mouse::EButtonState::Down;
				LOG_PRINT("[WM_XBUTTONDOWN] %d", eButton);
				break;
			}

			case WM_XBUTTONUP:
			{
				uint8 eButton = (uint8)(pData->mouseData >> 16);
				m_kMouse.Button[eButton] = Mouse::EButtonState::Up;
				LOG_PRINT("[WM_XBUTTONUP] %d", eButton);
				break;
			}

			case WM_XBUTTONDBLCLK:
			{
				uint8 eButton = (uint8)(pData->mouseData >> 16);
				m_kMouse.Button[eButton] = Mouse::EButtonState::Double;
				LOG_PRINT("[WM_XBUTTONDBLCLK] %d", eButton);
				break;
			}*/
		}

		if(eEvent != IInputListener::EEvent::Unknown)
		{
			const Mouse* pMouse = GetMouse();
			const uint32 uiListenerCount = m_vListener.GetSize();
			for(uint32 uiListener = 0; uiListener < uiListenerCount; ++uiListener)
			{
				IInputListener* pListener = m_vListener[uiListener];
				pListener->OnMouseChanged(pMouse, eEvent, eButton);
			}
		}
	}

	//!	@brief		KeyboardCallback 
	//!	@date		2015-11-08
	int64 Input::KeyboardCallback(int32 /*_iCode*/, uint64 _uiParam, int64 _iParam)
	{
		Input::GetInstance()->UpdateKeyboard(_uiParam, _iParam);
		return 0;
	}

	//!	@brief		UpdateKeyboard 
	//!	@date		2015-11-08
	void Input::UpdateKeyboard(uint64 _eInput, int64 _iParam)
	{
		Keyboard::EKeyState::Type eKeyState = (Keyboard::EKeyState::Type)((_iParam >> 31) + 1);
		m_aKeyboardState[_eInput] = eKeyState;

		const uint32 uiListenerCount = m_vListener.GetSize();
		for(uint32 uiListener = 0; uiListener < uiListenerCount; ++uiListener)
		{
			IInputListener* pListener = m_vListener[uiListener];
			pListener->OnKeyboardChanged((Keyboard::Type)_eInput, eKeyState);
		}
	}
}