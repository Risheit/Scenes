#pragma once
#include <functional>
#include <sstream>
#include <stdexcept>
#include <string>
// ReSharper disable once CppUnusedIncludeDirective
#include <typeinfo>
#include <utility>

#include "EventLog.hpp"

namespace Scenes
{
	template<class ...Args>
	class Event
	{
	public:
		Event(
			std::function<int(Args ...)> func,
			const std::string& name,
			EventLog& eventLogRef
		);

		int operator()(
			Args ... args
		);

		[[nodiscard]] std::string eventString() const;

		[[nodiscard]] const std::type_info& targetType() const;

		[[nodiscard]] const std::string& name() const;

	protected:
		const std::function<int(Args ...)> _function;
		const std::string _name;
		int _returnValue;
		EventLog& _eventLogRef;
	};

	/// <typeparam name="Ret">Return value.</typeparam>
	/// <typeparam name="...Args">Arguments.</typeparam>
	/// <param name="func">Contained functor.</param>
	/// <param name="name">The name of the event. Cannot contain ','.</param>
	/// <param name="eventLogRef">Reference to log to store event calls in.</param>
	template<class ...Args>
	Event<Args...>::Event(
		std::function<int(Args ...)> func, const std::string& name,
		EventLog& eventLogRef
	)
		: _function(std::move(func)), _name(name), _returnValue(int()), _eventLogRef(eventLogRef)
	{
		if (name.find(',') != std::string::npos)
			throw std::invalid_argument{ "Event name cannot contain ','." };
	}

	template<class ...Args>
	std::string Event<Args...>::eventString() const
	{
		std::stringstream ss;
		ss << _name << "," << _returnValue;
		return ss.str();
	}

	template<class ...Args>
	const std::type_info& Event<Args...>::targetType() const
	{
		return _function.target_type();
	}

	template<class ...Args>
	const std::string& Event<Args...>::name() const
	{
		return _name;
	}

	template<class ...Args>
	int Event<Args...>::operator()(Args ...args)
	{
		_returnValue = _function(args...);
		_eventLogRef.addLog(eventString());
		return _returnValue;
	}
}
