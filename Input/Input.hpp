#ifndef _CORE_INPUT_HPP_
#define _CORE_INPUT_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Core.hpp>
#include <Core/Singleton.hpp>
#include <Core/Input/Enum.hpp>

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Keyboard
	{
		friend class Input;

	public:
		enum Type : uint8
		{
			BackSpace	= VK_BACK,
			Tabulation	= VK_TAB,
			Clear		= VK_CLEAR,
			Enter		= VK_RETURN,
			Shift		= VK_SHIFT,
			Control		= VK_CONTROL,
			Alternative	= VK_MENU,
			Pause		= VK_PAUSE,
			Capital		= VK_CAPITAL,
			Escape		= VK_ESCAPE,
			Space		= VK_SPACE,
			PageUp		= VK_PRIOR,
			PageDown	= VK_NEXT,
			End			= VK_END,
			Home		= VK_HOME,
			Left		= VK_LEFT,
			Up			= VK_UP,
			Right		= VK_RIGHT,
			Down		= VK_DOWN,
			
			Num0		= 0x30,
			Num1		= 0x31,
			Num2		= 0x32,
			Num3		= 0x33,
			Num4		= 0x34,
			Num5		= 0x35,
			Num6		= 0x36,
			Num7		= 0x37,
			Num8		= 0x38,
			Num9		= 0x39,

			A			= 0x41,
			B			= 0x42,
			C			= 0x43,
			D			= 0x44,
			E			= 0x45,
			F			= 0x46,
			G			= 0x47,
			H			= 0x48,
			I			= 0x49,
			J			= 0x4A,
			K			= 0x4B,
			L			= 0x4C,
			M			= 0x4D,
			N			= 0x4E,
			O			= 0x4F,
			P			= 0x50,
			Q			= 0x51,
			R			= 0x52,
			S			= 0x53,
			T			= 0x54,
			U			= 0x55,
			V			= 0x56,
			W			= 0x57,
			X			= 0x58,
			Y			= 0x59,
			Z			= 0x5A,

			NumPad0		= VK_NUMPAD0,
			NumPad1		= VK_NUMPAD1,
			NumPad2		= VK_NUMPAD2,
			NumPad3		= VK_NUMPAD3,
			NumPad4		= VK_NUMPAD4,
			NumPad5		= VK_NUMPAD5,
			NumPad6		= VK_NUMPAD6,
			NumPad7		= VK_NUMPAD7,
			NumPad8		= VK_NUMPAD8,
			NumPad9		= VK_NUMPAD9,
			NumPadMul	= VK_MULTIPLY,
			NumPadAdd	= VK_ADD,
			NumPadSep	= VK_SEPARATOR,
			NumPadSub	= VK_SUBTRACT,
			NumPadDec	= VK_DECIMAL,
			NumPadDiv	= VK_DIVIDE,

			F1			= VK_F1,
			F2			= VK_F2,
			F3			= VK_F3,
			F4			= VK_F4,
			F5			= VK_F5,
			F6			= VK_F6,
			F7			= VK_F7,
			F8			= VK_F8,
			F9			= VK_F9,
			F10			= VK_F10,
			F11			= VK_F11,
			F12			= VK_F12,

			Equal		= VK_OEM_NEC_EQUAL,
			SemiColon	= VK_OEM_1,
			Plus		= VK_OEM_PLUS,
			Coma		= VK_OEM_COMMA,
			Minus		= VK_OEM_MINUS,
			Period		= VK_OEM_PERIOD,
			Slash		= VK_OEM_2,
			Tilde		= VK_OEM_3,
			BracketLeft	= VK_OEM_4,
			BackSlash	= VK_OEM_5,
			BracketRight= VK_OEM_6,
			Quote		= VK_OEM_7,

			Count		= 0xFF
		};

		struct EKeyState
		{
			enum Type : uint8
			{
				None = 0,
				Down,
				Up,
			};
		};
	};

	class Mouse
	{
		friend class Input;

	public:
		enum Type : uint8
		{
			Left = 0,
			Right,
			Middle,

			Count,
			Unknown = Count,
		};

	private:
		struct EKeyState
		{
			enum Type : uint8
			{
				None = 0,
				Down,
				Up,
				Double
			};
		};

	private:
		struct Data
		{
			EKeyState::Type		Button[Mouse::Count];
			Math::Vector2		Position;
			Math::Vector2		Relative;
			float32				Wheel;
		};

	
	public:
		inline const Math::Vector2&	GetPosition		() const { return GetData().Position; }
		inline const Math::Vector2&	GetRelative		() const { return GetData().Relative; }
		inline const float32		GetWheel		() const { return GetData().Wheel; }
																	  		
		inline bool					IsWheelForward	() const { return GetData().Wheel > 0.0f; }
		inline bool					IsWheelBackward	() const { return GetData().Wheel < 0.0f; }
		inline bool					IsWheelMoving	() const { return GetData().Wheel != 0.0f; }

		inline bool					IsPressed		(Mouse::Type _eInput) const { return GetData().Button[_eInput] == EKeyState::Down; }
		inline bool					IsReleased		(Mouse::Type _eInput) const { return GetData().Button[_eInput] == EKeyState::Up; }
		inline bool					IsDoublePressed	(Mouse::Type _eInput) const { return GetData().Button[_eInput] == EKeyState::Double; }

	private:
		inline const Data& GetData() const { return *reinterpret_cast<const Data*>(this); }
	};

	class IInputListener;

	class Input : public Singleton<Input>
	{
	public:
		Input();
		~Input();

		inline void AddListener(IInputListener* _pListener) { m_vListener.Add(_pListener); }
		inline void RemoveListener(IInputListener* _pListener) { m_vListener.Remove(_pListener); }

		// Mouse
		inline const Mouse*			GetMouse						() const { return reinterpret_cast<const Mouse*>(&m_kMouse); }

		void Update();
		inline bool					IsKeyPressed					(Keyboard::Type _eKey) const { return m_aKeyboardState[_eKey] == Mouse::EKeyState::Down; }

	private:
		static int64 __stdcall		MouseCallback(int32 _iCode, uint64 _uiParam, int64 _iParam);
		void						UpdateMouse(uint64 _eInput, int64 _iParam);

		static int64 __stdcall		KeyboardCallback(int32 _iCode, uint64 _uiParam, int64 _iParam);
		void						UpdateKeyboard(uint64 _eInput, int64 _iParam);
	
	private:
		// Device States
		Keyboard::EKeyState::Type	m_aKeyboardState[Keyboard::Count];
		Vector<IInputListener*>		m_vListener;

		// Device
		Mouse::Data m_kMouse;
	};

	class IInputListener
	{
		friend Input;

	public:
		struct EEvent
		{
			enum Type : uint8
			{
				Move = 0,
				Wheel,
				Button,

				Count,
				Unknown = Count,
			};
		};

	protected:
		virtual void OnMouseChanged(const Mouse* /*_pMouse*/, EEvent::Type /*_eEvent*/, Mouse::Type /*_eButton*/) { }
		virtual void OnKeyboardChanged(Keyboard::Type /*_eButton*/, Keyboard::EKeyState::Type /*_eState*/) { }
	};
}

#endif