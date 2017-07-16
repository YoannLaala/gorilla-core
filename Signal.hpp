#ifndef _CORE_SIGNAL_HPP_
#define _CORE_SIGNAL_HPP_

/******************************************************************************
**	Includes
******************************************************************************/
#include "PigeonHolePrinciple.hpp"
#include "Container/Vector.hpp"

/******************************************************************************
**	Class Declaration
******************************************************************************/
#define SIGNAL_CLASS_DEFINITION(...)																		\
namespace Gorilla																							\
{																											\
	PP_CLASS(SignalSender, __VA_ARGS__)																		\
	{																										\
	private:																								\
		/* Signal Sender */																					\
		typedef void (*fnCallback) (void* PP_COMA(__VA_ARGS__) PP_FUNCTION_ARGS(__VA_ARGS__));				\
		struct Signal																						\
		{																									\
			void* Handler;																					\
			fnCallback Callback;																			\
		};																									\
	public:																									\
		/* Signal Sender */																					\
		PP_MACRO(SignalSender, __VA_ARGS__)()																\
		{																									\
			/* Nothing to do*/																				\
		}																									\
																											\
		~PP_MACRO(SignalSender, __VA_ARGS__)()																\
		{																									\
			Release();																						\
		}																									\
																											\
		/* Connect a destination on a source for a specific Signal */										\
		void Connect(void* _pHandler, fnCallback _pCallback)												\
		{																									\
			if(GetSignalIndex(_pHandler) == (uint32)-1)														\
			{																								\
				Signal& kSignal = m_vSignal.Add();															\
				kSignal.Handler = _pHandler;																\
				kSignal.Callback = _pCallback;																\
			}																								\
		}																									\
																											\
		/* Disconnect a destination */																		\
		void Disconnect(void* _pHandler)																	\
		{																									\
			uint32 uiIndex = GetSignalIndex(_pHandler);														\
			if(uiIndex != (uint32)-1) m_vSignal.RemoveIndex(uiIndex);										\
		}																									\
																											\
		/* Send the Signal on all destination	*/															\
		void Send(PP_FUNCTION_ARGS(__VA_ARGS__))															\
		{																									\
			const uint32 uiSignalCount = m_vSignal.GetSize();												\
			for(uint32 uiSignal = 0; uiSignal < uiSignalCount; ++uiSignal)									\
			{																								\
				Signal& kSignal = m_vSignal[uiSignal];														\
				kSignal.Callback(kSignal.Handler PP_COMA(__VA_ARGS__) PP_FUNCTION_VALUE(__VA_ARGS__));		\
			}																								\
		}																									\
																											\
		void Release()																						\
		{																									\
			m_vSignal.Clear();																				\
		}																									\
																											\
	private:																								\
																											\
		/* Search for a specific Signal */																	\
		uint32 GetSignalIndex(void* _pHandler)																\
		{																									\
			const uint32 uiSignalCount = m_vSignal.GetSize();												\
			for(uint32 uiSignal = 0; uiSignal < uiSignalCount; ++uiSignal)									\
			{																								\
				Signal* pSignal = &m_vSignal[uiSignal];														\
				if(pSignal->Handler == _pHandler)															\
				{																							\
					return uiSignal;																		\
				}																							\
			}																								\
																											\
			return (uint32)-1;																				\
		}																									\
																											\
	private:																								\
		Vector<Signal> m_vSignal;																			\
	};																										\
}																											\

// Helper
#define SIGNAL_SENDER(_name_, ...) PP_TYPE(SignalSender, __VA_ARGS__) SignalSender##_name_
#define SIGNAL_RECEIVER(_type_, _name_, ...) inline static void SignalReceiver##_name_(void* _pHandler, PP_FUNCTION_ARGS(__VA_ARGS__)) { static_cast<_type_*>(_pHandler)->On##_name_(PP_FUNCTION_VALUE(__VA_ARGS__)); }
#define SIGNAL_SEND(_name_, ...) SignalSender##_name_.Send(__VA_ARGS__)
#define SIGNAL_SEND_EX(_source_, _name_, ...) (_source_)->SignalSender##_name_.Send(__VA_ARGS__)
#define SIGNAL_CONNECT(_source_, _destination_, _name_) (_source_)->SignalSender##_name_.Connect(_destination_, (_destination_)->SignalReceiver##_name_)
#define SIGNAL_DISCONNECT(_source_, _destination_, _name_) (_source_)->SignalSender##_name_.Disconnect(_destination_);
#define SIGNAL_DISCONNECT_ALL(_source_, _name_) (_source_)->SignalSender##_name_.Release()																											

// Definition of all template available
SIGNAL_CLASS_DEFINITION()
SIGNAL_CLASS_DEFINITION(T1)
SIGNAL_CLASS_DEFINITION(T1, T2)
SIGNAL_CLASS_DEFINITION(T1, T2, T3)
SIGNAL_CLASS_DEFINITION(T1, T2, T3, T4)
#endif