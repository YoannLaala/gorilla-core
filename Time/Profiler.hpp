#ifndef _CORE_PROFILER_HPP_
#define _CORE_PROFILER_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include <Core/Singleton.hpp>
#include <Core/Container/Dictionary.hpp>

/******************************************************************************
**	Defines
******************************************************************************/
#define PROFILE_SCOPE(_name_)	Gorilla::Profiler::Item Profiler_name_(_name_);
#define PROFILE_FUNCTION()		Gorilla::Profiler::Item Profiler_##__FUNCTION__(__FUNCTION__);
#define PROFILER_FRAME_COUNT	2

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Profiler : public Singleton<Profiler>
	{
	public:
		class Item
		{
		public:
			Item(const char* _szName)
			{
				Profiler::GetInstance()->PushMarker(_szName);
			}

			~Item()
			{
				Profiler::GetInstance()->PopMarker();
			}
		};

	public:
		Profiler();
		~Profiler();

		void StartFrame();
		void EndFrame();

		void Update();

		void PushMarker(const char* _szName);
		void PopMarker();

		inline Dictionary& GetValue() { return m_aValue[(m_uiFrame + 1) % PROFILER_FRAME_COUNT]; }

	private:
		void SwitchFrame();

	private:
		struct ItemInternal
		{
			Node	Node;
			float64 Time;
		};

		Dictionary				m_aValue[PROFILER_FRAME_COUNT];
		Vector<ItemInternal>	m_vItem;
		Vector<uint32>			m_vStack;
		uint32					m_uiTime;
		int64					m_iStartFrameTime;
		uint32					m_uiFrameCount;
		uint8					m_uiFrame;
	};	
}
#endif