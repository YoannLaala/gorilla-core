/******************************************************************************
**	Includes
******************************************************************************/
#include "Profiler.hpp"
#include <Core/Time/Time.hpp>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	Profiler::Profiler()
		: m_uiFrame(0)
		, m_uiFrameCount(0)
		, m_uiTime(0)
	{
		SwitchFrame();
	}

	Profiler::~Profiler()
	{

	}

	void Profiler::PushMarker(const char* _szName)
	{
		uint32 uiStackIndex = m_vStack.GetSize() - 1;
		uint32 uiItemInternalIndex = m_vStack[uiStackIndex];

		ItemInternal& kItemInternal = m_vItem.Add();
		ItemInternal& kParent = m_vItem[uiItemInternalIndex];
		m_vStack.Add(m_vItem.GetSize()-1);

		kItemInternal.Node = kParent.Node.Add(_szName);
		kItemInternal.Time = static_cast<float64>(Time::GetPerformanceCounter());
	}
	
	void Profiler::PopMarker()
	{
		uint32 uiStackIndex = m_vStack.GetSize() - 1;
		uint32 uiItemInternalIndex = m_vStack[uiStackIndex];

		ItemInternal& kItemInternal = m_vItem[uiItemInternalIndex];
		kItemInternal.Time = Time::GetElaspedTime(static_cast<int64>(kItemInternal.Time));

		m_vStack.RemoveIndex(uiStackIndex);
	}

	void Profiler::StartFrame()
	{
		if(m_uiFrameCount == 0) m_iStartFrameTime = Time::GetPerformanceCounter();
		++m_uiFrameCount;
	}

	void Profiler::EndFrame()
	{
		static Vector<float64> vTime;

		// Build time buffer
		const uint32 uiItemInternalCount = m_vItem.GetSize();
		vTime.Resize(uiItemInternalCount-1);
		for(uint32 uiItemInternal = 1; uiItemInternal < uiItemInternalCount; ++uiItemInternal)
		{
			vTime[uiItemInternal-1] = m_vItem[uiItemInternal].Time;
		}
		m_aValue[m_uiFrame]["time"] = vTime;

		// Compute fps
		if(Time::GetElaspedTime(m_iStartFrameTime) >= 1000.0f)
		{
			m_aValue[m_uiFrame]["fps"] = m_uiFrameCount;
			m_aValue[m_uiFrame]["frame"] = m_uiTime++;
			m_uiFrameCount = 0;
		}

		// Prepare new Frame
		SwitchFrame();
	}

	void Profiler::SwitchFrame()
	{
		m_uiFrame = (m_uiFrame + 1) % PROFILER_FRAME_COUNT;
		Dictionary& dValue = m_aValue[m_uiFrame];
		dValue.Clear();
		m_vItem.Clear();
		m_vStack.Clear();
		
		ItemInternal& kItemInternal = m_vItem.Add();
		kItemInternal.Node = dValue["structure"];
		m_vStack.Add(0);
	}
}