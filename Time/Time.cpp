/******************************************************************************
**	Includes
******************************************************************************/
#include "Time.hpp"
#include <Windows.h>

/******************************************************************************
**	Class Definition
******************************************************************************/
namespace Gorilla
{
	//!	@brief		Constructor
	//!	@date		2015-04-11
	Time::Time()
	{
		// Get current time
		m_iNow = GetPerformanceCounter();
		m_iLast = m_iNow;
		m_fTotalTime = 0.0f;
		m_fDeltaTime = 0.0f;
	}

	//!	@brief		Destructor
	//!	@date		2015-04-11
	Time::~Time()
	{
		// Nothing to do
	}

	//!	@brief		Update
	//!	@date		2015-04-11
	void Time::Update()
	{		
		// Compute DeltaTime
		m_iNow = GetPerformanceCounter();
		m_fDeltaTime = GetElaspedTime(m_iLast, m_iNow) * 0.001f;
		m_fTotalTime += m_fDeltaTime;		

		m_iLast = m_iNow;
	}

	//!	@brief		GetFrequency
	//!	@date		2015-05-08
	int64 Time::GetFrequency()
	{
		static int64 lFrequency;
		static BOOL bResult = QueryPerformanceFrequency((LARGE_INTEGER*)&lFrequency);

		return lFrequency;
	}

	//!	@brief		GetPerformanceCounter
	//!	@date		2015-04-11
	int64 Time::GetPerformanceCounter()
	{
		int64 iTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&iTime);

		return iTime;
	}

	//!	@brief		Get the time between 2 PerformanceCounter in ms
	//!	@date		2015-04-11
	float32 Time::GetElaspedTime(int64 iStart, int64 iEnd)
	{
		return (iEnd - iStart) / (GetFrequency() * 0.001f);
	}
}