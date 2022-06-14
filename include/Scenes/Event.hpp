#pragma once
#include <string>
#include <sstream>
#include <functional>
#include <type_traits>
#include <typeinfo>
#include "EventLog.h"

namespace Scenes
{
	template<class Ret, class ...Args> class Event;

	template<class Ret, class ...Args>
	class Event<Ret(Args...)>
	{
	public:
		Event(
			std::function<Ret(Args...)> func,
			std::string name,
			EventLog& eventLogRef
			);

		virtual Ret operator()(
			Args... args
			);

		virtual std::string eventString() const;	

		const std::type_info& targetType() const;

		const std::string& name() const;

	protected:
		const std::function<Ret(Args...)> _function;
		const std::string _name;
		Ret _returnValue;
		EventLog& _eventLogRef;
	};

	template<typename Ret, typename ...Args>
	inline Event<Ret(Args...)>::Event(std::function<Ret(Args...)> func, std::string name,
		EventLog& eventLogRef)
		: _function(func), _name(name), _returnValue(Ret()), _eventLogRef(eventLogRef)
	{}

	template<class Ret, class ...Args>
	inline std::string Event<Ret(Args...)>::eventString() const
	{
		std::stringstream ss;
		ss << _name << " " << _returnValue;
		return ss.str();
	}

	template<class Ret, class ...Args>
	inline const std::type_info& Event<Ret(Args...)>::targetType() const
	{
		return _function.target_type();
	}

	template<class Ret, class ...Args>
	inline const std::string& Event<Ret(Args...)>::name() const
	{
		return _name;
	}

	template<class Ret, class ...Args>
	inline Ret Event<Ret(Args...)>::operator()(Args ...args)
	{
		_returnValue = _function(args...);
		_eventLogRef.addLog(eventString());
		return _returnValue;
	}
}
