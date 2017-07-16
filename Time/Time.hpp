#ifndef _TIME_HPP_
#define _TIME_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "../TypeDef.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
namespace Gorilla
{
	class Time
	{
	public:
		Time();
		~Time();

		void				Update			();

		inline float32		GetToltalTime	() const { return m_fTotalTime; }
		inline float32		GetDeltaTime	() const { return m_fDeltaTime; }

		static int64		GetPerformanceCounter	(); 
		static float32		GetElaspedTime			(int64 _iStart, int64 _iEnd);
		static float32		GetElaspedTime			(int64 _iStart) { return GetElaspedTime(_iStart, GetPerformanceCounter()); }

	private:
		static int64 GetFrequency();

	private:
		int64			m_iNow;
		int64			m_iLast;
		int64			m_iFrequency;

		float32			m_fTotalTime;
		float32			m_fDeltaTime;
	};	
}

#endif